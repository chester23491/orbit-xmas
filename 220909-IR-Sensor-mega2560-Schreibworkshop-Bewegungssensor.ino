
// include the SPI library:
#include <SPI.h>

// L9958 slave select pins for SPI
#define SS_M4 14
#define SS_M3 13
#define SS_M2 12
#define SS_M1 11

// L9958 DIRection pins
#define DIR_M1 2
#define DIR_M2 3
#define DIR_M3 4
#define DIR_M4 7

// L9958 PWM pins
#define PWM_M1 9     // Timer2
#define PWM_M2 10    // Timer2
#define PWM_M3 5     // Timer3
#define PWM_M4 6     // Timer4

// L9958 Enable for all 4 motors
#define ENABLE_MOTORS 8

int ir_pin = 15;      // infra red sensor (detects paper position) connected to pin 15
int pir_pin = 16;     // motion sensor (detects humans passing by) connected to pin 16
int pwm1, pwm2, pwm3, pwm4;
int dir1, dir2, dir3, dir4;
int led_pin = 13;    // control LED connected to digital pin 13

void setup() {
  unsigned int configWord;
  // put your setup code here, to run once:
  
  pinMode(SS_M4, OUTPUT); digitalWrite(SS_M4, HIGH);  // HIGH = not selected
  pinMode(SS_M3, OUTPUT); digitalWrite(SS_M3, HIGH);
  pinMode(SS_M2, OUTPUT); digitalWrite(SS_M2, HIGH);
  pinMode(SS_M1, OUTPUT); digitalWrite(SS_M1, HIGH);
  
  // L9958 DIRection pins
  pinMode(DIR_M1, OUTPUT);
  pinMode(DIR_M2, OUTPUT);
  pinMode(DIR_M3, OUTPUT);
  pinMode(DIR_M4, OUTPUT);
  
  // L9958 PWM pins
  pinMode(PWM_M1, OUTPUT);  digitalWrite(PWM_M1, LOW);
  pinMode(PWM_M2, OUTPUT);  digitalWrite(PWM_M2, LOW);    // Timer1
  pinMode(PWM_M3, OUTPUT);  digitalWrite(PWM_M3, LOW);
  pinMode(PWM_M4, OUTPUT);  digitalWrite(PWM_M4, LOW);    // Timer0
  
  // L9958 Enable for all 4 motors
  pinMode(ENABLE_MOTORS, OUTPUT);  digitalWrite(ENABLE_MOTORS, HIGH);   // HIGH = disabled
  
  /******* Set up L9958 chips *********
  ' L9958 Config Register
  ' Bit
  '0 - RES
  '1 - DR - reset
  '2 - CL_1 - curr limit
  '3 - CL_2 - curr_limit
  '4 - RES
  '5 - RES
  '6 - RES
  '7 - RES
  '8 - VSR - voltage slew rate
  '9 - ISR - current slew rate
  '10 - ISR_DIS - current slew disable
  '11 - OL_ON - open load enable
  '12 - RES
  '13 - RES
  '14 - 0 - always zero
  '15 - 0 - always zero
  */
  
  // set to max current limit and disable ISR slew limiting
  configWord = 0b0000010000001100;
  
  SPI.begin();
  SPI.setBitOrder(LSBFIRST);
  SPI.setDataMode(SPI_MODE1);  // clock pol = low, phase = high
  
  // Motor 1
  digitalWrite(SS_M1, LOW);
  SPI.transfer(lowByte(configWord));
  SPI.transfer(highByte(configWord));
  digitalWrite(SS_M1, HIGH); 
  // Motor 2
  digitalWrite(SS_M2, LOW);
  SPI.transfer(lowByte(configWord));
  SPI.transfer(highByte(configWord));
  digitalWrite(SS_M2, HIGH);
  // Motor 3
  digitalWrite(SS_M3, LOW);
  SPI.transfer(lowByte(configWord));
  SPI.transfer(highByte(configWord));
  digitalWrite(SS_M3, HIGH);
  // Motor 4
  digitalWrite(SS_M4, LOW);
  SPI.transfer(lowByte(configWord));
  SPI.transfer(highByte(configWord));
  digitalWrite(SS_M4, HIGH);
  
  // Reduce the PWM frequency to about 8kHz
  // Note, this will screw up the timer functions that use Timer0 such as millis()
  setPwmFrequency(PWM_M1, 1);
  setPwmFrequency(PWM_M2, 1);
  setPwmFrequency(PWM_M3, 1);
  setPwmFrequency(PWM_M4, 1);
}


