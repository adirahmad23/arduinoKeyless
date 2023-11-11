
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>


#include <TinyGPS++.h>
#include <SoftwareSerial.h>
HardwareSerial gpsSerial(1);  // RX (D26), TX (D27)
TinyGPSPlus gps;


unsigned long previousGpsMillis = 0;
const unsigned long gpsInterval = 1000;  // Interval waktu untuk membaca GPS dalam milidetik (misalnya, 15 detik)


uint8_t id;
int hasil = 0;
const int relayPin = 18;
const int relayStart = 19;
const char* ssid = "SoemNoer";                                    // Nama jaringan WiFi Anda
const char* password = "adirahmad";                               // Kata sandi WiFi Anda
const char* serverName = "https://soemnoer.my.id/cekdaftar.php";  // URL server JSON sesuai kebutuhan Anda
unsigned long previousMillis = 0;
const long interval = 5000;  // Interval waktu dalam milidetik (5 detik)

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Alamat I2C layar LCD dan ukuran (16x2)

float latitude = 0.0;
float longitude = 0.0;

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 26, 27);
  lcdsetup();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Koneksi WiFi...");
  }
  Serial.println("Terhubung ke WiFi");
  setupfinger();
}

void loop() {

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverName);
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        String response = http.getString();
        // Serial.println("Respon JSON:");
        // Serial.println(response);

        JSONVar jsonObj = JSON.parse(response);
        if (JSON.typeof(jsonObj) == "array") {
          bool shouldTurnOffRelay = true;
          for (int i = 0; i < jsonObj.length(); i++) {
            JSONVar data = jsonObj[i];
            String idname = data["id"];
            String nama = data["nama"];
            String proses = data["proses"];
            String face = data["face"];


            if (proses == "0") {
              if (hasil == 1) {
                sendDataToDatabase();
              } else {
                enroll(idname);
              }
            } else if (proses == "1" && face == "1") {
              FunctionFinger(idname, nama);
            } else {
              lcdloop();
            }
            // Periksa apakah "face" ada dalam data dan nilainya "1"
            if (data.hasOwnProperty("face") && face == "1") {
              shouldTurnOffRelay = false;  // Set to not turn off the relay
            }
          }
          if (shouldTurnOffRelay) {
            digitalWrite(relayPin, LOW);
          }
        } else {
          Serial.println("Respon bukan tipe 'array'");
        }
        sendgps();
      } else {
        Serial.print("Gagal mengambil data. Kode respons HTTP: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    } else {
      Serial.println("WiFi Tidak Terhubung");
    }
  }
}

void sendDataToDatabase() {
  // Serial.println("berhasil");
  //Send an HTTP POST request every 10 minutes
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String serverPath = String(serverName) + "?iddaftar=" + String(id);


    // Your Domain name with URL path or IP address with path
    http.begin(serverPath.c_str());

    // If you need Node-RED/server authentication, insert user and password below
    //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      hasil = 0;
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.print("Error code: ");
      // Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
