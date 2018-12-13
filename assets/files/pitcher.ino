#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *shooterMotor = AFMS.getMotor(1);
Adafruit_DCMotor *indexerMotor = AFMS.getMotor(2);
Adafruit_StepperMotor *baseMotor = AFMS.getStepper(200, 2);

const int switchPin = 7;
const int LEDPin = 6;

int baseRange = 3; // range from +- 10 steps
int shooterSpeed = 200;

void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(0));

  AFMS.begin();

  baseMotor->setSpeed(100);
  baseMotor->step(1, FORWARD, MICROSTEP);

  pinMode(switchPin, INPUT_PULLUP);
  pinMode(LEDPin, OUTPUT);

  shooterMotor->run(RELEASE);
  indexerMotor->run(RELEASE);

  Serial.println("Ready, not active: press a to activate");
}

int active = 0;
int lastSwtch = 0;
int baseLocation = 0;

int numHits = 0;
int numMisses = 0;

int readInt() {
  int result = 0;
  while (true) {
    int digit = Serial.read();
    if (!('0' <= digit && digit <= '9')) {
      break;
    }
    result *= 10;
    result += digit - '0';
  }
  return result;
}

void difficultyUp() {
  if (shooterSpeed <= 220) {
    shooterSpeed = min(shooterSpeed + 20, 220);
  }
  if (baseRange <= 5) {
    baseRange = min(baseRange + 2, 5);
  }
}

void difficultyDown() {
  if (shooterSpeed >= 160) {
    shooterSpeed = max(shooterSpeed - 20, 160);
  }
  if (baseRange >= 0) {
    baseRange = max(baseRange - 2, 0);
  }
}

void recalibrate() {
  if (numHits + numMisses < 4) {
    return;
  }

  Serial.println("Recalibrating:");
  Serial.print(numHits);
  Serial.println(" hits");
  Serial.print(numMisses);
  Serial.println(" misses");
  Serial.print("Old speed: ");
  Serial.println(shooterSpeed);
  Serial.print("Old range: ");
  Serial.println(baseRange);
  double hitRatio = 1. * numHits / (numHits + numMisses);
  if (hitRatio > 0.8) {
    difficultyUp();
  } else if (hitRatio < 0.4) {
    difficultyDown();
  } else if (hitRatio < 0.2) {
    difficultyDown();
    difficultyDown();
  }
  Serial.print("New speed: ");
  Serial.println(shooterSpeed);
  Serial.print("New range: ");
  Serial.println(baseRange);
  numHits = 0;
  numMisses = 0;
}

void loop() {
  delay(100);
  while (Serial.available() > 0) {
    int incomingByte = Serial.read();
    if (incomingByte == 0xA) {
      // Newline
      continue;
    }
    Serial.print("Received byte ");
    Serial.print((char)incomingByte);
    Serial.println();
    if (incomingByte == 'd') {
      active = 0;
      Serial.println("Deactivated; press a to activate");
    } else if (incomingByte == 'a') {
      baseLocation = 0;
      active = 1;
      Serial.println("Activated: press d to deactivate");
    } else if (incomingByte == 's') {
      Serial.println("Status");
      Serial.print("Active: ");
      Serial.print(active);
      Serial.println();
      Serial.print("Speed: ");
      Serial.print(shooterSpeed);
      Serial.println();
      Serial.print("Range: ");
      Serial.print(baseRange);
      Serial.println();
      Serial.print("Current location: ");
      Serial.print(baseLocation);
      Serial.println();
      Serial.print("Current record: ");
      Serial.print(numHits);
      Serial.print(" hits and ");
      Serial.print(numMisses);
      Serial.println(" misses");
    } else if (incomingByte == 'm') {
      Serial.println("Setting manual settings");
      int speed = readInt();
      int range = readInt();
      Serial.print("Speed: ");
      Serial.println(speed);
      Serial.print("Range: ");
      Serial.println(range);
      shooterSpeed = speed;
      baseRange = range;
      numHits = 0;
      numMisses = 0;
      active = 0;
      Serial.println("Press a to confirm changes and activate");
    } else if (incomingByte == '0') {
      Serial.println("Recording miss");
      numMisses ++;
      recalibrate();
    } else if (incomingByte == '1') {
      Serial.println("Recording hit");
      numHits ++;
      recalibrate();
    } else {
      Serial.println("Invalid command; try one of these:");
      Serial.println("a - activate");
      Serial.println("d - deactivate");
      Serial.println("s - status");
      Serial.println("m<speed>,<range> - manually set difficulty");
      Serial.println("0 - record miss");
      Serial.println("1 - record hit");
    }
  }

  if (active == 0) {
    shooterMotor->run(RELEASE);
    indexerMotor->run(RELEASE);
    return;
  }

  int swtch = digitalRead(switchPin);

  if (swtch != lastSwtch) {
    Serial.print("Switch changed: ");
    Serial.print(swtch);
    Serial.println();

    digitalWrite(LEDPin, swtch);
  
    if (swtch == HIGH) {
      Serial.print("Starting shooter");
      Serial.println();

      Serial.print("Speed: ");
      Serial.print(shooterSpeed);
      Serial.println();

      shooterMotor->setSpeed(shooterSpeed);
      shooterMotor->run(FORWARD);
      indexerMotor->setSpeed(255);
      indexerMotor->run(FORWARD);

      int newBaseLocation = random(-baseRange, baseRange+1);
      Serial.print("Moving to location: ");
      Serial.print(newBaseLocation);
      Serial.println();
      int baseSteps = newBaseLocation - baseLocation;
      baseMotor->step(abs(baseSteps), (baseSteps >= 0 ? FORWARD : BACKWARD), MICROSTEP);
      baseLocation = newBaseLocation;
    } else {
      Serial.println("BAM!");
      Serial.println("Enter 0 or 1 to record hit or miss");
      shooterMotor->run(RELEASE);
      indexerMotor->run(RELEASE);
    }
  }

  lastSwtch = swtch;

  delay(100);
}
