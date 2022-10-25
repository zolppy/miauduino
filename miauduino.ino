#include <HX711.h> // Modulo conversor e amplificador de sinais para celulas de carga
#define DOUT_PIN xxxx
#define SCK_PIN xxxx
#define MIN_WEIGHT xxxx
#define RELAY_PIN1 xxxx
#define RELAY_PIN2 xxxx
#define RELAY_PIN3 xxxx
#define RELAY_PIN4 xxxx

HX711 load_cell;

float weight;
float calibration_factor = xxxx;

void setup(void) {
  // Para rele:
  // LOW: ligado
  // HIGH: desligado
  
  Serial.begin(9600); // Inicializa a comunicacao serial em 9600bps
  
  // Define pinos de saida para o rele
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(RELAY_PIN3, OUTPUT);
  pinMode(RELAY_PIN4, OUTPUT);
  
  // Rele inicia desligado
  digitalWrite(RELAY_PIN1, HIGH);
  digitalWrite(RELAY_PIN2, HIGH);
  digitalWrite(RELAY_PIN3, HIGH);
  digitalWrite(RELAY_PIN4, HIGH);
  
  load_cell.begin(DOUT_PIN, SCK_PIN);
  load_cell.set_scale();
  load_cell.tare(); // Zera
}

void loop(void) {
  load_cell.set_scale(calibration_factor); // Ajusta fator de calibracao
  
  if(weight == MIN_WEIGHT) {
    // Rele ativa tensao para ligar motor espiral de liberacao de racao
    digitalWrite(RELAY_PIN1, LOW);
  }
  
  delay(xxxx); // Em ms (1000 ms == 1 s)
}
