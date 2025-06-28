/* 
Project name: Sign Language Translation System
Author: Abhinav Deshpande
Date: 6th April 2024
Time: 18:23:46
*/
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

SoftwareSerial mySerial(3, 2);  // Rx = 2, Tx = 3;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

// To do any testing, make this 1. This will remove any delays and will give you continuous values of sensors
const int testing_phase = 0;

char temp = '0';

// Pin layout
int FLEX_PIN1 = A0;
int sensorIndex1 = 0;
int flexADC1 = 0;
int sensorMin1 = 1023;
int sensorMax1 = 0;

int FLEX_PIN2 = A1;
int flexADC2 = 0;
int sensorMin2 = 1023;
int sensorMax2 = 0;

int FLEX_PIN3 = A2;
int flexADC3 = 0;
int sensorMin3 = 1023;
int sensorMax3 = 0;

int FLEX_PIN4 = A3;
int flexADC4 = 0;
int sensorMin4 = 1023;
int sensorMax4 = 0;

int FLEX_PIN5 = A6;
int flexADC5 = 0;
int sensorMin5 = 1023;
int sensorMax5 = 0;

// Accelerometer ADXL335 pin layout
int xadc = A4;  // pin 2
int yadc = A5;  // pin 1


void setup() {
  Serial.begin(9600);   // Set baud rate for Serial and bluetooth
  mySerial.begin(9600);
  // Establish connection of ADXL345
  if (!accel.begin()) {
    Serial.println("No valid ADXL-345 sensor found");
    while (1);
  }
  int calibration = 0;
  while (millis() < 10000) {
    if(!calibration)
    {
      calibration = 1;
      Serial.println("FLEX SENSOR CALIBRATION PHASE");
    }

    // Read and calibrate all 5 flex sensors
    float flexADC1 = analogRead(FLEX_PIN1);
    if (flexADC1 < sensorMin1) {
      sensorMin1 = flexADC1 + 3;
    }
    if (flexADC1 > sensorMax1) {
      sensorMax1 = flexADC1 - 5;
    }
    float flexADC2 = analogRead(FLEX_PIN2);
    if (flexADC2 < sensorMin2) {
      sensorMin2 = flexADC2 + 3;
    }
    if (flexADC2 > sensorMax2) {
      sensorMax2 = flexADC2 - 5;
    }

    float flexADC3 = analogRead(FLEX_PIN3);

    if (flexADC3 < sensorMin3) {
      sensorMin3 = flexADC3 + 3;
    }
    if (flexADC3 > sensorMax3) {
      sensorMax3 = flexADC3 - 5;
    }
    

    float flexADC4 = analogRead(FLEX_PIN4);
    if (flexADC4 < sensorMin4) {
      sensorMin4 = flexADC4 + 5;
    }
    if (flexADC4 > sensorMax4) {
      sensorMax4 = flexADC4 - 5;
    }


    float flexADC5 = analogRead(FLEX_PIN5);

    if (flexADC5 < sensorMin5) {
      sensorMin5 = flexADC5 + 5;
    }
    if (flexADC5 > sensorMax5) {
      sensorMax5 = flexADC5 - 5;
    }
 
    delay(100);
  }
}
String single_word = "";
void printfun(char cp) {
  if (cp != temp) {
    single_word+=cp;
    if (mySerial.available()) {
      Serial.println(mySerial.readString());  // send from serial to bluetooth
    }
    if (Serial.available()) {
      mySerial.println(cp);  // send from bluetooth to serial
    }
    temp = cp;
    if(!testing_phase){
      delay(2000);
    }
  }
  
}
void Single_word_complete()
{
  // Speaks out an entire word upon completion
  Serial.println("\n");
  Serial.println(single_word);
  Serial.println("\n");
  if(mySerial.available()){
    mySerial.print(single_word);
    Serial.println("Inside the phone");
  }
  delay(1500);
  single_word = "";
}
void loop() {
  // Read and decode from the flex sensors and accelerometer values obtained. This phase is after calibration.
  float flexADC1 = analogRead(FLEX_PIN1);
  flexADC1 = constrain(flexADC1, sensorMin1, sensorMax1);
  float angle1 = map(flexADC1, sensorMin1, sensorMax1, 0, 90);
  AngleCorrectionSensor_1(angle1);
  Serial.print("Angle 1 is ");
  Serial.print(angle1);
  delay(100);

  float flexADC2 = analogRead(FLEX_PIN2);
  flexADC2 = constrain(flexADC2, sensorMin2, sensorMax2);
  float angle2 = map(flexADC2, sensorMin2, sensorMax2, 0, 90);
  Serial.print("  Angle 2 is ");
  Serial.print(angle2);

  float flexADC3 = analogRead(FLEX_PIN3);
  flexADC3 = constrain(flexADC3, sensorMin3, sensorMax3);
  float angle3 = map(flexADC3, sensorMin3, sensorMax3, 0, 90);
  AngleCorrectionSensor_1(angle3);
  Serial.print("  Angle 3 is ");
  Serial.print(angle3);

  float flexADC4 = analogRead(FLEX_PIN4);
  flexADC4 = constrain(flexADC4, sensorMin4, sensorMax4);
  float angle4 = map(flexADC4, sensorMin4, sensorMax4, 0, 90);
  AngleCorrectionSensor_1(angle4);
  Serial.print("  Angle 4 is ");
  Serial.print(angle4);

  float flexADC5 = analogRead(FLEX_PIN5);
  flexADC5 = constrain(flexADC5, sensorMin5, sensorMax5);
  float angle5 = map(flexADC5, sensorMin5, sensorMax5, 0, 90);
  AngleCorrectionSensor_1(angle5);
  Serial.print("  Angle 5 is ");
  Serial.println(angle5);
  delay(100);


  sensors_event_t event;  // instance of accelerometer ... Do not Change!
  accel.getEvent(&event);
  Serial.print("X: ");
  int x_val= abs(event.acceleration.x * 100);
  int y_val= abs(event.acceleration.y * 100);
  int z_val= abs(event.acceleration.z * 100);
  Serial.print(x_val);
  Serial.print("  ");
  Serial.print(event.acceleration.x);
  Serial.print("  ");
  Serial.print("Y: ");
  Serial.print(y_val);
  Serial.print("  ");
  Serial.print(event.acceleration.y);
  Serial.print("  ");
  Serial.print("Z: ");
  Serial.print(z_val);
  Serial.print("  ");
  Serial.print(event.acceleration.z);
  Serial.print("  ");
  Serial.println("m/s^2 ");
  

  // Actual conditions. Decode the alphabet or number from the flex sensor values
  if (((angle1 >= 55) && (angle1 <= 90)) && ((angle2 >= 55) && (angle2 <= 90)) && ((angle3 >= 55) && (angle3 <= 90)) && ((angle4 >= 55) && (angle4 <= 90)) && ((angle5 >= 0) && (angle5 <= 35)) && ((x_val >=0) && x_val <=250) && (y_val>=800 && y_val <=1100) && (z_val>=150 && z_val<=300))
    printfun('A');
  if (((angle1 >= 0) && (angle1 <= 10)) && ((angle2 >= 0) && (angle2 <= 10)) && ((angle3 >= 0) && (angle3 <= 12)) && ((angle4 >= 0) && (angle4 <= 10)) && ((angle5 >= 45) && (angle5 <= 90)) && (z_val>=550 && z_val<=1100) && (y_val>=0 && y_val<=700))
    printfun('B');  
  if (((angle1 >= 18) && (angle1 <= 55)) && ((angle2 >= 25) && (angle2 <= 55)) && ((angle3 >= 25) && (angle3 <= 55)) && ((angle4 >= 25) && (angle4 <= 55)) && ((angle5 >= 25) && (angle5 <= 80)) && (y_val>=700 && y_val<=1000) && (z_val>=0 && z_val<=700))
    printfun('C');
  if (((angle1 >= 0) && (angle1 <= 18)) && ((angle2 >= 45) && (angle2 <= 90)) && ((angle3 >= 45) && (angle3 <= 90)) && ((angle4 >= 45) && (angle4 <= 90)) && ((angle5 >= 18) && (angle5 <= 80)) && (z_val>=550 && z_val<=1100) && (y_val>=0 && y_val<=700)) 
    printfun('D');
  if (((angle1 >= 68) && (angle1 <= 88)) && ((angle2 >= 68) && (angle2 <= 90)) && ((angle3 >= 50) && (angle3 <= 80)) && ((angle4 >= 54) && (angle4 <= 80)) && ((angle5 >= 58) && (angle5 <= 88)))
    printfun('E');
  if (((angle1 >= 20) && (angle1 <= 65)) && ((angle2 >= 0) && (angle2 <= 18)) && ((angle3 >= 0) && (angle3 <= 18)) && ((angle4 >= 0) && (angle4 <= 18)) && ((angle5 >= 30) && (angle5 <= 85)) && (z_val>=550 && z_val<=1100) && (y_val>=0 && y_val<=700))
    printfun('F');
  if (((angle1 >= 0) && (angle1 <= 18)) && ((angle2 >= 40) && (angle2 <= 80)) && ((angle3 >= 40) && (angle3 <= 80)) && ((angle4 >= 40) && (angle4 <= 70)) && ((angle5 >= 0) && (angle5 <= 45)) && (x_val<=950 && x_val>=675))
    printfun('G');
  if (((angle1 >= 0) && (angle1 <= 25)) && ((angle2 >= 0) && (angle2 <= 25)) && ((angle3 >= 40) && (angle3 <= 65)) && ((angle4 >= 40) && (angle4 <= 65)) && ((angle5 >= 35) && (angle5 <= 65)) && (x_val<=950 && x_val>=675))
    printfun('H');  
  if (((angle1 >= 50) && (angle1 <=90 )) && ((angle2 >= 50) && (angle2 <= 90)) && ((angle3 >= 50) && (angle3 <= 90)) && ((angle4 >= 0) && (angle4 <= 25)) && ((angle5 >= 60) && (angle5 <= 85)))// && ((xadc >= 410) && (xadc <= 420)) && ((yadc >= 330) && (yadc <= 370))))
    printfun('I');
  if (((angle1 >= 0) && (angle1 <= 18)) && ((angle2 >= 50) && (angle2 <= 70)) && ((angle3 >= 50) && (angle3 <= 70)) && ((angle4 >= 0) && (angle4 <= 18)) && ((angle5 >= 75) && (angle5 <= 90)))// && (!((xadc >= 410) && (xadc <= 420)) && ((yadc >= 355) && (yadc <= 370))))
    printfun('J');
  if (((angle1 >= 60) && (angle1 <= 75)) && ((angle2 >= 60) && (angle2 <= 85)) && ((angle3 >= 0) && (angle3 <= 10)) && ((angle4 >= 0) && (angle4 <= 15)) && ((angle5 >= 30) && (angle5 <= 55)))// && (((xadc >= 404) && (xadc <= 415)) && ((yadc >= 368) && (yadc <= 380))))
    printfun('K');
  if (((angle1 >= 0) && (angle1 <= 30)) && ((angle2 >= 65) && (angle2 <= 90)) && ((angle3 >= 65) && (angle3 <= 90)) && ((angle4 >= 65) && (angle4 <= 90)) && ((angle5 >= 0) && (angle5 <= 30)) && (y_val>=700 && y_val<=1000) && (z_val>=0 && z_val<=700))
    printfun('L');
  if (((angle1 >= 40) && (angle1 <= 61)) && ((angle2 >= 72) && (angle2 <= 84)) && ((angle3 >= 45) && (angle3 <= 65)) && ((angle4 >= 62) && (angle4 <= 75)) && ((angle5 >= 65) && (angle5 <= 86)))
    printfun('M');
  if (((angle1 >= 54) && (angle1 <= 70)) && ((angle2 >= 50) && (angle2 <= 61)) && ((angle3 >= 48) && (angle3 <= 66)) && ((angle4 >= 60) && (angle4 <= 76)) && ((angle5 >= 50) && (angle5 <= 65)))// && (((xadc >= 400) && (xadc <= 435)) && ((yadc >= 350) && (yadc <= 390))))
    printfun('N');
  if (((angle1 >= 20) && (angle1 <= 55)) && ((angle2 >= 40) && (angle2 <= 65)) && ((angle3 >= 30) && (angle3 <= 65)) && ((angle4 >= 35) && (angle4 <= 65)) && ((angle5 >= 55) && (angle5 <= 90)) && (z_val>=550 && z_val<=1100) && (y_val>=0 && y_val<=700))
    printfun('O');
  if (((angle1 >= 0) && (angle1 <= 25)) && ((angle2 >= 0) && (angle2 <= 25)) && ((angle3 >= 35) && (angle3 <= 85)) && ((angle4 >= 35) && (angle4 <= 90)) && ((angle5 >=0) && (angle5 <= 35))&& (z_val>=550 && z_val<=1100) && (y_val>=0 && y_val<=700))
    printfun('P');
  if (((angle1 >= 0) && (angle1 <= 25)) && ((angle2 >= 35) && (angle2 <= 90)) && ((angle3 >= 35) && (angle3 <= 90)) && ((angle4 >= 35) && (angle4 <= 90)) && ((angle5 >= 0) && (angle5 <= 30)) && (z_val>=550 && z_val<=1100) && (y_val>=0 && y_val<=700))
    printfun('Q');
  if (((angle1 >= 40) && (angle1 <= 72)) && ((angle2 >= 45) && (angle2 <= 90)) && ((angle3 >= 20) && (angle3 <= 45)) && ((angle4 >= 0) && (angle4 <= 10)) && ((angle5 >= 45) && (angle5 <= 80)))// && (((xadc >= 412) && (xadc <= 418)) && ((yadc >= 340) && (yadc <= 360))))
    printfun('R');
  if (((angle1 >= 70) && (angle1 <= 90)) && ((angle2 >= 80) && (angle2 <= 90)) && ((angle3 >= 80) && (angle3 <= 90)) && ((angle4 >= 80) && (angle4 <= 90)) && ((angle5 >= 60) && (angle5 <= 80)))
    printfun('S');
  if (((angle1 >= 40) && (angle1 <= 61)) && ((angle2 >= 72) && (angle2 <= 84)) && ((angle3 >= 45) && (angle3 <= 65)) && ((angle4 >= 44) && (angle4 <= 63)) && ((angle5 >= 65) && (angle5 <= 86)) && (digitalRead(6) == HIGH))
    printfun('T');
  if (((angle1 >= 70) && (angle1 <= 90)) && ((angle2 >= 80) && (angle2 <= 90)) && ((angle3 >= 0) && (angle3 <= 10)) && ((angle4 >= 0) && (angle4 <= 10)) && ((angle5 >= 60) && (angle5 <= 80)))
    printfun('U');
  if (((angle1 >= 0) && (angle1 <= 18)) && ((angle2 >= 0) && (angle2 <= 18)) && ((angle3 >= 45) && (angle3 <= 80)) && ((angle4 >= 25) && (angle4 <= 60)) && ((angle5 >= 35) && (angle5 <= 80)) && (z_val>=550 && z_val<=1100) && (y_val>=0 && y_val<=700))
    printfun('V');
  if (((angle1 >= 0) && (angle1 <= 18)) && ((angle2 >= 0) && (angle2 <= 18)) && ((angle3 >= 0) && (angle3 <= 18)) && ((angle4 >= 45) && (angle4 <= 75)) && ((angle5 >= 35) && (angle5 <= 85)) && (z_val>=550 && z_val<=1100) && (y_val>=0 && y_val<=700))
    printfun('W');
  if (((angle1 >= 45) && (angle1 <= 90)) && ((angle2 >= 35) && (angle2 <= 85)) && ((angle3 >= 35) && (angle3 <= 85)) && ((angle4 >= 35) && (angle4 <= 89)) && ((angle5 >= 45) && (angle5 <= 90)) && (y_val>=750) && 0)
    printfun('X');
  if (((angle1 >= 45) && (angle1 <= 80)) && ((angle2 >= 55) && (angle2 <= 90)) && ((angle3 >= 60) && (angle3 <= 80)) && ((angle4 >= 0) && (angle4 <=35)) && ((angle5 >= 0) && (angle5 <= 35)))
    printfun('Y');
  if (((angle1 >= 0) && (angle1 <= 25)) && ((angle2 >= 35) && (angle2 <= 90)) && ((angle3 >= 35) && (angle3 <= 90)) && ((angle4 >= 0) && (angle4 <= 35)) && ((angle5 >= 20) && (angle5 <= 80)) && (x_val >=750 && y_val<=200) && 0)
    printfun('Z');
  
  // Decode Numbers
  if (((angle1 >= 0) && (angle1 <= 18)) && ((angle2 >= 0) && (angle2 <= 18)) && ((angle3 >= 0) && (angle3 <= 18)) && ((angle4 >= 0) && (angle4 <= 18)) && ((angle5 >= 0) && (angle5 <= 35))&& (y_val >= 850))
    printfun('5');
  if (((angle1 >= 0) && (angle1 <= 18)) && ((angle2 >= 0) && (angle2 <= 18)) && ((angle3 >= 0) && (angle3 <= 18)) && ((angle4 >= 0) && (angle4 <= 18)) && ((angle5 >= 50) && (angle5 <= 90)) && (y_val>=700 && y_val<=1000) && (z_val>=0 && z_val<=700))
    printfun('4');
  if (((angle1 >= 0) && (angle1 <= 18)) && ((angle2 >= 0) && (angle2 <= 18)) && ((angle3 >= 35) && (angle3 <= 85)) && ((angle4 >= 35) && (angle4 <= 65)) && ((angle5 >= 0) && (angle5 <= 30)) && (y_val>=850))
    printfun('3');
  if (((angle1 >= 0) && (angle1 <= 18)) && ((angle2 >= 0) && (angle2 <= 18)) && ((angle3 >= 35) && (angle3 <= 80)) && ((angle4 >= 15) && (angle4 <= 65)) && ((angle5 >= 35) && (angle5 <= 90))&& (y_val >= 850))
    printfun('2');
  if (((angle1 >= 0) && (angle1 <= 18)) && ((angle2 >= 65) && (angle2 <= 90)) && ((angle3 >= 45) && (angle3 <= 90)) && ((angle4 >= 65) && (angle4 <= 90)) && ((angle5 >= 50) && (angle5 <= 90)) && (y_val >= 850))
    printfun('1');
  if (((angle1 >= 0) && (angle1 <= 18)) && ((angle2 >= 0) && (angle2 <= 18)) && ((angle3 >= 0) && (angle3 <= 18)) && ((angle4 >= 50) && (angle4 <= 69)) && ((angle5 >= 50) && (angle5 <= 90)) && (y_val>= 850))
    printfun('6');
  if (((angle1 >= 0) && (angle1 <= 18)) && ((angle2 >= 0) && (angle2 <= 18)) && ((angle3 >= 65) && (angle3 <= 90)) && ((angle4 >= 0) && (angle4 <= 18)) && ((angle5 >= 50) && (angle5 <= 90))&& (y_val >= 850))
    printfun('7');
  if (((angle1 >= 0) && (angle1 <= 18)) && ((angle2 >= 40) && (angle2 <= 85)) && ((angle3 >= 0) && (angle3 <= 18)) && ((angle4 >= 0) && (angle4 <= 18)) && ((angle5 >= 50) && (angle5 <= 90))&& (y_val >= 850))
    printfun('8');
  if (((angle1 >= 45) && (angle1 <= 65)) && ((angle2 >= 0) && (angle2 <= 25)) && ((angle3 >= 0) && (angle3 <= 25)) && ((angle4 >= 0) && (angle4 <= 25)) && ((angle5 >= 50) && (angle5 <= 90))&& (y_val >= 850))
    printfun('9');

  // Upon Word completion. Send the entire word to bluetooth.
  if (((angle1 >= 55) && (angle1 <= 90)) && ((angle2 >= 55) && (angle2 <= 90)) && ((angle3 >= 55) && (angle3 <= 90)) && ((angle4 >= 55) && (angle4 <= 90)) && ((angle5 >= 0) && (angle5 <= 35)) && (x_val >= 750)){
    printfun('#');
    Single_word_complete();
  }
  
}
