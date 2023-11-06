#include <TinyGPS++.h>
#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(26, 25);  // RX, TX
TinyGPSPlus gps;

void gpssetup() {
  gpsSerial.begin(9600);
}

String gpsloop() {
    kordinat = ""; // Reset kordinat
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        // Mendapatkan data latitude dan longitude
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();
        kordinat = String(latitude, 6) + "," + String(longitude, 6);
        Serial.println(kordinat); // Cetak kordinat ke Serial Monitor
      }
    }
  }
}
