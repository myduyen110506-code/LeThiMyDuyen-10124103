
String command = "";

String tenBluetooth = "DYN";
String matKhau = "110506";

void setup() {
  Serial.begin(9600);

  Serial.println("MO PHONG LENH AT");
  Serial.println("Ten Bluetooth: MYDUYEN");
  Serial.println("Mat khau: 110506");
}

void loop() {
  if (Serial.available() > 0) {

    command = Serial.readStringUntil('\n');
    command.trim();

    if (command.length() == 0) {
      return;
    }

    Serial.print("Lenh nhan: ");
    Serial.println(command);

    // Kiem tra module
    if (command == "AT") {
      Serial.println("OK");
    }

    // Dat ten Bluetooth: ATNAMELINHTHUY
    else if (command.startsWith("ATNAME")) {
      tenBluetooth = command.substring(6);
      Serial.println("OK");
    }

    // Xem ten Bluetooth
    else if (command == "ATNAME?") {
      Serial.println(tenBluetooth);
    }

    // Dat mat khau: ATPASW151206
    else if (command.startsWith("ATPASW")) {
      matKhau = command.substring(6);
      Serial.println("OK");
    }

    // Xem mat khau
    else if (command == "ATPASW?") {
      Serial.println(matKhau);
    }

    // Xem phien ban
    else if (command == "ATVERSION?") {
      Serial.println("VERSION:2.0");
    }

    // Cau hinh UART
    else if (command.startsWith("ATUART")) {
      Serial.println("OK");
    }

    

    // Cai dat vai tro Bluetooth
    else if (command.startsWith("ATROLE")) {
      Serial.println("OK");
    }

    // Cai dat che do Bluetooth
    else if (command.startsWith("ATMODE")) {
      Serial.println("OK");
    }

    // Gan dia chi thiet bi
    else if (command.startsWith("ATBIND")) {
      Serial.println("OK");
    }

    // Ket noi thiet bi
    else if (command.startsWith("ATLINK")) {
      Serial.println("OK");
    }

    // Xem dia chi Bluetooth
    else if (command == "ATADDR?") {
      Serial.println("ADDR:123:34:232321");
    }

    // Lenh khong hop le
    else {
      Serial.println("ERROR");
    }
  }
}