// *******************************************
// ************** Main Loop ******************
// *******************************************

void loop() {
  //dir1 = 1; dir2 = 0; dir3 = 0; dir4 = 0;
  //pwm1 = 47; pwm2 = 154; pwm3 = 225; pwm4 = 3;
  //pwm1 = random(256); pwm2 = random(256); pwm3 = random(256); pwm4 = random(256);
  //dir1 = random(2); dir2 = random(2); dir3 = random(2); dir4 = random(2);
 
 
dir1 = 0;                                                     // opposite direction = !dir1;




if (digitalRead(pir_pin) == HIGH) {                           // if motion sensor reads human motion                       

  for (pwm1 = 0; pwm1 < 91 ; pwm1 +=10) {                     // ramp speed up for all motors
    analogWrite(PWM_M1, pwm1);  digitalWrite(DIR_M1, dir1);
    analogWrite(PWM_M2, pwm1);  digitalWrite(DIR_M2, dir1);
    analogWrite(PWM_M3, pwm1);  digitalWrite(DIR_M3, dir1);
    analogWrite(PWM_M4, pwm1);  digitalWrite(DIR_M4, dir1);
    digitalWrite(ENABLE_MOTORS, LOW);                         // enable = LOW
    delay(100);
  }
 
  delay(1200);                                                //drive out of black area, for activating the next while loop

 
  while (digitalRead(ir_pin) == 0) {                          // while paper sensor reads white, loop in this while loop
    // digitalWrite(led_pin, LOW);                            // turn control LED off
    digitalWrite(ENABLE_MOTORS, LOW);                         // motors shall run
  }
                                                              // if sensor detect black mark, it's jumping to next part
                                                              
  for (pwm1 = 90; pwm1 >= 0; pwm1 -=10) {                     // ramp speed down for all motors
    analogWrite(PWM_M1, pwm1);  digitalWrite(DIR_M1, dir1);
    analogWrite(PWM_M2, pwm1);  digitalWrite(DIR_M2, dir1);
    analogWrite(PWM_M3, pwm1);  digitalWrite(DIR_M3, dir1);
    analogWrite(PWM_M4, pwm1);  digitalWrite(DIR_M4, dir1);
    digitalWrite(ENABLE_MOTORS, LOW);  // enable = LOW
    delay(100);
  } 
 
  digitalWrite(ENABLE_MOTORS, HIGH);                          // disable = HIGH     // turn motor off
  delay(8000);                                                // Pause, Showtime of Sujet 
                                                              // the real time can be different because of the changed Timer's rate for PWM 

   for (pwm1 = 0; pwm1 < 91 ; pwm1 +=10) {                     // ramp speed up for all motors
    analogWrite(PWM_M1, pwm1);  digitalWrite(DIR_M1, dir1);
    analogWrite(PWM_M2, pwm1);  digitalWrite(DIR_M2, dir1);
    analogWrite(PWM_M3, pwm1);  digitalWrite(DIR_M3, dir1);
    analogWrite(PWM_M4, pwm1);  digitalWrite(DIR_M4, dir1);
    digitalWrite(ENABLE_MOTORS, LOW);                         // enable = LOW
    delay(100);
  }
 
  delay(1200);                                                //drive out of black area, for activating the next while loop

    while (digitalRead(ir_pin) == 0) {                        // while paper sensor reads white, loop in this while loop
    // digitalWrite(led_pin, LOW);                            // turn control LED off
    digitalWrite(ENABLE_MOTORS, LOW);                         // motors shall run
  }
                                                              // if sensor detect black mark, it's jumping to next part

    for (pwm1 = 90; pwm1 >= 0; pwm1 -=10) {                   // ramp speed down for all motors
    analogWrite(PWM_M1, pwm1);  digitalWrite(DIR_M1, dir1);
    analogWrite(PWM_M2, pwm1);  digitalWrite(DIR_M2, dir1);
    analogWrite(PWM_M3, pwm1);  digitalWrite(DIR_M3, dir1);
    analogWrite(PWM_M4, pwm1);  digitalWrite(DIR_M4, dir1);
    digitalWrite(ENABLE_MOTORS, LOW);  // enable = LOW
    delay(100);
  } 

  
 
  digitalWrite(ENABLE_MOTORS, HIGH);                          // disable = HIGH     // turn motor off
  delay(8000);                                                // Pause, Showtime of Sujet 
                                                              // the real time can be different because of the changed Timer's rate for PWM 

   for (pwm1 = 0; pwm1 < 91 ; pwm1 +=10) {                     // ramp speed up for all motors
    analogWrite(PWM_M1, pwm1);  digitalWrite(DIR_M1, dir1);
    analogWrite(PWM_M2, pwm1);  digitalWrite(DIR_M2, dir1);
    analogWrite(PWM_M3, pwm1);  digitalWrite(DIR_M3, dir1);
    analogWrite(PWM_M4, pwm1);  digitalWrite(DIR_M4, dir1);
    digitalWrite(ENABLE_MOTORS, LOW);                         // enable = LOW
    delay(100);
  }
 
  delay(1200);                                                //drive out of black area, for activating the next while loop

    while (digitalRead(ir_pin) == 0) {                        // while paper sensor reads white, loop in this while loop
    // digitalWrite(led_pin, LOW);                            // turn control LED off
    digitalWrite(ENABLE_MOTORS, LOW);                         // motors shall run
  }
                                                              // if sensor detect black mark, it's jumping to next part

    for (pwm1 = 90; pwm1 >= 0; pwm1 -=10) {                   // ramp speed down for all motors
    analogWrite(PWM_M1, pwm1);  digitalWrite(DIR_M1, dir1);
    analogWrite(PWM_M2, pwm1);  digitalWrite(DIR_M2, dir1);
    analogWrite(PWM_M3, pwm1);  digitalWrite(DIR_M3, dir1);
    analogWrite(PWM_M4, pwm1);  digitalWrite(DIR_M4, dir1);
    digitalWrite(ENABLE_MOTORS, LOW);  // enable = LOW
    delay(100);
  } 

  digitalWrite(ENABLE_MOTORS, HIGH);                          // disable = HIGH     // turn motor off, stop until next human is coming
  
 }                                                            

 

}
// ***********************************************
// ************** End Main Loop ******************
// ***********************************************


