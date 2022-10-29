#include <HX711.h> // Modulo conversor e amplificador de sinais para celulas de carga

// Macros (macroinstrucoes) nao precisam armazenar valores na memoria
#define CALIBRATION_FACTOR xxxx
#define SCALE_DOUT_PIN xxxx
#define SCALE_SCK_PIN xxxx
#define RELAY_DOUT_PIN xxxx
#define RELAY_SCK_PIN xxxx
#define MIN_MASS xxxx
#define MAX_MASS xxxx

HX711 scale;

float mass;

void setup(void) {
  Serial.begin(9600); // Inicializa a comunicacao serial em 9600bps
  
  // Configurando pinos de saida para o rele
  pinMode(RELAY_DOUT_PIN, OUTPUT);
  pinMode(RELAY_SCK_PIN, OUTPUT);
  
  // Rele inicia desligado
  digitalWrite(RELAY_DOUT_PIN, HIGH);
  digitalWrite(RELAY_SCK_PIN, HIGH);
  
  scale.begin(SCALE_DOUT_PIN, SCALE_SCK_PIN); // Configura pinos da celula de carga
  //scale.power_up(); // Liga sensor

  // Saida de dados no monitor serial
  Serial.println(); // Pula uma linha
  Serial.println("HX711 - Calibracao da celula de carga");
  Serial.println("Remova a massa da celula de carga");
  Serial.println("Depois que as leituras comecarem, coloque um massa conhecido sobre a celula de carga");

  scale.set_scale(); // Limpa valor da celula de carga
  zero_scale(); // Zera para desconsiderar massa da estrutura
}

void zero_scale(void) {
  Serial.println(); // Pula uma linha
  balanca.tare(); // Zera a celula de carga
  Serial.println("Celula de carga, zerada ");  
}

void loop(void) {
  scale.set_scale(CALIBRATION_FACTOR); // Ajusta fator de calibracao

  // Saida de dados no monitor serial
  Serial.print("Massa: ");
  Serial.print(balanca.get_units(), 3); // Imprime peso da celula de carga com 3 casas decimais
  Serial.print(" kg");
  Serial.print("      Fator de calibracao: ");
  Serial.println(CALIBRATION_FACTOR);
  delay(500); // Em ms (1000 ms == 1 s)
  
  if(mass == MIN_MASS) {
    // Rele ativa tensao para ligar motor espiral de liberacao de racao
    digitalWrite(RELAY_DOUT_PIN, LOW);
  }
  
  delay(xxxx);
}