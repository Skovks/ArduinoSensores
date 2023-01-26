

/*

    Board 3V to sensor VIN
    Board GND to sensor GND
    Board CLK 18 to sensor CLK
    Board MOSI 23 to sensor SDI
    Board MISO 19 to sensor SDO
    Board D5 5 to sensor CS (or any other digital I/O pin)

*/
//Controlador utilizado ESP32

//////////Datos de configuración MAX31865
#include <Adafruit_MAX31865.h>

//libreria wifi y mqtt
#include <WiFi.h>  // Biblioteca para el control de WiFi
#include <PubSubClient.h> //Biblioteca para conexion MQTT

//LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


///Datos de WiFi
const char* ssid = "STOUT";  // Aquí debes poner el nombre de tu red
const char* password = "WelcomeToBratDev";  // Aquí debes poner la contraseña de tu red

//Datos del broker MQTT
const char* mqtt_server = "192.168.0.117"; // Si estas en una red local, coloca la IP asignada, en caso contrario, coloca la IP publica
IPAddress server(192,168,0,117);

// Objetos
WiFiClient espClient; // Este objeto maneja los datos de conexion WiFi
PubSubClient client(espClient); // Este objeto maneja los datos de conexion al broker

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);  //

// Protocolo SPI: CS, DI, DO, CLK
Adafruit_MAX31865 thermo = Adafruit_MAX31865(5, 23, 19, 18);

/////////////Variables Sensor de temperatura
#define RREF      430.0 //definida por max
#define RNOMINAL  100.0 //definida por max
//variables
long timeNow, timeLast; // Variables de control de tiempo no bloqueante
int wait = 1000;  // Indica la espera cada 5 segundos para envío de mensajes MQTT
float T=0; //temperatura


//variables de sensor ultrasonico
int trigPin = 12;    // Trigger
int echoPin = 13;    // Echo
long duration;
float D=0;

void setup() {
  Serial.begin(115200); // Definir baudrate
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Inicializar el LCD
  lcd.begin();
  //Encender la luz de fondo.
  lcd.backlight();
  // Escribimos el Mensaje en el LCD.
  lcd.print("Temp:  Dist:");
  Serial.println();
  Serial.println();
  Serial.print("Conectar a ");
  Serial.println(ssid);

  Serial.println("Prueba MAX y PT100");
  thermo.begin(MAX31865_3WIRE);  // Definir la cantidad de cables 2Wire o 3Wire o 4Wire
 
  WiFi.begin(ssid, password); // Esta es la función que realiz la conexión a WiFi
 
  while (WiFi.status() != WL_CONNECTED) { // Este bucle espera a que se realice la conexión
    delay(500); //dado que es de suma importancia esperar a la conexión, debe usarse espera bloqueante
    Serial.print(".");  // Indicador de progreso
    delay (5);
  }
  
  // Cuando se haya logrado la conexión, el programa avanzará, por lo tanto, puede informarse lo siguiente
  Serial.println();
  Serial.println("WiFi conectado");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());

  // Si se logro la conexión, encender led
  if (WiFi.status () > 0){
  }
  
  delay (1000); // Esta espera es solo una formalidad antes de iniciar la comunicación con el broker

  // Conexión con el broker MQTT
  client.setServer(server, 1883); // Conectarse a la IP del broker en el puerto indicado
  //client.setCallback(callback); // Activar función de CallBack, permite recibir mensajes MQTT y ejecutar funciones a partir de ellos
  delay(1500);  // Esta espera es preventiva, espera a la conexión para no perder información

  timeLast = millis (); // Inicia el control de tiempo
  
}

