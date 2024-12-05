/*
Missuse of the Adafruit Motor Shield V2 library to control 4 LED 12V lamps. Since the library is used to control DC motors, the
commands are written in a way that makes sense for motors, but not for LEDs. setSpeed is basically setPWM, run(FORWARD) is the
direction in our case not important, but it is used to turn on the LED. run(RELEASE) is used to turn Stop the motor.
This removes power from the motor and is equivalent to setSpeed(0). The motor shield does not implement dynamic breaking,
so the motor may take some time to spin down
*/

#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *howToXmasQuestion = AFMS.getMotor(2);       // M2
Adafruit_DCMotor *howToXmasWithoutConsume = AFMS.getMotor(1); // M1
Adafruit_DCMotor *sundaySales = AFMS.getMotor(4);             // M4
Adafruit_DCMotor *sold = AFMS.getMotor(3);                    // M3

Adafruit_DCMotor *leds[4] = {howToXmasQuestion, howToXmasWithoutConsume, sundaySales, sold};

enum HowToXmasTransitionState
{
  OFF_TO_TOP,
  TOP_TO_BOOTH,
  BOOTH_TO_BUTTOM,
  BUTTOM_TO_OFF,
};
HowToXmasTransitionState currentHowToXmasTransitionState = OFF_TO_TOP;
int howToXmasQuestionState, howToXmasWithoutConsumeState;
unsigned long howToXmasTransitionStartTime = 0;
unsigned long howToXmasTransitionDuration = 3333;

enum SoldBlinkTransitionState
{
  SOLD_BLINK_SLOW_ON,
  SOLD_BLINK_SLOW_OFF,
  SOLD_BLINK_SLOW_OFF_SHORT,
  SOLD_BLINK_SLOW_ON_AGAIN,

};
SoldBlinkTransitionState currentSoldBlinkTransitionState = SOLD_BLINK_SLOW_ON;
unsigned long soldBlinkTransitionStartTime = 0;
unsigned long soldBlinkTransitionDuration = 333;
unsigned long soldBlinkTransitionLongOffDuration = 1000;
bool isSundaySalesEnabled = true;

bool isEnabled = true;

String command;

void setup()
{
  Serial.begin(9600); // set up Serial library at 9600 bps
  Serial.println("ORBIT XMAS LED control is starting!");

  if (!AFMS.begin(75)) // frequency set to 75Hz
  {
    Serial.println("Could not find Motor Shield. Check wiring.");
    // Stop execution
    while (1)
      ;
  }
  Serial.println("Motor Shield found.");

  // Set all motors to run in the forward direction
  for (Adafruit_DCMotor *led : leds)
  {
    // Set the speed to start, from 0 (off) to 255 (max speed)
    led->setSpeed(255);
    led->run(FORWARD);
    // turn off motor
    led->run(RELEASE);
  }
  Serial.println("Type Command (slow, fast, off)");
}

void soldBlinkLoop()
{
  unsigned long currentTimeInMs = millis();

  switch (currentSoldBlinkTransitionState)
  {
  case SOLD_BLINK_SLOW_OFF:
    if (currentTimeInMs - soldBlinkTransitionStartTime >= soldBlinkTransitionLongOffDuration)
    {
      currentSoldBlinkTransitionState = SOLD_BLINK_SLOW_ON;
      soldBlinkTransitionStartTime = currentTimeInMs;
    }
    break;

  case SOLD_BLINK_SLOW_ON:
    if (currentTimeInMs - soldBlinkTransitionStartTime >= soldBlinkTransitionDuration)
    {
      currentSoldBlinkTransitionState = SOLD_BLINK_SLOW_OFF_SHORT;
      soldBlinkTransitionStartTime = currentTimeInMs;
    }
    break;

  case SOLD_BLINK_SLOW_OFF_SHORT:
    if (currentTimeInMs - soldBlinkTransitionStartTime >= soldBlinkTransitionDuration)
    {
      currentSoldBlinkTransitionState = SOLD_BLINK_SLOW_ON_AGAIN;
      soldBlinkTransitionStartTime = currentTimeInMs;
    }
    break;

  case SOLD_BLINK_SLOW_ON_AGAIN:
    if (currentTimeInMs - soldBlinkTransitionStartTime >= soldBlinkTransitionDuration)
    {
      currentSoldBlinkTransitionState = SOLD_BLINK_SLOW_OFF;
      soldBlinkTransitionStartTime = currentTimeInMs;
    }
    break;
  }

  if (isEnabled && (currentSoldBlinkTransitionState == SOLD_BLINK_SLOW_ON || currentSoldBlinkTransitionState == SOLD_BLINK_SLOW_ON_AGAIN))
  {
    sold->run(FORWARD);
    sold->setSpeed(255);
  }
  else if (isEnabled && (currentSoldBlinkTransitionState == SOLD_BLINK_SLOW_OFF || currentSoldBlinkTransitionState == SOLD_BLINK_SLOW_OFF_SHORT))
  {
    sold->run(FORWARD);
    sold->setSpeed(0);
  }
  else
  {
    sold->setSpeed(0);
  }

  // Sunday sales is steady on
  if (isEnabled && isSundaySalesEnabled)
  {
    sundaySales->run(FORWARD);
    sundaySales->setSpeed(255);
  }
  else
  {
    sundaySales->setSpeed(0);
  }
}

