int inc=11;

void setup() {
  Serial.begin(9600);
  pinMode(inc,OUTPUT);
  digitalWrite(inc,HIGH);
  delay(5000);
  // put your setup code here, to run once:

}

void loop() {
  if(inc==HIGH){
    inc=LOW;
    Serial.println("incrementando");
    delay(5000); //espera de un segundo
  }
  else{
   inc=HIGH;
   delay(5000); //espera de un segundo 
  }
  // put your main code here, to run repeatedly:

}
