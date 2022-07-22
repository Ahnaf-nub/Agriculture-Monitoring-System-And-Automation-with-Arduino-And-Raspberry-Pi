void soil() {
  lcd.backlight();
  while (1) {
    soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
    soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
    MQ135 mq = MQ135(A0);
    float ppm = mq.getPPM();
    if (soilmoisturepercent <= 0) {
      lcd.setCursor(0, 0);
      lcd.print("Moisture 0%");
      lcd.setCursor(0, 1);
      lcd.print(String(bmp.readTemperature()) + " *C" + String(mq) + " ppm");
      lcd.clear();
    }
    else if (soilmoisturepercent <= 40) {
      lcd.setCursor(0, 0);
      lcd.print("Moisture less than 40%");
      lcd.setCursor(0, 1);
      lcd.print(String(bmp.readTemperature()) + " *C" + String(mq) + " ppm");
      motor();
    }
    else if (soilmoisturepercent >= 100) {
      lcd.setCursor(0, 0);
      lcd.print("Moisture 100%");
      lcd.setCursor(0, 1);
      lcd.print(String(bmp.readTemperature()) + " *C" + String(mq) + " ppm");
      lcd.clear();
      motoroff();
    }
    else if (soilmoisturepercent > 0 && soilmoisturepercent < 100) {
      MQ135 gasSensor = MQ135(A7);
      float mq = gasSensor.getPPM();
      lcd.setCursor(0, 0);
      lcd.print("Moisture" + String(soilmoisturepercent) + "%");
      lcd.setCursor(0, 1);
      lcd.print(String(bmp.readTemperature()) + " *C" + String(mq) + " ppm");
      lcd.clear();
    }
  }
}
void gsm() {
  if (soilmoisturepercent <= 40) {
    sim.println("AT+CMGF=1");
    delay(1000);
    sim.println("AT+CMGS=\"" + number + "\"\r");
    delay(1000);
    String SMS = "Sample SMS is sending";
    sim.println(SMS);
    delay(100);
    sim.println((char)26);// ASCII code of CTRL+Z
    delay(1000);
    motor();
  }
  else if (soilmoisturepercent >= 100) {
    sim.println("AT+CMGF=1");
    delay(1000);
    sim.println("AT+CMGS=\"" + number + "\"\r");
    delay(1000);
    String SMS = "Sample SMS is sending";
    sim.println(SMS);
    delay(100);
    sim.println((char)26);// ASCII code of CTRL+Z
    delay(1000);
    motoroff();
  }
}
