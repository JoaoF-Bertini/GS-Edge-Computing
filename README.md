# GS-Edge-Computing

## Integrantes 👋
<ul>
    <li>João Felipe Bertini (RM563478)</li>
    <li>Luan Durbano Almeida (RM563478)</li>
    <li>Pedro Batista (RM563220)</li>
<ul>


<img src="https://github.com/user-attachments/assets/5ccb86af-8a5a-40be-8fed-1fc9746f72c8" alt="print do circuito Arduino/printscreen Arduino circuit"/>

Link da simulação no <a href="https://wokwi.com/projects/432209112083671041">Wokwi</a>

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





