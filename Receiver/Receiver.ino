#include <elapsedMillis.h>
#include <SD.h>
File file;
const int dsize = 8;
elapsedMicros timer0;
const unsigned long INTERVAL  = 25000;
const unsigned long HALF = INTERVAL/2;
long threshold = 0; //To determine 0s and 1s
bool moveToNextBit = false;
const int precision = 1000;
int sensorPin = A0; //pin for ldr
int sensorValue = 0; //Value read in by sensor


byte convertToDecimal(int arr[dsize])
{
  int j = dsize;
  byte n;
  byte t = 1;
  byte rec = 0;
  while(j >= 0)
  {
    if(arr[j] <= threshold)
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

byte getValue()
{

  int d[dsize]; //array to store incoming 16 bit values
  bool startBitReceived = false;
  while(!startBitReceived)
      {
        sensorValue = analogRead(sensorPin); //Takes 100 microseconds
        if(sensorValue < threshold) //If we detected start bit
        {
          timer0 = 0;
          startBitReceived = true;     
        }
      }

      //Just delay here until we get to the middle of the start bit
      while(timer0 < HALF)
      {       
      }
      timer0 = 0;

      //Get bits based on timer0
      for(unsigned long i = 0; i < dsize; i++)
      {
        while(!moveToNextBit)
        {
          if(INTERVAL - timer0 < 10)
          {
            d[i] = analogRead(sensorPin);  
            timer0 = 0;
            moveToNextBit = true;
          }
        }  
        moveToNextBit = false;
      }


      

      byte recVal = convertToDecimal(d);
      
      //Busy wait on stop bit
      timer0 = 0;
      while(timer0 < INTERVAL)
      {}
      return recVal;
}

void setup() {



    Serial.begin(9600);

    //There are 4 basic states
    //Wait - Sampling the data line. When we see a start bit
    //start a timer at 1.5* bit-time and move to state "Data"
    //Data - Wait for the timer then sample the data line to record
    //a bit. Restart the timer to 1.0*bit time. Repeat as long as you
    //haven't received all bits. Move to state "Stop" when all bits
    //have been received
    //Stop - Wait for the timer and sample the data line to check the
    //stop bit. 

    //Calibrate 1 and 0 threshhold
 

    for(int i = 0; i < precision; i++)
    {
      threshold += analogRead(sensorPin);
    }
    threshold = threshold / precision;
    threshold = threshold - 15;
    Serial.print("Threshold: ");
    Serial.println(threshold);

      SD.begin(10);
      byte sizeLowByte = getValue();
      //int nameLength = int(getValue());
      //int fileSize = sizeLowByte | sizeHighByte << 8;
      char * fileName;
      /*fileName = (char*)malloc(sizeof(char)*nameLength);

      for (int i=0;i<nameLength;i++)
      {
         fileName[i]=char(getValue());
      }  */
      //File file = SD.open("test2.txt", FILE_WRITE);
      //free(fileName);
      int fileSize = int(sizeLowByte);
      for(int i=0;i<fileSize;i++);
      {
        file.write(getValue());
      }  
      free(fileName);
      file.close();
      //Move to state stop when full byte received
    
      //Stop state here, dont need to read stop bits

      //to start looking for the start bit again

    }
        

void loop() {
  // put your main code here, to run repeatedly:

}

byte convertToDecimalTest(int arr[dsize])
{
  byte recv = 0;
  for(int i = 0; i < 8; i++)
  {
    if(arr[dsize] <= threshold)
      recv |= true << i;
    else
      recv |= false << i;
  }
  return recv;
}



