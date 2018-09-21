//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the r// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN A0     // what pin we're connected to
#define LiIN A1     // Light connected to

#include <Wire.h>  //i2c
#include <SeeedOLED.h> //i2c


// Uncommentight) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
    Serial.begin(9600); 
    Serial.println("DHTxx test!");

    Wire.begin();
    SeeedOled.init();  //initialze SEEED OLED display

    dht.begin();
}

void loop() 
{
    SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
    SeeedOled.setPageMode();           //Set addressing mode to Page Mode
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float l = analogRead(LiIN);

    // check if returns are valid, if they are NaN (not a number) then something went wrong!
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
        Serial.print(" %\t");
        Serial.print("Temperature: "); 
        Serial.print(t);
        Serial.println(" *C");
        Serial.print("Light value: "); 
        Serial.println(l); 
        Serial.println("---------------------"); 
        
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


