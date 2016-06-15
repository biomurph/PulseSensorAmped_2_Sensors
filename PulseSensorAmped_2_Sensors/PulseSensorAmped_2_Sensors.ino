
/*  Pulse Sensor Multi Sensor    by Joel Murphy Winter 2016

----------------------  Notes ----------------------  ----------------------
This code:
1) Blinks an LED to User's Live Heartbeat   PIN 13
2) Fades an LED to User's Live HeartBeat    PIN 5
3) Determines BPMs``
4) Prints All of the Above to Serial

Read Me:
https://github.com/WorldFamousElectronics/PulseSensor_Amped_Arduino/blob/master/README.md
 ----------------------       ----------------------  ----------------------
*/

//  Variables
const int numPulseSensors = 2;
const int threshold = 600;
int pulsePin[numPulseSensors];                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin[numPulseSensors];                // pin to blink led at each beat
int fadePin[numPulseSensors];                  // pin to do fancy classy fading blink at each beat
int fadeRate[numPulseSensors];                 // used to fade LED on with PWM on fadePin



// Volatile Variables, used in the interrupt service routine!
volatile int BPM[numPulseSensors];                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal[numPulseSensors];                // holds the incoming raw data
volatile int IBI[numPulseSensors];             // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse[numPulseSensors];     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS[numPulseSensors];        // becomes true when Arduoino finds a beat.

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = false;   // Set to 'false' by Default.  Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse

volatile int rate[numPulseSensors][10];                    // array to hold last ten IBI values
volatile unsigned long sampleCounter = 0;          // used to determine pulse timing
volatile unsigned long lastBeatTime[numPulseSensors];           // used to find IBI
volatile int P[numPulseSensors];                      // used to find peak in pulse wave, seeded
volatile int T[numPulseSensors];                     // used to find trough in pulse wave, seeded
volatile int thresh[numPulseSensors];                // used to find instant moment of heart beat, seeded
volatile int amp[numPulseSensors];                   // used to hold amplitude of pulse waveform, seeded
volatile boolean firstBeat[numPulseSensors];        // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat[numPulseSensors];      // used to seed rate array so we startup with reasonable BPM


void setup() {

  void setStuph();
    
        blinkPin[0] = 13; blinkPin[1] = 12;
        fadePin[0] = 5; fadePin[1] = 9;
        pulsePin[0] = 0; pulsePin[1] = 1;
        pinMode(blinkPin[0], OUTPUT); digitalWrite(blinkPin[0], LOW);     
        pinMode(blinkPin[1], OUTPUT); digitalWrite(blinkPin[1], LOW);
        pinMode(fadePin[0], OUTPUT); digitalWrite(fadePin[0], HIGH);   
        pinMode(fadePin[1], OUTPUT); digitalWrite(fadePin[1], HIGH);

  Serial.begin(115200);             // we agree to talk fast!
  
  
  Serial.print("Pin[0] "); Serial.print(pulsePin[0]); Serial.print("\t");
  Serial.print("Pin[1] "); Serial.println(pulsePin[1]);
//  analogReference(EXTERNAL);

  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS
}


void loop() {

    serialOutput() ;

    for(int i=0; i<numPulseSensors; i++){
      if(QS[i] == true){
        if(BPM[i] < 150){
          fadeRate[i] = 0;         // Makes the LED Fade Effect Happen
                                    // Set 'fadeRate' Variable to 255 to fade LED with pulse
          serialOutputWhenBeatHappens(i);   // A Beat Happened, Output that to serial.
          QS[i] = false;
        }
      }
    }
//Serial.print(blinkPin[0]);Serial.print("\t");Serial.println(fadePin[0]);
//  digitalWrite(blinkPin[0], HIGH);
//  digitalWrite(fadePin[0], HIGH);
//  delay(200);
//  digitalWrite(blinkPin[0], LOW);
//  digitalWrite(fadePin[0], LOW);
//  delay(200);

    ledFadeToBeat();                      // Makes the LED Fade Effect Happen
    delay(20);                            //  take a break
}





void ledFadeToBeat() {
  for (int j = 0; j < numPulseSensors; j++) {
    fadeRate[j] += 15;
    fadeRate[j] = constrain(fadeRate[j], 0, 255); //  keep LED fade value from going into negative numbers!
    analogWrite(fadePin[j], fadeRate[j]);         //  fade LED
  }

}



void setStuph() {
  for (int i=0; i<numPulseSensors; i++) {
    lastBeatTime[i] = 0;
    P[i] = 0;
    T[i] = 0;
    amp[i] = 0;
    thresh[i] = threshold;
    amp[i] = 100;                   // used to hold amplitude of pulse waveform, seeded
    firstBeat[i] = true;        // used to seed rate array so we startup with reasonable BPM
    secondBeat[i] = false;
    IBI[i] = 600;             // int that holds the time interval between beats! Must be seeded!
    Pulse[i] = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
    QS[i] = false;
    //    switch(i){
    //      case  0:
    //        blinkPin[i] = 13;
    //        fadePin[i] = 5;
    //        break;
    //      case  1:
    //        blinkPin[i] = 12;
    //        fadePin[i] = 9;
    //        break;
    //      default:
    //        break;
    //    }

    //    pinMode(blinkPin[i],OUTPUT);         // pin that will blink to your heartbeat!
    //    digitalWrite(blinkPin[i],LOW);
    //    pinMode(fadePin[i],OUTPUT);          // pin that will fade to your heartbeat!
    //    analogWrite(fadePin[i],255);

  }
}

