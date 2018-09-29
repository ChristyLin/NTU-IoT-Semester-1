int wheelPin = 3;
int buttonpin = 5;
int val;
int fan_pwm = 0;                // value in terms of 0 to 255
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup()
{
  Serial.begin(9600);
  pinMode(buttonpin, INPUT);
  pinMode(wheelPin, OUTPUT);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  // Serial.println("Start");
}

void loop()
{
   if (stringComplete) {
    fan_pwm = inputString.toInt();
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  val = digitalRead (buttonpin);
   Serial.println(val);
 //Serial.println(fan_pwm);
  
  analogWrite(wheelPin, fan_pwm);
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
   
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == 'o') {
      stringComplete = true;
    }
    else{
      inputString += inChar;
    }
     
  }
}
