int pot_sp=1;
float sp;
int PWM_salida=6;
float pv;

int pinA=2;
volatile byte n=0;
unsigned long previousMillis=0;
long interval=1000;


void setup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(PWM_salida,OUTPUT);
  Serial.begin(9600);
  digitalWrite(pinA,HIGH);
  attachInterrupt(0,counter,FALLING);
  
}

void loop() {
  unsigned long currentMillis=millis();
  if((currentMillis-previousMillis)>=interval);{
    detachInterrupt(0);  
    previousMillis=currentMillis;
    pv=n;
    n=0;
    attachInterrupt(0, counter, FALLING); // Reiniciamos la interrupción
  }
  sp=analogRead(pot_sp)*(100.0/1023.0);
  analogWrite(PWM_salida,sp*(255.0)/100.0));
  Serial.println("sp: ", sp);
  Serial.println("pv: ",pv);

}


void counter(){
  n++;
}
