// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
#include <Wire.h>
#include "Adafruit_Trellis.h"
#define LATCHING 1
// set the mode here
#define MODE LATCHING

Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);

#define numKeys 16
#define INTPIN A2
int led = 13;

uint8_t trellis_light[16];
uint8_t trellis_user[] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
};

uint8_t trellis_number = 0;

uint8_t trellis_position[16];
int randomInt = 0;

//A variable to hold the previously assigned pseudo random non-consecutive integers.
int previousRandomInt = 0;
int trellis_inputSize;
int i;
int counter1 = 0;

// the setup routine runs once when you press reset:
void setup() {
        // initialize the digital pin as an output.
        int temp;
        int passCounter = 0;
        int elementsFilled = 0;
        int numberElements = 7;
        pinMode(led, OUTPUT);
        pinMode(INTPIN, INPUT);
        digitalWrite(INTPIN, HIGH);
        trellis.begin(0x70);

        randomSeed(analogRead(0));
        Serial.begin(9600);

        for (uint8_t i = 0; i < numKeys; i++) {
                if (i == 1)
                {
                        digitalWrite(led, HIGH);
                }
                trellis.setLED(i);
                Serial.print("v"); Serial.println(i);
                trellis.writeDisplay();
                delay(300);
        }
        // then turn them off
        for (uint8_t i = 0; i < numKeys; i++) {
                if (i == 1)
                {
                        digitalWrite(led, LOW);
                }
                trellis.clrLED(i);
                trellis.writeDisplay();
                delay(25);
        }

}
//Serial.print(trellis_inputSize);

// the loop routine runs over and over again forever:
void loop() {
        for (uint8_t i = 0; i < numKeys; i++) {
                trellis_light[i] = random(0, 2);
                // tell the trellis to set the LEDs we requested
                // trellis.writeDisplay();
                if (trellis_light[i] == 1)
                {
                        ++trellis_number;
                        trellis_user[i] = 1;
                        trellis.setLED(i);
                        if (i == 1)
                        {
                                digitalWrite(led, HIGH);
                        }
                        trellis.writeDisplay();
                        delay(50);
                }
        }
        for (int i = 0; i < 4; i++)
        {
                Serial.print(trellis_light[i]);
        }
        Serial.println("");
        for (int i = 4; i < 8; i++)
        {
                Serial.print(trellis_light[i]);
        }
        Serial.println("");
        for (int i = 8; i < 12; i++)
        {
                Serial.print(trellis_light[i]);
        }
        Serial.println("");
        for (int i = 12; i < 16; i++)
        {
                Serial.print(trellis_light[i]);
        }
        Serial.println("");

        for (uint8_t i = 0; i < numKeys; i++) {
                trellis_light[i] = random(0, 2);
                // tell the trellis to set the LEDs we requested
                trellis.writeDisplay();
                if (trellis_light[i] == 1)
                {
                        ++trellis_number;
                        trellis_user[i] = 1;
                        //trellis.setLED(i);
                        //trellis.writeDisplay();
                        delay(50);
                        if (i == 1)
                        {
                                digitalWrite(led, LOW);
                        }
                        trellis.clrLED(i);
                        trellis.writeDisplay();
                }
        }
        for (int i = 0; i < 4; i++)
        {
                Serial.print(trellis_light[i]);
        }
        Serial.println("");
        for (int i = 4; i < 8; i++)
        {
                Serial.print(trellis_light[i]);
        }
        Serial.println("");
        for (int i = 8; i < 12; i++)
        {
                Serial.print(trellis_light[i]);
        }
        Serial.println("");
        for (int i = 12; i < 16; i++)
        {
                Serial.print(trellis_light[i]);
        }
        Serial.println("");
        Serial.print("Number: ");
        Serial.println(trellis_number);


}
