/****************************************************************************************************************************
  PWM_StepperControl.ino
  
  For STM32F/L/H/G/WB/MP1 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/STM32_PWM
  Licensed under MIT license

  Hardware-based multi-channel PWM wrapper library for STM32F/L/H/G/WB/MP1 boards
  
  Credits of Paul van Dinther (https://github.com/dinther). Check https://github.com/khoih-prog/RP2040_PWM/issues/16
*****************************************************************************************************************************/

// Use with Stepper-Motor driver, such as TMC2209

#if !( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
       defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
       defined(STM32WB) || defined(STM32MP1) || defined(STM32L5))
  #error This code is designed to run on STM32F/L/H/G/WB/MP1 platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "ESP32_PWM.h"
// _PWM_LOGLEVEL_ from 0 to 4
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_      4

#define USING_MICROS_RESOLUTION       true    //false 

#include "STM32_PWM.h"

/////////////////////////////////////////////////

// Change the pin according to your STM32 board. There is no single definition for all boards.
#define pin0    PA0
#define pin1    D1
#define pin2    D2
#define pin3    D3
#define pin4    D4
#define pin5    D5
#define pin6    D6
#define pin7    D7

#define pin8    D8
#define pin9    D9
#define pin10   D10
#define pin11   D11
#define pin12   D12
#define pin13   D13
#define pin14   D14
#define pin15   D15
#define pin16   D16

//////////////////////////////////////////////////////

// Change the pin according to your STM32 board. There is no single definition for all boards.

#if ( defined(STM32F1) && ( defined(ARDUINO_BLUEPILL_F103CB) || defined(ARDUINO_BLUEPILL_F103C8) ) )

  #warning Using BLUEPILL_F103CB / BLUEPILL_F103C8 pins

  // For F103CB => pin0, pin4, pin10 ============>> TimerIndex = 1, 2, 0
  #define STEP_PIN        pin3
  
#elif ( defined(STM32F7) && defined(ARDUINO_NUCLEO_F767ZI) )

  #warning Using NUCLEO_F767ZI pins
  
  // For F767ZI => pin0, pin3, pin9/10 ============>> TimerIndex = 1, 0, 3
  #define STEP_PIN        pin3

#elif ( defined(STM32L5) && defined(ARDUINO_NUCLEO_L552ZE_Q) )

  #warning Using NUCLEO_L552ZE_Q pins
  
  // For NUCLEO_L552ZE_Q => pin0, pin3, pin9/10 ============>> TimerIndex = 1, 0, 3
  #define STEP_PIN        pin3

#elif ( defined(STM32H7) && defined(ARDUINO_NUCLEO_H743ZI2) )

  #warning Using NUCLEO_H743ZI2 pins
  
  // For NUCLEO_L552ZE_Q => pin0, pin3, pin9/10 ============>> TimerIndex = 1, 0, 3
  #define STEP_PIN        pin3

#else

  // For ??? => pin0, pin3, pin9/10 ============>> TimerIndex = 1, 0, 3
  #define STEP_PIN        pin3

#endif

//////////////////////////////////////////////////////

#define DIR_PIN         pin1

TIM_TypeDef *stepperTimer;

HardwareTimer *stepper;

uint32_t channel;

int previousSpeed = 0;

// The Stepper RPM will be ( speed * 60 ) / steps-per-rev
// For example, 28BYJ-48 Stepper Motor (https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/) has 32 Steps/Rev
// Speed = 640 Hz => Stepper RPM = (640 * 60 / 32) = 1200 RPM
void setSpeed(int speed)
{
  // Do nothing if same speed
  if (speed == previousSpeed)
    return;

  Serial.print(F("setSpeed = "));
  Serial.println(speed);

  // Create new instance for new speed  
  if (stepper)
    delete stepper;
    
  stepper = new HardwareTimer(stepperTimer);
  
  if (speed == 0)
  {
    // Use DC = 0 to stop stepper
    stepper->setPWM(channel, STEP_PIN, 500, 0, nullptr);
  }
  else
  {
    //  Set the frequency of the PWM output and a duty cycle of 50%
    digitalWrite(DIR_PIN, (speed < 0));
    stepper->setPWM(channel, STEP_PIN, abs(speed), 50, nullptr);
  }

  previousSpeed = speed;
}

void initPWM(uint32_t step_pin)
{
  // Using pin = PA0, PA1, etc.
  PinName pinNameToUse = digitalPinToPinName(step_pin);

  // Automatically retrieve TIM instance and channel associated to pin
  // This is used to be compatible with all STM32 series automatically.
  stepperTimer = (TIM_TypeDef *) pinmap_peripheral(pinNameToUse, PinMap_PWM);

  if (stepperTimer != nullptr)
  {
    uint8_t timerIndex = get_timer_index(stepperTimer);

    // pin => 0, 1, etc
    channel = STM_PIN_CHANNEL(pinmap_function( pinNameToUse, PinMap_PWM));

    Serial.print("stepperTimer = 0x");
    Serial.print( (uint32_t) stepperTimer, HEX);
    Serial.print(", channel = ");
    Serial.print(channel);
    Serial.print(", TimerIndex = ");
    Serial.print(get_timer_index(stepperTimer));
    Serial.print(", PinName = ");
    Serial.println( pinNameToUse );

    stepper = new HardwareTimer(stepperTimer);

    // SetPWM object and passed just a random frequency of 500 steps/s
    // The Stepper RPM will be ( speed * 60 ) / steps-per-revolution
    // The duty cycle is how you turn the motor on and off
    previousSpeed = 500;
    stepper->setPWM(channel, step_pin, previousSpeed, 0, nullptr);
  }
  else
  {
    Serial.println("ERROR => Wrong pin, You have to select another one. Skip NULL stepperTimer");
  }  
}

void setup() 
{
  pinMode(STEP_PIN, OUTPUT);
  digitalWrite(STEP_PIN, LOW);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(DIR_PIN, LOW);
  
  Serial.begin(115200);

  while (!Serial && millis() < 5000);

  delay(100);

  Serial.print(F("\nStarting PWM_StepperControl on "));
  Serial.println(BOARD_NAME);
  Serial.println(STM32_PWM_VERSION);

  initPWM(STEP_PIN);
}

void loop() 
{
  // The Stepper RPM will be ( speed * 60 ) / steps-per-rev
  setSpeed(2000);
  delay(3000);

  // Stop before reversing
  setSpeed(0);
  delay(3000);

  // Reversing
  setSpeed(-1000);
  delay(3000);

  // Stop before reversing
  setSpeed(0);
  delay(3000);
}
