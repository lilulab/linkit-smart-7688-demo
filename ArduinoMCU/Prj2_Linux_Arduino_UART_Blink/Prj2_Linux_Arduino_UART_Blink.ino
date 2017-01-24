/*
  Linux_Arduino_UART_Blink
  Turns on/off an LED on based on Linux command via UART.
  https://github.com/lilulab/linkit-smart-7688-demo
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// Pin 13 has the LED on LinkIt Smart 7688 Duo

// give it a name:
int led = 13;

// the setup routine runs once when you press reset:
void setup() {                
  Serial.begin(115200);   // open serial connection to USB Serial port (connected to your computer)
  Serial1.begin(57600);   // open internal serial connection to MT7688AN
                          // in MT7688AN, this maps to device
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {
  int c = Serial1.read(); // read from MT7688AN
    if (c != -1) {
      switch(c) {
      case '0': // turn off D13 when receiving "0"
        digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
        break;
      case '1': // turn off D13 when receiving "1"
        digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
        break;
    }
  }
}

