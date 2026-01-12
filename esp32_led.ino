#include <WiFi.h>
#include <WebServer.h>
#include <BluetoothSerial.h>

#define LED_PIN 2   // LED conectado ao GPIO 2 (LED onboard do ESP32)

// Credenciais Wi-Fi
const char* ssid = "SENAI";
const char* password = "senaiaraguaina";

// Objetos de servidor
WebServer server(80);
BluetoothSerial SerialBT;

bool ledState = false;

// Função para atualizar LED
void setLed(bool state) {
  ledState = state;
  digitalWrite(LED_PIN, state ? HIGH : LOW);
}

// Página HTML simples
String htmlPage() {
  String html = "<!DOCTYPE html><html><head><title>Controle LED ESP32</title></head><body>";
  html += "<h1>Controle do LED</h1>";
  html += "<p>Estado atual: ";
  html += (ledState ? "Ligado" : "Desligado");
  html += "</p>";
  html += "<a href=\"/on\"><button>Ligar</button></a>";
  html += "<a href=\"/off\"><button>Desligar</button></a>";
  html += "</body></html>";
  return html;
}

// Rotas do servidor
void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleOn() {
  setLed(true);
  server.send(200, "text/html", htmlPage());
}

void handleOff() {
  setLed(false);
  server.send(200, "text/html", htmlPage());
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  setLed(false);

  Serial.begin(9600);

  // Conexão Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Inicializa servidor web
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();
  Serial.println("Servidor Web iniciado.");

  // Inicializa Bluetooth
  SerialBT.begin("ESP32_LED"); // Nome do dispositivo Bluetooth
  Serial.println("Bluetooth iniciado. Conecte-se ao 'ESP32_LED'.");
}

void loop() {
  server.handleClient();

  // Controle via Bluetooth
  if (SerialBT.available()) {
    char c = SerialBT.read();
    if (c == '1') {
      setLed(true);
      SerialBT.println("LED Ligado");
    } else if (c == '0') {
      setLed(false);
      SerialBT.println("LED Desligado");
    }
  }
}
