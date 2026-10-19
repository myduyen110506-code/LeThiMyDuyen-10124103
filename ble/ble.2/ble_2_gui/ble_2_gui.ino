
#include <SoftwareSerial.h>
SoftwareSerial bluetooth(2, 3);

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

  Serial.println("HE THONG DIEU KHIEN DEN");
  Serial.println("Nhap A: Canh bao - den vang");
  Serial.println("Nhap B: An toan - den xanh");
  
}

void loop() {
  // 1. Nhan lenh tu Virtual Terminal
  if (Serial.available() > 0) {
    char lenh = Serial.read();
    if (lenh == '\r' || lenh == '\n') {
      return;
    }
    if (lenh == 'A' || lenh == 'a' ||
        lenh == 'B' || lenh == 'b' ) {
      bluetooth.write(lenh);

      Serial.print("Da gui lenh: ");
      Serial.println(lenh);
    }
    else {
      Serial.println("Lenh khong hop le!");
    }
  }

  if (bluetooth.available() > 0) {
    char phanHoi = bluetooth.read();

    Serial.print("ARD2 phan hoi: ");
    Serial.println(phanHoi);
  }
}
