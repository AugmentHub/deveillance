#include <avr/sleep.h>
#include <avr/power.h>

#define N_PORTS 1 // number of ports being used. 
#define N_DIVS 24 // number of steps in waveform

#define WAIT_LOT(a) __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop")
#define WAIT_MID(a) __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop")
#define WAIT_LIT(a) __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop")

// Defining where to output our wave

#define OUTPUT_WAVE(pointer, d)  PORTC = pointer[d*N_PORTS + 0]

//half a second
#define STEP_SIZE 1
#define BUTTON_SENS 2500 
#define N_FRAMES 24 // number of steps in waveform

static byte frame = 0;
// SETTING THIS FOR THE ANIMATION (WAVEFORM)

static byte animation[N_FRAMES][N_DIVS] = 
{{0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa},
{0x9,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x6,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa},
{0x9,0x9,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x6,0x6,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa},
{0x9,0x9,0x9,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x6,0x6,0x6,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa},
{0x9,0x9,0x9,0x9,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x6,0x6,0x6,0x6,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa},
{0x9,0x9,0x9,0x9,0x9,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x6,0x6,0x6,0x6,0x6,0xa,0xa,0xa,0xa,0xa,0xa,0xa},
{0x9,0x9,0x9,0x9,0x9,0x9,0x5,0x5,0x5,0x5,0x5,0x5,0x6,0x6,0x6,0x6,0x6,0x6,0xa,0xa,0xa,0xa,0xa,0xa},
{0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x5,0x5,0x5,0x5,0x5,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0xa,0xa,0xa,0xa,0xa},
{0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x5,0x5,0x5,0x5,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0xa,0xa,0xa,0xa},
{0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x5,0x5,0x5,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0xa,0xa,0xa},
{0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x5,0x5,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0xa,0xa},
{0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x5,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0xa},
{0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x5,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0xa,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x5,0x5,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0xa,0xa,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x5,0x5,0x5,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0xa,0xa,0xa,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x5,0x5,0x5,0x5,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0xa,0xa,0xa,0xa,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x5,0x5,0x5,0x5,0x5,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0xa,0xa,0xa,0xa,0xa,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x5,0x5,0x5,0x5,0x5,0x5,0x9,0x9,0x9,0x9,0x9,0x9,0xa,0xa,0xa,0xa,0xa,0xa,0x6,0x6,0x6,0x6,0x6,0x6},
{0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x9,0x9,0x9,0x9,0x9,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0x6,0x6,0x6,0x6,0x6},
{0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x9,0x9,0x9,0x9,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0x6,0x6,0x6,0x6},
{0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x9,0x9,0x9,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0x6,0x6,0x6},
{0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x9,0x9,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0x6,0x6},
{0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x9,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0x6}};


void setup()
{
   DDRC = 0b00001111; //A0 to A3 are the signal outputs
   PORTC = 0b00000000; // Port C initialized to 0
   
   pinMode(10, OUTPUT); //pin 10 (B2) generates a 25 kHz signal to sync 
   pinMode(11, INPUT_PULLUP); //pin 11 (B3) is the sync in
   //please connect pin 10 to pin 11

   for (int i = 2; i < 8; ++i){  //pin 2 to 7 (D2 to D7) are inputs for the buttons
    pinMode(i, INPUT_PULLUP); // setting these pins to be pull-up -> Sets pin to a high voltage level. ACTIVE LOW
   }

  // generate a sync signal of 25khz in pin 10
  noInterrupts();           // we dont' want the code to be interrupted so we are initiating no interrupts here.
  
  /* 
  This register controls the mode of operation and output behavrio fo the timer
  bit (WGM10) | bit (WGM11) => sets the timer to fast PWM mode, meaning that timer countrs from 0 to max and then resets to 0 creating a high-frequency PWM signal
  bit (COM1B1) => configures behavior of the Output Compare pin; 
  */
  TCCR1A = bit (WGM10) | bit (WGM11) | bit (COM1B1); // fast PWM, clear OC1B on compare

  /*
  This register is the timer / counter control register B; it controls the mode of operation and clock source.
  bit (WGM12) | bit (WGM13) continues setting te timer to Fast PWM mode
  bit (CS10) is setting the clock for the timer with no prescaler, meaning the timer runs at full speed.
  */
  TCCR1B = bit (WGM12) | bit (WGM13) | bit (CS10);   // fast PWM, no prescaler
// WHEN USING 40khZ Ultrasonic transducers use this

  // OCR1A =  (F_CPU / 40000L) - 1;
  // OCR1B = (F_CPU / 40000L) / 2;

  /* WHEN USING 25KHZ ULTRASONIC TRANSDUCERS USE THIS
  OCR1A = Output Compare Register A for Timer1
  This register holds the value that the timer countr up to before resetting to 0!
  F_CPU / 40000L - 1 calculate the value for OCR1A
  FCPU is the clock frequency of the Arduino (typically this is 16MHz)
  for 40khz we get OCR1a = 399, so the counter will count up from 0 to 399 creating the PWM signal with a frequency of 40Khz.
  */
  OCR1A =  (F_CPU / 25000L) - 1;

  /*
  OCR1B is the Output Compare Register B for Timer1, this register holds the vlaue at whihc the OC1B pin is cleared. 
  For 50% duty cycle, OCR1B is set to half of OCR1A
  This means that OC1B pin will be set HIGH when the timer starts counting and will be cleared (LOW) when the timer reaches 200 resulting in a 50% duty cycle.
  */

  OCR1B = (F_CPU / 25000L) / 2;
  
  // Renabling this after we have set up the timer configuration
  interrupts();             // enable all interrupts

  /*
  Here, we are disabling unecessary peripherals
  They are not needed because we don't need ADC to send a signal, neither do we need the SPI, I2C protocol, or timer 0
  */
  ADCSRA = 0;  // ADC
  power_adc_disable ();
  power_spi_disable();
  power_twi_disable();
  power_timer0_disable();
  //power_usart0_disable(); // this might be needed for serial communication
  Serial.begin(115200);

 byte* emittingPointer = &animation[frame][0];
 

  LOOP:
    while(PINB & 0b00001000); //wait for pin 11 (B3) to go low 
    
    OUTPUT_WAVE(emittingPointer, 0); WAIT_LIT();
    OUTPUT_WAVE(emittingPointer, 1); WAIT_MID();
    OUTPUT_WAVE(emittingPointer, 2); WAIT_MID();
    OUTPUT_WAVE(emittingPointer, 3); WAIT_MID();
    OUTPUT_WAVE(emittingPointer, 4); WAIT_MID();
    OUTPUT_WAVE(emittingPointer, 5); WAIT_MID();
    OUTPUT_WAVE(emittingPointer, 6); WAIT_MID();
    OUTPUT_WAVE(emittingPointer, 7); WAIT_MID();
    OUTPUT_WAVE(emittingPointer, 8); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 9); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 10); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 11); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 12); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 13); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 14); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 15); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 16); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 17); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 18); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 19); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 20); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 21); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 22); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 23); 

  goto LOOP;
  
}

void loop(){}

