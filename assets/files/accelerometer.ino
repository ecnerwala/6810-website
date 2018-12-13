const int analogPinX = 0;
const int analogPinY = 1;
const int analogPinZ = 2;

const int LEDPin = 13;

void setup() {
  Serial.begin(9600);
  pinMode(LEDPin, OUTPUT);
}

double analogRead33(int pin) {
  double v = analogRead(pin);
  v /= 1023 * (3.3 / 5);
  return (v - 0.5) * 2;
}

const double ACCEL_RANGE = 3;
const double ACCEL_THRESH = 0.2;

int numTicks = 0;

void loop() {
  // put your main code here, to run repeatedly:
  double x = analogRead33(analogPinX) * ACCEL_RANGE;
  double y = analogRead33(analogPinY) * ACCEL_RANGE;
  double z = analogRead33(analogPinZ) * ACCEL_RANGE;

  if (abs(z) >= ACCEL_THRESH) {
    numTicks = 10;
  }

  int led;
  if (numTicks > 0) {
    led = HIGH;
    numTicks --;
  } else {
    led = LOW;
  }
  digitalWrite(LEDPin, led);

  Serial.print(z);
  Serial.print("\t");
  Serial.print(sqrt(x*x + y*y));
  Serial.print("\t");
  Serial.print(atan2(y, x));
  Serial.print("\t");
  Serial.print(led == HIGH ? 3 : 0);
  Serial.println();
  delay(100);
}
