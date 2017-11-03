
/*  Pulse Sensor Multi Sensor    by Joel Murphy Winter 2016, Updated Winter 2017

----------------------  Notes ----------------------  ----------------------
This code:
1) Measures the shortest time between two different heartbeats
2) Can be used to determine PPT (Pulse Transit Time)
3) Determines BPM
4) Prints to Arduino Serial Plotter or our Processing Visualizer

Read Me:
https://github.com/WorldFamousElectronics/PulseSensor_Amped_Arduino/blob/master/README.md
 ----------------------       ----------------------  ----------------------
*/

#define PROCESSING_VISUALIZER 1
#define SERIAL_PLOTTER  2


//  Variables
const int numPulseSensors = 2;
const int threshold = 600;
int pulsePin[numPulseSensors];                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin[numPulseSensors];                // pin to blink led at each beat
int connectionMeter;
int longestIBI;
int PPT;


// Volatile Variables, used in the interrupt service routine!
volatile int BPM[numPulseSensors];                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal[numPulseSensors];                // holds the incoming raw data
volatile int IBI[numPulseSensors];             // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse[numPulseSensors];     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS[numPulseSensors];        // becomes true when Arduoino finds a beat.
volatile int Delta[numPulseSensors];
volatile unsigned long lastBeatSampleNumber[numPulseSensors];

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

// SET THE SERIAL OUTPUT TYPE TO YOUR NEEDS
// PROCESSING_VISUALIZER works with Pulse Sensor Processing Visualizer
//      https://github.com/WorldFamousElectronics/PulseSensor_Amped_Processing_Visualizer
// SERIAL_PLOTTER outputs sensor data for viewing with the Arduino Serial Plotter
//      run the Arduino Serial Plotter at 115200 baud: Tools/Serial Plotter or Command+L
static int outputType = PROCESSING_VISUALIZER;

void setup() {

  void setStuph();

  blinkPin[0] = 13; blinkPin[1] = 12;
  pulsePin[0] = 0; pulsePin[1] = 1;
  pinMode(blinkPin[0], OUTPUT); digitalWrite(blinkPin[0], LOW);
  pinMode(blinkPin[1], OUTPUT); digitalWrite(blinkPin[1], LOW);

  Serial.begin(115200);             // we agree to talk fast!


  Serial.print("Pin[0] "); Serial.print(pulsePin[0]); Serial.print("\t");
  Serial.print("Pin[1] "); Serial.println(pulsePin[1]);

  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS
}


void loop() {

//    serialOutput() ;

    for(int i=0; i<numPulseSensors; i++){
      if(QS[i] == true){
        if(BPM[i] < 140){
          connectionMeter = min(Delta[0],Delta[1]);
          longestIBI = max(IBI[0],IBI[1]);
          fadeRate[i] = 0;
//          serialOutputWhenBeatHappens(i);   // A Beat Happened, Output that to serial.
          QS[i] = false;
        }
      }
    }

    ledFadeToBeat();                      // Makes the LED Fade Effect Happen
    delay(20);                            //  take a break
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

  }
}
