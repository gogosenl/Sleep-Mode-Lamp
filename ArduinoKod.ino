#define LDR A5
#define Buzzer 11
#define Led 10
#define Led2 9

int deger;
unsigned long startTime;
unsigned long lightDetectedTime;
bool interruptFlag = false;
bool lightDetected = false;
unsigned long lightCheckInterval = 10000; // 10 saniye
unsigned long lastLightCheck = 0;
int lastLdrValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(Buzzer, OUTPUT);
  pinMode(Led, OUTPUT);
  pinMode(Led2, OUTPUT);
  delay(2000);
  startTime = millis();
}

void loop() {
  if (interruptFlag) {
    Serial.println("Interrupt detected, stopping loop.");
    return;
  }

  if (Serial.available() > 0) {
    char receivedChar = Serial.read();
    if (receivedChar == '\n') {
      interruptFlag = true;
      Serial.println("Enter key detected.");
      return;
    }
  }

  deger = analogRead(LDR);
  Serial.println(deger);

  unsigned long currentTime = millis();

  if (currentTime - lastLightCheck >= lightCheckInterval) {
    int change = deger - lastLdrValue;
    Serial.print("Işık seviyesindeki değişiklik: ");
    Serial.println(change);

    if (deger > 800) { // Eğer ışık seviyesi çok yüksekse, muhtemelen güneş ışığıdır
      Serial.println("Yüksek ışık seviyesi algılandı, muhtemelen güneş ışığı");
    } else if (change < 20 && deger < 800) {
      Serial.println("Düşük değişim ve orta düzeyde ışık seviyesi algılandı, muhtemelen yapay ışık.");
    } else if (change > 50) {
      Serial.println("Işık seviyesinde önemli değişiklik algılandı, değişen koşullar nedeniyle muhtemelen güneş ışığı.");
    }

    lastLdrValue = deger;
    lastLightCheck = currentTime;
  }

  if (deger < 1000) {
    if (!lightDetected) {
      lightDetectedTime = millis();
      lightDetected = true;
    }

    if (millis() - lightDetectedTime >= 3000) {
      digitalWrite(Led, HIGH);
      digitalWrite(Led2, HIGH);
      tone(Buzzer, 500);
      delay(200);
      digitalWrite(Led, LOW);
      digitalWrite(Led2, LOW);
      noTone(Buzzer);
      delay(200);
    }

    if (millis() - lightDetectedTime >= 20000) {
      digitalWrite(Led, HIGH);
      digitalWrite(Led2, HIGH);
      noTone(Buzzer);
      delay(100000);
      lightDetected = false;
    }
  } else {
    lightDetected = false;
  }
}
