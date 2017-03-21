// Arduino MIDI Controller
// Based on MINI UNTZtrument Sketch
// Spring 2017
// Brendon Kim, Dominic Gaiero, Elizabeth Davis, & Josiah Pang

// ===============================
// Import Libraries
// ===============================
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
uint8_t       mod;
uint8_t       vel;
uint8_t       fxc;
uint8_t       rate;

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
	mod = map(analogRead(0), 0, 1023, 0, 127);
	vel = map(analogRead(1), 0, 1023, 0, 127);
	fxc = map(analogRead(2), 0, 1023, 0, 127);
	rate = map(analogRead(3), 0, 1023, 0, 127);
	usbMIDI.sendControlChange(1, mod, CHANNEL);
	usbMIDI.sendControlChange(11, vel, CHANNEL);
	usbMIDI.sendControlChange(12, fxc, CHANNEL);
	usbMIDI.sendControlChange(13, rate, CHANNEL);
}
// ===============================
// Loop
// ===============================
void loop() {
	unsigned long t = millis();
	if ((t - prevReadTime) >= 20L) { // 20ms = min Trellis poll time
		if (trellis.readSwitches()) {  // Button state change?

			for (uint8_t i = 0; i<16; i++) { // For each button...
				if (trellis.justPressed(i)) {
					usbMIDI.sendNoteOn(note[i], 127, CHANNEL);

					trellis.setLED(i);
				}
				else if (trellis.justReleased(i)) {
					usbMIDI.sendNoteOff(note[i], 0, CHANNEL);
					trellis.clrLED(i);
				}
			}
			trellis.writeDisplay();
		}
		uint8_t newModulation = map(analogRead(0), 0, 1023, 0, 127);
		if (mod != newModulation) {
			mod = newModulation;
			usbMIDI.sendControlChange(1, mod, CHANNEL);
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
		prevReadTime = t;
		digitalWrite(LED, ++heart & 32); // Blink = alive
	}
	while (usbMIDI.read()); // Discard incoming MIDI messages
}