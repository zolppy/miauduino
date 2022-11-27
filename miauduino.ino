#include <HX711.h>
#include <EEPROM.h>

#define BPS 9600
#define SECOND 1000
#define MIN_WEIGHT 0.123
#define ACCEPTABLE_WEIGHT 1.234
#define CALIBRATION_FACTOR 1234567

#define RELAY_PIN 1
#define SCALE_SCK_PIN 2
#define SCALE_DOUT_PIN 3

HX711 scale;
float weight;
int calibration_factor;

void setup(void) { 
  if(EEPROM[0] != 1) {
    EEPROM[0] = 1;
    EEPROM[1] = CALIBRATION_FACTOR;
  }

  calibration_factor = EEPROM[1];

  Serial.begin(BPS);
  scale.set_scale(calibration_factor); // Ajusta fator de calibracao

  // Configuracao de pinos
  pinMode(RELAY_PIN, OUTPUT);
  scale.begin(SCALE_DOUT_PIN, SCALE_SCK_PIN);

  Serial.println();
  Serial.println("HX711 - Calibracao da balanca");
  Serial.println("Remova objetos da balanca");
  Serial.println("Depois que as leituras comecarem, coloque um objeto conhecido sobre a balanca");

  scale.set_scale(); // Define o valor padrao
  zero_scale(); // Zera para desconsiderar peso da estrutura
}

void zero_scale(void) {
  Serial.println();
  scale.tare(); // Zera a balanca
  Serial.println("Balanca zerada ");  
}

void loop(void) {
  Serial.println();
  Serial.print("Peso: ");
  Serial.print(scale.get_units(), 3); // Imprime peso da balanca com 3 casas decimais
  Serial.print(" kg");
  Serial.print("      Fator de calibracao: ");
  Serial.println(CALIBRATION_FACTOR);
  delay(0.5 * SECOND);

  weight = get_units();

  while(weight < ACCEPTABLE_WEIGHT) {
    digitalWrite(RELAY_PIN, HIGH);

    delay(0.5 * SECOND);

    weight = get_units();
  }

  digitalWrite(RELAY_PIN, LOW);
}
