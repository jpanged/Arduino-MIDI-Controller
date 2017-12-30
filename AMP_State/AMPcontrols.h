/*
  state.h - Library to hold state changes for AMP
  Created by Dominic Gaiero and Josiah Pang, December 29, 2017.
*/

#idndef AMPcontrols_h
#define AMPcontrols_h


#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif

class LED {
   public:
      Led(int redPin, int grnPin, int bluPin, float iBrightness);
      void setColor(int r, int g, int b);
      void setColorWithBrightness(int r, int g, int b, int brightness);
   private:
     // Define the pins for the three LEDs
     int redPin;
     int grnPin;
     int bluPin;

     float iBrightness; // Defines the brightness for the Led

};

class Pot {
   public:
      Pot(uint8_t pinNumber, uint8_t value, uint8_t control, uint8_t channel);
      void controlChange(uint8_t channel, uint8_t control, uint8_t value);
      void adjustPot(uint8_t pinNumber, uint8_t control);
   private:
      uint8_t pinNumber; // of given pot
      uint8_t value; // Current value of the pot (0 to 127)
      uint8_t control; // Specifies what is being controlled
      uint8_t channel; // MIDI channel

};

#endif
