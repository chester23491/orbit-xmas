int directionPinA = 12;
int pwmPinA = 3;
int brakePinA = 9;

//uncomment if using channel B, and remove above definitions
int directionPinB = 13;
int pwmPinB = 11;
int brakePinB = 8;
int delayTimeInMs = 3333;
int brightnessInPercent = 100;


void setup() {
  
//define pins
pinMode(directionPinA, OUTPUT);
pinMode(directionPinB, OUTPUT);
pinMode(pwmPinA, OUTPUT);
pinMode(pwmPinB, OUTPUT);
pinMode(brakePinA, OUTPUT);
pinMode(brakePinB, OUTPUT);

digitalWrite(directionPinA, LOW);
digitalWrite(directionPinB, LOW);
}

void loop() {
// turn channel A on
digitalWrite(brakePinA, LOW);
analogWrite(pwmPinA, brightnessInPercent);
delay(delayTimeInMs);

// turn channel B on
analogWrite(pwmPinB, brightnessInPercent);
digitalWrite(brakePinB, LOW);
delay(delayTimeInMs);

// turn channel A off
digitalWrite(brakePinA, HIGH);
analogWrite(pwmPinA, 0);
delay(delayTimeInMs);

// turn channel B off
digitalWrite(brakePinB, HIGH);
analogWrite(pwmPinB, 0);
delay(delayTimeInMs);
}