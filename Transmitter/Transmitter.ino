//Transmitter Code
#define DELAY 6

int i=0;

void setup() {
  int transPin = 13;
  int baudrate = 9600; //Must be same on both transmitter and receiver
  pinMode(transPin, OUTPUT);//light output port
  Serial.begin(baudrate);
  Serial.println("Transmission Pin: " + transPin);
 
  //Check sum of data

  //Send Data
  int testVal = 1001;
  while(true)
  {
    sendValue(testVal);
  }
}

void loop() {
  

}

void sendValue(int valToSend)
{
  //Send start bit
  digitalWrite(13, HIGH);
  delay(DELAY);
  digitalWrite(13, LOW);
  delay(DELAY);
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
  delay(DELAY*2);
}
