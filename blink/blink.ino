void setup() {
// inicializa el pin digital 2 como salida
pinMode(2, OUTPUT);
}
void loop() {
digitalWrite(2, HIGH); // Enciende el LED
delay(1000); // Espera un segundo
digitalWrite(2, LOW); // Apaga el LED
delay(1000); // Espera un segundo
}
