
//////////
/////////  All Serial Handling Code, 
/////////  It's Changeable with the 'serialVisual' variable
/////////  Set it to 'true' or 'false' when it's declared at start of code.  
/////////

void serialOutput(){   // Decide How To Output Serial. 
   for(int i=0; i<numPulseSensors; i++){
     sendDataToSerial('a'+i, Signal[i]);    
   }
}



void serialOutputWhenBeatHappens(int p){   
   char c;
      c = 'A'+ char(p);
      Serial.print("\t");
        sendDataToSerial(c,BPM[p]);   // send heart rate with a 'B' prefix
      c = 'M'+ char(p);
        Serial.print("\t");
        sendDataToSerial(c,IBI[p]);   // send time between beats with a 'Q' prefix
}



//  Sends Data to Pulse Sensor Processing App, Native Mac App, or Third-party Serial Readers. 
void sendDataToSerial(char symbol, int data ){
    Serial.print(symbol);

    Serial.println(data);                
  }



