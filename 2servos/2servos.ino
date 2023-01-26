/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
Servo servo2;
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int fin=180;
int medio=90;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  servo2.attach(3);
}

void loop() {
  myservo.write(0);              // tell servo to go to position in variable 'pos'
  servo2.write(180); 
  delay(5000);                       // waits 15 ms for the servo to reach the position

  myservo.write(90);              // tell servo to go to position in variable 'pos'  
  servo2.write(90);
  delay(5000);                       // waits 15 ms for the servo to reach the position

  myservo.write(180);              // tell servo to go to position in variable 'pos'
  servo2.write(0);
  delay(5000);                       // waits 15 ms for the servo to reach the position
  
}
