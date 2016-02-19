# Kit_Starter_F1
kit de desenvolvimento para robótica móvel com WiFi

# Descrição do kit:
Desenvolvido pela Davitec, este kit baseado no microcontrolador ESP8266 (NodeMCU V2) é uma plataforma de hardware simples e de baixo custo para estudos de princípios de robótica móvel e Internet das Coisas (IOT), contendo toda a estrutura de um robô quadriciclo. Além de várias possibilidades de expansão para aplicações avançadas.

# Descrição do Firmware:
Este firmware configura o kit F1 como um Access Point (AP) de WiFi e recebe dados UDP para controle dos movimentos do kit.<br/>
A velocidade de funcionamento do motor de tração é feita via PWM. Neste exemplo ela pode ser modificada  girando o trimpot do kit, ou seja, fazemos o uso do conversor analógico/digital para parametrizar o Duty cicle do PWM.<br />
Está Implementada também uma pequena rotina de controle PWM de baixa frequência para o Servo motor que controla a direção do veículo. As posições de controle podem ser calibradas ou alteradas nos #defines.<br />
Há ainda uma demonstração de como funciona os sensores de detecção de obstáculos com circuito integrado ID840, receptor de infravermelho. Essa demonstração é feita utilizando os LEDs sinalizadores frontais.<br />

[>>Código Exemplo](/Starter_F1.ino)

# Detalhamento do Hardware:

![Alt Text](https://github.com/DavitecBrasil/Kit_Starter_F1/blob/master/Tr1.1.1.png)

* Circuito integrado L293 - Dupla Ponte H conectada a um motor DC e com outra conexão livre para uso geral.
* Motor DC com redução e rodas.
*	Um servo motor acoplado ao sistema que direciona as duas rodas frontais. 
*	Dois LEDs infravermelho de 5 mm.
*	Circuito integrado ID840 receptor de infravermelho (38 KHz).
*	Dois LEDs Azuis auto brilho de 3 mm no Chassi.
*	Um LED SMD Azul na própria PCI do NodeMCU.
*	Um Trimpot mult-voltas de 10K.
*	Divisor de tensão para medição da bateria.
*	Jumper de realocação da entrada analógica, entre o Trimpot e um divisor para medição das baterias.
* Duas baterias de 4,2V – 4200mAh Ions de lítio recarregáveis.
* Circuito carregador para as baterias integrado na PCI.
*	Uma fonte de 12V-1A para o carregador. (externa).
*	Possibilidade de conexão módulo seguidor de linha.
*	Possibilidade de conexão módulo sensor de Ultra som, microfones de eletreto, entre outros.


# Motivação:
Esta plataforma foi concebida a priore para fins didáticos para alunos de cursos técnológicos e entusiastas em geral.

# Testes:
Para fazer o uso do kit F1, basta instalar a Arduíno IDE, conectar o Kit via USB e carregar o programa.<br />
Além disso será necessário outro dispositivo WIFI (smartphone, Módulo ESP, ou outros módulos) para se conectar ao kit e controlá-lo segundo o protocolo.


