//Transmitter Code
#include <elapsedMillis.h>
#include <SD.h>

const unsigned long INTERVAL = 30000;
int i=0;
elapsedMicros timer0;

File file;
void sendFile (char input[])
{
    //initialize SD card
   SD.begin(10);
   //variables
   byte holder = 0;
   boolean loop1 = true;
   //open file 
   File file = SD.open(input, FILE_READ);
   if(file==false)
      error();
   Serial.print("File Opened");
   //send file size
   sendValue (lowByte(file.size()));
   sendValue (highByte(file.size()));
      //send file name length
   sendValue(byte(sizeof(input)));

   //send file name
   for(int i=0; i<sizeof(input);i++)
      sendValue(byte(input[i]));

   //send data
   while(file.available())
   {
      sendValue(file.read());

   }
    return; 
}

void setup() {

  int transPin = 8;
  int baudrate = 9600; 
  pinMode(transPin, OUTPUT);//light output port
  Serial.begin(baudrate);
  Serial.print("Transmission Pin: ");
  Serial.println(transPin);
 
  //Check sum of data (not yet implemented);
  delay(10000);

  //Send Data
  /*byte k[6] = {65, 3, 250, 114, 76, 3};
  for(int j = 0; j < 6; j++) //255 not 256 to prevent infinte loop
  {
    sendValue(k[j]);
  }*/
 /* for(byte j = 0; j<256; j++)
  {
    sendValue(j);
  }*/
  char fileName [9] = "TEST.txt";
  sendFile(fileName);
}
void error(){}
void loop() {
  

}

void sendValue(byte valToSend)
{
  //Send start bit
  digitalWrite(8, HIGH);
  timer0 = 0;
  byte b;
  //Send actual value
  for(int i = 0; i < 8; i++)
  {
      b = valToSend%2;
      valToSend=valToSend/2;
      //Delay until time to send
      while(timer0 < INTERVAL)
      {
      }
      if(b==1)
      {
        digitalWrite(8, HIGH);
      }
      else
      {
        digitalWrite(8, LOW);
      }
      timer0 = 0;
  }
  while(timer0< INTERVAL)
  {}//Busy wait on last bit
  //Send stop bit
  digitalWrite(8, LOW);
  timer0 = 0;
  while(timer0 < INTERVAL)
  {
  }//Delay on stop bit
  
}
