//Transmitter Code
#include <elapsedMillis.h>

const unsigned long INTERVAL = 100000;
int i=0;
elapsedMicros timer0;

void setup() {
  int transPin = 13;
  int baudrate = 9600; 
  pinMode(transPin, OUTPUT);//light output port
  Serial.begin(baudrate);
  Serial.print("Transmission Pin: ");
  Serial.println(transPin);
 
  //Check sum of data
  delay(5000);

  //Send Data
  //byte testVal = 24;
 /*(for(byte i =0; i < 256; i++)
 {
  sendValue(i);
 }*/
  sendValue(140);
}

void loop() {
  

}

void sendValue(int valToSend)
{
  //Send start bit
  digitalWrite(13, HIGH);
  timer0 = 0;
  int b;
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
        digitalWrite(13, HIGH);
      }
      else
      {
        digitalWrite(13, LOW);
      }
      timer0 = 0;
  }
  //Send stop bit
  digitalWrite(13, LOW);
  timer0 = 0;
  while(timer0 < INTERVAL*1.5)
  {
  }//Delay on stop bit
  
}
/*void sendFile (char input)
{
   char holder = 0;
   boolean loop1 = true; 
   File file = FileSystem.Open(input, FILE_READ);
   if(file==false)
      error();
   sendValue (int(file.size()));
   while(loop1==true)
   {
      holder= file.Read();
      if(holder!=-1)
          sendValue(encode(holder));
      else
          loop1=false;
   }
    return; 
}*/

void error()
{
    Serial.print("File not found.");
}
 



