// Arduino MIDI Controller
// Based on MINI UNTZtrument Sketch
// Spring 2017
// Brendon Kim, Dominic Gaiero, Elizabeth Davis, & Josiah Pang

// ===============================
// Import Libraries
// ===============================
#include <Adafruit_UNTZtrument.h>
#include <Wire.h>
#include <Adafruit_Trellis.h>

// ===============================
// Define Variables
// ===============================
#define LED     13 // Pin for heartbeat LED (shows code is working)
#define CHANNEL 1  // MIDI channel number

Adafruit_Trellis trellis;

uint8_t       heart = 0;  // Heartbeat LED counter
unsigned long prevReadTime = 0L; // Keypad polling timer
// Designates POT variables as unsigned 8 bit integers
uint8_t       mod;
uint8_t       vel;
uint8_t       fxc;
uint8_t       rate;

// Defines MIDI notes for Trellis buttons using an array
uint8_t note[] = {
	60, 61, 62, 63,
	56, 57, 58, 59,
	52, 53, 54, 55,
	48, 49, 50, 51
};
// ===============================
// Setup
// ===============================
void setup() {
	pinMode(LED, OUTPUT);
	trellis.begin(0x70); // Pass I2C address
#ifdef __AVR__
						 // Default Arduino I2C speed is 100 KHz, but the HT16K33 supports
						 // 400 KHz.  We can force this for faster read & refresh, but may
						 // break compatibility with other I2C devices...so be prepared to
						 // comment this out, or save & restore value as needed.
	TWBR = 12;
#endif
	trellis.clear();
	trellis.writeDisplay();
	mod = map(analogRead(0), 0, 1023, 0, 127); // POT1
	vel = map(analogRead(1), 0, 1023, 0, 127); // POT2
	fxc = map(analogRead(2), 0, 1023, 0, 127); // POT3
	rate = map(analogRead(3), 0, 1023, 0, 127); // POT4
	usbMIDI.sendControlChange(1, mod, CHANNEL); // Sends initial POT1 signal
	usbMIDI.sendControlChange(11, vel, CHANNEL); // Sends initial POT2 signal
	usbMIDI.sendControlChange(12, fxc, CHANNEL); // Sends initial POT3 signal
	usbMIDI.sendControlChange(13, rate, CHANNEL); // Sends initial POT4 signal
}
// ===============================
// Loop
// ===============================
void loop() {
	unsigned long t = millis();
	if ((t - prevReadTime) >= 20L) { // 20ms = min Trellis poll time
		if (trellis.readSwitches()) {  // Button state change?

			for (uint8_t i = 0; i<16; i++) { // For each button...
				if (trellis.justPressed(i)) { // If it's pressed at the poll time
					usbMIDI.sendNoteOn(note[i], 127, CHANNEL); // Send note on

					trellis.setLED(i); // Turn LED on
				}
				else if (trellis.justReleased(i)) { // Else if it is no longer pressed at the poll time...
					usbMIDI.sendNoteOff(note[i], 0, CHANNEL); // Send note off
					trellis.clrLED(i); // Turn LED off
				}
			}
			trellis.writeDisplay();
		}
		uint8_t newModulation = map(analogRead(0), 0, 1023, 0, 127); // Updates current state of POT1
		if (mod != newModulation) { // If previous state does not match current state...
			mod = newModulation; // New state replaces previous state
			usbMIDI.sendControlChange(1, mod, CHANNEL); // Send new state
		}
		uint8_t newVelocity = map(analogRead(1), 0, 1023, 0, 127);
		if (vel != newVelocity) {
			vel = newVelocity;
			usbMIDI.sendControlChange(11, vel, CHANNEL);
		}
		uint8_t newEffect = map(analogRead(2), 0, 1023, 0, 127);
		if (fxc != newEffect) {
			fxc = newEffect;
			usbMIDI.sendControlChange(12, fxc, CHANNEL);
		}
		uint8_t newRate = map(analogRead(3), 0, 1023, 0, 127);
		if (rate != newRate) {
			rate = newRate;
			usbMIDI.sendControlChange(13, rate, CHANNEL);
		}
		prevReadTime = t; // Sets new time to check against
		digitalWrite(LED, ++heart & 32); // Blink = alive
	}
	while (usbMIDI.read()); // Discard incoming MIDI messages
}
