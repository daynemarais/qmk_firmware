/*
Copyright 2019 Danny Nguyen <danny@keeb.io>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6060
#define DEVICE_VER      0x0001
#define MANUFACTURER    daynemarais
#define PRODUCT         Ortho5x12
#define DESCRIPTION     Orthographic 5x12 Bluetooth Keyboard

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 10

#define TAPPING_TOGGLE 2
#define TAPPING_TERM 125

/* Keyboard Matrix Assignments */
/*#define DIRECT_PINS { \
    { D1, D0, C6 }, \
    { D7, B5, B6 }, \
    { B7, D6, F7 } \
}*/
// #define MATRIX_ROW_PINS { D1, D0, C6, D7, B5, B6}
// #define MATRIX_COL_PINS { B7, D6, C7, D3, D2, F0, F1, F4, F5, F6 }

#define MATRIX_ROW_PINS { F5, F4, F1, F0, D2, D3}
#define MATRIX_COL_PINS { D1, D0, C6, D7, B5, B6, B7, D6, C7, F6 }

#define DIODE_DIRECTION COL2ROW

//#define BACKLIGHT_PIN F6
// #define BACKLIGHT_BREATHING
//#define BACKLIGHT_LEVELS 5

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE
