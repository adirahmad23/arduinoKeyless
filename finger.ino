#include <Adafruit_Fingerprint.h>
#define mySerial Serial2  // use for ESP32
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
#define MODEM_RX 16
#define MODEM_TX 17
const int buzzerPin = 33;  // Pin GPIO yang digunakan untuk mengendalikan buzzer
int failedAttempts = 0;
void setupfinger() {
  while (!Serial)
    ;
  delay(100);
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  finger.getTemplateCount();
  Serial.print("Sensor contains ");
  Serial.print(finger.templateCount);
  Serial.println(" templates");
  Serial.println("Waiting for a valid finger...");
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  pinMode(relayStart, OUTPUT);
  digitalWrite(relayStart, LOW);
}

void enroll(String idname) {
  Serial.println("Ready to enroll a fingerprint!");                                              // 1
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");  //2

  lcd.clear();
  lcd.print("Ready to enroll");
  lcd.setCursor(0, 1);
  lcd.print("Enter ID # (1 to 127)");

  id = idname.toInt();  // Konversi string ke bilangan bulat;
  if (id == 0) {        // ID #0 not allowed, try again!
    return;
  }
  Serial.print("Enrolling ID #");  //3
  Serial.println(id);

  lcd.clear();
  lcd.print("Enrolling ID #");
  lcd.print(id);

  getFingerprintEnroll(id, hasil);
}

void FunctionFinger(String idname, String nama) {
  int ids = atoi(idname.c_str());  // Mengonversi string menjadi int
  int namas = atoi(nama.c_str());  // Mengonversi string menjadi int
  int faces = atoi(nama.c_str());  // Mengonversi string menjadi int
  getFingerprintID(ids, namas);    // Mengirim int ke fungsi getFingerprintID
  delay(1050);                     // Jangan lakukan ini dengan kecepatan penuh.
}

//FUNGSI ENROLL=================================================================================
uint8_t getFingerprintEnroll(int id, int &hasil) {
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #");
  Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        lcd.clear();
        lcd.print("Tap Finger Print");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");  // 5
      lcd.clear();
      lcd.print("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");  //6
  lcd.clear();
  lcd.print("Remove finger");

  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID ");
  Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");  // 7
  lcd.clear();
  lcd.print("Tap Finger Again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");
  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");  // 8
    lcd.clear();
    lcd.print("Finger matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Finger not match");
    lcd.clear();
    lcd.print("Finger not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
  Serial.print("ID ");
  Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    hasil = 1;
    Serial.println("Stored!");  // 9
    lcd.clear();
    lcd.print("Finger Print Added");

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
}

//==============================================================================================================================
//FUNGSI CEK FINGER
uint8_t getFingerprintID(int ids, int namas) {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("No finger detect");
      lcd.setCursor(0, 1);
      lcd.print("Tap Attempt:" + String(failedAttempts));
      delay(2000);  // Delay for 2000 milliseconds (2 seconds)
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a finger");
    lcd.clear();
    lcd.print("Found a finger");
    delay(2000);  // Delay for 2000 milliseconds (2 seconds)
    if (finger.fingerID == ids) {
      if ((digitalRead(buzzerPin) == HIGH) && (digitalRead(relayPin) == LOW)) {
        digitalWrite(buzzerPin, LOW);
      } else {
        digitalWrite(relayPin, !digitalRead(relayPin));
        if (digitalRead(relayPin) == HIGH) {
          digitalWrite(relayStart, HIGH);  // Nyalakan relayStart
          delay(5000);                     // Tunggu 5 detik
          digitalWrite(relayStart, LOW);
        }
        failedAttempts = 0;

        if ((digitalRead(buzzerPin) == HIGH)) {
          digitalWrite(buzzerPin, LOW);
          digitalWrite(relayPin, LOW);
        }
      }  // Toggle status relay

    } else if (ids == NULL) {
      digitalWrite(relayPin, LOW);
    }
    Serial.println(ids);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Finger not found");
    lcd.setCursor(0, 1);
    lcd.print("Tap Attempt:" + String(failedAttempts));
    delay(2000);       // Delay for 2000 milliseconds (2 seconds)
    failedAttempts++;  // Tambahkan jumlah percobaan yang gagal
    if (failedAttempts >= 3) {
      digitalWrite(buzzerPin, HIGH);  // Nyalakan buzzer
    }

    if ((digitalRead(relayPin) == HIGH) && ((digitalRead(buzzerPin) == HIGH))) {
      digitalWrite(relayPin, LOW);
    }
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  lcd.clear();
  lcd.print("Finger Found : " + String(finger.fingerID));
  delay(2000);  // Delay for 2000 milliseconds (2 seconds)
  return finger.fingerID;
}
