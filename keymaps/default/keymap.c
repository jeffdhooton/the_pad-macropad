#include QMK_KEYBOARD_H

#include "quantum.h"

#define ENCODER_BTN_1 B4
#define ENCODER_BTN_2 F6

enum layers {
  BASE
};

enum custom_keycodes {
    WIRE_SHADE_TOGGLE,
    ZOOM_TOGGLE_MIC,
    ZOOM_TOGGLE_VIDEO,
    MEET_TOGGLE_MIC,
    MEET_TOGGLE_VIDEO,
    OPEN,
    SAVE,
    CLOSE_WINDOW,
    RUN,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_ortho_4x4(
        KC_MPRV,            KC_MPLY,            KC_MNXT,            KC_MUTE,  // Row 1: Media controls and toggle layer
        KC_COPY,            KC_PASTE,           KC_CUT,             KC_UNDO,     // Row 2: Common editing commands
        OPEN,               SAVE,               CLOSE_WINDOW,       RUN,         // Row 3: Open, Save, Close Tab, Compile/Run
        ZOOM_TOGGLE_MIC,    ZOOM_TOGGLE_VIDEO,  MEET_TOGGLE_MIC,    MEET_TOGGLE_VIDEO      // Row 4: Zoom & google meet commands
    )
};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [BASE] = { ENCODER_CCW_CW(KC_VOLU, KC_VOLD), ENCODER_CCW_CW(KC_PGUP, KC_PGDN) },
};

void matrix_init_user(void) {
    // Configure ENCODER_BTN_1 (B4)
    DDRB &= ~(1<<4);  // Set pin B4 as input
    PORTB |= (1<<4);  // Enable pull-up resistor on B4

    // Configure ENCODER_BTN_2 (F6)
    DDRF &= ~(1<<6);  // Set pin F6 as input
    PORTF |= (1<<6);  // Enable pull-up resistor on F6
}

void keyboard_post_init_user(void) {
  debug_enable = true;
  debug_matrix = true;
  debug_keyboard = true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
switch (keycode) {
        case KC_COPY:
            if (record->event.pressed) {
                // Send Cmd+C for Copy
                register_code(KC_LGUI);
                tap_code(KC_C);
                unregister_code(KC_LGUI);
            }
            break;
        case KC_PASTE:
            if (record->event.pressed) {
                // Send Cmd+V for Paste
                register_code(KC_LGUI);
                tap_code(KC_V);
                unregister_code(KC_LGUI);
            }
            break;
        case KC_CUT:
            if (record->event.pressed) {
                // Send Cmd+X for Cut
                register_code(KC_LGUI);
                tap_code(KC_X);
                unregister_code(KC_LGUI);
            }
            break;
        case KC_UNDO:
            if (record->event.pressed) {
                // Send Cmd+X for Cut
                register_code(KC_LGUI);
                tap_code(KC_U);
                unregister_code(KC_LGUI);
            }
            break;

        case KC_O:
            if (record->event.pressed) {
                // Open - Using Cmd+O
                register_code(KC_LGUI);
                tap_code(KC_O);
                unregister_code(KC_LGUI);
            }
            break;

        // Save - Using Cmd+S
        case KC_S:
            if (record->event.pressed) {
                register_code(KC_LGUI);
                tap_code(KC_S);
                unregister_code(KC_LGUI);
            }
            break;

        // Close Tab - Using Cmd+W
        case KC_W:
            if (record->event.pressed) {
                register_code(KC_LGUI);
                tap_code(KC_W);
                unregister_code(KC_LGUI);
            }
            break;

        // Compile/Run - Using Cmd+B (commonly used in IDEs like Xcode)
        case KC_B:
            if (record->event.pressed) {
                register_code(KC_LGUI);
                tap_code(KC_B);
                unregister_code(KC_LGUI);
            }
            break;

        case ZOOM_TOGGLE_MIC:
            if (record->event.pressed) {
                register_code(KC_LGUI);
                register_code(KC_LSFT);
                tap_code(KC_A);
                unregister_code(KC_LSFT);
                unregister_code(KC_LGUI);
            }
            break;
        case ZOOM_TOGGLE_VIDEO:
            if (record->event.pressed) {
                register_code(KC_LGUI);
                register_code(KC_LSFT);
                tap_code(KC_V);
                unregister_code(KC_LSFT);
                unregister_code(KC_LGUI);
            }
            break;
        case MEET_TOGGLE_MIC:
            if (record->event.pressed) {
                register_code(KC_LGUI);
                tap_code(KC_D);
                unregister_code(KC_LGUI);
            }
            break;

        case MEET_TOGGLE_VIDEO:
            if (record->event.pressed) {
                register_code(KC_LGUI);
                tap_code(KC_E);
                unregister_code(KC_LGUI);
            }
            break;
    }
    return true;
}

void matrix_scan_user(void) {
  static uint8_t encoderBtn1State = 0;
  static uint8_t encoderBtn2State = 0;

  // Check if button is pressed
  uint8_t currentEncoderBtn1State = readPin(ENCODER_BTN_1);
  uint8_t currentEncoderBtn2State = readPin(ENCODER_BTN_2);

  // Check for state change from high to low (button pressed)
  if (encoderBtn1State && !currentEncoderBtn1State) {
      tap_code(KC_MUTE); // Assign a keycode to the first button
  }
  if (encoderBtn2State && !currentEncoderBtn2State) {
      tap_code(KC_CALC); // Assign a keycode to the second button
  }

  // Update state
  encoderBtn1State = currentEncoderBtn1State;
  encoderBtn2State = currentEncoderBtn2State;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
  uprintf("encoder_update_user: index=%d, clockwise=%d\n", index, clockwise);
  if (index == 0) {
      if (clockwise) {
          tap_code(KC_VOLU);
      } else {
          tap_code(KC_VOLD);
      }
  }
  else if (index == 1) {
      if (clockwise) {
          tap_code(KC_PGDN);
      } else {
          tap_code(KC_PGUP);
      }
  }
  return false;
}
