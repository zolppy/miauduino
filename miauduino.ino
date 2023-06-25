#include <HX711.h>
#include <EEPROM.h>

#define DOUT 5  // Pino DOUT do HX711
#define SCK 3   // Pino SCK do HX711
#define REL 4   // Pino de saída do relé
#define BTN 2   // Pino de entrada do botão
#define WEIGHT_DELAY 500    // Atraso entre as leituras de peso em milissegundos
#define MIN_WEIGHT 1.0f     // Peso mínimo em kg
#define CALIBRATION_ADDR 0  // Endereço da EEPROM para armazenar a calibração

HX711 scale(DOUT, SCK);
float calibrationFactor = 0.0f;

void setup() {
  pinMode(REL, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);
  Serial.begin(9600);
  scale.set_scale();
  scale.tare();

  if (!EEPROM.read(CALIBRATION_ADDR)) {
    calibrateScale();
    EEPROM.write(CALIBRATION_ADDR, 1);
    Serial.println("Calibração concluída e salva na memória.");
    while (true) {
      Serial.println("Reinicie o Arduino.");
      delay(5000);
    }
  } else {
    calibrationFactor = loadCalibrationFactor();
    scale.set_scale(calibrationFactor);
    scale.tare();
    Serial.println("Balança calibrada e pronta para uso.");
  }
}

void loop() {
  float weight = scale.get_units();
  bool buttonPressed = digitalRead(BTN) == LOW;

  if (buttonPressed || (weight < MIN_WEIGHT && millis() % 60000 < WEIGHT_DELAY)) {
    if (feedAnimal()) {
      Serial.println("Alimentação iniciada.");
      while (weight < MIN_WEIGHT) {
        digitalWrite(REL, HIGH);
        delay(3000);
        digitalWrite(REL, LOW);
        delay(500);
        weight = scale.get_units();
      }
      Serial.println("Alimentação concluída.");
    }
  }
}

void calibrateScale() {
  Serial.println("Calibração necessária.");
  Serial.println("Coloque um peso conhecido na balança e pressione ENTER.");
  while (!Serial.available()) {}
  Serial.read();  // Descarta o caractere ENTER

  long rawValue = scale.read_average(10);
  float knownWeight = getFloatFromSerial();

  calibrationFactor = rawValue / knownWeight;
  scale.set_scale(calibrationFactor);
  scale.tare();

  Serial.print("Fator de calibração: ");
  Serial.println(calibrationFactor);
}

float getFloatFromSerial() {
  char input[16];
  while (!Serial.available()) {}
  size_t size = Serial.readBytesUntil('\n', input, sizeof(input) - 1);
  input[size] = '\0';
  return atof(input);
}

float loadCalibrationFactor() {
  float factor = 0.0f;
  EEPROM.get(CALIBRATION_ADDR + 1, factor);
  return factor;
}

void saveCalibrationFactor(float factor) {
  EEPROM.put(CALIBRATION_ADDR + 1, factor);
}

bool feedAnimal() {
  Serial.println("Pressione o botão para iniciar a alimentação.");
  while (true) {
    if (digitalRead(BTN) == LOW) {
      while (digitalRead(BTN) == LOW) {}
      return true;
    }
  }
}
