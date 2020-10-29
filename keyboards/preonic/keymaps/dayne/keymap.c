/* Copyright 2015-2017 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "muse.h"

typedef struct {
    bool is_press_action;
    uint8_t state;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP, // Send two single taps
    TRIPLE_TAP,
    TRIPLE_HOLD
};

enum {
    TD_CAPS_SFT,
    TD_ESC_GRV,
    TD_PLUS_PSCR,
    X_CTL
};

enum custom_keycodes {
    QMK1 = SAFE_RANGE,
    QMK2,
    QMK3,
};


uint8_t cur_dance(qk_tap_dance_state_t *state);

// For the x tap dance. Put it here so it can be used in any keymap
void x_finished(qk_tap_dance_state_t *state, void *user_data);
void x_reset(qk_tap_dance_state_t *state, void *user_data);

// For the caps tap dance. Put it here so it can be used in any keymap
void caps_finished(qk_tap_dance_state_t *state, void *user_data);
void caps_reset(qk_tap_dance_state_t *state, void *user_data);

void esc_finished(qk_tap_dance_state_t *state, void *user_data);
void esc_reset(qk_tap_dance_state_t *state, void *user_data);

void plus_finished(qk_tap_dance_state_t *state, void *user_data);
void plus_reset(qk_tap_dance_state_t *state, void *user_data);

enum preonic_layers {
  _QWERTY,
  _COLEMAK,
  _DVORAK,
  _LOWER,
  _RAISE,
  _ADJUST
};

enum preonic_keycodes {
  QWERTY = SAFE_RANGE,
  COLEMAK,
  DVORAK,
  LOWER,
  RAISE,
  BACKLIT
};

#define LOWER  TT(_LOWER)
#define RAISE  TT(_RAISE)
#define TODEFAULT TO(_QWERTY)
#define ADJUST MO(_ADJUST)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Del  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl | GUI  | Alt  | APP  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_preonic_grid( \
  TD(TD_ESC_GRV),  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC, \
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_DEL,  \
  KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,  \
  KC_LCTL, KC_LGUI, KC_APP, KC_LALT,  LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT  \
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |      |      |      |      |      |      |      |      |      |   {  |   }  | Home |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      |   =  |      |      |      |  _   |  INS |      |   +  | End  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Reset|      |      |      |      |      |      |      |      |      |      |  |   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Caps |      |      |      |      |   \  |      |  -   |   {  |  }   |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Mute | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_preonic_grid( \
  KC_F12,   KC_F1,    KC_F2,   KC_F3 ,   KC_F4,    KC_F5,    KC_F6,   KC_F7,     KC_F8,   KC_F9,   KC_F10,  KC_F11, \
  _______,  QMK1   ,  QMK3   , KC_EQL,  _______,  _______,  _______, KC_UNDS,   KC_INS,  _______, KC_PSCR, KC_HOME,  \
  RESET,    _______,  KC_PSCR, QMK2  , _______,  _______,  _______, _______,   KC_LCBR, KC_RCBR, _______, KC_END, \
  KC_CAPS,  MU_ON, MU_OFF, MU_MOD,  LGUI(KC_L),   KC_BSLS, _______, KC_MINS,   KC_LBRC, KC_RBRC, KC_PIPE, _______, \
  _______,  _______,  _______, _______, TODEFAULT, _______, _______, TODEFAULT, KC_PGDOWN, KC_HOME, KC_END, KC_PGUP  \
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |  F12 |   F1 |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  | F10  |  F11 |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |  =   |      |      |      |  _   |  INS |      |  +   | Home |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Reset|      | PSCR | QMK2 |      |      |      |      |  {   |  }   |      | End  |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Caps |  BL1 | BL2  | BL3  | Lock |  \   |      |  -   |   [  |   ]  |   |  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |TG(1) |             |TG(1) | Mute | Vol- | Vol- |      |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_preonic_grid( \
  KC_F12,   KC_F1,    KC_F2,   KC_F3 ,   KC_F4,    KC_F5,    KC_F6,   KC_F7,     KC_F8,   KC_F9,   KC_F10,  KC_F11, \
  _______,  QMK1   ,  QMK3   , KC_EQL,  _______,  _______,  _______, KC_UNDS,   KC_INS,  _______, KC_PLUS, KC_HOME,  \
  RESET,    _______,  KC_PSCR, QMK2  , _______,  _______,  _______, _______,   KC_LCBR, KC_RCBR, _______, KC_END, \
  KC_CAPS,  MU_ON, MU_OFF, MU_MOD,  LGUI(KC_L),   KC_BSLS, _______, KC_MINS,   KC_LBRC, KC_RBRC, KC_PIPE, _______, \
  _______,  _______,  _______, _______, TODEFAULT, _______, _______, TODEFAULT, KC_MUTE, KC_VOLD, KC_VOLU, _______  \
),

};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QMK1:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                SEND_STRING("");
            } else {
                // when keycode QMKBEST is released
            }
            break;
        case QMK2:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                SEND_STRING("");
            } else {
                // when keycode QMKBEST is released
            }
            break;
        case QMK3:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                SEND_STRING("");
            } else {
                // when keycode QMKBEST is released
            }
            break;
    }
    return true;
};

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_ESC_GRV] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, esc_finished, esc_reset),
    [TD_PLUS_PSCR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, plus_finished, plus_reset),
    [X_CTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_finished, x_reset),
    [TD_CAPS_SFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, caps_finished, caps_reset)
};

/* Return an integer that corresponds to what kind of tap dance should be executed.
 *
 * How to figure out tap dance state: interrupted and pressed.
 *
 * Interrupted: If the state of a dance dance is "interrupted", that means that another key has been hit
 *  under the tapping term. This is typically indicitive that you are trying to "tap" the key.
 *
 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
 *
 * One thing that is currenlty not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
 *  feature. In general, advanced tap dances do not work well if they are used with commonly typed letters.
 *  For example "A". Tap dances are best used on non-letter keys that are not hit while typing letters.
 *
 * Good places to put an advanced tap dance:
 *  z,q,x,j,k,v,b, any function key, home/end, comma, semi-colon
 *
 * Criteria for "good placement" of a tap dance key:
 *  Not a key that is hit frequently in a sentence
 *  Not a key that is used frequently to double tap, for example 'tab' is often double tapped in a terminal, or
 *    in a web form. So 'tab' would be a poor choice for a tap dance.
 *  Letters used in common words as a double. For example 'p' in 'pepper'. If a tap dance function existed on the
 *    letter 'p', the word 'pepper' would be quite frustating to type.
 *
 * For the third point, there does exist the 'DOUBLE_SINGLE_TAP', however this is not fully tested
 *
 */
uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        // DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TRIPLE_TAP;
        else return TRIPLE_HOLD;
    } else return 8; // Magic number. At some point this method will expand to work for more presses
}

// Create an instance of 'tap' for the 'x' tap dance.
static tap xtap_state = {
    .is_press_action = true,
    .state = 0
};

void x_finished(qk_tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case SINGLE_TAP: register_code(KC_X); break;
        case SINGLE_HOLD: register_code(KC_LCTRL); break;
        // case DOUBLE_TAP: register_code(KC_ESC); break;
        // case DOUBLE_HOLD: register_code(KC_LALT); break;
        // // Last case is for fast typing. Assuming your key is `f`:
        // // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        // case DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X);
    }
}

void x_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (xtap_state.state) {
        case SINGLE_TAP: unregister_code(KC_X); break;
        case SINGLE_HOLD: unregister_code(KC_LCTRL); break;
        // case DOUBLE_TAP: unregister_code(KC_ESC); break;
        // case DOUBLE_HOLD: unregister_code(KC_LALT);
        // case DOUBLE_SINGLE_TAP: unregister_code(KC_X);
    }
    xtap_state.state = 0;
}

// Create an instance of 'tap' for the 'x' tap dance.
static tap capstap_state = {
    .is_press_action = true,
    .state = 0
};

void caps_finished(qk_tap_dance_state_t *state, void *user_data) {
    capstap_state.state = cur_dance(state);
    switch (capstap_state.state) {
        case SINGLE_TAP: register_code(KC_CAPS); break;
        case SINGLE_HOLD: register_code(KC_LSFT); break;
    }
}

void caps_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (capstap_state.state) {
        case SINGLE_TAP: unregister_code(KC_CAPS); break;
        case SINGLE_HOLD: unregister_code(KC_LSFT); break;
    }
    capstap_state.state = 0;
}

static tap esctap_state = {
    .is_press_action = true,
    .state = 0
};

