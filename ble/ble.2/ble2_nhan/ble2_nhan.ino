

const int LED_TIM = 12;
const int LED_DO = 13;

void setup() {
  // Khai bao 2 chan LED la ngo ra
  pinMode(LED_TIM, OUTPUT);
  pinMode(LED_DO, OUTPUT);

  // Ban dau tat ca LED
  digitalWrite(LED_TIM, LOW);
  digitalWrite(LED_DO, HIGH);

  // Khoi dong UART ket noi voi HC2
  Serial.begin(9600);
}

void loop() {
  // Kiem tra co du lieu tu HC2 hay khong
  if (Serial.available() > 0) {
    char lenh = Serial.read();

    if (lenh == '\r' || lenh == '\n') {
      return;
    }
    if (lenh == 'A' || lenh == 'a') {
      digitalWrite(LED_TIM, HIGH);
      digitalWrite(LED_DO, LOW);

      Serial.println("A: CANH BAO");
    }

    else if (lenh == 'B' || lenh == 'b') {
      digitalWrite(LED_TIM, LOW);
      digitalWrite(LED_DO, HIGH);

      Serial.println("B: BINH THUONG");
    }

    
  }
}
