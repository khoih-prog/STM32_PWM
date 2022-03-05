# STM32_Slow_PWM Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/STM32_Slow_PWM.svg?)](https://www.ardu-badge.com/STM32_Slow_PWM)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/STM32_Slow_PWM.svg)](https://github.com/khoih-prog/STM32_Slow_PWM/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/STM32_Slow_PWM/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/STM32_Slow_PWM.svg)](http://github.com/khoih-prog/STM32_Slow_PWM/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>

---
---

## Table of Contents

* [Important Change from v1.2.0](#Important-Change-from-v120)
* [Why do we need this STM32_Slow_PWM library](#why-do-we-need-this-STM32_Slow_PWM-library)
  * [Features](#features)
  * [Why using ISR-based PWM is better](#why-using-isr-based-pwm-is-better)
  * [Currently supported Boards](#currently-supported-boards)
  * [Important Notes about ISR](#important-notes-about-isr)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error) 
* [More useful Information about STM32 Timers](#more-useful-information-about-stm32-timers)
* [Available Timers for STM32](#available-timers-for-STM32)
* [Usage](#usage)
  * [1. Init Hardware Timer](#1-init-hardware-timer)
  * [2. Set PWM Frequency, dutycycle, attach irqCallbackStartFunc and irqCallbackStopFunc functions](#2-Set-PWM-Frequency-dutycycle-attach-irqCallbackStartFunc-and-irqCallbackStopFunc-functions)
* [Examples](#examples)
  * [ 1. ISR_16_PWMs_Array](examples/ISR_16_PWMs_Array)
  * [ 2. ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex)
  * [ 3. ISR_16_PWMs_Array_Simple](examples/ISR_16_PWMs_Array_Simple)
  * [ 4. ISR_Changing_PWM](examples/ISR_Changing_PWM)
  * [ 5. ISR_Modify_PWM](examples/ISR_Modify_PWM)
  * [ 6. multiFileProject](examples/multiFileProject) **New**
* [Example ISR_16_PWMs_Array_Complex](#Example-ISR_16_PWMs_Array_Complex)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. ISR_16_PWMs_Array_Complex on NUCLEO_H743ZI2](#1-ISR_16_PWMs_Array_Complex-on-NUCLEO_H743ZI2)
  * [2. ISR_16_PWMs_Array_Complex on NUCLEO_F767ZI](#2-ISR_16_PWMs_Array_Complex-on-NUCLEO_F767ZI)
  * [3. ISR_16_PWMs_Array_Complex on NUCLEO_L552ZE_Q](#3-ISR_16_PWMs_Array_Complex-on-NUCLEO_L552ZE_Q)
  * [4. ISR_16_PWMs_Array_Complex on BLUEPILL_F103CB](#4-ISR_16_PWMs_Array_Complex-on-BLUEPILL_F103CB)
  * [5. ISR_Modify_PWM on NUCLEO_F767ZI](#5-ISR_Modify_PWM-on-NUCLEO_F767ZI)
  * [6. ISR_Changing_PWM on NUCLEO_F767ZI](#6-ISR_Changing_PWM-on-NUCLEO_F767ZI)
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

### Important Change from v1.2.0

Please have a look at [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)

As more complex calculation and check **inside ISR** are introduced from v1.2.0, there is possibly some crash depending on use-case.

You can modify to use larger `HW_TIMER_INTERVAL_US`, (from current 20uS), according to your board and use-case if crash happens.


```
// Current 20uS
#define HW_TIMER_INTERVAL_US      20L
```


---
---

### Why do we need this [STM32_Slow_PWM library](https://github.com/khoih-prog/STM32_Slow_PWM)

### Features

This library enables you to use Hardware Timers on **STM32F/L/H/G/WB/MP1 boards** such as NUCLEO_H743ZI2, NUCLEO_L552ZE_Q, NUCLEO_F767ZI, BLUEPILL_F103CB, etc., to create and output PWM to pins. Because this library doesn't use the powerful hardware-controlled PWM with limitations, the maximum PWM frequency is currently limited at **1000Hz**, which is suitable for many real-life applications. Now you can also modify PWM settings on-the-fly.

---

This library enables you to use Interrupt from Hardware Timers on STM32F/L/H/G/WB/MP1 boards to create and output PWM to pins. It now supports 16 ISR-based synchronized PWM channels, while consuming only 1 Hardware Timer. PWM interval can be very long (uint32_t millisecs). The most important feature is they're ISR-based PWM channels. Therefore, their executions are not blocked by bad-behaving functions or tasks. This important feature is absolutely necessary for mission-critical tasks. These hardware PWM channels, using interrupt, still work even if other functions are blocking. Moreover, they are much more precise (certainly depending on clock frequency accuracy) than other software PWM using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

As **Hardware Timers are rare, and very precious assets** of any board, this library now enables you to use up to **16 ISR-based synchronized PWM channels, while consuming only 1 Hardware Timer**. Timers' interval is very long (**ulong millisecs**).

Now with these new **16 ISR-based PWM-channels**, the maximum interval is **practically unlimited** (limited only by unsigned long miliseconds) while **the accuracy is nearly perfect** compared to software timers. 

The most important feature is they're ISR-based PWM channels. Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**PWMs_Array_Complex**](examples/PWMs_Array_Complex) example will demonstrate the nearly perfect accuracy, compared to software PWM, by printing the actual period / duty-cycle in `microsecs` of each of PWM-channels.

Being ISR-based PWM, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet or Blynk services. You can also have many `(up to 16)` timers to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see `software-based` SimpleTimer is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

---

#### Why using ISR-based PWM is better

Imagine you have a system with a **mission-critical** function, measuring water level and control the sump pump or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is **blocking** the loop() or setup().

So your function **might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a Hardware Timer with **Interrupt** to call your function.

These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are much more **precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is **your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules.** More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

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
 2. [`Arduino Core for STM32 v2.2.0+`](https://github.com/stm32duino/Arduino_Core_STM32) for STM32F/L/H/G/WB/MP1 boards. [![GitHub release](https://img.shields.io/github/release/stm32duino/Arduino_Core_STM32.svg)](https://github.com/stm32duino/Arduino_Core_STM32/releases/latest)

 3. To use with certain example
   - [`SimpleTimer library`](https://github.com/jfturcot/SimpleTimer) for [ISR_16_Timers_Array example](examples/ISR_16_Timers_Array).
---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**STM32_Slow_PWM**](https://github.com/khoih-prog/STM32_Slow_PWM), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/STM32_Slow_PWM.svg?)](https://www.ardu-badge.com/STM32_Slow_PWM) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**STM32_Slow_PWM**](https://github.com/khoih-prog/STM32_Slow_PWM) page.
2. Download the latest release `STM32_Slow_PWM-master.zip`.
3. Extract the zip file to `STM32_Slow_PWM-master` directory 
4. Copy whole `STM32_Slow_PWM-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**STM32_Slow_PWM** library](https://registry.platformio.org/libraries/khoih-prog/STM32_Slow_PWM) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/STM32_Slow_PWM/installation). Search for **STM32_Slow_PWM** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using `xyz-Impl.h` instead of standard `xyz.cpp`, possibly creates certain `Multiple Definitions` Linker error in certain use cases.

You can include this `.hpp` file

```
// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "STM32_Slow_PWM.hpp"     //https://github.com/khoih-prog/STM32_Slow_PWM
```

in many files. But be sure to use the following `.h` file **in just 1 `.h`, `.cpp` or `.ino` file**, which must **not be included in any other file**, to avoid `Multiple Definitions` Linker Error

```
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "STM32_Slow_PWM.h"           //https://github.com/khoih-prog/STM32_Slow_PWM
```

Check the new [**multiFileProject** example](examples/multiFileProject) for a `HOWTO` demo.

Have a look at the discussion in [Different behaviour using the src_cpp or src_h lib #80](https://github.com/khoih-prog/ESPAsync_WiFiManager/discussions/80)

---
---

## More useful Information about STM32 Timers

The Timers of STM32s are numerous, yet very sophisticated and powerful.

In general, across the STM32 microcontrollers families, the timer peripherals that have the same name also have the same features set, but there are a few exceptions. 

For example, the **TIM1** timer peripheral is shared across the STM32F1 Series, STM32F2 Series and STM32F4 Series, but for the specific case of STM32F30x microcontrollers family, the TIM1 timer peripheral features a bit richer features set than the TIM1 present in the other families.

The general purpose timers embedded by the STM32 microcontrollers share the same backbone structure; they differ only on the level of features embedded by a given timer peripheral. 

The level of features integration for a given timer peripheral is decided based on the applications field that it targets.

The timer peripherals can be classified as:
• Advanced-configuration timers like TIM1 and TIM8 among others.
• General-purpose configuration timers like TIM2 and TIM3 among others
• Lite-configuration timers like TIM9, TIM10, TIM12 and TIM16 among others
• Basic-configuration timers like TIM6 and TIM7 among others.

For example, **STM32F103C8T6** has one advance timer, while **STM32F103VET6** has two advanced timers. **Nucleo-144 STM32F767ZI boards have 14 Timers, TIM1-TIM14**.


<p align="center">
    <img src="https://github.com/khoih-prog/STM32_TimerInterrupt/blob/main/pics/STM32Timers.png">
</p>


More information can be found at [**Embedded-Lab STM32 TIMERS**](http://embedded-lab.com/blog/stm32-timers/)

To be sure which Timer is available for the board you're using, check the Core Package's related files. For example, for **Nucleo-144 STM32F767ZI**, check these files:
1. `~/.arduino15/packages/STM32/hardware/stm32/1.9.0/system/Drivers/CMSIS/Device/ST/STM32F7xx/Include/stm32f7xx.h`
2. `~/.arduino15/packages/STM32/hardware/stm32/1.9.0/system/Drivers/CMSIS/Device/ST/STM32F7xx/Include/stm32f767xx.h`

The information will be as follows:

```
typedef struct
{
  __IO uint32_t CR1;         /*!< TIM control register 1,              Address offset: 0x00 */
  __IO uint32_t CR2;         /*!< TIM control register 2,              Address offset: 0x04 */
  __IO uint32_t SMCR;        /*!< TIM slave mode control register,     Address offset: 0x08 */
  __IO uint32_t DIER;        /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  __IO uint32_t SR;          /*!< TIM status register,                 Address offset: 0x10 */
  __IO uint32_t EGR;         /*!< TIM event generation register,       Address offset: 0x14 */
  __IO uint32_t CCMR1;       /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  __IO uint32_t CCMR2;       /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  __IO uint32_t CCER;        /*!< TIM capture/compare enable register, Address offset: 0x20 */
  __IO uint32_t CNT;         /*!< TIM counter register,                Address offset: 0x24 */
  __IO uint32_t PSC;         /*!< TIM prescaler,                       Address offset: 0x28 */
  __IO uint32_t ARR;         /*!< TIM auto-reload register,            Address offset: 0x2C */
  __IO uint32_t RCR;         /*!< TIM repetition counter register,     Address offset: 0x30 */
  __IO uint32_t CCR1;        /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  __IO uint32_t CCR2;        /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  __IO uint32_t CCR3;        /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  __IO uint32_t CCR4;        /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  __IO uint32_t BDTR;        /*!< TIM break and dead-time register,    Address offset: 0x44 */
  __IO uint32_t DCR;         /*!< TIM DMA control register,            Address offset: 0x48 */
  __IO uint32_t DMAR;        /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  __IO uint32_t OR;          /*!< TIM option register,                 Address offset: 0x50 */
  __IO uint32_t CCMR3;       /*!< TIM capture/compare mode register 3,      Address offset: 0x54 */
  __IO uint32_t CCR5;        /*!< TIM capture/compare mode register5,       Address offset: 0x58 */
  __IO uint32_t CCR6;        /*!< TIM capture/compare mode register6,       Address offset: 0x5C */
  __IO uint32_t AF1;         /*!< TIM Alternate function option register 1, Address offset: 0x60 */
  __IO uint32_t AF2;         /*!< TIM Alternate function option register 2, Address offset: 0x64 */

} TIM_TypeDef;
```

and

```
#define PERIPH_BASE            0x40000000UL /*!< Base address of : AHB/ABP Peripherals   
/*!< Peripheral memory map */
#define APB1PERIPH_BASE        PERIPH_BASE

/*!< APB1 peripherals */
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000UL)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400UL)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800UL)
#define TIM5_BASE             (APB1PERIPH_BASE + 0x0C00UL)
#define TIM6_BASE             (APB1PERIPH_BASE + 0x1000UL)
#define TIM7_BASE             (APB1PERIPH_BASE + 0x1400UL)
#define TIM12_BASE            (APB1PERIPH_BASE + 0x1800UL)
#define TIM13_BASE            (APB1PERIPH_BASE + 0x1C00UL)
#define TIM14_BASE            (APB1PERIPH_BASE + 0x2000UL)

/*!< APB2 peripherals */
#define TIM1_BASE             (APB2PERIPH_BASE + 0x0000UL)
#define TIM8_BASE             (APB2PERIPH_BASE + 0x0400UL)
#define TIM9_BASE             (APB2PERIPH_BASE + 0x4000UL)
#define TIM10_BASE            (APB2PERIPH_BASE + 0x4400UL)
#define TIM11_BASE            (APB2PERIPH_BASE + 0x4800UL)

...

#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)
#define TIM5                ((TIM_TypeDef *) TIM5_BASE)
#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
#define TIM7                ((TIM_TypeDef *) TIM7_BASE)
#define TIM12               ((TIM_TypeDef *) TIM12_BASE)
#define TIM13               ((TIM_TypeDef *) TIM13_BASE)
#define TIM14               ((TIM_TypeDef *) TIM14_BASE)
...
#define TIM1                ((TIM_TypeDef *) TIM1_BASE)
#define TIM8                ((TIM_TypeDef *) TIM8_BASE)
...
#define TIM9                ((TIM_TypeDef *) TIM9_BASE)
#define TIM10               ((TIM_TypeDef *) TIM10_BASE)
#define TIM11               ((TIM_TypeDef *) TIM11_BASE)

```


---

## Available Timers for STM32

Because STM32 boards and Timers are numerous, changing and so complex, there can never be a complete and correct list of available Timers to use.

This is the temporary list for **STM32F/L/H/G/WB/MP1** Timers which can possibly be used. The available Timers certainly depends on whether they are being used for other purpose (core, application, libraries, etc.) or not. You have to `exhausively` test, research, verify by yourself to be sure.

#### 1. OK to use

If exists, otherwise, compiler error will happen

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

```
// Depending on the board, you can select STM32H7 Hardware Timer from TIM1-TIM22
// If you select a Timer not correctly, you'll get a message from compiler
// 'TIMxx' was not declared in this scope; did you mean 'TIMyy'? 

// STM32F/L/H/G/WB/MP1 OK       : TIM1, TIM4, TIM7, TIM8, TIM12, TIM13, TIM14, TIM15, TIM16, TIM17
// STM32F/L/H/G/WB/MP1 Not OK   : TIM2, TIM3, TIM5, TIM6, TIM18, TIM19, TIM20, TIM21, TIM22
// STM32F/L/H/G/WB/MP1 No timer : TIM9, TIM10, TIM11. Only for STM32F2, STM32F4 and STM32L1 
// STM32F/L/H/G/WB/MP1 No timer : TIM18, TIM19, TIM20, TIM21, TIM22

// Init timer TIM1
STM32Timer ITimer(TIM1);

// Init STM32_Slow_PWM
STM32_Slow_PWM ISR_PWM;
```

#### 2. Set PWM Frequency, dutycycle, attach irqCallbackStartFunc and irqCallbackStopFunc functions

```
void irqCallbackStartFunc()
{

}

void irqCallbackStopFunc()
{

}

void setup()
{
  ....
  
  // You can use this with PWM_Freq in Hz
    ISR_PWM.setPWM(PWM_Pin, PWM_Freq, PWM_DutyCycle, irqCallbackStartFunc, irqCallbackStopFunc);
                   
  ....                 
}  
```

---
---

### Examples: 

 1. [ISR_16_PWMs_Array](examples/ISR_16_PWMs_Array)
 2. [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex)
 3. [ISR_16_PWMs_Array_Simple](examples/ISR_16_PWMs_Array_Simple)
 4. [ISR_Changing_PWM](examples/ISR_Changing_PWM)
 5. [ISR_Modify_PWM](examples/ISR_Modify_PWM)
 6. [**multiFileProject**](examples/multiFileProject) **New** 
 
---
---

### Example [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex)

https://github.com/khoih-prog/STM32_PWM/blob/4e4a7fe81e69b972ebab9e30b472887adfcd4250/examples/ISR_16_PWMs_Array_Complex/ISR_16_PWMs_Array_Complex.ino#L16-L592



---
---

### Debug Terminal Output Samples

### 1. ISR_16_PWMs_Array_Complex on NUCLEO_H743ZI2

The following is the sample terminal output when running example [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex) on **NUCLEO_H743ZI2** to demonstrate how to use multiple PWM channels with complex callback functions, the accuracy of ISR Hardware PWM-channels, **especially when system is very busy**.  The ISR PWM-channels is **running exactly according to corresponding programmed periods and duty-cycles**


```
Starting ISR_16_PWMs_Array_Complex on NUCLEO_H743ZI2
STM32_SLOW_PWM v1.2.3
[PWM] STM32TimerInterrupt: Timer Input Freq (Hz) = 240000000
[PWM] Frequency = 1000000.00 , _count = 20
Starting ITimer OK, micros() = 2015843
Channel : 0	Period : 1000000		OnTime : 50000	Start_Time : 2019319
Channel : 1	Period : 500000		OnTime : 50000	Start_Time : 2019319
Channel : 2	Period : 333333		OnTime : 66666	Start_Time : 2019319
Channel : 3	Period : 250000		OnTime : 75000	Start_Time : 2019319
Channel : 4	Period : 200000		OnTime : 80000	Start_Time : 2019319
Channel : 5	Period : 166666		OnTime : 74999	Start_Time : 2019319
Channel : 6	Period : 142857		OnTime : 71428	Start_Time : 2019319
Channel : 7	Period : 125000		OnTime : 68750	Start_Time : 2019319
Channel : 8	Period : 111111		OnTime : 66666	Start_Time : 2019319
Channel : 9	Period : 100000		OnTime : 65000	Start_Time : 2019319
Channel : 10	Period : 66666		OnTime : 46666	Start_Time : 2019319
Channel : 11	Period : 50000		OnTime : 37500	Start_Time : 2019319
Channel : 12	Period : 40000		OnTime : 32000	Start_Time : 2019319
Channel : 13	Period : 33333		OnTime : 28333	Start_Time : 2019319
Channel : 14	Period : 25000		OnTime : 22500	Start_Time : 2019319
Channel : 15	Period : 20000		OnTime : 19000	Start_Time : 2019319
SimpleTimer (ms): 2000, us : 12111000, Dus : 10091682
PWM Channel : 0, programmed Period (uS): 1000000, actual (uS) : 1000000, programmed DutyCycle : 5, actual : 5.00
PWM Channel : 1, programmed Period (uS): 500000, actual (uS) : 500000, programmed DutyCycle : 10, actual : 10.00
PWM Channel : 2, programmed Period (uS): 333333, actual (uS) : 333340, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 3, programmed Period (uS): 250000, actual (uS) : 250000, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 4, programmed Period (uS): 200000, actual (uS) : 200020, programmed DutyCycle : 40, actual : 40.00
PWM Channel : 5, programmed Period (uS): 166666, actual (uS) : 166680, programmed DutyCycle : 45, actual : 45.00
PWM Channel : 6, programmed Period (uS): 142857, actual (uS) : 142860, programmed DutyCycle : 50, actual : 49.99
PWM Channel : 7, programmed Period (uS): 125000, actual (uS) : 125020, programmed DutyCycle : 55, actual : 54.98
PWM Channel : 8, programmed Period (uS): 111111, actual (uS) : 111120, programmed DutyCycle : 60, actual : 59.99
PWM Channel : 9, programmed Period (uS): 100000, actual (uS) : 100020, programmed DutyCycle : 65, actual : 64.99
PWM Channel : 10, programmed Period (uS): 66666, actual (uS) : 66680, programmed DutyCycle : 70, actual : 69.98
PWM Channel : 11, programmed Period (uS): 50000, actual (uS) : 50000, programmed DutyCycle : 75, actual : 75.00
PWM Channel : 12, programmed Period (uS): 40000, actual (uS) : 40000, programmed DutyCycle : 80, actual : 80.00
PWM Channel : 13, programmed Period (uS): 33333, actual (uS) : 33340, programmed DutyCycle : 85, actual : 84.94
PWM Channel : 14, programmed Period (uS): 25000, actual (uS) : 25000, programmed DutyCycle : 90, actual : 90.00
PWM Channel : 15, programmed Period (uS): 20000, actual (uS) : 20000, programmed DutyCycle : 95, actual : 95.00
SimpleTimer (ms): 2000, us : 22266000, Dus : 10155000
PWM Channel : 0, programmed Period (uS): 1000000, actual (uS) : 1000000, programmed DutyCycle : 5, actual : 5.00
PWM Channel : 1, programmed Period (uS): 500000, actual (uS) : 500000, programmed DutyCycle : 10, actual : 10.00
PWM Channel : 2, programmed Period (uS): 333333, actual (uS) : 333340, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 3, programmed Period (uS): 250000, actual (uS) : 250020, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 4, programmed Period (uS): 200000, actual (uS) : 200020, programmed DutyCycle : 40, actual : 40.00
PWM Channel : 5, programmed Period (uS): 166666, actual (uS) : 166680, programmed DutyCycle : 45, actual : 44.98
PWM Channel : 6, programmed Period (uS): 142857, actual (uS) : 142860, programmed DutyCycle : 50, actual : 49.99
PWM Channel : 7, programmed Period (uS): 125000, actual (uS) : 125000, programmed DutyCycle : 55, actual : 54.99
PWM Channel : 8, programmed Period (uS): 111111, actual (uS) : 111120, programmed DutyCycle : 60, actual : 59.99
PWM Channel : 9, programmed Period (uS): 100000, actual (uS) : 100000, programmed DutyCycle : 65, actual : 65.00
PWM Channel : 10, programmed Period (uS): 66666, actual (uS) : 66680, programmed DutyCycle : 70, actual : 69.98
PWM Channel : 11, programmed Period (uS): 50000, actual (uS) : 50000, programmed DutyCycle : 75, actual : 75.00
PWM Channel : 12, programmed Period (uS): 40000, actual (uS) : 40000, programmed DutyCycle : 80, actual : 80.00
PWM Channel : 13, programmed Period (uS): 33333, actual (uS) : 33340, programmed DutyCycle : 85, actual : 84.94
PWM Channel : 14, programmed Period (uS): 25000, actual (uS) : 25020, programmed DutyCycle : 90, actual : 89.93
PWM Channel : 15, programmed Period (uS): 20000, actual (uS) : 20000, programmed DutyCycle : 95, actual : 94.91
```

---

### 2. ISR_16_PWMs_Array_Complex on NUCLEO_F767ZI

The following is the sample terminal output when running example [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex) on **NUCLEO_F767ZI** to demonstrate how to use multiple PWM channels with complex callback functions, the accuracy of ISR Hardware PWM-channels, **especially when system is very busy**.  The ISR PWM-channels is **running exactly according to corresponding programmed periods and duty-cycles**


```
Starting ISR_16_PWMs_Array_Complex on NUCLEO_F767ZI
STM32_SLOW_PWM v1.2.3
[PWM] STM32TimerInterrupt: Timer Input Freq (Hz) = 216000000 , Timer Clock Frequency = 1000000.00
[PWM] Timer Frequency = 50000.00 , _count = 20
Starting ITimer OK, micros() = 2016555
Channel : 0	    Period : 1000000		OnTime : 50000	Start_Time : 2022112
Channel : 1	    Period : 500000		OnTime : 50000	Start_Time : 2027666
Channel : 2	    Period : 333333		OnTime : 66666	Start_Time : 2033231
Channel : 3	    Period : 250000		OnTime : 75000	Start_Time : 2038795
Channel : 4	    Period : 200000		OnTime : 80000	Start_Time : 2044365
Channel : 5	    Period : 166666		OnTime : 74999	Start_Time : 2049930
Channel : 6	    Period : 142857		OnTime : 71428	Start_Time : 2055495
Channel : 7	    Period : 125000		OnTime : 68750	Start_Time : 2061063
Channel : 8	    Period : 111111		OnTime : 66666	Start_Time : 2066628
Channel : 9	    Period : 100000		OnTime : 65000	Start_Time : 2072361
Channel : 10	    Period : 66666		OnTime : 46666	Start_Time : 2083313
Channel : 11	    Period : 50000		OnTime : 37500	Start_Time : 2088886
Channel : 12	    Period : 40000		OnTime : 32000	Start_Time : 2094451
Channel : 13	    Period : 33333		OnTime : 28333	Start_Time : 2100013
Channel : 14	    Period : 25000		OnTime : 22500	Start_Time : 2105584
Channel : 15	    Period : 20000		OnTime : 19000	Start_Time : 2111150
SimpleTimer (ms): 2000, us : 12116002, Dus : 10093908
PWM Channel : 0, programmed Period (uS): 1000000.00, actual (uS) : 1000000, programmed DutyCycle : 5.00, actual : 5.00
PWM Channel : 1, programmed Period (uS): 500000.00, actual (uS) : 500000, programmed DutyCycle : 10.00, actual : 10.00
PWM Channel : 2, programmed Period (uS): 333333.34, actual (uS) : 333340, programmed DutyCycle : 20.00, actual : 20.00
PWM Channel : 3, programmed Period (uS): 250000.00, actual (uS) : 250000, programmed DutyCycle : 30.00, actual : 30.00
PWM Channel : 4, programmed Period (uS): 200000.00, actual (uS) : 200000, programmed DutyCycle : 40.00, actual : 40.00
PWM Channel : 5, programmed Period (uS): 166666.67, actual (uS) : 166680, programmed DutyCycle : 45.00, actual : 44.98
PWM Channel : 6, programmed Period (uS): 142857.14, actual (uS) : 142860, programmed DutyCycle : 50.00, actual : 49.99
PWM Channel : 7, programmed Period (uS): 125000.00, actual (uS) : 125001, programmed DutyCycle : 55.00, actual : 54.99
PWM Channel : 8, programmed Period (uS): 111111.11, actual (uS) : 111120, programmed DutyCycle : 60.00, actual : 59.99
PWM Channel : 9, programmed Period (uS): 100000.00, actual (uS) : 100000, programmed DutyCycle : 65.00, actual : 65.00
PWM Channel : 10, programmed Period (uS): 66666.66, actual (uS) : 66680, programmed DutyCycle : 70.00, actual : 69.98
PWM Channel : 11, programmed Period (uS): 50000.00, actual (uS) : 50000, programmed DutyCycle : 75.00, actual : 75.00
PWM Channel : 12, programmed Period (uS): 40000.00, actual (uS) : 40000, programmed DutyCycle : 80.00, actual : 80.00
PWM Channel : 13, programmed Period (uS): 33333.33, actual (uS) : 33340, programmed DutyCycle : 85.00, actual : 84.94
PWM Channel : 14, programmed Period (uS): 25000.00, actual (uS) : 25000, programmed DutyCycle : 90.00, actual : 90.00
PWM Channel : 15, programmed Period (uS): 20000.00, actual (uS) : 20000, programmed DutyCycle : 95.00, actual : 95.00
SimpleTimer (ms): 2000, us : 22284001, Dus : 10167999
PWM Channel : 0, programmed Period (uS): 1000000.00, actual (uS) : 1000000, programmed DutyCycle : 5.00, actual : 5.00
PWM Channel : 1, programmed Period (uS): 500000.00, actual (uS) : 500000, programmed DutyCycle : 10.00, actual : 10.00
PWM Channel : 2, programmed Period (uS): 333333.34, actual (uS) : 333340, programmed DutyCycle : 20.00, actual : 20.00
PWM Channel : 3, programmed Period (uS): 250000.00, actual (uS) : 250000, programmed DutyCycle : 30.00, actual : 30.00
PWM Channel : 4, programmed Period (uS): 200000.00, actual (uS) : 200000, programmed DutyCycle : 40.00, actual : 40.00
PWM Channel : 5, programmed Period (uS): 166666.67, actual (uS) : 166680, programmed DutyCycle : 45.00, actual : 44.98
PWM Channel : 6, programmed Period (uS): 142857.14, actual (uS) : 142860, programmed DutyCycle : 50.00, actual : 49.99
PWM Channel : 7, programmed Period (uS): 125000.00, actual (uS) : 125000, programmed DutyCycle : 55.00, actual : 54.99
PWM Channel : 8, programmed Period (uS): 111111.11, actual (uS) : 111120, programmed DutyCycle : 60.00, actual : 59.99
PWM Channel : 9, programmed Period (uS): 100000.00, actual (uS) : 100000, programmed DutyCycle : 65.00, actual : 65.00
PWM Channel : 10, programmed Period (uS): 66666.66, actual (uS) : 66679, programmed DutyCycle : 70.00, actual : 69.98
PWM Channel : 11, programmed Period (uS): 50000.00, actual (uS) : 50000, programmed DutyCycle : 75.00, actual : 75.00
PWM Channel : 12, programmed Period (uS): 40000.00, actual (uS) : 40000, programmed DutyCycle : 80.00, actual : 80.00
PWM Channel : 13, programmed Period (uS): 33333.33, actual (uS) : 33341, programmed DutyCycle : 85.00, actual : 84.94
PWM Channel : 14, programmed Period (uS): 25000.00, actual (uS) : 25001, programmed DutyCycle : 90.00, actual : 90.00
PWM Channel : 15, programmed Period (uS): 20000.00, actual (uS) : 20000, programmed DutyCycle : 95.00, actual : 95.00
```

---

### 3. ISR_16_PWMs_Array_Complex on NUCLEO_L552ZE_Q

The following is the sample terminal output when running example [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex) on **NUCLEO_L552ZE_Q** to demonstrate how to use multiple PWM channels with complex callback functions, the accuracy of ISR Hardware PWM-channels, **especially when system is very busy**.  The ISR PWM-channels is **running exactly according to corresponding programmed periods and duty-cycles**


```
Starting ISR_16_PWMs_Array_Complex on NUCLEO_L552ZE_Q
STM32_SLOW_PWM v1.2.3
[PWM] STM32TimerInterrupt: Timer Input Freq (Hz) = 110000000
[PWM] Frequency = 1000000.00 , _count = 20
Starting ITimer OK, micros() = 2016141
Channel : 0	Period : 1000000		OnTime : 50000	Start_Time : 2019722
Channel : 1	Period : 500000		OnTime : 50000	Start_Time : 2019722
Channel : 2	Period : 333333		OnTime : 66666	Start_Time : 2019722
Channel : 3	Period : 250000		OnTime : 75000	Start_Time : 2019722
Channel : 4	Period : 200000		OnTime : 80000	Start_Time : 2019722
Channel : 5	Period : 166666		OnTime : 74999	Start_Time : 2019722
Channel : 6	Period : 142857		OnTime : 71428	Start_Time : 2019722
Channel : 7	Period : 125000		OnTime : 68750	Start_Time : 2019722
Channel : 8	Period : 111111		OnTime : 66666	Start_Time : 2019722
Channel : 9	Period : 100000		OnTime : 65000	Start_Time : 2019722
Channel : 10	Period : 66666		OnTime : 46666	Start_Time : 2019722
Channel : 11	Period : 50000		OnTime : 37500	Start_Time : 2019722
Channel : 12	Period : 40000		OnTime : 32000	Start_Time : 2019722
Channel : 13	Period : 33333		OnTime : 28333	Start_Time : 2019722
Channel : 14	Period : 25000		OnTime : 22500	Start_Time : 2019722
Channel : 15	Period : 20000		OnTime : 19000	Start_Time : 2019722
SimpleTimer (ms): 2000, us : 12115022, Dus : 10095303
PWM Channel : 0, programmed Period (uS): 1000000, actual (uS) : 1000000, programmed DutyCycle : 5, actual : 5.00
PWM Channel : 1, programmed Period (uS): 500000, actual (uS) : 500003, programmed DutyCycle : 10, actual : 10.00
PWM Channel : 2, programmed Period (uS): 333333, actual (uS) : 333339, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 3, programmed Period (uS): 250000, actual (uS) : 250005, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 4, programmed Period (uS): 200000, actual (uS) : 200007, programmed DutyCycle : 40, actual : 40.00
PWM Channel : 5, programmed Period (uS): 166666, actual (uS) : 166680, programmed DutyCycle : 45, actual : 44.99
PWM Channel : 6, programmed Period (uS): 142857, actual (uS) : 142858, programmed DutyCycle : 50, actual : 49.99
PWM Channel : 7, programmed Period (uS): 125000, actual (uS) : 124991, programmed DutyCycle : 55, actual : 54.99
PWM Channel : 8, programmed Period (uS): 111111, actual (uS) : 111121, programmed DutyCycle : 60, actual : 59.99
PWM Channel : 9, programmed Period (uS): 100000, actual (uS) : 99988, programmed DutyCycle : 65, actual : 65.01
PWM Channel : 10, programmed Period (uS): 66666, actual (uS) : 66680, programmed DutyCycle : 70, actual : 69.98
PWM Channel : 11, programmed Period (uS): 50000, actual (uS) : 50020, programmed DutyCycle : 75, actual : 74.97
PWM Channel : 12, programmed Period (uS): 40000, actual (uS) : 40001, programmed DutyCycle : 80, actual : 80.00
PWM Channel : 13, programmed Period (uS): 33333, actual (uS) : 33340, programmed DutyCycle : 85, actual : 84.94
PWM Channel : 14, programmed Period (uS): 25000, actual (uS) : 24999, programmed DutyCycle : 90, actual : 90.00
PWM Channel : 15, programmed Period (uS): 20000, actual (uS) : 20020, programmed DutyCycle : 95, actual : 94.91
SimpleTimer (ms): 2000, us : 22275008, Dus : 10159986
PWM Channel : 0, programmed Period (uS): 1000000, actual (uS) : 1000000, programmed DutyCycle : 5, actual : 5.00
PWM Channel : 1, programmed Period (uS): 500000, actual (uS) : 500003, programmed DutyCycle : 10, actual : 10.00
PWM Channel : 2, programmed Period (uS): 333333, actual (uS) : 333339, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 3, programmed Period (uS): 250000, actual (uS) : 249995, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 4, programmed Period (uS): 200000, actual (uS) : 199993, programmed DutyCycle : 40, actual : 40.00
PWM Channel : 5, programmed Period (uS): 166666, actual (uS) : 166680, programmed DutyCycle : 45, actual : 44.99
PWM Channel : 6, programmed Period (uS): 142857, actual (uS) : 142860, programmed DutyCycle : 50, actual : 49.99
PWM Channel : 7, programmed Period (uS): 125000, actual (uS) : 125002, programmed DutyCycle : 55, actual : 54.99
PWM Channel : 8, programmed Period (uS): 111111, actual (uS) : 111120, programmed DutyCycle : 60, actual : 59.99
PWM Channel : 9, programmed Period (uS): 100000, actual (uS) : 99999, programmed DutyCycle : 65, actual : 65.00
PWM Channel : 10, programmed Period (uS): 66666, actual (uS) : 66680, programmed DutyCycle : 70, actual : 69.98
PWM Channel : 11, programmed Period (uS): 50000, actual (uS) : 50000, programmed DutyCycle : 75, actual : 75.00
PWM Channel : 12, programmed Period (uS): 40000, actual (uS) : 39997, programmed DutyCycle : 80, actual : 80.00
PWM Channel : 13, programmed Period (uS): 33333, actual (uS) : 33340, programmed DutyCycle : 85, actual : 84.94
PWM Channel : 14, programmed Period (uS): 25000, actual (uS) : 25000, programmed DutyCycle : 90, actual : 90.00
PWM Channel : 15, programmed Period (uS): 20000, actual (uS) : 20000, programmed DutyCycle : 95, actual : 95.00
```

---

### 4. ISR_16_PWMs_Array_Complex on BLUEPILL_F103CB

The following is the sample terminal output when running example [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex) on **BLUEPILL_F103CB** to demonstrate how to use multiple PWM channels with complex callback functions, the accuracy of ISR Hardware PWM-channels, **especially when system is very busy**.  The ISR PWM-channels is **running exactly according to corresponding programmed periods and duty-cycles**


```
Starting ISR_16_PWMs_Array_Complex on BLUEPILL_F103CB
STM32_SLOW_PWM v1.2.3
[PWM] STM32TimerInterrupt: Timer Input Freq (Hz) = 72000000
[PWM] Frequency = 1000000.00 , _count = 20
Starting ITimer OK, micros() = 3390333
Channel : 0	Period : 1000000		OnTime : 50000	Start_Time : 3390427
Channel : 1	Period : 500000		OnTime : 50000	Start_Time : 3390427
Channel : 2	Period : 333333		OnTime : 66666	Start_Time : 3390427
Channel : 3	Period : 250000		OnTime : 75000	Start_Time : 3390427
Channel : 4	Period : 200000		OnTime : 80000	Start_Time : 3390427
Channel : 5	Period : 166666		OnTime : 74999	Start_Time : 3390427
Channel : 6	Period : 142857		OnTime : 71428	Start_Time : 3390427
Channel : 7	Period : 125000		OnTime : 68750	Start_Time : 3390427
Channel : 8	Period : 111111		OnTime : 66666	Start_Time : 3390427
Channel : 9	Period : 100000		OnTime : 65000	Start_Time : 3390427
Channel : 10	Period : 66666		OnTime : 46666	Start_Time : 3390427
Channel : 11	Period : 50000		OnTime : 37500	Start_Time : 3390427
Channel : 12	Period : 40000		OnTime : 32000	Start_Time : 3390427
Channel : 13	Period : 33333		OnTime : 28333	Start_Time : 3390427
Channel : 14	Period : 25000		OnTime : 22500	Start_Time : 3390427
Channel : 15	Period : 20000		OnTime : 19000	Start_Time : 3390427
SimpleTimer (ms): 2000, us : 13397013, Dus : 10006588
PWM Channel : 0, programmed Period (uS): 1000000, actual (uS) : 1000000, programmed DutyCycle : 5, actual : 5.00
PWM Channel : 1, programmed Period (uS): 500000, actual (uS) : 500003, programmed DutyCycle : 10, actual : 10.00
PWM Channel : 2, programmed Period (uS): 333333, actual (uS) : 333342, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 3, programmed Period (uS): 250000, actual (uS) : 250006, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 4, programmed Period (uS): 200000, actual (uS) : 199999, programmed DutyCycle : 40, actual : 40.00
PWM Channel : 5, programmed Period (uS): 166666, actual (uS) : 166679, programmed DutyCycle : 45, actual : 44.99
PWM Channel : 6, programmed Period (uS): 142857, actual (uS) : 142865, programmed DutyCycle : 50, actual : 49.99
PWM Channel : 7, programmed Period (uS): 125000, actual (uS) : 125000, programmed DutyCycle : 55, actual : 54.99
PWM Channel : 8, programmed Period (uS): 111111, actual (uS) : 111121, programmed DutyCycle : 60, actual : 59.99
PWM Channel : 9, programmed Period (uS): 100000, actual (uS) : 100003, programmed DutyCycle : 65, actual : 65.00
PWM Channel : 10, programmed Period (uS): 66666, actual (uS) : 66678, programmed DutyCycle : 70, actual : 69.98
PWM Channel : 11, programmed Period (uS): 50000, actual (uS) : 50006, programmed DutyCycle : 75, actual : 74.99
PWM Channel : 12, programmed Period (uS): 40000, actual (uS) : 40008, programmed DutyCycle : 80, actual : 79.99
PWM Channel : 13, programmed Period (uS): 33333, actual (uS) : 33338, programmed DutyCycle : 85, actual : 84.95
PWM Channel : 14, programmed Period (uS): 25000, actual (uS) : 25011, programmed DutyCycle : 90, actual : 89.96
PWM Channel : 15, programmed Period (uS): 20000, actual (uS) : 19984, programmed DutyCycle : 95, actual : 94.91
SimpleTimer (ms): 2000, us : 23412013, Dus : 10015000
PWM Channel : 0, programmed Period (uS): 1000000, actual (uS) : 1000000, programmed DutyCycle : 5, actual : 5.00
PWM Channel : 1, programmed Period (uS): 500000, actual (uS) : 500003, programmed DutyCycle : 10, actual : 10.00
PWM Channel : 2, programmed Period (uS): 333333, actual (uS) : 333342, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 3, programmed Period (uS): 250000, actual (uS) : 250005, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 4, programmed Period (uS): 200000, actual (uS) : 199999, programmed DutyCycle : 40, actual : 40.00
PWM Channel : 5, programmed Period (uS): 166666, actual (uS) : 166679, programmed DutyCycle : 45, actual : 44.99
PWM Channel : 6, programmed Period (uS): 142857, actual (uS) : 142865, programmed DutyCycle : 50, actual : 49.99
PWM Channel : 7, programmed Period (uS): 125000, actual (uS) : 125020, programmed DutyCycle : 55, actual : 54.98
PWM Channel : 8, programmed Period (uS): 111111, actual (uS) : 111121, programmed DutyCycle : 60, actual : 59.99
PWM Channel : 9, programmed Period (uS): 100000, actual (uS) : 100023, programmed DutyCycle : 65, actual : 64.99
PWM Channel : 10, programmed Period (uS): 66666, actual (uS) : 66678, programmed DutyCycle : 70, actual : 69.98
PWM Channel : 11, programmed Period (uS): 50000, actual (uS) : 50025, programmed DutyCycle : 75, actual : 74.96
PWM Channel : 12, programmed Period (uS): 40000, actual (uS) : 40028, programmed DutyCycle : 80, actual : 79.90
PWM Channel : 13, programmed Period (uS): 33333, actual (uS) : 33338, programmed DutyCycle : 85, actual : 84.97
PWM Channel : 14, programmed Period (uS): 25000, actual (uS) : 24988, programmed DutyCycle : 90, actual : 90.03
PWM Channel : 15, programmed Period (uS): 20000, actual (uS) : 19984, programmed DutyCycle : 95, actual : 95.00
```

---

### 5. ISR_Modify_PWM on NUCLEO_F767ZI

The following is the sample terminal output when running example [ISR_Modify_PWM](examples/ISR_Modify_PWM) on **NUCLEO_F767ZI** to demonstrate how to modify PWM settings on-the-fly without deleting the PWM channel

```
Starting ISR_Modify_PWM on NUCLEO_F767ZI
STM32_SLOW_PWM v1.2.3
[PWM] STM32TimerInterrupt: Timer Input Freq (Hz) = 216000000 , Timer Clock Frequency = 1000000.00
[PWM] Timer Frequency = 50000.00 , _count = 20
Starting ITimer OK, micros() = 2016546
Using PWM Freq = 200.00, PWM DutyCycle = 1.00
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 2022139
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 12027668
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 22022668
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 32027668
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 42022668
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 52027668
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 62032668
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 72032668
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 82027668
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 92032668
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 102027668
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 112037668
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 122032668
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 132037668
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 142032668
```

---

### 6. ISR_Changing_PWM on NUCLEO_F767ZI

The following is the sample terminal output when running example [ISR_Changing_PWM](examples/ISR_Changing_PWM) on **NUCLEO_F767ZI** to demonstrate how to modify PWM settings on-the-fly by deleting the PWM channel and reinit the PWM channel

```
Starting ISR_Changing_PWM on NUCLEO_F767ZI
STM32_SLOW_PWM v1.2.3
[PWM] STM32TimerInterrupt: Timer Input Freq (Hz) = 216000000 , Timer Clock Frequency = 1000000.00
[PWM] Timer Frequency = 50000.00 , _count = 20
Starting ITimer OK, micros() = 2016548
Using PWM Freq = 1.00, PWM DutyCycle = 50.00
Channel : 0	    Period : 1000000		OnTime : 500000	Start_Time : 2022140
Using PWM Freq = 2.00, PWM DutyCycle = 90.00
Channel : 0	    Period : 500000		OnTime : 450000	Start_Time : 12027037
Using PWM Freq = 1.00, PWM DutyCycle = 50.00
Channel : 0	    Period : 1000000		OnTime : 500000	Start_Time : 22032037
Using PWM Freq = 2.00, PWM DutyCycle = 90.00
Channel : 0	    Period : 500000		OnTime : 450000	Start_Time : 32038037
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

Submit issues to: [STM32_Slow_PWM issues](https://github.com/khoih-prog/STM32_Slow_PWM/issues)

---

## TO DO

1. Search for bug and improvement.
2. Similar features for remaining Arduino boards

---

## DONE

 1. Basic hardware multi-channel PWM for **STM32F/L/H/G/WB/MP1 boards** such as NUCLEO_H743ZI2, NUCLEO_L552ZE_Q, NUCLEO_F767ZI, BLUEPILL_F103CB, etc., using [`Arduino Core for STM32`](https://github.com/stm32duino/Arduino_Core_STM32)
 2. Add Table of Contents
 3. Add functions to modify PWM settings on-the-fly
 4. Fix `multiple-definitions` linker error. Drop `src_cpp` and `src_h` directories
 5. Add example [multiFileProject](examples/multiFileProject) to demo for multiple-file project
 6. Improve accuracy by using `float`, instead of `uint32_t` for `dutycycle`
 7. Optimize library code by using `reference-passing` instead of `value-passing`
 8. Fix reattachInterrupt() bug. Check [bugfix: reattachInterrupt() pass wrong frequency value to setFrequency() #19](https://github.com/khoih-prog/ESP8266TimerInterrupt/pull/19)
 9. DutyCycle to be optionally updated at the end current PWM period instead of immediately.
10. Display informational warning only when `_PWM_LOGLEVEL_` > 3

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.


---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/STM32_Slow_PWM/blob/master/LICENSE)

---

## Copyright

Copyright 2021- Khoi Hoang


