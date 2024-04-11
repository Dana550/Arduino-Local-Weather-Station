#include <SimpleDHT.h>

#include <SimpleDHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// for DHT11,
// VCC: 5V or 3V
// GND: GND
// DATA: 2
int pinDHT11 = 2;
SimpleDHT11 dht11(pinDHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int sensorValue;

int minTemperature = 100; // Initialize with a high value
int maxTemperature = -100; // Initialize with a low value
unsigned long lastResetTime = 0;
const unsigned long resetInterval = 24 * 60 * 60 * 1000; // Reset interval in milliseconds (24 hours)

void setup()
{
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lastResetTime = millis(); // Initialize the last reset time
}

void loop()
{
  unsigned long currentTime = millis();
  // if 24 hours have passed, and reset min/max values
  if (currentTime - lastResetTime >= resetInterval)
  {
    minTemperature = 100; // Reset to a high value
    maxTemperature = -100; // Reset to a low value
    lastResetTime = currentTime; // Update the last reset time
  }

  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");

  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
  {
    Serial.print("Read DHT11 failed, err=");
    Serial.println(err);
    delay(1000);
    return;
  }

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.clear();


  //air q:
  sensorValue = analogRead(0); 
  Serial.print("AirQua=");
  Serial.print(sensorValue, DEC);               // prints the value read
  Serial.println(" PPM");
  int thresholdValue = 500; 

  // dectecting the gaz
  if (sensorValue > thresholdValue) {
    Serial.println("Gas detected!");
    lcd.setCursor(11, 1);
    lcd.print("GAZ!");
    
  } else {
    Serial.println("No gas detected.");
    lcd.setCursor(11, 1);
    lcd.print("<3");
  }
  lcd.setCursor(10, 0);
  lcd.print((int)sensorValue);
  lcd.setCursor(13, 0);
  lcd.print("PPM");

  lcd.setCursor(0, 0);
  lcd.print("t");
  lcd.print((int)temperature);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("h");
  lcd.print((int)humidity);
  lcd.print("%");

  // Update min and max temperatures
  updateMinMaxTemperature(temperature);

  // Display min and max temperatures
  lcd.setCursor(5, 1);
  lcd.print("mi");
  lcd.print(minTemperature);
  lcd.setCursor(5, 0);
  lcd.print("mx");
  lcd.print(maxTemperature);

  delay(1000);
}

void updateMinMaxTemperature(int currentTemperature)
{
  if (currentTemperature < minTemperature)
  {
    minTemperature = currentTemperature;
  }

  if (currentTemperature > maxTemperature)
  {
    maxTemperature = currentTemperature;
  }
}
