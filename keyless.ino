#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#define mySerial Serial2  // use for ESP32
uint8_t id;
int hasil = 0;
const int relayPin = 18;  // Pin GPIO yang digunakan untuk mengendalikan relay
const char* ssid = "SoemNoer";
const char* password = "adirahmad";
const unsigned long interval = 5000;  // Waktu interval dalam milidetik (5 detik)
//Your Domain name with URL path or IP address with path
const char* serverName = "http://Soemnoer.my.id/cekdaftar.php";
unsigned long previousMillis = 0;
String sensorReadings;
String kordinat = ""; 
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  setupfinger();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      sensorReadings = httpGETRequest(serverName);
      // Serial.println(sensorReadings);
      // gpsloop();
      JSONVar myObject = JSON.parse(sensorReadings);

      if (JSON.typeof(myObject) == "array") {
         bool shouldTurnOffRelay = true;
        for (int i = 0; i < myObject.length(); i++) {
          JSONVar data = myObject[i];
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
          }
          // Periksa apakah "face" ada dalam data dan nilainya "1"
          if (data.hasOwnProperty("face") && face == "1") {
            shouldTurnOffRelay = false; // Set to not turn off the relay
          }
        }

        // Matikan relay jika shouldTurnOffRelay adalah true
        if (shouldTurnOffRelay) {
          digitalWrite(relayPin, LOW);
        }
      } else {
        Serial.println("Objek JSON bukan tipe 'array'");
      }
    } else {
      Serial.println("WiFi Tidak Terhubung");
    }
  }
}

void sendDataToDatabase() {
  Serial.println("berhasil");
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
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}


String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);
  int httpResponseCode = http.GET();
  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}