# Kit_Starter_F1
kit de desenvolvimento para robótica móvel com WiFi

# Descrição do kit:
Desenvolvido pela Davitec, este kit baseado no microcontrolador ESP8266 (NodeMCU V2) é uma plataforma de hardware simples e de baixo custo para estudos de principios de robótica móvel, contendo toda a estrutura de um robô quadriciclo. Além de varias possíbilidade de expanssão para aplicações avançadas.

# Descrição do Firmware:
<tab />Este firmware configura o kit F1 como um Acess Point (AP) de WiFi e recebe dados UDP para controle dos movimentos do kit, é possível controlar o Servo motor que faz o direcionamento e o motor de tração.<br />
<tab />A velocidade de funcionamento do motor de tração é feita via PWM, neste exemplo ela pode ser modificada  girando o trimpot do kit, ou seja, fazemos o uso do conversor analógico/digital para parametrizar o Duty do PWM.<br />
<tab />Implementado também uma pequena rotina de controle PWM de baixa frequência para o Servo motor que faz controla a direção do veiculo. As posições de controle podem ser calibradas o alteradas nos defines.<br />
<tab />Há ainda uma demonstração de como funciona os sensores de detecção de obstáculos com circuito integrado ID840, receptor de infravermelho, sua demonstração é feita utilizando os LEDs sinalizadores frontais.<br />
[Código](/Starter_F1.ino)

# Detalhamento do Hardware:

![Alt Text](https://github.com/DavitecBrasil/Kit_Starter_F1/blob/master/Tr1.1.1.png)

* Circuito integrado L293 - Dupla Ponte H conectada a um motor DC, e com outra conexão livre para uso geral
* Motor DC com redução (48:1) e rodas
*	Um servo motor acoplado ao sistema que direciona as duas rodas móveis
*	Dois LEDs infravermelho de 5 mm
*	Circuito integrado ID840 receptor de infravermelho (38 KHz)
*	Dois LEDs Azuis auto brilho de 3 mm no Chassi
*	Um LED SMD Azul na própria PCI do NodeMCU
*	Um Trimpot mult-voltas de 10K
*	Divisor de tensão para medição da bateria
*	Jumper de realocação da entrada analógica, entre o Trimpot e um divisor para medição das baterias
* Duas baterias de 4,2V – 4200mAh
* Integrado na PCI o circuito carregador para as baterias
 *	Uma fonte de 12V-1A para o carregador (externa)
*	Possibilidade de conexão módulo seguidor de linha

# Motivação:
Esta plataforma foi consebida a priore para fins didáticos para alunos de cursos técnicos e entusiastas em geral.

# Testes:
Para fazer o uso do kit F1, basta instalar a Arduíno IDE, conectar o Kit via USB e carregar o programa.<br />
Além disso será necessário outro dispositivo para se conectar via WiFi ao kit e controla-lo, segundo o protocolo.


