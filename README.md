# GS-Edge-Computing

## Integrantes 👋
<ul>
    <li>João Felipe Bertini (RM563478)</li>
    <li>Luan Durbano Almeida (RM563478)</li>
    <li>Pedro Batista (RM563220)</li>
<ul>

<img src="https://github.com/user-attachments/assets/5ccb86af-8a5a-40be-8fed-1fc9746f72c8" alt="print do circuito Arduino/printscreen Arduino circuit"/>

Link da simulação no <a href="https://wokwi.com/projects/432209112083671041">Wokwi</a>
Link do <a href="https://youtu.be/kNCwKiQ6C4A">VÍDEO</a>

## Instruções
O Wokwi pode apresentar pequenos bugs gráficos, especialmente em displays I2C. Recarregar a página pode resolver.

## Explicação do Projeto 📖
Este sistema simula a detecção de riscos ambientais (enchentes) e estruturais (gás e inclinação) utilizando sensores conectados a uma placa Arduino Uno.

Módulo Enchente 🌊

Escala de Risco 💥

<ul>
<li>
    Baixo: LED verde + mensagem no LCD
</li>
<li>
    Moderado: LED amarelo + mensagem no LCD
</li>
<li>
    Crítico: LED Vermelho + mensagem no LCD + buzzer 
</li>
</ul>
<br>

Leitor de Proximidade atráves de um HC-SR04	〽️

<ul>
<li>
    Sem Risco de enchente: se o nível da água for menor que a medida programada, sem pontos
</li>
<li>
    Risco de enchente: se o nível da água for maior que a medida progamada, 3 pontos na escala de risco
</li>
</ul>
<br>

Força da Chuva, simulando o YL-83 com o primeiro potenciômetro 	💧

<ul>
<li>
    Sem Chuva: Potenciômetro maior que 700 sem risco na escala
</li>
<li>
    Chuva leve: Potenciômetro menor que 700, 1 pontos na escala de risco
</li>
    <li>
    Chuva forte: Potenciômetro menor que 500, 2 pontos na escala de risco
</li>
</ul>
<br>

Temperatura (em °C), através do DHT 🌡
 
<ul>
<li>
  Temperatura ideal: Sem risco na escala
</li>
<li>
  Temperatura elevada: 1 ponto na escala de risco
</li>
</ul>
<br>

Umidade, também através do DHT ☁️
 
<ul>
<li>
  Umidade ideal: Sem risco na escala
</li>
<li>
  Umidade elevada: 2 pontos na escala de risco
</li>
</ul>
<br>

Pressão atmosférica, simulando o BMP280 com o segundo potenciômetro
 
<ul>
<li>
  Pressão atmosférica ideal: Sem risco na escala
</li>
<li>
  Pressão atmosférica reduzida: 1 ponto na escala de risco
</li>
</ul>
<br>


Módulo Estrutura (para uma uma área afetada pela enchente) 🏗️

Detector de gás, com MQ2 gas sensor 💨

<ul>
<li>
  Sem gás no local: LED apagado
</li>
<li>
  Gás detectado na área: LED rosa + buzzer
</li>
</ul>
<br>

Sensor de inclinação, simulando o MPU 6050 com o terceiro potenciômetro ↕️

<ul>
<li>
  Estrutura estável: LED apagado
