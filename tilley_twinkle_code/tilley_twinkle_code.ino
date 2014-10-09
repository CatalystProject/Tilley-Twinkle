//libraries
#include <TimerOne.h>

/*
This code is intended to control the "Tilley Twinkle" hardware.
It cycles through an array of 144 "datapoints" that represent the 
turbine output for the 'Tilley' wind turbine located on Tiree. The
data iterated through is the data for the 23rd Dec 2013. 

Depending on ourput (n/910), the connected bi-colour LEDs (pwm 3 
& 5) will show a colour between red and green. Currently, the lights
are updated each second, and once 144 seconds has passed, the array
starts again at position 0.

Due to power constraints and number of LEDS powered from Arduino,
PWM reimplemented through digitaloutput to prevent duty cycles aligning.

Author - Catalyst Project (Peter Newman)
*/
//pins for colours
const int GREEN_PIN = 5;
const int RED_PIN = 3;

//144 data points of dec 23th last year...
float dataPoints[] = {552,552,552,552,552,552,386,386,386,386,386,386,265,265,265,265,265,265,265,265,265,265,265,265,243,243,243,243,243,243,243,243,243,243,243,243,114,114,114,114,114,114,44,44,44,44,44,44,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,53,53,53,53,53,55,55,55,67,67,67,67,67,67,67,67,67,67,114,114,114,114,114,114,157,157,157,157,157,157,243,243,243,243,243,243,287,287,287,287,287,287,311,311,311,311,311,311,360,360,360,360,360,360,335,335,335,335,335,335,287,287,287,287,287,287,265,265,265,265,265,265};
//current position in dataPoints array
int dpPos = 0;
float currentKW = -1;

//percent values of LED colours - used to determine overall LED colour
int greenPercent = 0;
int redPercent = 0;

void setup(){
  
  //start communication over connected USB connection (if attached to computer)
  Serial.begin(9600);
  
  //colour pins (green and red) and their pin mode (OUTPUT)
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  
  //setup pin 13 - for heart beat (and onboard LED)
  pinMode(13, OUTPUT);
  
  //initialise timer to 1 second (max value with current prescaler - 1000000 microseconds)
  Timer1.initialize(1000000);
  
  //attach the callback function - call function timerTick when time has elapsed 
  Timer1.attachInterrupt(timerTick);
}

void timerTick()
{ 
  //onboard led used as timer status - invert led output - if it is off, turn it on 
  digitalWrite(13, digitalRead(13) ^ 1);
  
  //get the next value from array at position dpPos (starts at 0)
  float currentKW = dataPoints[dpPos];
  
  //increment dpPos (array position)
 
  dpPos++;
  //check whether dpPos is larger than size of array ( array size in bytes / size of float (4)) - if larger, reset to 0
  if ( dpPos > sizeof(dataPoints)/sizeof(float) )
    dpPos = 0;  
  
  //get percentage contribution of each colour.    
  float green = floor(currentKW / 950 * 100);
  float red = 100 - green;
  
  greenPercent = green;
  redPercent = red;
}

//manual implementation of PWM ( If we had less than 7 (bi-colour) LEDS, we could have just done ( analogWrite(GREEN_PIN, greenVal) )
void bitBangGreenRed( int greenVal, int redVal ){
  //if green value is more than 0, do this...
  if ( greenVal > 0 ){
    digitalWrite(GREEN_PIN, HIGH);
    //keep green LED on for greenVal * 10 microseconds
    delayMicroseconds(greenVal * 10); 
    digitalWrite(GREEN_PIN, LOW);
  }
  
  //if red value is more than 0, do this...
  if ( redVal > 0 ){
    digitalWrite(RED_PIN, HIGH);
    //keep red LED on for redVal * 10 microseconds
    delayMicroseconds(redVal * 10); 
    digitalWrite(RED_PIN, LOW);
  }
}

void loop(){
  //whenever possible, illuminate lights with colour values set by last interrupt (using function just above). 
  bitBangGreenRed(greenPercent, redPercent );
}


