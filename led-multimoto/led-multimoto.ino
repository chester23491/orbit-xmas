/*
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2
---->	http://www.adafruit.com/products/1438
*/

#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *howToXmasQuestion = AFMS.getMotor(1); // M1
Adafruit_DCMotor *howToXmasWithoutConsume = AFMS.getMotor(2); // M2
Adafruit_DCMotor *sundaySales = AFMS.getMotor(3); // M3
Adafruit_DCMotor *sold = AFMS.getMotor(4); // M4

Adafruit_DCMotor* leds[4] = {howToXmasQuestion, howToXmasWithoutConsume, sundaySales, sold};

enum HowToXmasTransitionState {
  OFF_TO_TOP,
  TOP_TO_BOOTH,
  BOOTH_TO_BUTTOM,
  BUTTOM_TO_OFF,
};
HowToXmasTransitionState currentTransitionState = OFF_TO_TOP;
int howToXmasQuestionState, howToXmasWithoutConsumeState;
unsigned long transitionStartTime = 0;
unsigned long transitionDuration = 3333;
bool isEnabled = true;

String command;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  if (!AFMS.begin(75)) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  for (Adafruit_DCMotor* led : leds) {
    // Set the speed to start, from 0 (off) to 255 (max speed)
    led->setSpeed(255);
    led->run(FORWARD);
    // turn on motor
    led->run(RELEASE);
  }
  Serial.println("Type Command (slow, fast, off)");
}

void howToXmasLoop() {
  unsigned long currentTimeInMs = millis();

  if (currentTransitionState == OFF_TO_TOP &&
      currentTimeInMs - transitionStartTime >= transitionDuration) {
    currentTransitionState = TOP_TO_BOOTH;
    transitionStartTime = currentTimeInMs;
  } else if (currentTransitionState == TOP_TO_BOOTH &&
             currentTimeInMs - transitionStartTime >= transitionDuration) {
    currentTransitionState = BOOTH_TO_BUTTOM;
    transitionStartTime = currentTimeInMs;
  } else if (currentTransitionState == BOOTH_TO_BUTTOM &&
             currentTimeInMs - transitionStartTime >= transitionDuration) {
    currentTransitionState = BUTTOM_TO_OFF;
    transitionStartTime = currentTimeInMs;
  } else if (currentTransitionState == BUTTOM_TO_OFF &&
             currentTimeInMs - transitionStartTime >= transitionDuration) {
    currentTransitionState = OFF_TO_TOP;
    transitionStartTime = currentTimeInMs;
  }

  switch (currentTransitionState) {
    case OFF_TO_TOP:
    howToXmasQuestionState = 255; howToXmasWithoutConsumeState = 0;
    break;
    case TOP_TO_BOOTH:
    howToXmasQuestionState = 255; howToXmasWithoutConsumeState = 255;
    break;
    case BOOTH_TO_BUTTOM:
    howToXmasQuestionState = 0; howToXmasWithoutConsumeState = 255;
    break;
    case BUTTOM_TO_OFF:
    howToXmasQuestionState = 0; howToXmasWithoutConsumeState = 0;
    break;
  }
  if(isEnabled){  
    howToXmasQuestion->run(FORWARD);
    howToXmasQuestion->setSpeed(howToXmasQuestionState);
    howToXmasWithoutConsume->run(FORWARD);
    howToXmasWithoutConsume->setSpeed(howToXmasWithoutConsumeState);

    //temp
    sundaySales->run(FORWARD);
    sundaySales->setSpeed(howToXmasWithoutConsumeState);
    sold->run(FORWARD);
    sold->setSpeed(howToXmasWithoutConsumeState);
  }else {
    howToXmasQuestion->setSpeed(0);
    howToXmasWithoutConsume->setSpeed(0);
    sundaySales->setSpeed(0);
    sold->setSpeed(0);
  }
}

void loop() {
  howToXmasLoop();

  if (Serial.available()) {
      command = Serial.readStringUntil('\n');
      command.trim();
      if (command.equals("slow")) {
        isEnabled = true;
        transitionDuration = 10000;
      }
      else if (command.equals("fast")) {
         isEnabled = true;
        transitionDuration = 3333;
      }
      else if (command.equals("off")) {
        isEnabled = false;
      }
      Serial.print("Command: ");
      Serial.println(command);
  }
  //delay(1000);
}