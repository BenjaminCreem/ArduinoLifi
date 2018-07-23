void sendFile (char input)
{
   byte holder = 0;
   boolean loop1 = true; 
   File file = FileSystem.Open(input, FILE_READ);
   if(file==false)
      error();
   sendValue (int(file.size()));
   //while(loop1==true)
   {
      holder= file.Read();
      if(holder!=-1)
          sendValue(encode(holder));
      else
          loop1=false;
   }
    return; 
}

