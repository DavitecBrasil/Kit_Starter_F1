/**********************************************************************************************************
 * Código de exemplo para teste de periféricos Kit Starter F1
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Desenvolvido por Junio Cesar Ferreira (junio@davitec.com.br)
 * Davitec - 18/02/2016
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Descrição:
 *   Este programa configura o kit F1 como um Access Point (AP) e recebe dados UDP para controle dos movimentos
 * do kit.
 *   A velocidade de funcionamento do motor de tração é feita via PWM, neste exemplo ela pode ser modificada 
 * girando o trimpot do kit, ou seja, fazemos o uso do conversr analógico digital para parametrizar o PWM.
 *   Temos também uma pequena rotina de controle PWM de baixa frequência para o Servo motor que faz controla a
 * direção do veiculo. As posições de controle podem ser calibradas o alteradas nos defines do programa.
 *   Há ainda uma demonstração de como funciona os sensores de detecção de obstáculos com circuito integrado 
 * ID840, receptor de infravermelho, sua demonstração é feita utilizando os LEDs sinalizadores frontais.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
***********************************************************************************************************/
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// I/O
#define LED        16
#define PonteA      4 
#define PonteB      5
#define LED_L       0
#define LED_R       2
#define EIR_L      10
#define EIR_R       9
#define RecIR      13
#define PinServo   15
#define ADCPin	   A0

// Modo AP
#define  std_SSID       "Starter_F1"
#define  std_PASSWORD   "10101010"

#define PORTA_CONEXAO   5555
#define LBuf            255

// Constantes de calibração limeares do Servo 
#define Dir 69
#define Esq 53
#define Ret 61

// Protótipos Funções 
void PWM_Servo(unsigned short CP);
bool GeraSinalIR(const int Channel);
void WIFI_Send(char* pointer, unsigned short lenght);

