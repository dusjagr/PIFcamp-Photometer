/* ####################################################
simple code to measure the LDR of the DIY photometer
display it on the tm1637 4-digit display module
send it via serial to the computer
or use the PWM output as a CV signal for a synth

by dusjagr, made during PIFcamp Trenta 2015

this code is part of the Public Domain
#################################################### */

#include "TM1637.h"
#define CLK 2            //pins definitions for TM1637 and can be changed to other ports       
#define DIO 3
TM1637 tm1637(CLK,DIO);

int8_t digits[4]= {0,0,0,0};
int8_t nulls[4]= {0,0,0,0};
uint8_t digit = 0;

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to
const int ledPin = 12; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

double sum=0;
int count=0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  
  //initialize the display
  tm1637.init();         
  tm1637.set(7);         //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  
  //set pins
  pinMode (ledPin, OUTPUT);
  delay(1000);
}

void loop() {
  
  // turn on the LED by setting pin to GND
  digitalWrite (ledPin,LOW);
  // wait a bit
  delay(20);
  
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  delay(5);
  
  // turn off the LED by setting pin to HIGH
  digitalWrite (ledPin,HIGH);
  // wait a bit
  delay(20);
  
  // add the measured value to the one before and count
  sum = sum + sensorValue;
  count = count + 1;
  
  // after a defined number of measurements average and display them
  if (count > 10) {
  
    unsigned long averaged =  sum / count;
    
    // map it to the range of AnalogVoltage Reference:
    outputValue = map(averaged, 0, 1023, 0, 5000);
    
    // clear the 4-Digit display
    tm1637.clearDisplay(); 
    
    // display the individual digits on the display
    for (int i = 3; i >= 0; i--) {
      digit = averaged % 10;
      tm1637.display(i,digit);
      averaged /= 10;
    }
  
    // print the results to the serial monitor:
    Serial.print("sensor = " );
    Serial.println(sensorValue);
    
    // set back the counter and sum and wait a bit
    sum = 0;
    count = 0;
    delay(100);
  }
  
}
