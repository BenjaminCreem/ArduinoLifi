int startInt = 32767;
int stopInt = -32768;
int del= 2;
const int dsize = 16;


void setup() {
  int sensorPin = A0; //pin for ldr
  int sensorValue = 0; //store value incoming from sensor

  int d[dsize]; //store incoming 16 bit values
  int i;
  int k;
  int temp = 1;
  int recvVal = 0;
  bool isStartVal = false;
  bool recvStopInt = false;
  Serial.begin(9600);
  Serial.println("Sensor Pin" + sensorPin);

  //Constantly look for the main start value to come through.
  //That value is known by both arduinos to be 32767
  //If we receive that value, then we look for the start bit. 
  //The delay between the start bit and the bits in the value we want to read is 10
  //The delay between each bit in the value is 6. 
  while(true)
  {
    while(!isStartVal)
    {
        delay(del); 
        //Receive bit
        sensorValue = analogRead(sensorPin);
        //Move current bits up 1 index
        //Set new bit to 0 index
        d[0] = sensorValue;
        //Test if new array corresponds to main start code
        recvVal = convertToDecimal(d);
        Serial.println("Received Start Val: " + recvVal);
        if(recvVal == startInt)
        {
          isStartVal = true;
        }
    }

    //Primary start value has been received. Read start bit, value,
    //and stop bit all while looking for the primary stop value

    while(!recvStopInt)
    {

      //Read start bit
      sensorValue = analogRead(sensorPin);
      delay(del); 

      //Read in actual value
      for(i = 0; i < dsize; i++)
      {
          sensorValue = analogRead(sensorPin);
          d[i] = sensorValue;
          delay(del);
      }
    
      //Convert to decimal
      while(i >= 0)
      {
        if(d[i] >= 75)
        {
          k = i;
          while(k != 0)
          {
            temp=temp*2;
            k--; 
          }
          recvVal = recvVal + temp;
          temp = 1;
        }  
        i--;
      }

      //If the value we read in is the final stop value, we stop receiving more values
      if(recvVal == stopInt)
      {
          recvStopInt = true;
      }
      else //Otherwise we print the value 
      {
        Serial.println(recvVal);  
      }

      //Read stop bit
      sensorValue = analogRead(sensorPin);
      delay(del); 
    
    }
    
    
    
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
