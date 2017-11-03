
//////////
/////////  All Serial Handling Code,
/////////  It's Changeable with the 'outputType' variable
/////////  It's declared at start of code.
/////////

void serialOutput(){   // Decide How To Output Serial.
  switch(outputType){
    case PROCESSING_VISUALIZER:
      for(int i=0; i<numPulseSensors; i++){
        sendDataToSerial('a'+i, Signal[i]);
      }
      break;
    case SERIAL_PLOTTER:  // open the Arduino Serial Plotter to visualize these data
      for(int i=0; i<numPulseSensors; i++){
//      	Serial.print(BPM[i]);   //
//      	Serial.print(",");      //  UNCOMMENT THESE LINES TO SEE THE BPM AND IBI DATA
//      	Serial.print(IBI[i]);   //
//      	Serial.print(",");      //
      	Serial.print(Signal[i]);
	      if(i!=numPulseSensors-1){ Serial.print(","); }
      }
      Serial.println();
      break;
    default:
      break;
  }

}



void serialOutputWhenBeatHappens(int p){
	char c;
  switch(outputType){
      case PROCESSING_VISUALIZER:    //
	      c = 'A'+ char(p);
        sendDataToSerial(c,BPM[p]);   // send heart rate with a 'B' prefix
      	c = 'M'+ char(p);
        sendDataToSerial(c,IBI[p]);   // send time between beats with a 'Q' prefix
      break;
    default:
      break;
  }
}


//  Sends Data to Pulse Sensor Processing App, Native Mac App, or Third-party Serial Readers.
void sendDataToSerial(char symbol, int data ){
    Serial.print(symbol);
    Serial.println(data);
  }

/************************************************************************/

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
        c = 'm'+ char(p);
          Serial.print("\t\t");
          sendDataToSerial(c,Delta[p]);

  }



  //  Sends Data to Pulse Sensor Processing App, Native Mac App, or Third-party Serial Readers.
  void sendDataToSerial(char symbol, int data ){
      Serial.print(symbol);

      Serial.println(data);
    }
