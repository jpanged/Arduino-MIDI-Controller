// Arduino MIDI Controller
// Based on MINI UNTZtrument Sketch
// Spring 2017
// Brendon Kim, Dominic Gaiero, Elizabeth Davis, & Josiah Pang
#include <Wire.h>
#include <Adafruit_Trellis.h>
#include "MIDIUSB.h"

// Define Variables

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).
#define LED     13 // Pin for heartbeat LED (shows code is working)
#define CHANNEL 1  // MIDI channel number
#define buttonPin 4 // Define pin button is connected to


Adafruit_Trellis trellis;

uint8_t heart        = 0;        // Heartbeat LED counter
unsigned long prevReadTime = 0L; // Keypad polling timer
uint8_t mod;
uint8_t vel;
uint8_t fxc;
uint8_t rate;

const uint8_t redPin = 9; // Red
const uint8_t grnPin = 6; // Green
const uint8_t bluPin = 5; // Blue

const uint8_t maxState = 2; // Define maximum number of states

static unsigned int button;
int state = 0;

uint8_t * note;

uint8_t noteCase0[] = {
  60, 61, 62, 63,
  56, 57, 58, 59,
  52, 53, 54, 55,
  48, 49, 50, 51
};

uint8_t noteCase1[] = {
  60, 61, 62, 63,
  56, 57, 58, 59,
  52, 53, 54, 55,
  64, 49, 50, 51
};

uint8_t noteCase2[] = {
  60, 61, 62, 63,
  56, 57, 58, 59,
  52, 53, 54, 55,
  65, 49, 50, 51
};


void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

// Sets LED
void ledColor(int r, int g, int b, float brightness = 0.25) {
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

  // Initialize variables
  int buttonState = 0; // Keeps track of whether the button is pressed
  int lastButtonState = 0; // Initializes button as not pressed (LOW)



  // mod = map(analogRead(0), 0, 1023, 0, 127);
  mod = map(analogRead(0), 0, 1023, 0, 127);
  vel = map(analogRead(1), 0, 1023, 0, 127);
  fxc = map(analogRead(2), 0, 1023, 0, 127);
  rate = map(analogRead(3), 0, 1023, 0, 127);
  controlChange(CHANNEL, 1, mod);
  controlChange(CHANNEL, 11, vel);
  controlChange(CHANNEL, 12, fxc);
  controlChange(CHANNEL, 13, rate);

  // Set Button pin
  pinMode(buttonPin, INPUT);
  // Set RGB LED pins
  pinMode(redPin, OUTPUT);
  pinMode(grnPin, OUTPUT);
  pinMode(bluPin, OUTPUT);
}

void loop() {

  Serial.println(state);

  button = digitalRead(buttonPin);
  switch(state) {
    case 0:
      if (button = HIGH) {
        state = 1;
      }
      note = noteCase0;
      ledColor(255, 0, 0);
      break;
    case 1:
      if (button = HIGH) {
        state = 2;
      }
      note = noteCase1;
      ledColor(0, 255, 0);
      break;
    case 2:
      if (button = HIGH) {
        state = 0;
      }
      note = noteCase2;
      ledColor(0, 0, 255);
      break;
  }

  // Checks if buttons are pressed every 20ms
  unsigned long t = millis();
  if ((t - prevReadTime) >= 20) { // 20ms = min Trellis poll time
    if (trellis.readSwitches()) { // Button state change?

      for (uint8_t i = 0; i < 16; i++) { // For each button...
        if (trellis.justPressed(i)) {
          noteOn(0, note[i], 0xFF);
          Serial.println(note[i]);
          MidiUSB.flush();
          trellis.setLED(i);
        } else if (trellis.justReleased(i)) {
          noteOff(0, note[i], 0xFF);
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
    uint8_t newModulation = map(analogRead(0), 0, 1023, 0, 127);
    if (mod != newModulation) {
      mod = newModulation;
      controlChange(CHANNEL, 1, mod);
    }
    uint8_t newVelocity = map(analogRead(1), 0, 1023, 0, 127);
    if(vel != newVelocity) {
           vel = newVelocity;
           controlChange(CHANNEL,11, vel);
    }
    uint8_t newEffect = map(analogRead(2), 0, 1023, 0, 127);
    if(fxc != newEffect) {
           fxc = newEffect;
           controlChange(CHANNEL,12, fxc);
    }
    uint8_t newRate = map(analogRead(3), 0, 1023, 0, 127);
    if(rate !=newRate) {
           rate = newRate;
           controlChange(CHANNEL,13, rate);
     }
  }
}

