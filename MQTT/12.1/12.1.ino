#include <WiFi.h> // Sử dụng <WiFi.h> nếu dùng ESP32
#include <PubSubClient.h>
#include <ArduinoJson.h>

/* --- CẤU HÌNH THÔNG TIN --- */
const char* device_name = "ESP_Device_";
const char* ssid = "H2SO4+2NH3=Pass";          // Tên WiFi
const char* password = "DuyenLinh";  // Mật khẩu WiFi

const char* mqtt_server = "192.168.1.17";    // IP của Home Assistant
const int mqtt_port = 1883;                  // Cổng MQTT mặc định
const char* mqtt_user = "mqtt-user";      // Username tài khoản MQTT trên HA
const char* mqtt_pass = "110506";      // Password tài khoản MQTT trên HA
const char* student_id = "10124103";              // Thay bằng Mã Sinh Viên của bạn

// Topic MQTT: iot/lab1/<MASV>/sensor
const char* mqtt_topic = "iot/lab1/10124103/sensor";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect() {
  while (!client.connected()) {
    String clientId = String(device_name) + String(student_id);
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("MQTT connected");
    } else {
      Serial.print("MQTT failed, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Đóng gói dữ liệu Giả lập Nhiệt độ & Độ ẩm dưới dạng JSON
  StaticJsonDocument<200> doc;
  doc["temp"] = random(250, 350) / 10.0; // Tạo giá trị từ 25.0 - 35.0
  doc["hum"] = random(400, 800) / 10.0;  // Tạo giá trị từ 40.0 - 80.0

  char payload[128];
  serializeJson(doc, payload);

  // Publish payload lên Broker
  client.publish(mqtt_topic, payload);

  delay(5000); // Gửi dữ liệu định kỳ mỗi 5 giây
}
