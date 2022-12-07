#include <HX711.h>
#include <EEPROM.h>

#define DOUT 5 // porta dout do microcrontrolador hx711
#define SCK 3 // porta sck do microcrontrolador hx711
#define REL 4 // porta de saida do relay
#define BTN 5 // porta de saida do botão
#define TMP ((long) 0.5f * 60.0f * powf(10, 3)) // conversão de minutos em milisegundos x * 60 * 10^3 altere o valor de x para os minutos desejados
#define PESO 1.0f // peso minimo em kg

unsigned long tempo = {}; // variavel de tempo, indicarar o timestamp do momento que a alimentação ocorre
bool mem = EEPROM[0]; // variavel que ler a EEPROM
HX711 carga; // representa a balança
float peso = {}; // representa o atual peso da balança
bool btn = false; // representa se o botão foi precionado ou não

void setup() {
  
  pinMode(REL, OUTPUT);
  pinMode(BTN, INPUT);
  Serial.begin(9600);
  carga.begin(DOUT, SCK);
  carga.set_scale();
  carga.tare();
  char string[100];
  float base = {};
  long media = {};
  float escala = {};
// caso não exista um valor salvo na EEPROM, a calibração é feita, logo deve em uma primeira execução EEPROM[0] deve ser falso
  if(!mem){
    Serial.println("Calibracao Necessaria");
releitura: // ponto de repetição
    Serial.println("Coloque um peso conhecido na balanca");
    Serial.println("Presione enter para comecar");
    while(!Serial.available()) ; // enquanto o usurio não digita nada
    Serial.read(); // ler o enter e descarta
    media = carga.read_average(5); // pega a leitura media da balança
    sprintf(string, "Valor da media: %ld", media);
    Serial.println(string);

    Serial.println("Digite o valor do peso conhecido");
    while(!Serial.available()) ;
    int terminador = Serial.readBytes(string, sizeof(string)); //ler a string digitada no monitor serial
    string[terminador] = '\0'; // coloca o terminador nulo no final, no lugar do enter
    
    base = (float) atof(string); // converte a string para um numero float
    escala = media/base; // caucula a escala
    sprintf(string, "Valor da media: %ld", media);
    Serial.print(string);
    Serial.print(" Valor do denominador: "); Serial.print(base); Serial.print(" Valor da escala: "); Serial.println(escala);
    Serial.println("Esses valores confirmam - 'S' para avancar ou 'N' para repetir");
denovo:
    while(!Serial.available()) ;
    char resposta = Serial.read();
    if(toupper(resposta) == 'S') goto ok;
    if(toupper(resposta) == 'N') goto releitura;
    goto denovo;
ok: Serial.println("Calibracao concluida e salva na memoria");
    carga.set_scale(escala); //seta a escala
    carga.tare();
    EEPROM[0] = true;
    EEPROM.put(1, escala); //salva o valor na EEPROM[1]
    while(true){
      Serial.println("Reinicie o arduino"); // reinicialização do arduino necessaria
      delay(5000);
    }
  }else{ //caso exista um valor salvo na memoria
    Serial.println("Deixe a balanca em repouso sem nenhum peso");
    delay(10000);
    EEPROM.get(1, escala);
    Serial.println(escala);
    carga.set_scale(escala);
    carga.tare();
  }
}

void loop() {
  //Serial.println(carga.get_units()); usado para teste
  //delay(1000); usado para teste
  peso = carga.get_units();
  btn = digitalRead(BTN);
  delay(500);
  if((btn == true) || (tempo == 0 && peso < PESO) || (tempo - millis() >= TMP && peso < PESO)){
    tempo = millis();
    btn = false;
    while(true){
      if(peso >= PESO){
        digitalWrite(REL, LOW);
        break;
      }
      digitalWrite(REL, HIGH);
      delay(3000);
    }
    
  }
}