#include <elapsedMillis.h>
#include <SD.h>

unsigned long INTERVAL  = 30000;
const unsigned long HALF = INTERVAL/2;
const int dsize = 8;
elapsedMicros timer0;
long threshold = 0; //To determine 0s and 1s
bool moveToNextBit = false;
const int precision = 1000;
int sensorPin = A0; //pin for ldr
int sensorValue = 0; //Value read in by sensor
int CS_PIN = 10; //SD card pin
File file;



void setup() {
    Serial.begin(9600);
    initializeSD();
    char fname[] = "test.txt";
    createFile(fname);

    //Calibrate 1 and 0 threshhold

    for(int i = 0; i < precision; i++)
    {
      threshold += analogRead(sensorPin);
    }
    threshold = threshold / precision;
    threshold = threshold - 15;
    Serial.print("Threshold: ");
    Serial.println(threshold);

    //Get file size
    byte buf[4];
    for(int i = 0; i < 4; i++)
    {
      buf[i] = getValue();
    }
    unsigned long fileSize = buf[0] | (buf[1] << 8) | ((unsigned long )buf[2] << 16) | ((unsigned long)buf[3] << 24);
    Serial.print("File Size: ");
    Serial.println(fileSize);

    //Get file
    for(unsigned long i = 0; i < fileSize; i++)
    {
      file.write(getValue());
    }

    

    
    Serial.println("Done!");
    file.close();
}
        

void loop() {
}

byte getValue()
{
  unsigned long interval = INTERVAL;
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
            interval = interval - 100;
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

void initializeSD()
{
  Serial.println("Initializing SD card...");
  pinMode(CS_PIN, OUTPUT);

  if (SD.begin(10))
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
}

int createFile(char filename[])
{
  file = SD.open(filename, FILE_WRITE);

  if (file)
  {
    Serial.println("File created successfully.");
    return 1;
  } else
  {
    Serial.println("Error while creating file.");
    return 0;
  }
}


