#include <HX711.h> // Modulo da balanca
#include <string>

const int BPS = 9600;
const int SECOND = 1000;
const float MIN_WEIGHT = 0.320;

// Pinos
const string RELAY_PIN_1("A1");
const string RELAY_PIN_2("A2");
const string SCALE_SCK_PIN("A3");
const string SCALE_DOUT_PIN("A4");

float weight;
const float ACCEPTABLE_WEIGHT = 1.500;
const float CALIBRATION_FACTOR = 24560;

HX711 scale;

void setup(void) {
  Serial.begin(BPS);
  
  // Configuracao de pinos
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
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
  scale.set_scale(CALIBRATION_FACTOR); // Ajusta fator de calibracao

  Serial.println();
  Serial.print("Peso: ");
  Serial.print(scale.get_units(), 3); // Imprime peso da balanca com 3 casas decimais
  Serial.print(" kg");
  Serial.print("      Fator de calibracao: ");
  Serial.println(CALIBRATION_FACTOR);
  delay(0.5 * SECOND);

  weight = 0;

  while(weight < ACCEPTABLE_WEIGHT) {
    digitalWrite(RELAY_PIN_1, LOW);
    digitalWrite(RELAY_PIN_2, HIGH);

    delay(0.5 * SECOND);

    weight = get_units();
  }

  digitalWrite(RELAY_PIN_1, HIGH);
  digitalWrite(RELAY_PIN_2, LOW);
}