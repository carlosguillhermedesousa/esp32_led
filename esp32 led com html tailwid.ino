#include <WiFi.h>
#include <WebServer.h>
#include <BluetoothSerial.h>

#define LED_PIN 2   // LED conectado ao GPIO 2 (LED onboard do ESP32)

// Credenciais   Wi-Fi
const char* ssid = "SENAI-LAB3";
const char* password = "*senailab3@2023*";

// Objetos de servidor
WebServer server(80);
BluetoothSerial SerialBT;

bool ledState = false;

// Função para atualizar LED
void setLed(bool state) {
  ledState = state;
  digitalWrite(LED_PIN, state ? HIGH : LOW);
}

String htmlPage() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html lang="pt-BR">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Controle LED ESP32</title>
    <script src="https://cdn.tailwindcss.com"></script>
  </head>
  <body class="bg-gray-100 flex items-center justify-center h-screen">
    <div class="bg-white shadow-lg rounded-lg p-8 max-w-md text-center">
  )rawliteral";

  // Exibe o IP em um título h1 centralizado e em negrito
  html += "<h1 class='text-2xl font-bold text-center mb-4'>IP do ESP32: ";
  html += WiFi.localIP().toString();
  html += "</h1>";

  html += R"rawliteral(
      <h2 class="text-xl font-semibold text-blue-600 mb-4">Controle do LED ESP32</h2>
      <p class="mb-6 text-lg">Estado atual: <span class="font-semibold text-gray-700">)rawliteral";

  html += (ledState ? "Ligado" : "Desligado");

  html += R"rawliteral(</span></p>
      <label class="switch">
        <input type="checkbox" onchange="toggleLed(this.checked)" )rawliteral";

  if (ledState) {
    html += "checked";
  }

  html += R"rawliteral(>
        <span class="slider"></span>
      </label>
      <footer class="mt-6 text-sm text-gray-500">Projeto ESP32</footer>
    </div>
    <script>
      function toggleLed(state) {
        if(state) {
          fetch('/on');
        } else {
          fetch('/off');
        }
      }
    </script>
  </body>
  </html>
  )rawliteral";

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