// Variaveis Globais
bool B;                  // Bit auxiliar para Pisca LED
WiFiUDP Udp;             // Instancia UDP
IPAddress MyIP;          // Armazena IP do dispositivo
IPAddress remoteIp;      // Armazena IP do remetente   
char packetBuffer[LBuf]; // Buffer para armazenar pacotes recebidos.
char ReplyBuffer[LBuf];  // Buffer de retorno.
unsigned short Dservo=53;// Duty cicle para PWM do Servo motor
unsigned int Duty=1023;  // Duty cicle para PWM do motor DC
char sts_motor='0';      // Status do motor DC

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
void setup() {
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
  pinMode(PonteA,OUTPUT);
  pinMode(PonteB,OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(LED_L, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(EIR_L, OUTPUT);
  pinMode(EIR_R, OUTPUT);
  pinMode(PinServo, OUTPUT);
  pinMode(RecIR, INPUT);
  // Inicializa saídas
  digitalWrite(LED, 1);
  digitalWrite(LED_L, 1);
  digitalWrite(LED_R, 1);
  digitalWrite(EIR_L, 1);
  digitalWrite(EIR_R, 1);
  digitalWrite(PonteA,0);
  digitalWrite(PonteB,0);
  digitalWrite(PinServo,0);
  B=true;
  // Inicializa Serial
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting... F1!!!");
  // Inicializa wifi
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(std_SSID, std_PASSWORD);
  Udp.begin(PORTA_CONEXAO);
  // Limpa Watchdog Timer
  yield();
  // Pisca LEDs frontais e alinha direção
  digitalWrite(LED_L,true);
  digitalWrite(LED_R,true);
  Dservo=Ret;
  for (int index=0;index<10;index++) PWM_Servo(Dservo);
  yield();
  delay(500);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
void loop() {
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
  // Indexador de uso geral
  unsigned short index;   
  // Limpa watchdog timer
  yield();                  
  // utiliza valor do conversor analógio/digital como parâmetro de PWM para o motor de tração
  Duty = analogRead(ADCPin);
  Serial.print("ADC=");
  Serial.println(Duty);
  // ---------------------------------------------------------------------------------------------
  // Pisca-pisca LED do NodeMCU
  B=!B;
  digitalWrite(LED,B);
  // ---------------------------------------------------------------------------------------------
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Gera sinal IR e verifica retorno, acende o LED do lado que tem obstáculo
  if (GeraSinalIR(EIR_L)) digitalWrite(LED_L,true);
  else digitalWrite(LED_L,false);
  delay(50);
  if (GeraSinalIR(EIR_R)) digitalWrite(LED_R,true);
  else digitalWrite(LED_R,false);
  delay(50); 
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // ---------------------------------------------------------------------------------------------
  // Recebe dados WiFi se houver e trata comandos
  int packetSize = Udp.parsePacket();
  if (packetSize > 0){
    Serial.print("Received package size :");
    Serial.println(packetSize);
    Serial.print("From ");
    remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", Port ");
    Serial.println(Udp.remotePort());
    // Leitura do pacote -> buffer:
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;
    Serial.println("Content: ");
    Serial.println(packetBuffer);
	String string = String(packetBuffer);
	// Trata comandos recebidos
	if (string.indexOf("Motor=0")!=(-1)){
		analogWrite(PonteA, 0);
        analogWrite(PonteB, 0);
	}else if (string.indexOf("Motor=1")!=(-1)){
		analogWrite(PonteA, 0);
        analogWrite(PonteB, Duty);
	}else if (string.indexOf("Motor=2")!=(-1)){
		analogWrite(PonteB, 0);
        analogWrite(PonteA, Duty);
	}else if (string.indexOf("Esq")!=(-1)){
		Dservo=Esq;
		for (index=0;index<10;index++) PWM_Servo(Dservo);
	}else if (string.indexOf("Dir")!=(-1)){
		Dservo=Dir;
		for (index=0;index<10;index++) PWM_Servo(Dservo);
	}else if (string.indexOf("Ret")!=(-1)){
		Dservo=Ret;
		for (index=0;index<10;index++) PWM_Servo(Dservo);
	}else if (string.indexOf("SERVO=")!=(-1)){
		index=string.indexOf("SERVO=");
		Dservo=((packetBuffer[index+6]-48)*100)+((packetBuffer[index+7]-48)*10)+(packetBuffer[index+8]-48);
		PWM_Servo(Dservo);
		Serial.print("Duty Servo: ");
		Serial.println(Dservo);
		for (index=0;index<20;index++) PWM_Servo(Dservo);
	}else if (string.indexOf("PWM=")!=(-1)){
		index=string.indexOf("PWM=");
		Duty=((packetBuffer[index+4]-48)*1000)+((packetBuffer[index+5]-48)*100)+((packetBuffer[index+6]-48)*10)+(packetBuffer[index+7]-48);
		PWM_Servo(Duty);
		Serial.print("Duty Motor: ");
		Serial.println(Duty);
	}
  }
  // ---------------------------------------------------------------------------------------------
}// Fim Loop

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// * Subrotina de controle Servo motor
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
void PWM_Servo(unsigned short CP){
	// CP -> 0-106
	unsigned short CZ, i;
	CZ=106-CP;
	digitalWrite(PinServo,1);
	delayMicroseconds(480);
	for (i=0;i<CP;i++) delayMicroseconds(20);
	digitalWrite(PinServo,0);
	for (i=0;i<CZ;i++) delayMicroseconds(20);
	delayMicroseconds(400);
	delay(17);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// * Subrotina de geração de sinal nos LEDs IR
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
bool GeraSinalIR(const int Channel){
	bool R=false;
	unsigned int Cont;
	for (Cont=0;Cont<1000;Cont++){
		digitalWrite(Channel,0);
		delayMicroseconds(30);
		digitalWrite(Channel,1);
		delayMicroseconds(30);
		if (!digitalRead(RecIR)) R=true;
	}
	return R;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Rotina de envio de dados via WiFi
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
void WIFI_Send(char* pointer, unsigned short lenght){
	unsigned short i;
	for (i=0;i<lenght;i++) ReplyBuffer[i]=pointer[i];
	ReplyBuffer[i]=0;
	WiFiUDP packtSend;
	packtSend.beginPacket(remoteIp, Udp.remotePort());
	packtSend.write(ReplyBuffer);
	packtSend.endPacket();
}
