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

Leitor de Proximidade

<ul>
<li>
    Sem Risco de enchente: se o nível da água for maior menor que a medida programada, sem pontos.
</li>
<li>
    Risco de enchente: se o nível da água for maior que a medida progamada, 3 pontos na escala de risco.
</li>
</ul>
<br>
