/****************************************************************************************************************************
  PWM_Multi_Args.ino
  For STM32F/L/H/G/WB/MP1 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/STM32_PWM
  Licensed under MIT license

  Hardware-based multi-channel PWM wrapper library for STM32F/L/H/G/WB/MP1 boards

  Version: 1.0.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      30/09/2021 Initial coding for STM32F/L/H/G/WB/MP1 boards
*****************************************************************************************************************************/

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

#define LED_ON        LOW
#define LED_OFF       HIGH

#ifndef LED_BUILTIN
  #define LED_BUILTIN       13
#endif

#ifndef LED_BLUE
  #define LED_BLUE          2
#endif

#ifndef LED_RED
  #define LED_RED           3
#endif

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

// For F103CB
//

// Change the pin according to your STM32 board. There is no single definition for all boards.

#if ( defined(STM32F1) && ( defined(ARDUINO_BLUEPILL_F103CB) || defined(ARDUINO_BLUEPILL_F103C8) ) )

#warning Using BLUEPILL_F103CB / BLUEPILL_F103C8 pins

// For F103CB => pins[] = pin0, pin4, pin10 ============>> TimerIndex = 1, 2, 0
uint32_t pins[]       = { pin0, pin4, pin10 };

#elif ( defined(STM32F7) && defined(ARDUINO_NUCLEO_F767ZI) )

#warning Using NUCLEO_F767ZI pins

// For F767ZI => pins[] = pin0, pin3, pin9/10 ============>> TimerIndex = 1, 0, 3
uint32_t pins[]       = { pin0, pin3, pin9 };

#elif ( defined(STM32L5) && defined(ARDUINO_NUCLEO_L552ZE_Q) )

#warning Using NUCLEO_L552ZE_Q pins

// For NUCLEO_L552ZE_Q => pins[] = pin0, pin3, pin9/10 ============>> TimerIndex = 1, 0, 3
uint32_t pins[]       = { pin0, pin3, pin9 };

#elif ( defined(STM32H7) && defined(ARDUINO_NUCLEO_H743ZI2) )

#warning Using NUCLEO_H743ZI2 pins

// For NUCLEO_L552ZE_Q => pins[] = pin0, pin3, pin9/10 ============>> TimerIndex = 1, 0, 3
uint32_t pins[]       = { pin0, pin3, pin9 };

#else

// For ??? => pins[] = pin0, pin3, pin9/10 ============>> TimerIndex = 1, 0, 3
uint32_t pins[]       = { pin0, pin3, pin9 };

#endif

//////////////////////////////////////////////////////

#define NUM_OF_PINS       ( sizeof(pins) / sizeof(uint32_t) )

#define NUMBER_ISR_PWMS         NUM_OF_PINS

uint32_t dutyCycle[NUM_OF_PINS]  = { 10, 20, 30 };

uint32_t freq[]       = { 1, 2, 5 };

TIM_TypeDef *TimerInstance[] = { TIM1, TIM2, TIM3 };

uint32_t callbackTime[] = { 0, 0, 0 };

//////////////////////////////////////////////////////

typedef void (*timerPWMCallback)  (uint32_t* data);

void PeriodCallback0(uint32_t* cbTime)
{
  static bool ledON = LED_OFF;

  *cbTime = *cbTime + 1;

  digitalWrite(LED_BUILTIN, ledON);

  ledON = !ledON;
}


void PeriodCallback1(uint32_t* cbTime)
{
  static bool ledON = LED_OFF;

  *cbTime = *cbTime + 1;

  digitalWrite(LED_BLUE, ledON);

  ledON = !ledON;
}

void PeriodCallback2(uint32_t* cbTime)
{
  static bool ledON = LED_OFF;

  *cbTime = *cbTime + 1;

  digitalWrite(LED_RED, ledON);

  ledON = !ledON;
}

//////////////////////////////////////////////////////

timerPWMCallback PeriodCallback[] =
{
  PeriodCallback0,  PeriodCallback1,  PeriodCallback2
};

