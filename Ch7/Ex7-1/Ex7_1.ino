#include <LWiFi.h>
#include <WiFiClient.h>
#include "MCS.h"
#include "DHT.h"

// Assign AP ssid / password here
#define _SSID "your_ssid" // [todo]
#define _KEY  "your_password" // [todo]

// Assign device id / key of your test device
MCSDevice mcs("your_device_id", "your_device_key"); // [todo]

// Assign channel id 
MCSDisplayFloat humidity("your_humidity_channel_id"); // [todo]
MCSDisplayFloat temperature("your_temperature_channel_id"); // [todo]
MCSDisplayInteger light("your_light_channel_id"); // [todo]

#define DHTPIN A0     // what pin we're connected to
#define LiIN A1     // Light connected to

#include <Wire.h>  //i2c
#include <SeeedOLED.h> //i2c


// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
  Serial.begin(9600); 
  Serial.println("DHTxx test!");
  
  // setup Wifi connection
  while(WL_CONNECTED != WiFi.status())
  {
    Serial.print("WiFi.begin(");
    Serial.print(_SSID);
    Serial.print(",");
    Serial.print(_KEY);
    Serial.println(")...");
    WiFi.begin(_SSID, _KEY);
  }
  Serial.println("WiFi connected !!");

  // setup MCS connection
  mcs.addChannel(humidity);
  mcs.addChannel(temperature);
  mcs.addChannel(light);
  while(!mcs.connected())
  {
    Serial.println("MCS.connect()...");
    mcs.connect();
  }
  Serial.println("MCS connected !!");

  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display
  dht.begin();
}

void loop() 
{
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)   float h = dht.readHumidity();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int l = analogRead(LiIN);
  
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h) || isnan(l)) 
  {
    Serial.println("Failed to read from sensor");
  } 
  else 
  {
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.print(" *C\t");
    Serial.print("Light value: "); 
    Serial.println(l); 
    Serial.println("---------------------"); 

    //set MCS humidity channel
    if(!humidity.set(h))
    {
      Serial.print("Failed to update humidity ");
      Serial.println(h);
    }

    //set MCS temperature channel
    if(!temperature.set(t))
    {
      Serial.print("Failed to update temperature ");
      Serial.println(t);
    }

    //set MCS light channel
    if(!light.set(l))
    {
      Serial.print("Failed to update light value ");
      Serial.println(l);
    }
        
    SeeedOled.setTextXY(0,0);          //Set the cursor to Xth Page, Yth Column  
    SeeedOled.putString("Humid:");
    SeeedOled.putNumber(h); //Print the String
    SeeedOled.putString("%");
    SeeedOled.putString("  ");
     
    SeeedOled.setTextXY(1,0); 
    SeeedOled.putString("Temp:");
    SeeedOled.putNumber(t); //Print the String
    SeeedOled.putString("*C");
    SeeedOled.putString(" ");
 
    SeeedOled.setTextXY(2,0);
    SeeedOled.putString("Light:");
    SeeedOled.putNumber(l); //Print the String
    SeeedOled.putString("  ");

  }
  delay(500);
}
