#include <HX711.h> // Modulo da balanca

const int BPS = 9600;
const int SECOND = 1000;
const float MIN_WEIGHT = 0.320;

// Pinos
const char[] RELAY_DOUT_PIN = "A1";
const char[] SCALE_SCK_PIN = "A2";
const char[] SCALE_DOUT_PIN = "A3";

HX711 scale;

void setup(void) {
  Serial.begin(BPS);
  
  // Configura pino do rele
  pinMode(RELAY_DOUT_PIN, OUTPUT);
  
  // Configura pinos da balanca
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
  float weight = 0;
  const float ACCEPTABLE_WEIGHT = 1.500;
  const float CALIBRATION_FACTOR = 24560;

  scale.set_scale(CALIBRATION_FACTOR); // Ajusta fator de calibracao

  Serial.println();
  Serial.print("Peso: ");
  Serial.print(scale.get_units(), 3); // Imprime peso da balanca com 3 casas decimais
  Serial.print(" kg");
  Serial.print("      Fator de calibracao: ");
  Serial.println(CALIBRATION_FACTOR);
  delay(0.5 * SECOND);

  while(weight < ACCEPTABLE_WEIGHT) {
    // Liga rele
    digitalWrite(RELAY_DOUT_PIN, LOW);

    delay(1 * SECOND);

    weight = get_units();
  }

  // Desliga rele
  digitalWrite(RELAY_DOUT_PIN, HIGH);
}