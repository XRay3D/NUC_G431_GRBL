/*
  grbl.h - main Grbl include file
  Part of Grbl

  Copyright (c) 2015-2016 Sungeun K. Jeon for Gnea Research LLC

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef grbl_h
#define grbl_h

// Grbl versioning system
#define GRBL_VERSION       "1.1h"
#define GRBL_VERSION_BUILD "20190825"

// Define standard libraries used by Grbl.
// #include <avr/io.h>
// #include <avr/pgmspace.h>
// #include <avr/interrupt.h>
// #include <avr/wdt.h>
// #include <util/delay.h>
#define _USE_MATH_DEFINES

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "stm32g4xx.h"
// #include <numbers> // std::numbers
/*

__enable_irq()
__disable_irq()
__get_PRIMASK()
__set_PRIMASK()
*/
inline void cli() [[deprecated]] { /*__disable_irq(); */
}
inline void sei() [[deprecated]] { /* __enable_irq(); */
}

inline void _delay_us(uint32_t us) {
    volatile uint32_t delay = (us * (SystemCoreClock / 1'000'000) / 4);
    while(delay--)
        continue;
}

inline void _delay_ms(uint32_t us) {
    volatile uint32_t delay = (us * (SystemCoreClock / 1'000) / 4);
    while(delay--)
        continue;
}

// void Delay_ms(uint32_t ms) {
//     volatile uint32_t nCount;
//     SAI_Block_TypeDef RCC_Clocks;
//     RCC_GetClocksFreq(&RCC_Clocks);

//    nCount = (RCC_Clocks.HCLK_Frequency / 10000) * ms;
//    while (nCount--)
//        continue;
//}

#define M_PI 3.14159265358979323846 /* pi */

// Define the Grbl system include files. NOTE: Do not alter organization.
// clang-format off
#include "nuts_bolts.h"
#include "config.h"
#include "settings.h"
#include "system.h"
#include "defaults.h"
#include "cpu_map.h"
#include "planner.h"
#include "coolant_control.h"
#include "eeprom.h"
#include "gcode.h"
#include "limits.h"
#include "motion_control.h"
#include "planner.h"
#include "print.h"
#include "probe.h"
#include "protocol.h"
#include "report.h"
#include "serial.h"
#include "spindle_control.h"
#include "stepper.h"
#include "jog.h"
// clang-format on

// ---------------------------------------------------------------------------------------
// COMPILE-TIME ERROR CHECKING OF DEFINE VALUES:
inline void COMPILE_TIME_ERROR_CHECKING() {

#if defined(HOMING_CYCLE_0)
#error "Required HOMING_CYCLE_0 not defined."
#endif

#if defined(USE_SPINDLE_DIR_AS_ENABLE_PIN) && !defined(VARIABLE_SPINDLE)
#error "USE_SPINDLE_DIR_AS_ENABLE_PIN may only be used with VARIABLE_SPINDLE enabled"
#endif

#if defined(USE_SPINDLE_DIR_AS_ENABLE_PIN) && !defined(CPU_MAP_ATMEGA328P)
#error "USE_SPINDLE_DIR_AS_ENABLE_PIN may only be used with a 328p processor"
#endif

#if !defined(USE_SPINDLE_DIR_AS_ENABLE_PIN) && defined(SPINDLE_ENABLE_OFF_WITH_ZERO_SPEED)
#error "SPINDLE_ENABLE_OFF_WITH_ZERO_SPEED may only be used with USE_SPINDLE_DIR_AS_ENABLE_PIN enabled"
#endif

#if defined(PARKING_ENABLE)
#if defined(HOMING_FORCE_SET_ORIGIN)
#error "HOMING_FORCE_SET_ORIGIN is not supported with PARKING_ENABLE at this time."
#endif
#endif

#if defined(ENABLE_PARKING_OVERRIDE_CONTROL)
#if !defined(PARKING_ENABLE)
#error "ENABLE_PARKING_OVERRIDE_CONTROL must be enabled with PARKING_ENABLE."
#endif
#endif

#if defined(SPINDLE_PWM_MIN_VALUE)
    static_assert((SPINDLE_PWM_MIN_VALUE > 0), "SPINDLE_PWM_MIN_VALUE must be greater than zero.");
#endif

#if(REPORT_WCO_REFRESH_BUSY_COUNT < REPORT_WCO_REFRESH_IDLE_COUNT)
#error "WCO busy refresh is less than idle refresh."
#endif
#if(REPORT_OVR_REFRESH_BUSY_COUNT < REPORT_OVR_REFRESH_IDLE_COUNT)
#error "Override busy refresh is less than idle refresh."
#endif

    static_assert(!(REPORT_WCO_REFRESH_IDLE_COUNT < 2), "WCO refresh must be greater than one.");

    static_assert(!(REPORT_OVR_REFRESH_IDLE_COUNT < 1), "Override refresh must be greater than zero.");

#if defined(ENABLE_DUAL_AXIS)
#if !((DUAL_AXIS_SELECT == AXIS::X) || (DUAL_AXIS_SELECT == AXIS::Y))
#error "Dual axis currently supports X or Y axes only."
#endif
#if defined(DUAL_AXIS_CONFIG_CNC_SHIELD_CLONE) && defined(VARIABLE_SPINDLE)
#error "VARIABLE_SPINDLE not supported with DUAL_AXIS_CNC_SHIELD_CLONE."
#endif
#if defined(DUAL_AXIS_CONFIG_CNC_SHIELD_CLONE) && defined(DUAL_AXIS_CONFIG_PROTONEER_V3_51)
#error "More than one dual axis configuration found. Select one."
#endif
#if !defined(DUAL_AXIS_CONFIG_CNC_SHIELD_CLONE) && !defined(DUAL_AXIS_CONFIG_PROTONEER_V3_51)
#error "No supported dual axis configuration found. Select one."
#endif
#if defined(COREXY)
#error "CORE XY not supported with dual axis feature."
#endif
#if defined(USE_SPINDLE_DIR_AS_ENABLE_PIN)
#error "USE_SPINDLE_DIR_AS_ENABLE_PIN not supported with dual axis feature."
#endif
#if defined(ENABLE_M7)
#error "ENABLE_M7 not supported with dual axis feature."
#endif
#endif
}
// ---------------------------------------------------------------------------------------

#endif
