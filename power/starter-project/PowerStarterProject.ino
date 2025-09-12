void setup() {
  // put your setup code here, to run once:
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT);

  pinMode(15, INPUT);
  pinMode(15, INPUT_PULLUP); 
  pinMode(A0, INPUT); 
}

void loop() {
  // POTENTIOMETER
  int potVal = analogRead(A0); 
  int pwmVal = map(potVal, 0, 1023, 0, 255);  // Scale to 0–255
  analogWrite(10, pwmVal);  // PWM output

  // SWITCH 
  int dir = digitalRead(15);
  if(dir == 0){
    digitalWrite(9, 1); 
    digitalWrite(8, 0);  
  }
  else{
    digitalWrite(9, 0); 
    digitalWrite(8, 1);  
  }
  
}
