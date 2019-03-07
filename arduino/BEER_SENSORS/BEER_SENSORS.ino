#define PhPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.38            //deviation compensate
#define printInterval 2000
#define ArrayLenth  40    //times of collection

#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it
#define rx 0                                          //define what pin rx is going to be
#define tx 1                                          //define what pin tx is going to be

SoftwareSerial myserial(rx, tx);  

// PH
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;  
int inputPh = 0;

// TEMP
String inputStringTemp = "";                              //a string to hold incoming data from the PC
boolean inputStringTempComplete = false;

// INIT
void setup(void) { 
  Serial.begin(9600);  
  inputStringTemp.reserve(10);                            //set aside some bytes for receiving data from the PC
}

// TEMP
void serialEvent() {                                  //if the hardware serial port_0 receives a char
  while (Serial.available()) {
    inputStringTemp = Serial.readStringUntil(13);           //read the string until we see a <CR>
    inputStringTempComplete = true;
  }
 
}

void loop(void) {

  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;

  if(millis() - printTime > printInterval)   {

    // TEMP
    if (inputStringTempComplete) {
      Serial.print("[TEMP]");
      Serial.println(inputStringTemp);
      inputStringTemp = "";
      inputStringTempComplete = false;
    }

    // PH
    inputPh = analogRead(PhPin);
    voltage = inputPh*5.0/1024;
    pHValue = 3.5*voltage+Offset;
    Serial.print("[PH]");
    Serial.println(pHValue,2);

    // update print loop
    printTime=millis();
  }

}