void loop (){
  T=Leer_temperatura();
  D=distancia();
  
  // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  lcd.setCursor(0, 1);
   // Escribimos el número de segundos trascurridos
  lcd.print(T);
  lcd.print((char)223);
  lcd.print("C  ");
  
  lcd.print(D);
  lcd.print(" cm");

  
  if (!client.connected()) {
    reconnect();  // En caso de que no haya conexión, ejecutar la función de reconexión, definida despues del void setup ()
  }// fin del if (!client.connected())
  client.loop(); // Esta función es muy importante, ejecuta de manera no bloqueante las funciones necesarias para la comunicación con el broker
  
  timeNow = millis(); // Control de tiempo para esperas no bloqueantes
  if (timeNow - timeLast > wait) { // Manda un mensaje por MQTT cada cinco segundos
    timeLast = timeNow; // Actualización de seguimiento de tiempo
    char dataString[8]; // Define una arreglo de caracteres para enviarlos por MQTT, especifica la longitud del mensaje en 8 caracteres
    dtostrf(T, 1, 2, dataString);  // Esta es una función nativa de leguaje AVR que convierte un arreglo de caracteres en una variable String
    Serial.print("Temperatura: "); // Se imprime en monitor solo para poder visualizar que el evento sucede
    Serial.println(dataString);
    client.publish("esp32/temperatura", dataString); // Esta es la función que envía los datos por MQTT, especifica el tema y el valor

    //datos de distancia
    char dataString1[8]; // Define una arreglo de caracteres para enviarlos por MQTT, especifica la longitud del mensaje en 8 caracteres
    dtostrf(D, 1, 2, dataString1);  // Esta es una función nativa de leguaje AVR que convierte un arreglo de caracteres en una variable String
    Serial.print("Distancia: "); // Se imprime en monitor solo para poder visualizar que el evento sucede
    Serial.println(dataString1);
    client.publish("esp32/distancia", dataString1); // Esta es la función que envía los datos por MQTT, especifica el tema y el valor

    
  }// fin del if (timeNow - timeLast > wait)
  delay(100);
}

// Funciones de usuario

/*// Esta función permite tomar acciones en caso de que se reciba un mensaje correspondiente a un tema al cual se hará una suscripción
void callback(char* topic, byte* message, unsigned int length) {

  // Indicar por serial que llegó un mensaje
  Serial.print("Llegó un mensaje en el tema: ");
  Serial.print(topic);

  // Concatenar los mensajes recibidos para conformarlos como una varialbe String
  String messageTemp; // Se declara la variable en la cual se generará el mensaje completo  
  for (int i = 0; i < length; i++) {  // Se imprime y concatena el mensaje
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  // Se comprueba que el mensaje se haya concatenado correctamente
  Serial.println();
  Serial.print ("Mensaje concatenado en una sola variable: ");
  Serial.println (messageTemp);

  // En esta parte puedes agregar las funciones que requieras para actuar segun lo necesites al recibir un mensaje MQTT

  // Ejemplo, en caso de recibir el mensaje true - false, se cambiará el estado del led soldado en la placa.
  // El ESP323CAM está suscrito al tema esp/output
  if (String(topic) == "esp32/output") {  // En caso de recibirse mensaje en el tema esp32/output
    if(messageTemp == "true"){
      Serial.println("Led encendido");
      digitalWrite(flashLedPin, HIGH);
    }// fin del if (String(topic) == "esp32/output")
    else if(messageTemp == "false"){
      Serial.println("Led apagado");
      digitalWrite(flashLedPin, LOW);
    }// fin del else if(messageTemp == "false")
  }// fin del if (String(topic) == "esp32/output")
}// fin del void callback
*/

// Función para reconectarse
void reconnect() {
  // Bucle hasta lograr conexión
  while (!client.connected()) { // Pregunta si hay conexión
    Serial.print("Tratando de contectarse...");
    // Intentar reconexión
    if (client.connect("ESP32Client")) { //Pregunta por el resultado del intento de conexión
      Serial.println("Conectado");
      client.subscribe("esp32/output"); // Esta función realiza la suscripción al tema
    }// fin del  if (client.connect("ESP32CAMClient"))
    else {  //en caso de que la conexión no se logre
      Serial.print("Conexion fallida, Error rc=");
      Serial.print(client.state()); // Muestra el codigo de error
      Serial.println(" Volviendo a intentar en 5 segundos");
      // Espera de 5 segundos bloqueante
      delay(5000);
      Serial.println (client.connected ()); // Muestra estatus de conexión
    }// fin del else
  }// fin del bucle while (!client.connected())
}// fin de void reconnect(

////Lectura de sensor
float Leer_temperatura (){
   uint16_t rtd = thermo.readRTD();
    float ratio = rtd;
  ratio /= 32768;
  return (thermo.temperature(RNOMINAL, RREF));
}

float distancia(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  D = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  return(D);
 // delay(250);
}
