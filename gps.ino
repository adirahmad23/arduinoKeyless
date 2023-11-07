// #include <TinyGPS++.h>
// #include <SoftwareSerial.h>
// #include <Arduino_JSON.h>

// SoftwareSerial gpsSerial(26, 25);  // RX, TX
// TinyGPSPlus gps;
// unsigned long previousGpsMillis = 0;
// const unsigned long gpsInterval = 1000;  // Interval waktu untuk membaca GPS dalam milidetik (misalnya, 15 detik)

// void gpssetup() {
// }

// // void gpsloop() {
// //   String kordinat = "";  // Reset kordinat
// //   unsigned long currentMillis = millis();
// //   if (currentMillis - previousGpsMillis >= gpsInterval) {
// //     previousGpsMillis = currentMillis;
// //     while (gpsSerial.available() > 0) {
// //       if (gps.encode(gpsSerial.read())) {
// //         if (gps.location.isValid()) {
// //           // Mendapatkan data latitude dan longitude
// //           float latitude = gps.location.lat();
// //           float longitude = gps.location.lng();

// //           // Mencetak data latitude dan longitude ke Serial Monitor
// //           Serial.print(latitude, 6);
// //           Serial.print(",");
// //           Serial.print(longitude, 6);
// //           Serial.println();
// //           delay(500);

// //           sendGPSData(latitude, longitude);
// //         }
// //       }
// //     }
// //   }
// // }

// // void sendGPSData() {
// //   // Serial.println("berhasil");
// //   //Send an HTTP POST request every 10 minutes


// //   if (WiFi.status() == WL_CONNECTED) {
// //     HTTPClient http;
// //     //  gpsloop();
// //     //Check WiFi connection status
// //     while (gpsSerial.available() > 0) {
// //       if (gps.encode(gpsSerial.read())) {
// //         if (gps.location.isValid()) {
// //           // Mendapatkan data latitude dan longitude
// //           float latitude = gps.location.lat();
// //           float longitude = gps.location.lng();

// //           // Mencetak data latitude dan longitude ke Serial Monitor
// //           // Serial.print(latitude, 6);
// //           // Serial.print(",");
// //           // Serial.print(longitude, 6);
// //           // Serial.println();
// //           // delay(500);
// //           String serverPath = String(serverName) + "?gps=" + String(latitude, 6) + "," + String(longitude, 6);


// //           // Your Domain name with URL path or IP address with path
// //           http.begin(serverPath.c_str());

// //           // If you need Node-RED/server authentication, insert user and password below
// //           //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

// //           // Send HTTP GET request
// //           int httpResponseCode = http.GET();

// //           if (httpResponseCode > 0) {
// //             hasil = 0;
// //             Serial.print("HTTP Response code: ");
// //             Serial.println(httpResponseCode);
// //             String payload = http.getString();
// //             Serial.println(payload);
// //           } else {
// //             Serial.print("Error code: ");
// //             Serial.println(httpResponseCode);
// //           }
// //           // Free resources
// //           http.end();
// //         } else {
// //           Serial.println("WiFi Disconnected");
// //         }
// //       }