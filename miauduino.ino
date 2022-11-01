#include <HX711.h> // Modulo conversor e amplificador de sinais para balancas

#define CALIBRATION_FACTOR xxxx
#define BALANCE_DOUT_PIN xxxx
#define BALANCE_SCK_PIN xxxx
#define RELAY_DOUT_PIN xxxx
#define RELAY_SCK_PIN xxxx
#define MIN_MASS xxxx
#define ACCETABLE_WEIGHT xxxx
#define BPS 9600

HX711 balance;

float weight;

void setup(void) {
  Serial.begin(BPS);
  
  // Configura pinos de saida para o rele
  pinMode(RELAY_DOUT_PIN, OUTPUT);
  pinMode(RELAY_SCK_PIN, OUTPUT);
  
  // Desliga rele
  digitalWrite(RELAY_DOUT_PIN, HIGH);
  digitalWrite(RELAY_SCK_PIN, HIGH);
  
  // Configura pinos da balanca
  balance.begin(SCALE_DOUT_PIN, SCALE_SCK_PIN);

  // Saida de dados no monitor serial
  Serial.println();
  Serial.println("HX711 - Calibracao da balanca");
  Serial.println("Remova o objeto da balanca");
  Serial.println("Depois que as leituras comecarem, coloque um objeto conhecido sobre a balanca");

  balance.set_scale(); // Limpa valor da balanca
  zero_scale(); // Zera para desconsiderar peso da estrutura
}

void zero_scale(void) {
  Serial.println();
  balance.tare(); // Zera a balanca
  Serial.println("balanca zerada ");  
}

void loop(void) {
  balance.set_scale(CALIBRATION_FACTOR); // Ajusta fator de calibracao

  // Saida de dados no monitor serial
  Serial.print("Peso: ");
  Serial.print(balance.get_units(), 3); // Imprime peso da balanca com 3 casas decimais
  Serial.print(" kg");
  Serial.print("      Fator de calibracao: ");
  Serial.println(CALIBRATION_FACTOR);
  delay(500); // Em ms (1000 ms == 1 s)


  if(weight == MIN_MASS) {
    // Rele ativa tensao para ligar motor espiral de liberacao de racao
    while(weight < ACCETABLE_WEIGHT) {
      // Liga rele
      digitalWrite(RELAY_DOUT_PIN, LOW);
    }

    // Desliga rele
    digitalWrite(RELAY_DOUT_PIN, HIGH)
  }
}
