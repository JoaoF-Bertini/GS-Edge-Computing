#include <Wire.h>
#include "RTClib.h"
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

// === Pinos ===
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

void setup() {
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

  // rtc.adjust(DateTime(2025, 5, 27, 17, 10, 0)); // use apenas uma vez se necessário
}

void loop() {
  DateTime agora = rtc.now();

  // Sensor ultrassônico
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duracao = pulseIn(ECHO_PIN, HIGH);
  float distancia = (duracao > 0) ? duracao * 0.034 / 2.0 : -1;

  int gas = analogRead(MQ2_SENSOR_PIN);
  int inclinacao = analogRead(MPU_SENSOR_PIN);

  // Leitura de sensores
  int chuva = analogRead(RAIN_SENSOR_PIN);
  int pressaoRaw = analogRead(PRESSURE_SENSOR_PIN);
  float pressao = map(pressaoRaw, 0, 1023, 970, 1050);

  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  // === Risco da Chuva ===
  int risco = 0;
  if (distancia > 0 && distancia < 200) risco += 3;
  if (!isnan(umidade) && umidade > 80) risco += 2;
  if (!isnan(temperatura) && temperatura > 25) risco += 1;
  if (chuva <= 700) risco += 1;
  if (chuva <= 500) risco += 2;
  else if (pressao <= 1000) risco += 1;

  String nivelRisco = "BAIXO";
  if (risco >= 6) nivelRisco = "CRIT";
  else if (risco >= 3) nivelRisco = "MOD";

  // Resetar saídas
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_ROSA, LOW);
  digitalWrite(LED_LARANJA, LOW);
  noTone(BUZZER);
  noTone(BUZZER_ESTRUTURA);

  // Alertas de enchente
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

  // Alerta de inclinação simulada
  if (inclinacao > 700 || inclinacao < 300) {
    digitalWrite(LED_LARANJA, HIGH);
  }

  // LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(agora.day() < 10 ? "0" : ""); lcd.print(agora.day()); lcd.print("/");
  lcd.print(agora.month() < 10 ? "0" : ""); lcd.print(agora.month()); lcd.print("/");
  lcd.print(agora.year()); lcd.print(" ");
  lcd.print(agora.hour()); lcd.print(":");
  if (agora.minute() < 10) lcd.print("0");
  lcd.print(agora.minute());

  lcd.setCursor(0, 1);
  if (distancia > 0) {
    lcd.print("Agua:");
    lcd.print(distancia, 1);
    lcd.print("cm ");
  } else {
    lcd.print("Agua:ERRO ");
  }
  lcd.print("P:");
  lcd.print((int)pressao);

  lcd.setCursor(0, 2);
  lcd.print("Temp:");
  if (!isnan(temperatura)) {
    lcd.print((int)temperatura);
    lcd.print((char)223); lcd.print("C ");
  } else {
    lcd.print("ERR ");
  }
  lcd.print("Umid:");
  if (!isnan(umidade)) {
    lcd.print((int)umidade); lcd.print("%");
  } else {
    lcd.print("ERR");
  }

  lcd.setCursor(0, 3);
  lcd.print("Chuva:");
  if (chuva <= 500) lcd.print("Forte ");
  else if (chuva <= 700) lcd.print("Leve  ");
  else lcd.print("Nenhum ");
  lcd.print("R:");
  lcd.print(nivelRisco);

  delay(1000);
}
