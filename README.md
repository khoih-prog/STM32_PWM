# STM32_PWM Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/STM32_PWM.svg?)](https://www.ardu-badge.com/STM32_PWM)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/STM32_PWM.svg)](https://github.com/khoih-prog/STM32_PWM/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/STM32_PWM/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/STM32_PWM.svg)](http://github.com/khoih-prog/STM32_PWM/issues)



<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>


---
---

## Table of Contents

* [Why do we need this STM32_PWM library](#why-do-we-need-this-STM32_PWM-library)
  * [Features](#features)
  * [Why using hardware-based PWM is the best](#why-using-hardware-based-pwm-is-the-best)
  * [Currently supported Boards](#currently-supported-boards)
  * [Important Notes about ISR](#important-notes-about-isr)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [More useful Information about STM32 Timers](#more-useful-information-about-stm32-timers)
* [Available Timers for STM32](#available-timers-for-STM32)
* [Usage](#usage)
  * [1. Init Hardware Timer](#1-init-hardware-timer)
  * [2. Set PWM Frequency, dutycycle, attach irqCallbackStartFunc and irqCallbackStopFunc functions](#2-Set-PWM-Frequency-dutycycle-attach-irqCallbackStartFunc-and-irqCallbackStopFunc-functions)
* [Examples](#examples)
  * [ 1. PWM_Multi](examples/PWM_Multi)
  * [ 2. PWM_Multi_Args](examples/PWM_Multi_Args)
  * [ 3. PWMs_Array_Complex](examples/PWMs_Array_Complex)
  * [ 4. PWM_StepperControl](examples/PWM_StepperControl) **New**
* [Example PWM_StepperControl](#example-PWM_StepperControl)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. PWMs_Array_Complex on NUCLEO_F767ZI](#1-PWMs_Array_Complex-on-NUCLEO_F767ZI)
  * [2. PWMs_Array_Complex on NUCLEO_H743ZI2](#2-PWMs_Array_Complex-on-NUCLEO_H743ZI2)
  * [3. PWMs_Array_Complex on NUCLEO_L552ZE_Q](#3-PWMs_Array_Complex-on-NUCLEO_L552ZE_Q)
  * [4. PWMs_Array_Complex on BLUEPILL_F103CB](#4-PWMs_Array_Complex-on-BLUEPILL_F103CB)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Why do we need this [STM32_PWM library](https://github.com/khoih-prog/STM32_PWM)

### Features

This **wrapper library** enables you to use Hardware-based PWM on STM32F/L/H/G/WB/MP1 boards to create and output PWM to pins.

This library is using the **same or similar functions** as other FastPWM libraries, as follows, to enable you to **port your PWM code easily between platforms**

 1. [**RP2040_PWM**](https://github.com/khoih-prog/RP2040_PWM)
 2. [**AVR_PWM**](https://github.com/khoih-prog/AVR_PWM)
 3. [**megaAVR_PWM**](https://github.com/khoih-prog/megaAVR_PWM)
 4. [**ESP32_FastPWM**](https://github.com/khoih-prog/ESP32_FastPWM)
 5. [**SAMD_PWM**](https://github.com/khoih-prog/SAMD_PWM)
 6. [**SAMDUE_PWM**](https://github.com/khoih-prog/SAMDUE_PWM)
 7. [**nRF52_PWM**](https://github.com/khoih-prog/nRF52_PWM)
 8. [**Teensy_PWM**](https://github.com/khoih-prog/Teensy_PWM)
 9. [**ATtiny_PWM**](https://github.com/khoih-prog/ATtiny_PWM)
10. [**Dx_PWM**](https://github.com/khoih-prog/Dx_PWM)
11. [**Portenta_H7_PWM**](https://github.com/khoih-prog/Portenta_H7_PWM)
12. [**MBED_RP2040_PWM**](https://github.com/khoih-prog/MBED_RP2040_PWM)
13. [**nRF52_MBED_PWM**](https://github.com/khoih-prog/nRF52_MBED_PWM)
14. [**STM32_PWM**](https://github.com/khoih-prog/STM32_PWM)

---

The most important feature is they're purely hardware-based PWM channels. Therefore, their executions are **very precise and not blocked by bad-behaving functions or tasks**. This important feature is absolutely necessary for mission-critical tasks. 

These hardware PWM channels still work even if other functions are blocking. Moreover, they are much more precise (certainly depending on clock frequency accuracy) than other ISR-based or software-based PWM using millis() or micros(). That's necessary if you need to measure some data requiring very high frequency and much better accuracy.

The [**PWMs_Array_Complex**](examples/PWMs_Array_Complex) example will demonstrate the nearly perfect accuracy, compared to software timers, by printing the actual period / duty-cycle in `microsecs` of each of PWM-channels.

The [**PWM_Multi_Args**](examples/PWM/PWM_Multi_Args) will demonstrate the usage of multichannel PWM using multiple Hardware Timers. The 4 independent Hardware Timers are used **to control 4 different PWM outputs**, with totally independent frequencies and dutycycles.

Being hardware-based PWM, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet or Blynk services.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see `software-based` SimpleTimer is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task in loop(), using delay() function as an example. The elapsed time then is very unaccurate

---

#### Why using hardware-based PWM is the best

Imagine you have a system with a **mission-critical** function, controlling a self-driving machine or robot or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is **blocking** the loop() or setup().

So your function **might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use hardware-based PWM to control the PWM motors.

These hardware-based PWM channels, still work even if other functions are blocking. Moreover, they are much more **precise** (certainly depending on clock frequency accuracy) than other ISR-based or software-based PWM using millis() or micros(). That's necessary if you need to measure some data requiring very high accuracy.

Functions using normal software-based PWM, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.


---

### Currently supported Boards

1. **STM32F/L/H/G/WB/MP1 boards** such as NUCLEO_H743ZI2, NUCLEO_L552ZE_Q, NUCLEO_F767ZI, BLUEPILL_F103CB, etc., using [`Arduino Core for STM32`](https://github.com/stm32duino/Arduino_Core_STM32)

---

#### Important Notes about ISR

1. Inside the attached function, **delay() won’t work and the value returned by millis() will not increment.** Serial data received while in the function may be lost. You should declare as **volatile any variables that you modify within the attached function.**

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

---
---

## Prerequisites

 1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
 2. [`Arduino Core for STM32 v2.4.0+`](https://github.com/stm32duino/Arduino_Core_STM32) for STM32F/L/H/G/WB/MP1 boards. [![GitHub release](https://img.shields.io/github/release/stm32duino/Arduino_Core_STM32.svg)](https://github.com/stm32duino/Arduino_Core_STM32/releases/latest)
 3. To use with certain example
   - [`SimpleTimer library`](https://github.com/jfturcot/SimpleTimer) for [ISR_16_Timers_Array example](examples/ISR_16_Timers_Array).
---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**STM32_PWM**](https://github.com/khoih-prog/STM32_PWM), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/STM32_PWM.svg?)](https://www.ardu-badge.com/STM32_PWM) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**STM32_PWM**](https://github.com/khoih-prog/STM32_PWM) page.
2. Download the latest release `STM32_PWM-main.zip`.
3. Extract the zip file to `STM32_PWM-main` directory 
4. Copy whole `STM32_PWM-main` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**STM32_PWM** library](https://registry.platformio.org/libraries/khoih-prog/STM32_PWM) by using [Library Manager](https://registry.platformio.org/libraries/khoih-progSTM32_PWM/installation). Search for **STM32_PWM** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---


## More useful Information about STM32 Timers

The Timers of STM32s are numerous, yet very sophisticated and powerful.

In general, across the STM32 microcontrollers families, the timer peripherals that have the same name also have the same features set, but there are a few exceptions. 

The general purpose timers embedded by the STM32 microcontrollers share the same backbone structure; they differ only on the level of features embedded by a given timer peripheral. 

The level of features integration for a given timer peripheral is decided based on the applications field that it targets.

The timer peripherals can be classified as:

• Advanced-configuration timers like TIM1 and TIM8 among others.
• General-purpose configuration timers like TIM2 and TIM3 among others
• Lite-configuration timers like TIM9, TIM10, TIM12 and TIM16 among others
• Basic-configuration timers like TIM6 and TIM7 among others.


More information can be found at [**Embedded-Lab STM32 TIMERS**](http://embedded-lab.com/blog/stm32-timers/)

To be sure which Timer is available for the board you're using, check the Core Package's related files. For example, for **STM32 using STM32H747XI**, check this file:

1. `~/.arduino15/packages/STM32/hardware/stm32/2.0.0/system/Drivers/CMSIS/Device/ST/STM32H7xx/Include/stm32h7xx.h`


The information will be as follows:

```cpp
typedef struct
{
  __IO uint32_t CR1;         /*!< TIM control register 1,                   Address offset: 0x00 */
  __IO uint32_t CR2;         /*!< TIM control register 2,                   Address offset: 0x04 */
  __IO uint32_t SMCR;        /*!< TIM slave mode control register,          Address offset: 0x08 */
  __IO uint32_t DIER;        /*!< TIM DMA/interrupt enable register,        Address offset: 0x0C */
  __IO uint32_t SR;          /*!< TIM status register,                      Address offset: 0x10 */
  __IO uint32_t EGR;         /*!< TIM event generation register,            Address offset: 0x14 */
  __IO uint32_t CCMR1;       /*!< TIM capture/compare mode register 1,      Address offset: 0x18 */
  __IO uint32_t CCMR2;       /*!< TIM capture/compare mode register 2,      Address offset: 0x1C */
  __IO uint32_t CCER;        /*!< TIM capture/compare enable register,      Address offset: 0x20 */
  __IO uint32_t CNT;         /*!< TIM counter register,                     Address offset: 0x24 */
  __IO uint32_t PSC;         /*!< TIM prescaler,                            Address offset: 0x28 */
  __IO uint32_t ARR;         /*!< TIM auto-reload register,                 Address offset: 0x2C */
  __IO uint32_t RCR;         /*!< TIM repetition counter register,          Address offset: 0x30 */
  __IO uint32_t CCR1;        /*!< TIM capture/compare register 1,           Address offset: 0x34 */
  __IO uint32_t CCR2;        /*!< TIM capture/compare register 2,           Address offset: 0x38 */
  __IO uint32_t CCR3;        /*!< TIM capture/compare register 3,           Address offset: 0x3C */
  __IO uint32_t CCR4;        /*!< TIM capture/compare register 4,           Address offset: 0x40 */
  __IO uint32_t BDTR;        /*!< TIM break and dead-time register,         Address offset: 0x44 */
  __IO uint32_t DCR;         /*!< TIM DMA control register,                 Address offset: 0x48 */
  __IO uint32_t DMAR;        /*!< TIM DMA address for full transfer,        Address offset: 0x4C */
  uint32_t      RESERVED1;   /*!< Reserved, 0x50                                                 */
  __IO uint32_t CCMR3;       /*!< TIM capture/compare mode register 3,      Address offset: 0x54 */
  __IO uint32_t CCR5;        /*!< TIM capture/compare register5,            Address offset: 0x58 */
  __IO uint32_t CCR6;        /*!< TIM capture/compare register6,            Address offset: 0x5C */
  __IO uint32_t AF1;         /*!< TIM alternate function option register 1, Address offset: 0x60 */
  __IO uint32_t AF2;         /*!< TIM alternate function option register 2, Address offset: 0x64 */
  __IO uint32_t TISEL;       /*!< TIM Input Selection register,             Address offset: 0x68 */
} TIM_TypeDef;
```

and

```cpp
#define PERIPH_BASE            0x40000000UL /*!< Base address of : AHB/ABP Peripherals   
/*!< Peripheral memory map */
#define APB1PERIPH_BASE        PERIPH_BASE

/*!< APB1 peripherals */
/*!< D2_APB1PERIPH peripherals */
#define TIM2_BASE             (D2_APB1PERIPH_BASE + 0x0000UL)
#define TIM3_BASE             (D2_APB1PERIPH_BASE + 0x0400UL)
#define TIM4_BASE             (D2_APB1PERIPH_BASE + 0x0800UL)
#define TIM5_BASE             (D2_APB1PERIPH_BASE + 0x0C00UL)
#define TIM6_BASE             (D2_APB1PERIPH_BASE + 0x1000UL)
#define TIM7_BASE             (D2_APB1PERIPH_BASE + 0x1400UL)
#define TIM12_BASE            (D2_APB1PERIPH_BASE + 0x1800UL)
#define TIM13_BASE            (D2_APB1PERIPH_BASE + 0x1C00UL)
#define TIM14_BASE            (D2_APB1PERIPH_BASE + 0x2000UL)

/*!< APB2 peripherals */
#define TIM1_BASE             (D2_APB2PERIPH_BASE + 0x0000UL)
#define TIM8_BASE             (D2_APB2PERIPH_BASE + 0x0400UL)
...
#define TIM9_BASE             (APB2PERIPH_BASE + 0x4000UL)
#define TIM10_BASE            (APB2PERIPH_BASE + 0x4400UL)
#define TIM11_BASE            (APB2PERIPH_BASE + 0x4800UL)
...
#define TI15_BASE            (D2_APB2PERIPH_BASE + 0x4000UL)
#define TIM16_BASE            (D2_APB2PERIPH_BASE + 0x4400UL)
#define TIM17_BASE            (D2_APB2PERIPH_BASE + 0x4800UL)
...
#define HRTIM1_BASE           (D2_APB2PERIPH_BASE + 0x7400UL)
#define HRTIM1_TIMA_BASE      (HRTIM1_BASE + 0x00000080UL)
#define HRTIM1_TIMB_BASE      (HRTIM1_BASE + 0x00000100UL)
#define HRTIM1_TIMC_BASE      (HRTIM1_BASE + 0x00000180UL)
#define HRTIM1_TIMD_BASE      (HRTIM1_BASE + 0x00000200UL)
#define HRTIM1_TIME_BASE      (HRTIM1_BASE + 0x00000280UL)
#define HRTIM1_COMMON_BASE    (HRTIM1_BASE + 0x00000380UL)
...
#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)
#define TIM5                ((TIM_TypeDef *) TIM5_BASE)
#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
#define TIM7                ((TIM_TypeDef *) TIM7_BASE)
#define TIM13               ((TIM_TypeDef *) TIM13_BASE)
#define TIM14               ((TIM_TypeDef *) TIM14_BASE)
...
#define TIM1                ((TIM_TypeDef *) TIM1_BASE)
#define TIM8                ((TIM_TypeDef *) TIM8_BASE)
...
#define TIM12               ((TIM_TypeDef *) TIM12_BASE)
#define TIM15               ((TIM_TypeDef *) TIM15_BASE)
#define TIM16               ((TIM_TypeDef *) TIM16_BASE)
#define TIM17               ((TIM_TypeDef *) TIM17_BASE)
...
#define HRTIM1              ((HRTIM_TypeDef *) HRTIM1_BASE)
#define HRTIM1_TIMA         ((HRTIM_Timerx_TypeDef *) HRTIM1_TIMA_BASE)
#define HRTIM1_TIMB         ((HRTIM_Timerx_TypeDef *) HRTIM1_TIMB_BASE)
#define HRTIM1_TIMC         ((HRTIM_Timerx_TypeDef *) HRTIM1_TIMC_BASE)
#define HRTIM1_TIMD         ((HRTIM_Timerx_TypeDef *) HRTIM1_TIMD_BASE)
#define HRTIM1_TIME         ((HRTIM_Timerx_TypeDef *) HRTIM1_TIME_BASE)
#define HRTIM1_COMMON       ((HRTIM_Common_TypeDef *) HRTIM1_COMMON_BASE)
```

---

## Available Timers for STM32

This is the temporary list for STM32 Timers which can be used. The available Timers certainly depends on they are being used for other purpose (core, application, libraries, etc.) or not. You have to exhausively test yourself to be sure.

#### 1. OK to use

**TIM1, TIM4, TIM7, TIM8, TIM12, TIM13, TIM14, TIM15, TIM16, TIM17**

#### 2. Not exist

**TIM9, TIM10, TIM11. Only for STM32F2, STM32F4 or STM32L1**

#### 3.Not declared

**TIM18, TIM19, TIM20, TIM21, TIM22**

#### 3. Not OK => conflict or crash

**TIM2, TIM3, TIM5, TIM6**


---
---

## Usage

Before using any Timer for a PWM channel, you have to make sure the Timer has not been used by any other purpose.

#### 1. Init Hardware Timer

```cpp
// Automatically retrieve TIM instance and channel associated to pin
// This is used to be compatible with all STM32 series automatically.
TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(pinNameToUse, PinMap_PWM);
```

#### 2. Set PWM Frequency, dutycycle, attach irqCallbackStartFunc and irqCallbackStopFunc functions

```cpp
void PeriodCallback()
{
}

void setup()
{
  ....
  
  MyTim->setPWM(channel, pins, freq, dutyCycle, PeriodCallback);
}  
```

---
---

### Examples: 

 1. [PWM_Multi](examples/PWM_Multi)
 2. [PWM_Multi_Args](examples/PWM_Multi_Args)
 3. [PWMs_Array_Complex](examples/PWMs_Array_Complex)
 4. [PWM_StepperControl](examples/PWM_StepperControl) **New**

 
---
---

### Example [PWM_StepperControl](examples/PWM_StepperControl)

https://github.com/khoih-prog/STM32_PWM/blob/c9117a8a65a5dfaa9dd3313b2f3b04c1895dfc71/examples/PWM_StepperControl/PWM_StepperControl.ino#L15-L214


---
---

### Debug Terminal Output Samples

### 1. PWMs_Array_Complex on NUCLEO_F767ZI

The following is the sample terminal output when running example [PWMs_Array_Complex](examples/PWMs_Array_Complex) on **NUCLEO_F767ZI** to demonstrate the accuracy of Hardware-based PWM, **especially when system is very busy**.


```cpp
Starting PWMs_Array_Complex on NUCLEO_F767ZI
STM32_PWM v1.0.1
Index = 0, Instance = 0x40000000, channel = 1, TimerIndex = 1, PinName = 0
Index = 1, Instance = 0x40010000, channel = 3, TimerIndex = 0, PinName = 77
Index = 2, Instance = 0x40000800, channel = 4, TimerIndex = 3, PinName = 63
SimpleTimer (ms): 2000, us : 12025001, Dus : 10019423
PWM Channel : 0100000, programmed Period (us): 100000, actual : 100000, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 150000, programmed Period (us): 50000, actual : 50000, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 22000, programmed Period (us): 2000, actual : 2000, programmed DutyCycle : 50, actual : 50.00
SimpleTimer (ms): 2000, us : 22058001, Dus : 10033000
PWM Channel : 0100000, programmed Period (us): 100000, actual : 99999, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 150000, programmed Period (us): 50000, actual : 49999, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 22000, programmed Period (us): 2000, actual : 2000, programmed DutyCycle : 50, actual : 50.00
SimpleTimer (ms): 2000, us : 32091001, Dus : 10033000
PWM Channel : 0100000, programmed Period (us): 100000, actual : 99999, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 150000, programmed Period (us): 50000, actual : 50000, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 22000, programmed Period (us): 2000, actual : 2000, programmed DutyCycle : 50, actual : 50.00
SimpleTimer (ms): 2000, us : 42124001, Dus : 10033000
PWM Channel : 0100000, programmed Period (us): 100000, actual : 99999, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 150000, programmed Period (us): 50000, actual : 50000, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 22000, programmed Period (us): 2000, actual : 2000, programmed DutyCycle : 50, actual : 50.00
SimpleTimer (ms): 2000, us : 52157001, Dus : 10033000
PWM Channel : 0100000, programmed Period (us): 100000, actual : 99999, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 150000, programmed Period (us): 50000, actual : 49999, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 22000, programmed Period (us): 2000, actual : 2000, programmed DutyCycle : 50, actual : 49.93
SimpleTimer (ms): 2000, us : 62190001, Dus : 10033000
PWM Channel : 0100000, programmed Period (us): 100000, actual : 99999, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 150000, programmed Period (us): 50000, actual : 50000, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 22000, programmed Period (us): 2000, actual : 2000, programmed DutyCycle : 50, actual : 50.00
```

---

### 2. PWMs_Array_Complex on NUCLEO_H743ZI2

The following is the sample terminal output when running example [PWMs_Array_Complex](examples/PWMs_Array_Complex) on **NUCLEO_H743ZI2** to demonstrate the accuracy of Hardware-based PWM, **especially when system is very busy**.

```cpp
Starting PWMs_Array_Complex on NUCLEO_H743ZI2
STM32_PWM v1.0.1
Index = 0, Instance = 0x40000000, channel = 1, TimerIndex = 1, PinName = 0
Index = 1, Instance = 0x40010000, channel = 3, TimerIndex = 0, PinName = 77
Index = 2, Instance = 0x40000800, channel = 4, TimerIndex = 3, PinName = 63
SimpleTimer (ms): 2000, us : 12025000, Dus : 10019435
PWM Channel : 0100000, programmed Period (us): 100000, actual : 100000, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 150000, programmed Period (us): 50000, actual : 50000, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 22000, programmed Period (us): 2000, actual : 2000, programmed DutyCycle : 50, actual : 50.00
SimpleTimer (ms): 2000, us : 22058000, Dus : 10033000
PWM Channel : 0100000, programmed Period (us): 100000, actual : 99999, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 150000, programmed Period (us): 50000, actual : 49999, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 22000, programmed Period (us): 2000, actual : 2000, programmed DutyCycle : 50, actual : 50.00
SimpleTimer (ms): 2000, us : 32091000, Dus : 10033000
PWM Channel : 0100000, programmed Period (us): 100000, actual : 100000, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 150000, programmed Period (us): 50000, actual : 49999, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 22000, programmed Period (us): 2000, actual : 2000, programmed DutyCycle : 50, actual : 50.00
```

---

### 3. PWMs_Array_Complex on NUCLEO_L552ZE_Q

The following is the sample terminal output when running example [PWMs_Array_Complex](examples/PWMs_Array_Complex) on **NUCLEO_L552ZE_Q** to demonstrate the accuracy of Hardware-based PWM, **especially when system is very busy**.

```cpp
Starting PWMs_Array_Complex on NUCLEO_L552ZE_Q
STM32_PWM v1.0.1
Index = 0, Instance = 0x40000000, channel = 1, TimerIndex = 1, PinName = 0
Index = 1, Instance = 0x40012C00, channel = 3, TimerIndex = 0, PinName = 77
Index = 2, Instance = 0x40000800, channel = 4, TimerIndex = 3, PinName = 63
SimpleTimer (ms): 2000, us : 12026006, Dus : 10020290
PWM Channel : 0100000, programmed Period (us): 100000, actual : 99999, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 150000, programmed Period (us): 50000, actual : 49999, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 22000, programmed Period (us): 2000, actual : 2000, programmed DutyCycle : 50, actual : 50.00
SimpleTimer (ms): 2000, us : 22060006, Dus : 10034000
PWM Channel : 0100000, programmed Period (us): 100000, actual : 99999, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 150000, programmed Period (us): 50000, actual : 50000, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 22000, programmed Period (us): 2000, actual : 2000, programmed DutyCycle : 50, actual : 50.00
SimpleTimer (ms): 2000, us : 32094006, Dus : 10034000
PWM Channel : 0100000, programmed Period (us): 100000, actual : 100000, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 150000, programmed Period (us): 50000, actual : 50000, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 22000, programmed Period (us): 2000, actual : 1999, programmed DutyCycle : 50, actual : 50.00
```

---

### 4. PWMs_Array_Complex on BLUEPILL_F103CB

The following is the sample terminal output when running example [PWMs_Array_Complex](examples/PWMs_Array_Complex) on **BLUEPILL_F103CB** to demonstrate the accuracy of Hardware-based PWM, **especially when system is very busy**.

```cpp
Starting PWMs_Array_Complex on BLUEPILL_F103CB
STM32_PWM v1.0.1
Using pin = 0, 1, etc => Index = 0, Instance = 0x40000000, channel = 1, TimerIndex = 1, PinName = 0
Using pin = 0, 1, etc => Index = 1, Instance = 0x40000400, channel = 2, TimerIndex = 2, PinName = 21
Using pin = 0, 1, etc => Index = 2, Instance = 0x40012C00, channel = 3, TimerIndex = 0, PinName = 10
SimpleTimer (ms): 2000, us : 12970007, Dus : 9999926
PWM Channel : 0, programmed Period (us): 50000, actual : 50000, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 1, programmed Period (us): 20000, actual : 20000, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 2, programmed Period (us): 2000, actual : 2000, programmed DutyCycle : 50, actual : 50.00
SimpleTimer (ms): 2000, us : 22971007, Dus : 10001000
PWM Channel : 0, programmed Period (us): 50000, actual : 49999, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 1, programmed Period (us): 20000, actual : 20000, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 2, programmed Period (us): 2000, actual : 2000, programmed DutyCycle : 50, actual : 50.00
SimpleTimer (ms): 2000, us : 32972007, Dus : 10001000
PWM Channel : 0, programmed Period (us): 50000, actual : 49999, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 1, programmed Period (us): 20000, actual : 20000, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 2, programmed Period (us): 2000, actual : 2000, programmed DutyCycle : 50, actual : 50.00
SimpleTimer (ms): 2000, us : 42973007, Dus : 10001000
PWM Channel : 0, programmed Period (us): 50000, actual : 49999, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 1, programmed Period (us): 20000, actual : 19999, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 2, programmed Period (us): 2000, actual : 2000, programmed DutyCycle : 50, actual : 50.00
SimpleTimer (ms): 2000, us : 52974007, Dus : 10001000
PWM Channel : 0, programmed Period (us): 50000, actual : 49999, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 1, programmed Period (us): 20000, actual : 20000, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 2, programmed Period (us): 2000, actual : 2000, programmed DutyCycle : 50, actual : 50.00
```
---

### 5. PWM_StepperControl on NUCLEO_F767ZI

The following is the sample terminal output when running example [PWM_StepperControl](examples/PWM_StepperControl) on **NUCLEO_F767ZI** to demonstrate how to control Stepper Motor using PWM


```
Starting PWM_StepperControl on NUCLEO_F767ZI
STM32_PWM v1.0.1
stepperTimer = 0x40010000, channel = 3, TimerIndex = 0, PinName = 77
setSpeed = 2000
setSpeed = 0
setSpeed = -1000
setSpeed = 0
setSpeed = 2000
setSpeed = 0
setSpeed = -1000
```


---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level `_PWM_LOGLEVEL_` from 0 to 4

```cpp
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_     0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

### Issues

Submit issues to: [STM32_PWM issues](https://github.com/khoih-prog/STM32_PWM/issues)

---

## TO DO

1. Search for bug and improvement.
2. Similar features for remaining Arduino boards

---

## DONE

1. Basic hardware-based multi-channel PWM for **STM32F/L/H/G/WB/MP1 boards**.
2. Add Table of Contents
3. Add example [PWM_StepperControl](https://github.com/khoih-prog/STM32_PWM/tree/main/examples/PWM_StepperControl) to demo how to control Stepper Motor using PWM

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.

1. Thanks to [Paul van Dinther](https://github.com/dinther) for proposing new way to use PWM to drive Stepper-Motor in [Using PWM to step a stepper driver #16](https://github.com/khoih-prog/RP2040_PWM/issues/16), leading to v2.0.3


<table>
  <tr>
    <td align="center"><a href="https://github.com/dinther"><img src="https://github.com/dinther.png" width="100px;" alt="dinther"/><br /><sub><b>Paul van Dinther</b></sub></a><br /></td>
  </tr>
</table>


---

## Contributing

If you want to contribute to this project:

- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/STM32_PWM/blob/main/LICENSE)

---

## Copyright

Copyright (c) 2021- Khoi Hoang