void howToXmasLoop()
{
  unsigned long currentTimeInMs = millis();

  if (currentHowToXmasTransitionState == OFF_TO_TOP &&
      currentTimeInMs - howToXmasTransitionStartTime >= howToXmasTransitionDuration)
  {
    currentHowToXmasTransitionState = TOP_TO_BOOTH;
    howToXmasTransitionStartTime = currentTimeInMs;
  }
  else if (currentHowToXmasTransitionState == TOP_TO_BOOTH &&
           currentTimeInMs - howToXmasTransitionStartTime >= howToXmasTransitionDuration)
  {
    currentHowToXmasTransitionState = BOOTH_TO_BUTTOM;
    howToXmasTransitionStartTime = currentTimeInMs;
  }
  else if (currentHowToXmasTransitionState == BOOTH_TO_BUTTOM &&
           currentTimeInMs - howToXmasTransitionStartTime >= howToXmasTransitionDuration)
  {
    currentHowToXmasTransitionState = BUTTOM_TO_OFF;
    howToXmasTransitionStartTime = currentTimeInMs;
  }
  else if (currentHowToXmasTransitionState == BUTTOM_TO_OFF &&
           currentTimeInMs - howToXmasTransitionStartTime >= howToXmasTransitionDuration)
  {
    currentHowToXmasTransitionState = OFF_TO_TOP;
    howToXmasTransitionStartTime = currentTimeInMs;
  }

  switch (currentHowToXmasTransitionState)
  {
  case OFF_TO_TOP:
    howToXmasQuestionState = 255;
    howToXmasWithoutConsumeState = 0;
    break;
  case TOP_TO_BOOTH:
    howToXmasQuestionState = 255;
    howToXmasWithoutConsumeState = 255;
    break;
  case BOOTH_TO_BUTTOM:
    howToXmasQuestionState = 0;
    howToXmasWithoutConsumeState = 255;
    break;
  case BUTTOM_TO_OFF:
    howToXmasQuestionState = 0;
    howToXmasWithoutConsumeState = 0;
    break;
  }
  if (isEnabled)
  {
    howToXmasQuestion->run(FORWARD);
    howToXmasQuestion->setSpeed(howToXmasQuestionState);
    howToXmasWithoutConsume->run(FORWARD);
    howToXmasWithoutConsume->setSpeed(howToXmasWithoutConsumeState);
  }
  else
  {
    howToXmasQuestion->setSpeed(0);
    howToXmasWithoutConsume->setSpeed(0);
    sundaySales->setSpeed(0);
    sold->setSpeed(0);
  }
}

void loop()
{
  howToXmasLoop();
  soldBlinkLoop();
  if (Serial.available())
  {
    command = Serial.readStringUntil('\n');
    command.trim();
    if (command.equals("slow"))
    {
      isEnabled = true;
      // howToXmasTransitionDuration = 10000;
      isSundaySalesEnabled = false;
      soldBlinkTransitionDuration = 2500;
    }
    else if (command.equals("fast"))
    {
      isEnabled = true;
      // howToXmasTransitionDuration = 3333;
      isSundaySalesEnabled = true;
      soldBlinkTransitionDuration = 333;
    }
    else if (command.equals("off"))
    {
      isEnabled = false;
    }
    Serial.print("Command: ");
    Serial.println(command);
  }
}
