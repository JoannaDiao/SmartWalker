void setup() {
  // DONT USE PIN 34 IT SHIT
  pinMode(47, OUTPUT); //right 
  pinMode(49, OUTPUT); //left
}
void loop() {
  //digitalWrite(47, HIGH);   
  digitalWrite(49, HIGH);
  delay(150);              
  //digitalWrite(47, LOW);      
  digitalWrite(49, LOW); 
  delay(1000);             
}
