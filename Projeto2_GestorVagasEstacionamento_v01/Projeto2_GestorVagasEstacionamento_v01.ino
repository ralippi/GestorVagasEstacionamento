//Programa : Medidor de distancia com HC-SR04
//Autor : Arduino e Cia

#include <Ultrasonic.h>
#include <LiquidCrystal.h>  //Carrega a biblioteca LCD

//Define o pino do Arduino a ser utilizado com o pino Trigger do sensor
#define PINO_TRIGGER 9
#define ledVerde 8 // verde
#define ledVermelho 7 // vermelho
//Define o pino do Arduino a ser utilizado com o pino Echo do sensor
#define PINO_ECHO 10

//Inicializa o sensor ultrasonico
Ultrasonic ultrasonic(PINO_TRIGGER, PINO_ECHO);

//Define os pinos que serão ligados ao LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int statusVG = 0;

void setup()
{
  Serial.begin(9600); //Inicializa a serial
  lcd.begin(16, 2); //Inicializa LCD
  lcd.clear();     //Limpa o LCD
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
}

void loop()
{

  //  digitalWrite(ledVerde,HIGH);
  //  digitalWrite(ledVermelho,HIGH);
  //float cmMsec, inMsec;

  //Le os dados do sensor, com o tempo de retorno do sinal
  //long microsec = ultrasonic.timing();

  //  //Calcula a distancia em centimetros
  //  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  //  //Calcula a distancia em polegadas
  //  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);

  //int distancia = 20;

  int distancia = ultrasonic.distanceRead();

  // Escreve o valor da distância no painel Serial
  // Serial.print("Distance in CM: ");
  // Serial.println(distancia);

  delay(250);
  //Apresenta os dados, em centimetros, no LCD e na Serial
  lcd.setCursor(0, 0);
  lcd.print("Cent.: ");
  lcd.print("        ");
  lcd.setCursor(7, 0);
  lcd.print(distancia);

  Serial.print("Cent:  ");
  Serial.println(distancia);

  //Apresenta os dados, em polegadas, no LCD e na Serial
  //  if(msg.equals("")){
  //  lcd.setCursor(0,1);
  //  lcd.print("msg : ");
  //  }
  //  else{
  //  lcd.setCursor(0,1);
  //  lcd.print("msg : ");
  //  lcd.setCursor(7,1);
  //  lcd.print("         ");
  //  lcd.setCursor(7,1);
  //  lcd.print(msg);
  //  }

  if (Serial.available() > 0) {
    String msg = Serial.readString();
    lcd.setCursor(0, 1);
    lcd.print("msg : ");
    lcd.setCursor(7, 1);
    lcd.print("         ");
    lcd.setCursor(7, 1);
    lcd.print(msg);
  }

  //  Serial.print(", Pol. : ");
  //  Serial.println(inMsec);

  delay(100);


  statusVaga(distancia);
  //  if (distancia >= 20 && distancia <= 40) {
  //    Serial.println("Vaga Ocupada");
  //     acenderLed (ledVermelho);
  //     apagarLed(ledVerde);
  //  }
  //  else if (distancia >= 40 && distancia <= 60) {
  //    Serial.println("Vaga Livre");
  //     acenderLed (ledVerde);
  //     apagarLed(ledVermelho);
  //  }
  //  else {
  //    Serial.println("Nada");
  //    apagarLed(ledVermelho);
  //    apagarLed(ledVerde);
  //
  //}

}

//////////////////////////////////////////////
//FUNÇÕES
//////////////////////////////////////////////
/*Função ACENDER Luz*/
int statusVaga(int distancia) {
  if (distancia <= 40) {
    Serial.println("Vaga Ocupada");
    acenderLed (ledVermelho);
    apagarLed(ledVerde);
    return 1;
  }
  else {
    Serial.println("Vaga Livre");
    acenderLed (ledVerde);
    apagarLed(ledVermelho);
    return 0;
  }
  //  else {
  //    Serial.println("Nada");
  //    apagarLed(ledVermelho);
  //    apagarLed(ledVerde);
  //    return false;
  //}

}

/*Função ACENDER Luz*/
void acenderLed (int led) {
  digitalWrite(led, HIGH);
  delay(250);
}

/*Função APAGAR Luz*/
void apagarLed (int led) {
  digitalWrite(led, LOW);
  delay(250);
}

// Função para (re)conectar no mqtt
//boolean reconnect() {
//  if (client.connect("celular", "iot2", "iot2")) {
//    Serial.println("Connected");
//
//    /*Envia uma mensagem para o cloud no topic portao2*/
//    client.publish("portao2", "Online");
//    Serial.println("portao2 sent");
//    feedbackProcessando();
//
//    /*Conecta no topic para receber mensagens*/
//    client.subscribe("portao2");
//    Serial.println("conectado portao2");
//    feedbackConectado();
//  } else {
//    Serial.println("Failed to connect to MQTT server");
//    feedbackFalha();
//  }
//}

//void ligarDesligarLed() {
//  digitalWrite(led, !digitalRead(led));
//  delay(500);
//}

//////////////////////////////////////////////
//FUNÇÕES para FEEDBACK
//////////////////////////////////////////////

///*Feedback CONECTADO Ethernet & MQTT*/
//void feedbackConectado () {
//  digitalWrite(ledVerde, HIGH);
//  digitalWrite(ledVermelho, LOW);
//  digitalWrite(ledAmarelo, LOW);
//}
//
///*Feedback CONECTADO Ethernet*/
//void feedbackConectado1 () {
//  digitalWrite(ledVerde, LOW);
//  digitalWrite(ledVermelho, LOW);
//  digitalWrite(ledAmarelo, HIGH);
//}
//
///*Feedback FALHA NA CONEXÃO*/
//void feedbackFalha () {
//  digitalWrite(ledVerde, LOW);
//  digitalWrite(ledVermelho, HIGH);
//  digitalWrite(ledAmarelo, LOW);
//}
//
///*Feedback PROCESSANDO*/
//void feedbackProcessando () {
//  int x = 1;
//  for (x = 1; x <= 7; x++) {
//    digitalWrite(ledAmarelo, HIGH);
//    delay(100);
//    digitalWrite(ledAmarelo, LOW);
//    delay(100);
//  }
//}
