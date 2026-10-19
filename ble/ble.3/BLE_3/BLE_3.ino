
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>
#include <Adafruit_NeoPixel.h>

// LED RGB tich hop tren ESP32-S3
#define LED_PIN 48
#define NUM_LEDS 1

Adafruit_NeoPixel led(
  NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800
);

// Ten BLE hien tren dien thoai
#define BLE_NAME "DYN"

// UUID
#define SERVICE_UUID "12345678-1234-1234-1234-1234567890ab"
#define CHAR_UUID    "12345678-1234-1234-1234-1234567890ac"

// Xu ly lenh dieu khien LED
class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) override {
    auto value = pCharacteristic->getValue();

    if (value.length() == 0) return;

    char lenh = value[0];

    Serial.print("Nhan lenh: ");
    Serial.println(lenh);

    switch (lenh) {
      case '1':
        led.setPixelColor(0, led.Color(255, 255, 255));
        break;

      case '0':
        led.setPixelColor(0, led.Color(0, 0, 0));
        break;

      case '2':
        led.setPixelColor(0, led.Color(255, 0, 0));
        break;

      case '3':
        led.setPixelColor(0, led.Color(0, 255, 0));
        break;

      case '4':
        led.setPixelColor(0, led.Color(0, 0, 255));
        break;

      default:
        Serial.println("Lenh khong hop le!");
        return;
    }

    led.show();
  }
};

// Xu ly ket noi BLE
class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) override {
    Serial.println("Dien thoai da ket noi!");
  }

  void onDisconnect(BLEServer *pServer) override {
    Serial.println("Da ngat ket noi!");
    BLEDevice::startAdvertising();
    Serial.println("Dang phat lai BLE...");
  }
};

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Khoi tao LED RGB
  led.begin();
  led.setBrightness(100);
  led.clear();
  led.show();

  Serial.println("Dang khoi tao BLE...");

  // Khoi tao BLE
  BLEDevice::init(BLE_NAME);

  // Tao Server
  BLEServer *server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());

  // Tao Service
  BLEService *service = server->createService(SERVICE_UUID);

  // Tao Characteristic nhan lenh
  BLECharacteristic *characteristic =
    service->createCharacteristic(
      CHAR_UUID,
      BLECharacteristic::PROPERTY_WRITE |
      BLECharacteristic::PROPERTY_WRITE_NR
    );

  characteristic->setCallbacks(new MyCallbacks());

  // Khoi dong Service
  service->start();

  // Cau hinh quang ba BLE
  BLEAdvertising *advertising = BLEDevice::getAdvertising();

  advertising->addServiceUUID(SERVICE_UUID);
  advertising->setScanResponse(true);

  // Bat dau quang ba
  BLEDevice::startAdvertising();

  Serial.println("==========================");
  Serial.println("BLE SERVER DA KHOI DONG");
  Serial.println("TEN BLE: DYN");
  Serial.println("DANG CHO DIEN THOAI KET NOI");
  Serial.println("==========================");
}

void loop() {
  delay(100);
}
