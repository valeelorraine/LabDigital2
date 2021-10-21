void setup() {
  // put your setup code here, to run once:
  pinMode(12,OUTPUT);
}

void loop() {
  //Tone(pin, frecuency, duration)
  //notone(pin) -- stops tone

  tone(12,1);
  tone(12,400);   // D
  delay(15);
  tone(12, 600);
  delay(10);
  noTone(12);
  tone(12,1500);
  delay(2);
  noTone(12);
  delay(3000);
  
  
  


}
