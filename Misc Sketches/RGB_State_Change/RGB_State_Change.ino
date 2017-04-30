/*
  State change detection (edge detection)

  Often, you don't need to know the state of a digital input all the time,
  but you just need to know when the input changes from one state to another.
  For example, you want to know when a button goes from OFF to ON.  This is called
  state change detection, or edge detection.

  This example shows how to detect when a button or button changes from off to on
  and on to off.

  The circuit:
   pushbutton attached to pin 2 from +5V
   10K resistor attached to pin 2 from ground
   LED attached from pin 13 to ground (or use the built-in LED on
   most Arduino boards)

  created  27 Sep 2005
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/ButtonStateChange

*/

// ==========
// Variables
// ==========
// this constant won't change:
const int  buttonPin = 16;    // the pin that the pushbutton is attached to
const int ledPin = 21;       // the pin that the LED is attached to
// Declares RGB LED pins
int redPin = 5;   // Red LED,   connected to digital pin 5
int grnPin = 6;  // Green LED, connected to digital pin 6
int bluPin = 9;  // Blue LED,  connected to digital pin 9


// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);
  // initialize serial communication:
  pinMode(redPin, OUTPUT);   // sets the pins as output
  pinMode(grnPin, OUTPUT);
  pinMode(bluPin, OUTPUT);
  //Serial.begin(9600);
  //Serial.println("----BEGIN----");
}


void loop() {
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      if (buttonPushCounter == 5) {
        buttonPushCounter = 0;
      }
      else {
        buttonPushCounter++;
      }
      //Serial.println("on");
      //Serial.print("number of button pushes:  ");
      //Serial.println(buttonPushCounter);
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButtonState = buttonState;


  // turns on the LED every four button pushes by
  // checking the modulo of the button push counter.
  // the modulo function gives you the remainder of
  // the division of two numbers:
  if (buttonPushCounter == 0) {
    //digitalWrite(ledPin, HIGH);
    ledColor(255, 0, 0, 0.5); // Red
  }
  else if (buttonPushCounter == 1) {
    ledColor(0, 255, 0, 0.5); // Green
  }
  else if (buttonPushCounter == 2) {
    ledColor(0, 0, 255, 0.5); // Blue
  }
  else if (buttonPushCounter == 3) {
    ledColor(255, 255, 255, 0.5); // White is right
  }
  else if (buttonPushCounter == 4) {
    ledColor(100, 50, 100, 0.5); // ?
  }
  else if (buttonPushCounter == 5) {
    ledColor(75, 223, 17, 0.5); // ??
  }
  else {
    ledColor(0, 0, 0, 0); // Off
  }

}

// Function to write LED
// brightness is a float between 0 to 1 representing brightness percentage
void ledColor(int r, int g, int b, float brightness)
{
  int redVal = (r * brightness);
  int grnVal = (g * brightness);
  int bluVal = (b * brightness);

  analogWrite(redPin, redVal);   // Write current values to LED pins
  analogWrite(grnPin, grnVal);
  analogWrite(bluPin, bluVal);
}