/*
 * Divides a given PWM pin frequency by a divisor.
 * 
 * The resulting frequency is equal to the base frequency divided by the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64, 256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64, 128, 256, and 1024.
 * 
 * PWM frequencies are tied together in pairs of pins. If one in a pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired (Timer0)
 *   - Pins 9 and 10 are paired (Timer1)
 *   - Pins 3 and 11 are paired (Timer2)
 * 
 * Note that this function will have side effects on anything else that uses timers:
 *   - Changes on pins 5, 6 may cause the delay() and millis() functions to stop working. Other timing-related functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function incorrectly.
 * 
 * Thanks to macegr of the Arduino forums for his documentation of the PWM frequency divisors. His post can be viewed at: http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1235060559/0#4
 */
 
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 4 || pin == 13) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR0B = TCCR0B & 0b11111000 | mode; // Timer0
  }


   else if(pin == 11 || pin == 12) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    TCCR1B = TCCR1B & 0b11111000 | mode; // Timer1
  }

  
   else if(pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode; // Timer2
  }

  
   else if(pin == 2 || pin == 3 || pin == 5) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    TCCR3B = TCCR3B & 0b11111000 | mode; // Timer3
  }


   else if(pin == 6 || pin == 7 || pin == 8) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    TCCR4B = TCCR4B & 0b11111000 | mode; // Timer4
  }

   else if(pin == 44 || pin == 45 || pin == 46) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    TCCR5B = TCCR5B & 0b11111000 | mode; // Timer5
  }

}
