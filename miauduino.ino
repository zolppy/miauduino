#include <HX711.h> // Modulo da balanca

const int BPS = 9600;
const float MIN_WEIGHT = xxxx;
const float ACCETABLE_WEIGHT = xxxx;
const float CALIBRATION_FACTOR = xxxx;

// Pinos
const char[] RELAY_DOUT_PIN = "xxxx";
const char[] SCALE_SCK_PIN = "xxxx";
const char[] SCALE_DOUT_PIN = "xxxx";

float weight;
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
  scale.set_scale(CALIBRATION_FACTOR); // Ajusta fator de calibracao

  Serial.print("Peso: ");
  Serial.print(scale.get_units(), 3); // Imprime peso da balanca com 3 casas decimais
  Serial.print(" kg");
  Serial.print("      Fator de calibracao: ");
  Serial.println(CALIBRATION_FACTOR);
  delay(500);

    while(weight < ACCETABLE_WEIGHT) {
      // Liga rele
      digitalWrite(RELAY_DOUT_PIN, LOW);
    }

    // Desliga rele
    digitalWrite(RELAY_DOUT_PIN, HIGH)
}
