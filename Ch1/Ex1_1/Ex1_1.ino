#define LiIN A1     // Light sensor connected to A1

void setup() {
  Serial.begin(9600);
}

void loop() {
  float l = analogRead(LiIN);
  Serial.print("Light value: ");
  Serial.println(l);
  Serial.println("------------------");
}
