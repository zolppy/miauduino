#include <HX711.h> // Modulo conversor e amplificador de sinais para celulas de carga
#include <Servo.h> // Servo motor
// Com macros de pre-processamentos, nao eh necessario alocar espaco na memoria, diferentemente de constantes
// O compilador apenas substitui a macro pelo seu valor, durante o pre-processamento
#define CALIBRATION_FACTOR xxxx
#define MIN_WEIGHT xxxx // Nao eh 0, pois o peso da propria tigela vazia tambem sera computado
#define DOUT_PIN xxxx
#define SCK_PIN xxxx

HX711 load_cell;

float weight;

void setup(void) {
  // A depender do modelo, nao sera necessario "setar" taxa de transmissao e configuracao para comunicacao
  Serial.begin(9600); // Inicia a porta serial, configura a taxa de transmissao para 9600 bps
  load_cell.begin(DOUT_PIN, SCK_PIN); // Inicializa a celula de carga
  load_cell.set_scale(); // Configura a escala da celula de carga
  load_cell.tare(); // Zera a celula de carga
}

void loop(void) {
  load_cell.set_scale(CALIBRATION_FACTOR); // ajusta fator de calibracao
  delay(xxxx); // Em ms (1000 ms == 1 s)
}
