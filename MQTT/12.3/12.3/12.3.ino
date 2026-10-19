#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

/* --- CẤU HÌNH THÔNG TIN --- */
const char* ssid = "H2SO4+2NH3=Pass";
const char* password = "DuyenLinh";
const char* mqtt_server = "192.168.1.17";
const int mqtt_port = 1883;

const char* mqtt_user = "mqtt-user";
const char* mqtt_pass = "110506";
const char* student_id = "10124103";

WiFiClient espClient;
PubSubClient client(espClient);

/* --- ĐỊNH NGHĨA TOPIC --- */
String topic_cmd;
String topic_state;
String topic_note;
String topic_lwt;

// Cấu hình LED tích hợp NeoPixel trên GPIO 48 của ESP32-S3
#define PIN_RGB 48
#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_RGB, NEO_GRB + NEO_KHZ800);

bool ledState = false;

void applyLed(bool on) {
  ledState = on;
  if (on) {
    pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // Bật LED màu Xanh lá
  } else {
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));   // Tắt LED
  }
  pixels.show();
}

void publishStateRetained() {
  client.publish(topic_state.c_str(), ledState ? "ON" : "OFF", true);
}

void setup_wifi() {
  Serial.print("Connecting WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  msg.trim();
  
  Serial.print("MQTT [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(msg);

  if (String(topic) == topic_cmd) {
    if (msg == "ON") applyLed(true);
    else if (msg == "OFF") applyLed(false);
    else if (msg == "TOGGLE") applyLed(!ledState);
    
    publishStateRetained();
  }
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP-Lab3-" + String(student_id);
    Serial.print("Connecting MQTT... ");

    if (client.connect(
        clientId.c_str(),
        mqtt_user,
        mqtt_pass,
        topic_lwt.c_str(),
        0,
        true,
        "OFFLINE"
    )) {
      Serial.println("connected");
      client.subscribe(topic_cmd.c_str());
      
      client.publish(topic_note.c_str(), "ESP BOOT / RECONNECT (note not retained)", false);
      client.publish(topic_lwt.c_str(), "ONLINE", true);
      publishStateRetained();
    } else {
      Serial.print("failed rc=");
      Serial.println(client.state());
      delay(1500);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  // Khởi động LED tích hợp
  pixels.begin();
  pixels.setBrightness(50);
  applyLed(false);

  topic_cmd = "iot/lab3/" + String(student_id) + "/cmd";
  topic_state = "iot/lab3/" + String(student_id) + "/state";
  topic_note = "iot/lab3/" + String(student_id) + "/note";
  topic_lwt = "iot/lab3/" + String(student_id) + "/lwt";

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.setKeepAlive(60);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}