void esc_finished(qk_tap_dance_state_t *state, void *user_data) {
    esctap_state.state = cur_dance(state);
    switch (esctap_state.state) {
        case SINGLE_TAP: register_code(KC_ESC); break;
        case SINGLE_HOLD: register_code(KC_GRV); break;
    }
}

void esc_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (esctap_state.state) {
        case SINGLE_TAP: unregister_code(KC_ESC); break;
        case SINGLE_HOLD: unregister_code(KC_GRV); break;
    }
    esctap_state.state = 0;
}

static tap plustap_state = {
    .is_press_action = true,
    .state = 0
};

void plus_finished(qk_tap_dance_state_t *state, void *user_data) {
    plustap_state.state = cur_dance(state);
    switch (plustap_state.state) {
        // case SINGLE_TAP: register_code(KC_PLUS); break;
        // case SINGLE_HOLD: register_code(KC_PSCR); break;
    }
}

void plus_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (plustap_state.state) {
        // case SINGLE_TAP: unregister_code(KC_PLUS); break;
        // case SINGLE_HOLD: unregister_code(KC_PSCR); break;
    }
    plustap_state.state = 0;
}

// const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
// 	keyevent_t event = record->event;

// 	switch (id) {

// 	}
// 	return MACRO_NONE;
// }

// void matrix_init_user(void) {
// }

// void matrix_scan_user(void) {
// }

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//   switch (keycode) {
//         case QWERTY:
//           if (record->event.pressed) {
//             set_single_persistent_default_layer(_QWERTY);
//           }
//           return false;
//           break;
//         case LOWER:
//           if (record->event.pressed) {
//             layer_on(_LOWER);
//             update_tri_layer(_LOWER, _RAISE, _ADJUST);
//           } else {
//             layer_off(_LOWER);
//             update_tri_layer(_LOWER, _RAISE, _ADJUST);
//           }
//           return false;
//           break;
//         case RAISE:
//           if (record->event.pressed) {
//             layer_on(_RAISE);
//             update_tri_layer(_LOWER, _RAISE, _ADJUST);
//           } else {
//             layer_off(_RAISE);
//             update_tri_layer(_LOWER, _RAISE, _ADJUST);
//           }
//           return false;
//           break;
//         case BACKLIT:
//           if (record->event.pressed) {
//             register_code(KC_RSFT);
//             #ifdef BACKLIGHT_ENABLE
//               backlight_step();
//             #endif
//             #ifdef __AVR__
//             writePinLow(E6);
//             #endif
//           } else {
//             unregister_code(KC_RSFT);
//             #ifdef __AVR__
//             writePinHigh(E6);
//             #endif
//           }
//           return false;
//           break;
//       }
//     return true;
// };

// void dip_switch_update_user(uint8_t index, bool active) {
//     switch (index) {
//         case 0:
//             if (active) {
//                 layer_on(_ADJUST);
//             } else {
//                 layer_off(_ADJUST);
//             }
//             break;
//         // case 1:
//         //     if (active) {
//         //         muse_mode = true;
//         //     } else {
//         //         muse_mode = false;
//         //     }
//     }
// }

// layer_state_t layer_state_set_user(layer_state_t state) {
//   return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
// }


// const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
//     #ifndef TESTMODE
//     // MEH - hold down ctrl, alt, shift, and press key
//     [0] = LAYOUT(
//         MEH(KC_1), MEH(KC_2), MEH(KC_3),
//         MEH(KC_4), MEH(KC_5), MEH(KC_6),
//         MEH(KC_7), MEH(KC_8), LT(1, MEH(KC_9))
//     ),
//     #else
//     [0] = LAYOUT(
//         KC_A, KC_B, KC_C,
//         KC_D, KC_E, KC_F,
//         KC_G, KC_H, KC_I
//     ),
//     #endif
//     /*
//      * ------------------------------------------------------------------------
//      * |                | Automatic BLE output detection | Reset               |
//      * | Backlight Step | Use USB port for output        |                     |
//      * | Backlight Tgl  | Use Bluetooth for output       | (currently pressed) |
//      * ------------------------------------------------------------------------
//      */
//     [1] = LAYOUT(
//         _______, OUT_AUTO, RESET,
//         BL_STEP, OUT_USB, _______,
//         BL_TOGG, OUT_BT,  _______
//     )
// };
