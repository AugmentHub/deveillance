// DEFINING PINS

const int IN1 = A0;  // IN1 => A0
const int IN2 = A1;  // IN2 => A1

void setup() {
  pinMode(IN1, OUTPUT); // Output for waves
  pinMode(IN2, OUTPUT);
}

void loop() {
  // Generate approx. 25 kHz square wave
  for (;;) {
    // Toggle IN1 and IN2 
    PORTC |= 0b00000001;  // Set A0 HIGH, A1 LOW
    PORTC &= 0b11111101;
    delayMicroseconds(20); // 20 or 21 microseconds delay for half the period (high time)

    PORTC |= 0b00000010;  // Set A1 HIGH, A0 LOW
    PORTC &= 0b11111110;
    delayMicroseconds(20); // delay for lowtime
  }
}
  