//////////////////////////////////////////////////////

void printLine()
{
  Serial.println(
    F("\n=========================================================================================================="));
}

void printCount()
{
  static uint32_t num = 0;

  if (num++ % 50 == 0)
  {
    printLine();

    for (uint8_t index = 0; index < NUM_OF_PINS; index++)
    {
      Serial.print(F("Count "));
      Serial.print(index);
      Serial.print(F("\t\t"));
    }

    printLine();
  }

  if (num > 1)
  {
    for (uint8_t index = 0; index < NUM_OF_PINS; index++)
    {
      Serial.print(callbackTime[index]);
      Serial.print(F("\t\t"));
    }

    Serial.println();
  }
}

//////////////////////////////////////////////////////

#define PRINT_INTERVAL    10000L

void check_status()
{
  static unsigned long checkstatus_timeout = 0;

  // Print every PRINT_INTERVAL (10) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    printCount();
    checkstatus_timeout = millis() + PRINT_INTERVAL;
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  digitalWrite(LED_BUILTIN, LED_OFF);
  digitalWrite(LED_BLUE, LED_OFF);
  digitalWrite(LED_RED, LED_OFF);

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    pinMode(pins[index], OUTPUT);
    digitalWrite(pins[index], LOW);
  }

  Serial.begin(115200);

  while (!Serial);

  delay(100);

  Serial.print(F("\nStarting PWM_Multi_Args on "));
  Serial.println(BOARD_NAME);
  Serial.println(STM32_PWM_VERSION);

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    //TIM_TypeDef *Instance = TimerInstance[index];

    uint16_t Timer_Used[NUM_OF_PINS];

    // Set unused TimerIndex = 0xFFFF
    memset(Timer_Used, 0xFFFF, sizeof(Timer_Used));

    // Using pin = PA0, PA1, etc.
    PinName pinNameToUse = digitalPinToPinName(pins[index]);

    // Automatically retrieve TIM instance and channel associated to pin
    // This is used to be compatible with all STM32 series automatically.
    TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(pinNameToUse, PinMap_PWM);

    if (Instance != nullptr)
    {
      uint8_t timerIndex = get_timer_index(Instance);

      // pin => 0, 1, etc
      uint32_t channel = STM_PIN_CHANNEL(pinmap_function( pinNameToUse, PinMap_PWM));

      Serial.print("Index = ");
      Serial.print(index);
      Serial.print(", Instance = 0x");
      Serial.print( (uint32_t) Instance, HEX);
      Serial.print(", channel = ");
      Serial.print(channel);
      Serial.print(", TimerIndex = ");
      Serial.print(get_timer_index(Instance));
      Serial.print(", PinName = ");
      Serial.println( pinNameToUse );

      for ( uint8_t i = 0; i < index; i++)
      {
        if (timerIndex == Timer_Used[i])
        {
          Serial.print("ERROR => Already used TimerIndex for index = ");
          Serial.println(index);
          Serial.print(", TimerIndex  = ");
          Serial.println(timerIndex);

          //break;
          while (true)
            delay(100);
        }
      }

      // Update Timer_Used with current timerIndex
      Timer_Used[index] = timerIndex;

      HardwareTimer *MyTim = new HardwareTimer(Instance);

      MyTim->setMode(channel, TIMER_OUTPUT_COMPARE_PWM1, pins[index]);
      MyTim->setOverflow(freq[index], HERTZ_FORMAT);
      MyTim->setCaptureCompare(channel, dutyCycle[index], PERCENT_COMPARE_FORMAT);

      if (PeriodCallback[index])
      {
        MyTim->attachInterrupt( std::bind(PeriodCallback[index], &callbackTime[index]) );
      }

      MyTim->resume();
    }
    else
    {
      Serial.print("ERROR => Wrong pin, You have to select another one. Skip NULL Instance for index = ");
      Serial.println(index);
    }
  }
}

void loop()
{
  check_status();
}
