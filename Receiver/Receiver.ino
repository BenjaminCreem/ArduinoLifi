#include <elapsedMillis.h>

const int dsize = 16;

elapsedMillis timer0;
#define INTERVAL 10
#define MIDSTART INTERVAL/2

void setup() {
    int sensorPin = A0; //pin for ldr
    int sensorValue = 0; //Value read in by sensor
    int d[dsize]; //array to store incoming 16 bit values
    bool startBitReceived = false;
    Serial.begin(9600);

    //There are 4 basic states
    //Wait - Sampling the data line. When we see a start bit
    //start a timer at 1.5* bit-time and move to state "Data"
    //Data - Wait for the timer then sample the data line to record
    //a bit. Restart the timer to 1.0*bit time. Repeat as long as you
    //haven't received all bits. Move to state "Stop" when all bits
    //have been received
    //Stop - Wait for the timer and sample the data line to check the
    //stop bit. Move to state "Error" if it is wrong, add a byte to
    //the receive buffer if it is not. Go back to state "Wait"
    //Error - Complain. Wait for deus ex machina to go back
    //to state "wait"

    //while(true)
    //{
      //Wait state here
      while(!startBitReceived)
      {
        sensorValue = analogRead(sensorPin);
        Serial.println(sensorValue);
        if(sensorValue < 1008) //If we detected start bit
        {
          timer0 = 0;
          startBitReceived = true;     
        }
      }

      //First bit is offset by 1 + half. Rest are offset by 1
      while(timer0 <= INTERVAL + MIDSTART)
      {
        if(timer0 >= INTERVAL + MIDSTART)
          {
            d[0] = analogRead(sensorPin);
            timer0 = 0;
          }  
      }

      //Data state here
      //For each bit beside the first
      for(int i = 1; i < dsize; i++)
      { 
        //Dont read until timer
        while(timer0 <= INTERVAL)
        {
          //Is it time to read
          if(timer0 >= INTERVAL)
          {
            d[i] = analogRead(sensorPin);
          }    
        }
        timer0 = 0; //Set timer to 0 for each bit
      }

      
      Serial.print("RECEIVED VALUE: ");
      Serial.println(convertToDecimal(d));
      //Move to state stop when all bits received (16)
    
      //Stop state here
      analogRead(sensorPin);
      //to start looking for the start bit again
      startBitReceived = false;

      for(int i = 0; i < dsize; i++)
      {
        Serial.print("d at ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(d[i]);  
      }

    //}
   
        
}

void loop() {
  // put your main code here, to run repeatedly:

}

int convertToDecimal(int arr[dsize])
{
  int j = dsize;
  int n;
  int t = 1;
  int rec = 0;
  while(j >= 0)
  {
    if(arr[j] <= 75)
    {
      n = j;
      while(n != 0)
      {
        t=t*2;
        n--; 
      }
      rec = rec + t;
      t = 1;
    }  
    j--;
  }
  return rec;
}



