#define BLYNK_TEMPLATE_ID "TMPL293dayfR8"
#define BLYNK_TEMPLATE_NAME "LED ESP32"
#define BLYNK_AUTH_TOKEN "yosBQKJyUs5AMZbwsj0KF1r3cVGK_Flp"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define LED_PIN 2   // LED onboard do ESP32

char ssid[] = "SENAI-LAB5";
char pass[] = "*senailab5@2023*";

BlynkTimer timer;

// Função chamada quando o botão no app Blynk (V0) é acionado
BLYNK_WRITE(V0) {
  int valor = param.asInt(); // Lê valor do botão (0 ou 1)

  if (valor == 1) {
    digitalWrite(LED_PIN, HIGH);
    Blynk.virtualWrite(V1, 1); // Atualiza widget de status (ex: LED virtual no app)

    // Agenda para desligar após 500 ms
    timer.setTimeout(500, []() {
      digitalWrite(LED_PIN, LOW);
      Blynk.virtualWrite(V1, 0); // Atualiza status no app
    });
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  timer.run(); // Necessário para o BlynkTimer funcionar
}
