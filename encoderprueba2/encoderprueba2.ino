volatile unsigned tiempoInterrupcionActual=0;

volatile unsigned tiempoInterrupcionAnterior=0;

volatile unsigned deltaTiempoInterrupcion=0;



volatile unsigned muestreoAnterior=0; //variables de medicion de tiempo

volatile unsigned muestreoActual=0;   //controlan el tiempo de muestreo

volatile double deltaMuestreo=0;



int adelanteMotor=12;   //pin habilitacion motor

int atrasMotor=13;      //pin habilitacion motor

int habilitarMotor=8;   //pin habilitacion motor PWM (por medio de este se controla)



int senalPWM=0; //señal PWM  (modulación por ancho de pulsos)

int contador=0;

double frecuencia =0;   //frecuencia de las interrupciones

double rpm =0;        //velocidad llanta en rpm (revoluciones por minuto)



void setup() {

  attachInterrupt(digitalPinToInterrupt(2),Encoder,FALLING);

  pinMode(adelanteMotor,OUTPUT);  //Define como pin de salida

  pinMode(atrasMotor,OUTPUT);     //Define como pin de salida

  Serial.begin(9600);             //Inicio de comunicacion serial, velocidad de comunicacion



}





void Encoder ()

{

  contador++;

  frecuencia = (1*1000)/ (double) deltaTiempoInterrupcion; //frecuencia de las interrupciones

  

  tiempoInterrupcionAnterior= tiempoInterrupcionActual;

  Serial.println(contador);

}





void loop() {

  tiempoInterrupcionActual= millis();

  muestreoActual=millis(); //tiempo actual

  deltaMuestreo = (double) muestreoActual - muestreoAnterior; //Diferencia de tiempo (delta de tiempo de muestro)



  if(deltaMuestreo >=1) //Si la diferencia es mayor o igual a un milisegundo

  {

    deltaTiempoInterrupcion = tiempoInterrupcionActual -tiempoInterrupcionAnterior;



    rpm = frecuencia*(60/1)*(1/10);



    digitalWrite(adelanteMotor,HIGH);     //habilitar giro motor

    digitalWrite(atrasMotor,LOW);         //habilitar giro motor

    digitalWrite(habilitarMotor,senalPWM); //habilitar potencia motor



    Serial.println(rpm);

    delay(1000);

    muestreoAnterior = muestreoActual;

  }



}
