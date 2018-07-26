#include <SD.h>
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
   while(loop1==true)
   {
      holder= file.read();
      if(holder!=-1)
          sendValue(holder);
      else
          loop1=false;
   }
    return; 
}

void error()
{


}
//to get rid of error messages till this is merged back to transmitter
void sendValue(byte aa)
{
}
