#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h> // Thư viện điều khiển LED tích hợp trên ESP32-S3

const char* ssid = "H2SO4+2NH3=Pass";
const char* password = "DuyenLinh";
const char* mqtt_server = "192.168.1.17";
const int mqtt_port = 1883;

const char* mqtt_user = "mqtt-user";
const char* mqtt_pass = "110506";
const char* student_id = "10124103";

String topic_cmd = "iot/lab2/10124103/cmd";
String topic_state = "iot/lab2/10124103/state";

WiFiClient espClient;
PubSubClient client(espClient);

// Cấu hình LED tích hợp trên GPIO 48 của ESP32-S3
#define PIN_RGB 48
#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_RGB, NEO_GRB + NEO_KHZ800);

bool ledState = false;

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  message.trim();

  if (message == "ON") {
    ledState = true;
    // Bật LED tích hợp (Sáng màu Xanh lá, bạn có thể đổi màu tùy ý)
    pixels.setPixelColor(0, pixels.Color(0, 255, 0)); 
    pixels.show();
  } else if (message == "OFF") {
    ledState = false;
    // Tắt LED tích hợp
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); 
    pixels.show();
  }

  client.publish(topic_state.c_str(), ledState ? "ON" : "OFF");
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP-Lab2-" + String(student_id);
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      client.subscribe(topic_cmd.c_str());
    } else {
      delay(2000);
    }
  }
}

void setup() {
  // Khởi động LED tích hợp
  pixels.begin();
  pixels.setBrightness(50); // Chỉnh độ sáng (0 - 255)
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Tắt lúc khởi động
  pixels.show();

  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.setKeepAlive(60);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}