</li>
<li>
  Estrutura inst[avel: LED laranja 
</li>
</ul>
<br>

## Componentes 🛠️
<ul>
    <li>1x Placa Arduino Uno</li>
    <li>1x Breadboard</li>
    <li>3x Potenciômetro</li>
    <li>1x DHT11/22</li>
    <li>2x Buzzer</li>
    <li>1x Display LCD</li>
    <li>5x LEDs</li>
    <li>1x MQ2 gas sensor</li>
    <li>1x Sensor de proximidade (HC-SR04)</li>
    <li>7x Resistores</li>
</ul>
 
<br>

## Dependências 📦
<ul>
    <li>LiquidCrystal I2C</li>
    <li>DHT Sensor library</li>
    <li>RTClib</li>
</ul>
 
<br>

## Explicando o <a href="https://github.com/JoaoF-Bertini/GS-Edge-Computing/blob/main/arduino-code.cc">Código</a> 🧑‍💻

Precisamos incluir as bibliotecas para podermos ter acesso a métodos dos quais vamos precisar.
 
Assim como também é preciso declarar, iniciando tais valores para podermos usá-los depois na aplicação. 📖

```c
#include <Wire.h>
#include "RTClib.h"
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
```
<hr>

Depois vamos definir os pinos dos sensores: ultrassônico, gás (MQ2), inclinação (MPU analógico), chuva, pressão, temperatura/umidade (DHT22), LEDs e buzzers.

Instancia os objetos para RTC, DHT e LCD com endereço 0x27. 🔌

```c
#define TRIG_PIN 3
#define ECHO_PIN 2
#define RAIN_SENSOR_PIN A1
#define PRESSURE_SENSOR_PIN A2
#define DHTPIN 4
#define DHTTYPE DHT22
#define MQ2_SENSOR_PIN A3
#define MPU_SENSOR_PIN A0

#define LED_VERDE 5
#define LED_AMARELO 6
#define LED_VERMELHO 7
#define BUZZER 8

#define LED_ROSA 9
#define LED_LARANJA 10
#define BUZZER_ESTRUTURA 12

RTC_DS1307 rtc; 
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);
```
<hr>

void setup() para inicializar a comunicação serial, pinos, sensores e módulos ⚙️

```c
Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_ROSA, OUTPUT);
  pinMode(LED_LARANJA, OUTPUT);
  pinMode(BUZZER_ESTRUTURA, OUTPUT);

  dht.begin();
  rtc.begin();
  lcd.init();
  lcd.backlight();
```
<hr>

Com void loop() se programa a lógica de monitonaramento 🔁

```c
// Emite pulso ultrassônico e mede o tempo de retorno para calcular distância da água (em cm).
digitalWrite(TRIG_PIN, LOW);
delayMicroseconds(2);
digitalWrite(TRIG_PIN, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG_PIN, LOW);
long duracao = pulseIn(ECHO_PIN, HIGH);
float distancia = duracao * 0.034 / 2.0;

//Leitura de sensores da chuva 
int chuva = analogRead(RAIN_SENSOR_PIN);
int pressaoRaw = analogRead(PRESSURE_SENSOR_PIN);
float pressao = map(pressaoRaw, 0, 1023, 970, 1050);
float temperatura = dht.readTemperature();
float umidade = dht.readHumidity();

// Cálculo e classificação do risco de enchente
if (distancia < 200) risco += 3;
if (umidade > 80) risco += 2;
if (temperatura > 25) risco += 1;
if (chuva <= 700) risco += 1;
if (chuva <= 500) risco += 2;
else if (pressao <= 1000) risco += 1;

if (risco >= 6) nivelRisco = "CRIT";
else if (risco >= 3) nivelRisco = "MOD";
else nivelRisco = "BAIXO";

// Alerta de enchente
if (nivelRisco == "BAIXO") digitalWrite(LED_VERDE, HIGH);
else if (nivelRisco == "MOD") digitalWrite(LED_AMARELO, HIGH);
else {
  digitalWrite(LED_VERMELHO, HIGH);
  tone(BUZZER, 2000);
}

// Alerta de gás
if (gas > 400) {
  digitalWrite(LED_ROSA, HIGH);
  tone(BUZZER_ESTRUTURA, 1000);
}

// Alerta de Estrutura
if (inclinacao > 700 || inclinacao < 300) {
  digitalWrite(LED_LARANJA, HIGH);
}

// Exibição no LCD Mostra em 4 linhas: Data/hora - Distância da água e pressão - Temperatura e umidade - Estado do sensor de chuva + risco calculado
lcd.setCursor(0, 0); // Data e hora
lcd.setCursor(0, 1); // Nível da água e pressão
lcd.setCursor(0, 2); // Temperatura e umidade
lcd.setCursor(0, 3); // Nível de chuva e risco
```
<hr>


## Funções Principais 🛠️

<ul>
<li>
displayTwoLineText: Exibe duas linhas de texto no LCD.
</li>
<li>
verificaRiscoEnchente: Lê sensores de chuva, água, temperatura, umidade e calcula o nível de risco (baixo, médio, alto).
</li>
<li>
verificaRiscoEstrutura: Lê o valor do sensor de gás e do potenciômetro. Ativa LED e buzzer específicos se valores ultrapassarem os limites críticos.
</li>
<li>
loop: Executa a verificação de riscos continuamente, com alertas visuais e sonoros adequados.
</li>
</ul>
<br>

Módulo Enchente:
<ul>
<li>
Define três níveis de risco baseados nos sensores de chuva e água.
</li>
<li>
Imprime no LCD data/hora junto das medições.
</li>
<li>
Buzzer emite som somente no nível crítico.
</li>
</ul>
<br>

Módulo Estrutura:

<ul>
<li>
O potenciômetro simula variação de inclinação.
</li>
<li>
O LED Laranja indica risco por inclinação.
</li>
<li>
O sensor MQ-2 identifica gases inflamáveis.
</li>
<li>
O LED Rosa e o buzzer específico são ativados em caso de risco de gás.
</li>
<li>
Ambas as verificações ocorrem de forma independente mas são coordenadas no loop() principal.
</li>
</ul>
<br>







