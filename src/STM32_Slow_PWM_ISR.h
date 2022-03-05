/****************************************************************************************************************************
  STM32_Slow_PWM_ISR.h
  For STM32F/L/H/G/WB/MP1 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/STM32_Slow_PWM
  Licensed under MIT license

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one STM32 timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Version: 1.2.3

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      22/09/2021 Initial coding for STM32F/L/H/G/WB/MP1
  1.1.0   K Hoang      10/11/2021 Add functions to modify PWM settings on-the-fly
  1.2.0   K Hoang      29/01/2022 Fix multiple-definitions linker error. Improve accuracy
  1.2.1   K Hoang      30/01/2022 DutyCycle to be updated at the end current PWM period
  1.2.2   K Hoang      01/02/2022 Use float for DutyCycle and Freq, uint32_t for period. Optimize code
  1.2.3   K Hoang      03/03/2022 Fix `DutyCycle` and `New Period` display bugs. Display warning only when debug level > 3
*****************************************************************************************************************************/

#pragma once

#ifndef STM32_SLOW_PWM_ISR_H
#define STM32_SLOW_PWM_ISR_H


#include "STM32_Slow_PWM_ISR.hpp"
#include "STM32_Slow_PWM_ISR_Impl.h"

#endif    // STM32_SLOW_PWM_ISR_H


