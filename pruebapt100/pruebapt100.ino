//Controlador utilizado ESP32

//////////Datos de configuración MAX31865
#include <Adafruit_MAX31865.h>
// Protocolo SPI: CS, DI, DO, CLK
Adafruit_MAX31865 thermo = Adafruit_MAX31865(5, 23, 19, 18);

/////////////Variables Sensor de temperatura
#define RREF      430.0 //definida por max
#define RNOMINAL  100.0 //definida por max

void setup() {
  Serial.begin(115200); // Definir baudrate
  Serial.println("Prueba MAX y PT100");
  thermo.begin(MAX31865_3WIRE);  // Definir la cantidad de cables 2Wire o 3Wire o 4Wire
}

void loop (){
  Serial.print("Temperatura");
  Serial.println(Leer_temperatura());
  delay(1000);
}

////Lectura de sensor
float Leer_temperatura (){
   uint16_t rtd = thermo.readRTD();
    float ratio = rtd;
  ratio /= 32768;
  return (thermo.temperature(RNOMINAL, RREF));
}
