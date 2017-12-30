#include "AMPcontrols.h"
#include "Arduino.h"


LED::LED(int redPin, int grnPin, int bluPin, float iBrightness) {
  redPin = redPin;
  grnPin = grnPin;
  bluPin = bluPin;
  iBrightness = iBrightness;
}

//public methods implementation

void LED::setColor(int r, intg, int b) {
  int redVal = r * _brightness;
  int grnVal = g * _brightness;
  int bluVal = b * _brightness;

  analogWrite(redPin, redVal);   // Write current values to LED pins
  analogWrite(grnPin, grnVal);
  analogWrite(bluPin, bluVal);
}

void LED::setLEDColorWithBrightness(int r, intg, int b, float brighness) {
  int redVal = r * brightness;
  int grnVal = g * brightness;
  int bluVal = b * brightness;

  analogWrite(redPin, redVal);   // Write current values to LED pins
  analogWrite(grnPin, grnVal);
  analogWrite(bluPin, bluVal);
}

// Pot object constructor
Pot::Pot(uint8_t pinNumber, uint8_t value, uint8_t control, uint8_t channel) {
   pinNumber = pinNumber;
   value = value;
   control = control;
   channel = channel;
}

// Changes pot
void Pot::controlChange(uint8_t channel, uint8_t control, uint8_t value) {
   midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
   MidiUSB.sendMIDI(event);
}

// Adjusts pot value
void Pot:adjustPot(uint8_t pinNumber, uint8_t control) {
   uint8_t newPotValue = map(analogRead(pinNumber), 0, 1023, 0, 127);
   if (potValue != newPotValue) {
          potValue = newPotValue;
          controlChange(CHANNEL, control, potValue);
   }
}
