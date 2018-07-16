//Transmitter Code
#define DELAY 6
#define STARTSTOP 10
int i=0;

void setup() {
  int transPin = 13;
  pinMode(transPin, OUTPUT);//light output port
  Serial.begin(9600);
  Serial.println("Transmission Pin: " + transPin);
 
  //Check sum of data

  //Send Data
  int startInt = 32767;
  int stopInt = -32768;
  int testVal = 1001;
  sendMain(startInt);
  sendValue(testVal);
  sendMain(stopInt);
}

void loop() {
  

}

void sendValue(int valToSend)
{
  //Send start bit
  digitalWrite(13, HIGH);
  delay(STARTSTOP);
  int b;
  //Send actual value
  while(valToSend != 0)
  {
      b = valToSend%2;
      valToSend=valToSend/2;
      if(b==1)
      {
        digitalWrite(13, HIGH);
        delay(DELAY);
      }
      else
      {
        digitalWrite(13, LOW);
        delay(DELAY);
      }
  }
  //Send stop bit
  digitalWrite(13, LOW);
  delay(STARTSTOP);
}

//These are not surrounded by start and stop bits because they are
//How the receiver will know that the transmitter is about to start
void sendMain(int val)
{
  int b;
  //Send Data
  while(val != 0)
  {
      b = val%2;
      val=val/2;
      if(b==1)
      {
        digitalWrite(13, HIGH);
        delay(DELAY);
      }
      else
      {
        digitalWrite(13, LOW);
        delay(DELAY);
      }
  }
}
void sendFile (char input)
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
}



