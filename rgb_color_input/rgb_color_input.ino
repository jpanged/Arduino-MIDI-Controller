int redPin = 10;   // Red LED,   connected to digital pin 9
int grnPin = 9;  // Green LED, connected to digital pin 10
int bluPin = 11;  // Blue LED,  connected to digital pin 11



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
    Serial.print("R: ");
    Serial.print(r);
    Serial.print("; G: ");
    Serial.print(g);
    Serial.print("; B: ");
    Serial.println(b);
    ledColor(r, g, b);
    // Do something awesome!
  }
}


void ledColor(int r, int g, int b)
{
  int redVal = (r * 255) / 100;
  int grnVal = (g * 255) / 100;
  int bluVal = (b * 255) / 100;

  analogWrite(redPin, redVal);   // Write current values to LED pins
  analogWrite(grnPin, grnVal);
  analogWrite(bluPin, bluVal);
}

