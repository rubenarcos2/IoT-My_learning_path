#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); //Change address if this is not applicable

void setup() {
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
  lcd.print("Iniciando...");
}

void loop() {
  if (Serial.available()) {
    delay(100);
    lcd.clear();
    lcd.print("CPU    RAM");
    lcd.setCursor(0, 1);
    while (Serial.available() > 0) {
      lcd.print(Serial.read());
    }
  }
}
