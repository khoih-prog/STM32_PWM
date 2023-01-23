/****************************************************************************************************************************
  STM32_PWM.h
  For STM32F/L/H/G/WB/MP1 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/STM32_PWM
  Licensed under MIT license
  
  Hardware-based multi-channel PWM wrapper library for STM32F/L/H/G/WB/MP1 boards

  Version: 1.0.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      30/09/2021 Initial coding for STM32F/L/H/G/WB/MP1 boards
  1.0.1   K.Hoang      23/01/2023 Add `PWM_StepperControl` example
*****************************************************************************************************************************/

#pragma once

#ifndef STM32_PWM_H
#define STM32_PWM_H

#if !( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
       defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
       defined(STM32WB) || defined(STM32MP1) || defined(STM32L5))
  #error This code is designed to run on STM32F/L/H/G/WB/MP1 platform! Please check your Tools->Board setting.
#endif

#ifndef STM32_PWM_VERSION
  #define STM32_PWM_VERSION       "STM32_PWM v1.0.1"
#endif


///////////////////////////////////////////

#include "HardwareTimer.h"

///////////////////////////////////////////

#include "PWM_Generic_Debug.h"


#endif    // STM32_PWM_H

