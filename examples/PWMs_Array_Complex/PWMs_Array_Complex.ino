/****************************************************************************************************************************
  PWMs_Array_Complex.ino
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

#define boolean           bool

#include <SimpleTimer.h>              // https://github.com/jfturcot/SimpleTimer

#define HW_TIMER_INTERVAL_US      10L

volatile uint64_t startMicros = 0;


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
// Pin PA0 (D32), D3 and D9
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

#define NUM_OF_PINS             ( sizeof(pins) / sizeof(uint32_t) )

#define NUMBER_ISR_PWMS         NUM_OF_PINS

TIM_TypeDef *TimerInstance[] = { TIM1, TIM2, TIM3 };

volatile uint32_t callbackTime[] = { 0, 0, 0 };

//////////////////////////////////////////////////////

#define USE_COMPLEX_STRUCT      true

//////////////////////////////////////////////////////

#if USE_COMPLEX_STRUCT

typedef struct
{
  //uint32_t      PWM_Pin;

  callback_function_t irqCallbackStartFunc;
  callback_function_t irqCallbackStopFunc;

  uint32_t      PWM_Freq;
  uint32_t      PWM_DutyCycle;

  uint64_t      deltaMicrosStart;
  uint64_t      previousMicrosStart;

  uint64_t      deltaMicrosStop;
  uint64_t      previousMicrosStop;

} ISR_PWM_Data;

// Avoid doing something fancy in ISR, for example Serial.print()
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash

void doingSomethingStart(int index);

void doingSomethingStop(int index);

#else   // #if USE_COMPLEX_STRUCT

volatile uint64_t deltaMicrosStart    [] = { 0, 0, 0 };
volatile uint64_t previousMicrosStart [] = { 0, 0, 0 };

volatile uint64_t deltaMicrosStop     [] = { 0, 0, 0 };
volatile uint64_t previousMicrosStop  [] = { 0, 0, 0 };

// You can assign any interval for any timer here, in Hz
uint32_t PWM_Freq[] =
{
  10, 20, 500
};

// You can assign any interval for any timer here, in milliseconds
uint32_t PWM_DutyCycle[] =
{
  20, 30, 50
};

void doingSomethingStart(int index)
{
  uint64_t currentMicros  = micros();

  deltaMicrosStart[index]    = currentMicros - previousMicrosStart[index];
  previousMicrosStart[index] = currentMicros;
}

void doingSomethingStop(int index)
{
  uint64_t currentMicros  = micros();

  // Count from start to stop PWM pulse
  deltaMicrosStop[index]    = currentMicros - previousMicrosStart[index];

  previousMicrosStop[index] = currentMicros;
}

#endif    // #if USE_COMPLEX_STRUCT

////////////////////////////////////
// Shared
////////////////////////////////////

void doingSomethingStart0()
{
  static bool ledON = LED_OFF;

  doingSomethingStart(0);

  callbackTime[0]++;

  digitalWrite(LED_BUILTIN, ledON);

  ledON = !ledON;
}

void doingSomethingStart1()
{
  static bool ledON = LED_OFF;

  doingSomethingStart(1);

  callbackTime[1]++;

  digitalWrite(LED_BLUE, ledON);

  ledON = !ledON;
}

void doingSomethingStart2()
{
  static bool ledON = LED_OFF;

  doingSomethingStart(2);

  digitalWrite(LED_RED, ledON);

  callbackTime[2]++;

  ledON = !ledON;
}


//////////////////////////////////////////////////////

void doingSomethingStop0()
{
  doingSomethingStop(0);
}

void doingSomethingStop1()
{
  doingSomethingStop(1);
}

void doingSomethingStop2()
{
  doingSomethingStop(2);
}


//////////////////////////////////////////////////////

#if USE_COMPLEX_STRUCT

ISR_PWM_Data curISR_PWM_Data[] =
{
  // irqCallbackStartFunc, irqCallbackStopFunc, PWM_Freq, deltaMicrosStart, previousMicrosStart, deltaMicrosStop, previousMicrosStop
  { doingSomethingStart0,    doingSomethingStop0,     10,  20, 0, 0, 0, 0 },
  { doingSomethingStart1,    doingSomethingStop1,     20,  30, 0, 0, 0, 0 },
  { doingSomethingStart2,    doingSomethingStop2,    500,  50, 0, 0, 0, 0 },
};

void doingSomethingStart(int index)
{
  uint64_t currentMicros  = micros();

  curISR_PWM_Data[index].deltaMicrosStart    = currentMicros - curISR_PWM_Data[index].previousMicrosStart;
  curISR_PWM_Data[index].previousMicrosStart = currentMicros;
}

void doingSomethingStop(int index)
{
  uint64_t currentMicros  = micros();

  // Count from start to stop PWM pulse
  curISR_PWM_Data[index].deltaMicrosStop     = currentMicros - curISR_PWM_Data[index].previousMicrosStart;
  curISR_PWM_Data[index].previousMicrosStop  = currentMicros;
}

#else   // #if USE_COMPLEX_STRUCT

callback_function_t irqCallbackStartFunc[] =
{
  doingSomethingStart0,   doingSomethingStart1,  doingSomethingStart2
};

callback_function_t irqCallbackStopFunc[] =
{
  doingSomethingStop0,    doingSomethingStop1,  doingSomethingStop2
};

#endif    // #if USE_COMPLEX_STRUCT

//////////////////////////////////////////////////////

#define SIMPLE_TIMER_MS        2000L

// Init SimpleTimer
SimpleTimer simpleTimer;

// Here is software Timer, you can do somewhat fancy stuffs without many issues.
// But always avoid
// 1. Long delay() it just doing nothing and pain-without-gain wasting CPU power.Plan and design your code / strategy ahead
// 2. Very long "do", "while", "for" loops without predetermined exit time.
void simpleTimerDoingSomething2s()
{
  static uint64_t previousMicrosStart = startMicros;

  uint64_t currMicros = micros();

  Serial.print(F("SimpleTimer (ms): "));
  Serial.print(SIMPLE_TIMER_MS);
  Serial.print(F(", us : "));
  Serial.print(currMicros);
  Serial.print(F(", Dus : "));
  Serial.println(currMicros - previousMicrosStart);

  for (uint16_t i = 0; i < NUMBER_ISR_PWMS; i++)
  {
#if USE_COMPLEX_STRUCT
    Serial.print(F("PWM Channel : "));
    Serial.print(i);
    Serial.print(F(", programmed Period (us): "));

    Serial.print(1000000 / curISR_PWM_Data[i].PWM_Freq);

    Serial.print(F(", actual : "));
    Serial.print(curISR_PWM_Data[i].deltaMicrosStart);

    Serial.print(F(", programmed DutyCycle : "));

    Serial.print(curISR_PWM_Data[i].PWM_DutyCycle);

    Serial.print(F(", actual : "));
    Serial.println((float) curISR_PWM_Data[i].deltaMicrosStop * 100.0f / curISR_PWM_Data[i].deltaMicrosStart);

#else

    Serial.print(F("PWM Channel : "));
    Serial.print(i);

    Serial.print(1000000 / PWM_Freq[i]);

    Serial.print(F(", programmed Period (us): "));
    Serial.print(1000000 / PWM_Freq[i]);
    Serial.print(F(", actual : "));
    Serial.print(deltaMicrosStart[i]);

    Serial.print(F(", programmed DutyCycle : "));

    Serial.print(PWM_DutyCycle[i]);

    Serial.print(F(", actual : "));
    Serial.println( (float) deltaMicrosStop[i] * 100.0f / deltaMicrosStart[i]);
#endif
  }

  previousMicrosStart = currMicros;
}

//////////////////////////////////////////////////////

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

  delay(2000);

  Serial.print(F("\nStarting PWMs_Array_Complex on "));
  Serial.println(BOARD_NAME);
  Serial.println(STM32_PWM_VERSION);
  // Automatically retrieve TIM instance and channel associated to pin
  // This is used to be compatible with all STM32 series automatically.

  startMicros = micros();

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

#if USE_COMPLEX_STRUCT

      MyTim->setPWM(channel, pins[index], curISR_PWM_Data[index].PWM_Freq, curISR_PWM_Data[index].PWM_DutyCycle,
                    curISR_PWM_Data[index].irqCallbackStartFunc, curISR_PWM_Data[index].irqCallbackStopFunc);

#else

      MyTim->setPWM(channel, pins[index], PWM_Freq[index], PWM_DutyCycle[index], irqCallbackStartFunc[index],
                    irqCallbackStopFunc[index]);

#endif
    }
    else
    {
      Serial.print("ERROR => Wrong pin, You have to select another one. Skip NULL Instance for index = ");
      Serial.println(index);
    }
  }

  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  simpleTimer.setInterval(SIMPLE_TIMER_MS, simpleTimerDoingSomething2s);
}

//////////////////////////////////////////////////////

#define BLOCKING_TIME_MS      10000L

void loop()
{
  // This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
  // You see the time elapse of ISR_PWM still accurate, whereas very unaccurate for Software Timer
  // The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
  // While that of ISR_PWM is still prefect.
  delay(BLOCKING_TIME_MS);

  // You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
  // You don't need to and never call ISR_PWM.run() here in the loop(). It's already handled by ISR timer.
  simpleTimer.run();
}
