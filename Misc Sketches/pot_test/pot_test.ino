// Arduino MIDI Controller
// Based on MINI UNTZtrument Sketch
// Spring 2017
// Brendon Kim, Dominic Gaiero, Elizabeth Davis, & Josiah Pang
#include <Wire.h>
#include <Adafruit_Trellis.h>
#include "MIDIUSB.h"

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).
#define LED     13 // Pin for heartbeat LED (shows code is working)
#define CHANNEL 1  // MIDI channel number


Adafruit_Trellis trellis;

uint8_t heart        = 0;        // Heartbeat LED counter
unsigned long prevReadTime = 0L; // Keypad polling timer
uint8_t mod;
uint8_t vel;
uint8_t fxc;
uint8_t rate;

uint8_t note[] = {
  60, 61, 62, 63,
  56, 57, 58, 59,
  52, 53, 54, 55,
  48, 49, 50, 51
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

void setup() {
  Serial.begin(115200);
  trellis.begin(0x70); // Pass I2C address
  trellis.clear();
  trellis.writeDisplay();

  // mod = map(analogRead(0), 0, 1023, 0, 127);
  mod = map(analogRead(0), 0, 1023, 0, 127);
  vel = map(analogRead(1), 0, 1023, 0, 127);
  fxc = map(analogRead(2), 0, 1023, 0, 127);
  rate = map(analogRead(3), 0, 1023, 0, 127);
  controlChange(CHANNEL, 1, mod);
  controlChange(CHANNEL, 11, vel);
  controlChange(CHANNEL, 12, fxc);
  controlChange(CHANNEL, 13, rate);
}

void loop() {


  // put your main code here, to run repeatedly:
  delay(30);

  unsigned long t = millis();
  if ((t - prevReadTime) >= 20L) { // 20ms = min Trellis poll time


    uint8_t newModulation = map(analogRead(0), 0, 1023, 0, 127);
    if (mod != newModulation) {
      mod = newModulation;
      controlChange(CHANNEL, 1, mod);
    }
    prevReadTime = t;
    digitalWrite(LED, ++heart & 32); // Blink = alive

  }
}
