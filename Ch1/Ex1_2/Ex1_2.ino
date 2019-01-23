#include <DHT.h>        //teperature&humidity
#include <Wire.h>       //i2c
#include <SeeedOLED.h>  //OLED

#define DHTPIN A0       // A0 for temperature & humidity
#define LiIN A1         // A1 for light sensor

#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); 
  Wire.begin();
  dht.begin();
  SeeedOled.init();  //initialze SEEED OLED display

  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode

  SeeedOled.setTextXY(0,0);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("Humid:");
  SeeedOled.setTextXY(0,12); 
  SeeedOled.putString("%");

  SeeedOled.setTextXY(1,0); 
  SeeedOled.putString("Tempe:");
  SeeedOled.setTextXY(1,12);
  SeeedOled.putString("*C");

  SeeedOled.setTextXY(2,0);
  SeeedOled.putString("Light:");
  
  Serial.println("DHTxx test!");
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float l = analogRead(LiIN);

  if (isnan(t) || isnan(h)) 
  {
    Serial.println("Failed to read from DHT");
    Serial.print("Light value: "); 
    Serial.println(l); 
  } 
  else 
  {
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.println(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");
    Serial.print("Light value: "); 
    Serial.println(l); 
    Serial.println("---------------------"); 
      
    SeeedOled.setTextXY(0,8); 
    SeeedOled.putNumber(h);
    SeeedOled.putString(" ");
       
    SeeedOled.setTextXY(1,8); 
    SeeedOled.putNumber(t);
    SeeedOled.putString(" ");
    
    SeeedOled.setTextXY(2,7);
    SeeedOled.putNumber(l);
    SeeedOled.putString("  ");
    }
    delay(500);
}
