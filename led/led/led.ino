void setup() {
  // DONT USE PIN 34 IT SHIT
  pinMode(47, OUTPUT);  
  pinMode(49, OUTPUT);
}
void loop() {
  digitalWrite(47, HIGH);   
  digitalWrite(49, HIGH);
  delay(1000);              
  digitalWrite(47, LOW);      
  digitalWrite(49, LOW); 
  delay(1000);             
}
