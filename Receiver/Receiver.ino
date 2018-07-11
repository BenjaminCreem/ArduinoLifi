int del= 10; //Needs to be same for both receiver and transmitter
const int dsize = 16;


void setup() {
    int sensorPin = A0; //pin for ldr
    int sensorValue; //Value read in by sensor
    int d[dsize]; //array to store incoming 16 bit values
    bool startBitReceived = false;
    bool startBitFallOff = false;
    //bool stopBitReceived = false;
    int baudrate = 9600; //Must be same on both receiver and transmitter
    Serial.begin(baudrate);

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

    while(true)
    {
      //Wait state here
      while(!startBitReceived && !startBitFallOff)
      {
        sensorValue = analogRead(sensorPin);
        Serial.println(sensorValue); //For testing
        if(sensorValue >= 300) //If we detected start bit
        {
          startBitReceived = true;  
        }
        else if(startBitReceived) //Received start bit, but its fallen off now
        {
          startBitFallOff = true;
        }
      }

      //Data state here
      //Look at data stream to Record a Bit
      for(int i = 0; i < dsize; i++)
      { 
        delay(del); 
        d[i] = analogRead(sensorPin);     
      }
      Serial.println(convertToDecimal(d));
      //Move to state stop when all bits received (16)
    
      //Stop state here
      analogRead(sensorPin);
      delay(del); //Transmitter delays * 2, so receiver will have time
      //to start looking for the start bit again
      startBitReceived = false;
      startBitFallOff = false;

    }
   
        
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
    if(arr[j] >= 75)
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
