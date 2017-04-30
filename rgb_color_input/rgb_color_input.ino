int redPin = 5;   // Red LED,   connected to digital pin 9
int grnPin = 6;  // Green LED, connected to digital pin 10
int bluPin = 9;  // Blue LED,  connected to digital pin 11



void setup() {
  // put your setup code here, to run once:
  pinMode(redPin, OUTPUT);   // sets the pins as output
  pinMode(grnPin, OUTPUT);
  pinMode(bluPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("----BEGIN----");
}

void loop() {
  // put your main code here, to run repeatedly:


  if (Serial.available() > 0)
  {
    int r = Serial.readStringUntil(',').toInt();
    int g = Serial.readStringUntil(',').toInt();
    int b = Serial.readStringUntil(',').toInt();
    float brightness = Serial.readStringUntil(',').tofloat();
    Serial.print("R: ");
    Serial.print(r);
    Serial.print("; G: ");
    Serial.print(g);
    Serial.print("; B: ");
    Serial.print(b);
    Serial.print("; Brightness: ");
    Serial.println(brightness);
    ledColor(r, g, b, brightness);
    // Do something awesome!
  }
}


void ledColor(int r, int g, int b, float brightness)
{
  int redVal = r * brightness;
  int grnVal = g * brightness;
  int bluVal = b * brightness;

  analogWrite(redPin, redVal);   // Write current values to LED pins
  analogWrite(grnPin, grnVal);
  analogWrite(bluPin, bluVal);
}
