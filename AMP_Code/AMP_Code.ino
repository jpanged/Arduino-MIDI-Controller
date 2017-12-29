// Arduino MIDI Controller
// Based on MINI UNTZtrument Sketch
// Spring 2017
// Brendon Kim, Dominic Gaiero, Elizabeth Davis, & Josiah Pang
#include <Wire.h>
#include <Adafruit_Trellis.h>
#include "MIDIUSB.h"

// Define Variables & Pins

// Defintion of a midiEventPacket_t:
// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

#define LED     13 // Pin for LED
#define CHANNEL 1  // MIDI channel number

Adafruit_Trellis trellis;

uint8_t heart = 0;        // Heartbeat LED counter
unsigned long prevReadTime = 0L; // Keypad polling timer
// A potX is
// An integer ranging from 0 to 127
uint8_t pot0; // Pot 0
uint8_t pot1; // Pot 1
uint8_t pot2; // Pot 2
uint8_t pot3; // Pot 3

// RGB LED Pin Mappings
const uint8_t redPin = 9; // Red
const uint8_t grnPin = 6; // Green
const uint8_t bluPin = 5; // Blue

uint8_t state = 0; // Define starting state of program
const uint8_t maxState = 2; // Define maximum number of states

const uint8_t buttonPin = 4; // Define pin button is connected to
int buttonState = 0; // Keeps track of whether the button is pressed
int lastButtonState = 0; // Initializes button as not pressed (LOW)

uint8_t note[] = { // Matrix for Trellis buttons
        60, 61, 62, 63,
        56, 57, 58, 59,
        52, 53, 54, 55,
        48, 49, 50, 51
};

// Methods

// Turns note on
void noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {
        midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
        MidiUSB.sendMIDI(noteOn);
}

// Turns note off
void noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity) {
        midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
        MidiUSB.sendMIDI(noteOff);
}

// Changes pot
void controlChange(uint8_t channel, uint8_t control, uint8_t value) {
        midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
        MidiUSB.sendMIDI(event);
}

// Adjusts pot value
void adjustPot(uint8_t pinNumber, uint8_t control) {
        uint8_t newPotValue = map(analogRead(pinNumber), 0, 1023, 0, 127);
        if (potValue != newPotValue) {
                potValue = newPotValue;
                controlChange(CHANNEL, control, potValue);
        }
}

// Sets LED
void ledColor(int r, int g, int b, float brightness = 0.5) {
  int redVal = r * brightness;
  int grnVal = g * brightness;
  int bluVal = b * brightness;

  analogWrite(redPin, redVal);   // Write current values to LED pins
  analogWrite(grnPin, grnVal);
  analogWrite(bluPin, bluVal);
}

void setup() {
        Serial.begin(115200);
        trellis.begin(0x70); // Pass I2C address
        trellis.clear();
        trellis.writeDisplay();

        pot0 = map(analogRead(0), 0, 1023, 0, 127);
        pot1 = map(analogRead(1), 0, 1023, 0, 127);
        pot2 = map(analogRead(2), 0, 1023, 0, 127);
        pot3 = map(analogRead(3), 0, 1023, 0, 127);
        controlChange(CHANNEL, 1, pot0);
        controlChange(CHANNEL, 11, pot1);
        controlChange(CHANNEL, 12, pot2);
        controlChange(CHANNEL, 13, pot3);

        // Set Button pin
        pinMode(buttonPin, INPUT);
        // Set RGB LED pins
        pinMode(redPin, OUTPUT);
        pinMode(grnPin, OUTPUT);
        pinMode(bluPin, OUTPUT);
}

void loop() {

   // Check if button was pushed
   buttonState = digitalRead(buttonPin);
   if (buttonState != lastButtonState) {
     if (buttonState == HIGH) { // (HIGH = 1)
       if (state == maxState) { // Reached the max state
          state = 0;
       }
       else {
         state++;
       }
    }
   delay(50);
   }
   lastButtonState = buttonState;


   delay(30); // Makes everything work

   // Checks if buttons are pressed every 20ms
   unsigned long t = millis();
   if ((t - prevReadTime) >= 20) { // 20ms = min Trellis poll time
      if (trellis.readSwitches()) { // Button state change?

         for (uint8_t i = 0; i < 16; i++) { // For each button...
            if (trellis.justPressed(i)) {
               noteOn(0, note[i], 64);
               Serial.println(note[i]);
               MidiUSB.flush();
               trellis.setLED(i);
            } else if (trellis.justReleased(i)) {
               noteOff(0, note[i], 64);
               Serial.println(note[i]);
               MidiUSB.flush();
               trellis.clrLED(i);
            }
         }
         trellis.writeDisplay();
      }
      prevReadTime = t;
      digitalWrite(LED, ++heart & 32); // Blink = alive

   }
   if ((t - prevReadTime) >= 1) { // 20ms = min Trellis poll time

      adjustPot(0,1);
      adjustPot(1,11);
      adjustPot(2,12);
      adjustPot(3,13);
   }
}
