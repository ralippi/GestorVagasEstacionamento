/*Para o Modulo Ethernet*/
#include <UIPEthernet.h>
#include <utility/logging.h>
/*Para o Sensor Ultrassonico*/
#include <Ultrasonic.h>
/*Para o LCD*/
#include <LiquidCrystal.h>
/*Para o MQTT*/
#include <PubSubClient.h>

/*Define o pino do Arduino a ser utilizado com o pino Trigger do sensor*/
#define PINO_TRIGGER 9
/*Define o pino do Arduino a ser utilizado com o pino Echo do sensor*/
#define PINO_ECHO 8
#define ledVerde 7
#define ledVermelho 6
int statusVaga = 0;
int qtdeVagasOcupadas = 0;
int qtdeVagasLivres = 0;

/*Inicializa o sensor ultrasonico*/
Ultrasonic ultrasonic(PINO_TRIGGER, PINO_ECHO);

/*Define os pinos que serão ligados ao LCD*/
LiquidCrystal lcd(A2, A3, 5, 4, 3, A4);

/*Atualizar ultimo valor para ID do seu Kit para evitar duplicatas*/
byte mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xF1, 0x40 };

/*Endereço do Cloud MQTT*/
char* server = "m14.cloudmqtt.com";

/*Valor da porta do servidor MQTT*/
int port = 16086;

EthernetClient ethClient;


//////////////////////////////////////////////
//CALLBACK MQTT
//////////////////////////////////////////////
/* FUNÇÃO que irá receber o retorno do servidor*/
void whenMessageReceived(char* topic, byte* payload, unsigned int length) {

  /*Converter pointer do tipo `byte` para typo `char`*/
  char* payloadAsChar = payload;

  /*Converter em tipo String para conveniência*/
  String msg = String(payloadAsChar);
  Serial.print("Topic received: "); Serial.println(topic);

  /*Dentro do whenMessageReceived (callback) da biblioteca MQTT,
    devemos usar Serial.flush() para garantir que as mensagens serão enviadas */
  Serial.flush();

  /*Converter em tipo Int para conveniência e para usar dentro do switch*/
  int msgComoNumero = msg.toInt();

  Serial.print("Retorno lido: "); Serial.println(msgComoNumero);

  Serial.flush();

  switch (msgComoNumero) {
    case 0:
      vagaDesocupada();
      break;
    case 1:
      vagaOcupada();
      break;
    default:
      Serial.println("Fail! Invalid return");
      break;
  }
}

///*Dados do MQTT Cloud*/
PubSubClient client(server, port, whenMessageReceived, ethClient);
long lastReconnectAttempt = 0;


//////////////////////////////////////////////
//SETUP
//////////////////////////////////////////////
void setup() {
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  lcd.begin(16, 2);
  lcd.clear();
  /*Inicializa a porta Serial*/
  Serial.begin(9600);
  
  Serial.println("Connecting...");

  while (!Serial) {}
  if (!Ethernet.begin(mac)) {
    Serial.println("DHCP Failed");
  } else {
    Serial.println(Ethernet.localIP());
  }
  lastReconnectAttempt = 0;
  
}

//////////////////////////////////////////////
//LOOP
//////////////////////////////////////////////
void loop() {
    /*Faz a conexão no cloud com nome do dispositivo, usuário e senha respectivamente*/
    if (!client.connected()) {
      long now = millis();
      if (now - lastReconnectAttempt > 5000) {
        lastReconnectAttempt = now;
        if (reconnect()) {
          lastReconnectAttempt = 0;
        }
      }
    } else {
      /*A biblioteca PubSubClient precisa que este método seja chamado em cada iteração de `loop()` para manter a conexão MQTT e processar mensagens recebidas (via a função callback)*/
      client.loop();
    }

  int distancia = ultrasonic.distanceRead();

   /*Apresenta os dados, em centimetros, no LCD*/
  lcd.setCursor(0, 0);
  lcd.print("Cent.: ");
  lcd.print("        ");
  lcd.setCursor(7, 0);
  lcd.print(distancia);

  /*Apresenta os dados, em centimetros, na Serial*/
  Serial.print("Cent:  ");
  Serial.println(distancia);

  if (Serial.available() > 0) {
    String msg = Serial.readString();
    lcd.setCursor(0, 1);
    lcd.print("msg : ");
    lcd.setCursor(7, 1);
    lcd.print("         ");
    lcd.setCursor(7, 1);
    lcd.print(msg);
  }

  delay(100);
  statusVaga = funcaoStatusVaga(distancia);

  /*Envia o retorno para o cloud no topic estacionamento*/
  client.publish("estacionamento", "oi");
  Serial.println("statusVaga sent");
  Serial.println(statusVaga);
  
  
}


//////////////////////////////////////////////
//FUNÇÕES
//////////////////////////////////////////////

/*Função para (re)conectar no MQTT*/
boolean reconnect() {
  if (client.connect("estacionamento", "iot4", "iot4")) {
    Serial.println("Connected");

    /*Envia uma mensagem para o cloud no topic portao2*/
    client.publish("estacionamento", "Online");
    Serial.println("estacionamento sent");
//    feedbackProcessando();

    /*Conecta no topic para receber mensagens*/
    client.subscribe("estacionamento");
    Serial.println("conectado estacionamento");
//    feedbackConectado();
  } else {
    Serial.println("Failed to connect to MQTT server");
//    feedbackFalha();
  }
}

/*Função STATUS VAGA LED*/
int funcaoStatusVaga(int distancia) {
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
  delay(500);
}

/*Função APAGAR Luz*/
void apagarLed (int led) {
  digitalWrite(led, LOW);
  delay(500);
}

/*Função que aumenta vagas disponiveis e diminui vagas ocupadas*/
void vagaDesocupada () {

}

/*Função que diminui vagas disponiveis e aumenta vagas ocupadas*/
void vagaOcupada () {

}

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
