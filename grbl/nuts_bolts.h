/*
  nuts_bolts.h - Header file for shared definitions, variables, and functions
  Part of Grbl

  Copyright (c) 2011-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2009-2011 Simen Svale Skogsrud

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

#ifndef nuts_bolts_h
#define nuts_bolts_h

#include <stdint.h>

//#define false 0
//#define true 1

#define SOME_LARGE_VALUE 1.0E+38

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

// Axis array index values. Must start with 0 and be continuous.
enum AXIS {
    X = AXIS_X, // Axis indexing value.
    Y = AXIS_Y,
    Z = AXIS_Z,
    N = 3, // Number of axes
};

// #define A_AXIS 3

// CoreXY motor assignments. DO NOT ALTER.
// NOTE: If the A and B motor axis bindings are changed, this effects the CoreXY equations.
#ifdef COREXY
    #define A_MOTOR AXIS::X // Must be AXIS::X
    #define B_MOTOR AXIS::Y // Must be AXIS::Y
#endif

// Conversions
#define MM_PER_INCH           (25.40)
#define INCH_PER_MM           (0.0393701)
#define TICKS_PER_MICROSECOND (SystemCoreClock / 1000000)
enum {
    DELAY_MODE_DWELL = 0,
    DELAY_MODE_SYS_SUSPEND = 1,
};
// Useful macros
#define clear_vector(a)       memset(a, 0, sizeof(a))
#define clear_vector_float(a) memset(a, 0.0, sizeof(float) * AXIS::N)
// #define clear_vector_long(a) memset(a, 0.0, sizeof(long)*AXIS::N)
#define MAX(a, b)                     (((a) > (b)) ? (a) : (b))
#define MIN(a, b)                     (((a) < (b)) ? (a) : (b))
#define isequal_position_vector(a, b) !(memcmp(a, b, sizeof(float) * AXIS::N))

// Bit field and masking macros
#define BIT(n)            (1 << n)
#define BIT_SET(x, mask)  (x) |= (mask)
#define BIT_CLR(x, mask)  (x) &= ~(mask)
#define BIT_IS_1(x, mask) ((x & mask) != 0)
#define BIT_IS_0(x, mask) ((x & mask) == 0)

// Read a floating point value from a string. Line points to the input buffer, char_counter
// is the indexer pointing to the current character of the line, while float_ptr is
// a pointer to the result variable. Returns true when it succeeds
uint8_t read_float(char* line, uint8_t* char_counter, float* float_ptr);

// Non-blocking delay function used for general operation and suspend features.
void delay_sec(float seconds, uint8_t mode);

// Delays variable-defined milliseconds. Compiler compatibility fix for _delay_ms().
void delay_ms(uint16_t ms);

// Delays variable-defined microseconds. Compiler compatibility fix for _delay_us().
void delay_us(uint32_t us);

// Computes hypotenuse, avoiding avr-gcc's bloated version and the extra error checking.
float hypot_f(float x, float y);

float convert_delta_vector_to_unit_vector(float* vector);
float limit_value_by_axis_maximum(const float* max_value, float* unit_vec);

#endif