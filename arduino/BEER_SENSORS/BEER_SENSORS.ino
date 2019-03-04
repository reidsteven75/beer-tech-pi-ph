/*
 # This sample code is used to test the pH meter V1.0.
 # Editor : Steve Reid
 # Ver    : 1.0
 # Product: analog pH meter
 # SKU    : SEN0169
*/
#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.38            //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection


#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it
#define rx 0                                          //define what pin rx is going to be
#define tx 1                                          //define what pin tx is going to be

SoftwareSerial myserial(rx, tx);  

// PH
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;  

// TEMP
String inputstring = "";                              //a string to hold incoming data from the PC

// INIT
void setup(void) {
  pinMode(LED,OUTPUT);  
  Serial.begin(9600);  
//  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
//  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
//  Serial.println("sensor experiment!");    //Test the serial monitor
}

// TEMP
void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  Serial.print("[TEMP]");
  Serial.println(inputstring);
}

// PH
void loop(void) {

  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;

  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+Offset;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    Serial.print("[PH]");
    Serial.println(pHValue,2);
    digitalWrite(LED,digitalRead(LED)^1);
    printTime=millis();
  }

}

// PH
double avergearray(int* arr, int number) {
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}
