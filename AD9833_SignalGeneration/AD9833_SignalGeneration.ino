const long BAUDRATE  = 115200;  // Baud rate of UART in bps


// Global Variables

// Defining the pins that we will be using on the Signal generator.
const int SG_fsyncPin = 2;
const int SG_CLK = 3;
const int SG_DATA = 4;
const int SG_POWER = 6;

// Hardcode the frequency to 25kHz // a) for testing but also because this is the center frequency of the transducers
const long FREQUENCY = 25000;


/* SG_WriteRegister

This function sends a 16 bit word to the signl generator by toggling specific control lines.

16-bit word: This means the data unit being processed or transmitted is 16 bits in length. 
This is the data we want to send, consisting of 16 bits.
6-bit word (dat) is sent bit by bit to the signal generator by toggling the clock (SG_CLK) and data (SG_DATA) pins
*/

const int wSine     = 0b0000000000000000;
const int wTriangle = 0b0000000000000010;
const int wSquare   = 0b0000000000101000;

void WriteRegister(word dat) {

  digitalWrite(SG_CLK, LOW); // setting clock pin low and then high to have a known state of the clock
  digitalWrite(SG_CLK, HIGH);

  digitalWrite(SG_fsyncPin, LOW); // setting the fsync pin to low, indicating the begin of data transfer 

  for (byte i = 0; i < 16; i++) { // loop through each bit in the 16 bit word 
    if (dat & 0x8000) // checks the most significant bit; if its 1 SG_data high otherwise low 
      digitalWrite(SG_DATA, HIGH); // Send each bit
    else
      digitalWrite(SG_DATA, LOW);
    dat = dat << 1; // Shifting this so the next bit is ready to be sent. 
    digitalWrite(SG_CLK, HIGH); // this line and next are creating a clock impulse => signaling to the generator to read the bit on the data pin
    digitalWrite(SG_CLK, LOW);
  }
  digitalWrite(SG_CLK, HIGH); // signal to end data transfer
  digitalWrite(SG_fsyncPin, HIGH); 
}


// Reset
void reset() {
  delay(100);
  WriteRegister(0x100);
  delay(100);
}

/*
Here we are setting the frequency and the waveform.
*/
// void set_frequency(long frequency) {
//   long fl = frequency * (0x10000000 / 25000000.0);
//   WriteRegister(0x2000); // Setting the wavetype
//   WriteRegister((int)(fl & 0x3FFF) | 0x4000);
//   WriteRegister((int)((fl & 0xFFFC000) >> 14) | 0x4000);
// }

void set_frequency(long frequency, int waveType) {
  long fl = frequency * (0x10000000 / 25000000.0);
  WriteRegister(0x2000 | waveType); // Setting the wavetype
  WriteRegister((int)(fl & 0x3FFF) | 0x4000);
  WriteRegister((int)((fl & 0xFFFC000) >> 14) | 0x4000);
}

// InitSigGen
//-----------------------------------------------------------------------------
void InitSigGen(void) {
  pinMode(SG_POWER, OUTPUT);
  digitalWrite(SG_POWER, HIGH);

  pinMode(SG_DATA, OUTPUT);
  pinMode(SG_CLK, OUTPUT);
  pinMode(SG_fsyncPin, OUTPUT);
  digitalWrite(SG_fsyncPin, HIGH);
  digitalWrite(SG_CLK, HIGH);
  reset();
  set_frequency(FREQUENCY, wSine);
}

// Main functions 
void setup(){
  // Opening serial bud 
  Serial.begin(BAUDRATE);
  Serial.println("Generation started."); 

  // Initializing the Signal 
  InitSigGen(); 
}

void loop(){

  // Once the signal is initiated nothing has to happen anymore. 
}