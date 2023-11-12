void sendgps() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousGpsMillis >= gpsInterval) {
    previousGpsMillis = currentMillis;


    bool gpsDataAvailable = false;

    while (gpsSerial.available() > 0) {
      if (gps.encode(gpsSerial.read())) {
        if (gps.location.isValid()) {
          latitude = gps.location.lat();
          longitude = gps.location.lng();
          gpsDataAvailable = true;
        }
      }
    }
    if (gpsDataAvailable) {
      sendDataToServer(latitude, longitude);
    }
    lcdgps(latitude, longitude);
  }
}

void sendDataToServer(float latitude, float longitude) {
  HTTPClient http;
  String serverPath = String(serverName) + "?gps=" + String(latitude, 6) + "," + String(longitude, 6);

  http.begin(serverPath);

  int httpCode = http.GET();

  // if (httpCode == HTTP_CODE_OK) {
  //   // Serial.println("Data berhasil dikirim ke server.");
  // } else {
  //   // Serial.println("Gagal mengirim data ke server.");
  // }

  http.end();
}

void lcdgps(float latitude, float longitude) {

  String combinedString = String(longitude, 4) + "," + String(latitude, 4);
  // lcd.print(combinedString);  // Ganti dengan nilai longitude yang sesuai
}
