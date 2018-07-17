//Transmitter Code
#include <elapsedMillis.h>

const unsigned long INTERVAL = 50000;
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
  byte sendVal = 24;

  //Send Data
  //byte testVal = 0;
  for(int j = 0; j < 256; j++)
  {
    sendValue(j);
  }
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
  while(timer0 < INTERVAL*2)
  {
  }//Delay on stop bit
  
}
