unsigned long splashStartTime;              // Waktu mulai splash screen
const unsigned long splashDuration = 2000;  // Durasi splash screen dalam milidetik (2 detik)

void lcdsetup() {
  // Inisialisasi layar LCD
  lcd.begin();

  // Aktifkan backlight layar LCD
  lcd.backlight();

  // Tampilkan pesan selamat datang
  lcd.setCursor(0, 0);
  lcd.print("Keyless!");
  lcd.setCursor(0, 1);
  lcd.print("Face Recognition");
  splashStartTime = millis();  // Catat waktu mulai splash screen
}

void lcdloop() {
  lcd.clear();
  unsigned long currentTime = millis();
  if (currentTime - splashStartTime >= splashDuration) {
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Posisi GPS");  // Ganti dengan nilai latitude yang sesuai
    lcd.setCursor(0, 1);
    lcdgps(longitude, latitude);
  }
}
