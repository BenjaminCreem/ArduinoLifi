//Transmitter Code
#include <elapsedMillis.h>

const unsigned long INTERVAL = 40000;
int i=0;
elapsedMicros timer0;

void setup() {
  int transPin = 13;
  int baudrate = 9600; 
  pinMode(transPin, OUTPUT);//light output port
  Serial.begin(baudrate);
  Serial.print("Transmission Pin: ");
  Serial.println(transPin);
 
  //Check sum of data (not yet implemented);
  delay(10000);

  //Send Data
  for(byte j = 0; j < 254; j++) //255 not 256 to prevent infinte loop
  {
    sendValue(j);
  }
}

void loop() {
  

}

void sendValue(byte valToSend)
{
  //Send start bit
  digitalWrite(13, HIGH);
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
        digitalWrite(13, HIGH);
      }
      else
      {
        digitalWrite(13, LOW);
      }
      timer0 = 0;
  }
  while(timer0< INTERVAL)
  {}//Busy wait on last bit
  //Send stop bit
  digitalWrite(13, LOW);
  timer0 = 0;
  while(timer0 < INTERVAL*2)
  {
  }//Delay on stop bit
  
}
