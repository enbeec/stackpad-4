#include <Arduino.h>

/* 
 *  defines K_{1,2,4,8} macros as well as keypadSetup()
 *  depends on globals defined below
 */
#include <keypad.h>

#define DEBUG_KEYPAD

// globals
Adafruit_NeoKey_1x4 neokey;
volatile uint32_t keypadState = 0;
volatile uint32_t keypadDelta = 0;

// getters
#define KEY(i) bitRead(keypadState, i)
#define KEY_DELTA(i) bitRead(keypadDelta, i)

// colors
#define PIX_OFF 0x000000
#define PIX_PURPLE  0xAA00AA

void keyHook(int key) {
    if (KEY(key)) {
        #ifdef DEBUG_KEYPAD
        Serial.printf("%i down\n", key);
        #endif
    } else {
        #ifdef DEBUG_KEYPAD
        Serial.printf("%i up\n", key);
        #endif
    }
}

inline void keyHandler() {
    #ifdef DEBUG_KEYPAD
    if (keypadDelta) {
        Serial.printf(
            "%i%i%i%i\n", 
            KEY(0), KEY(1), KEY(2), KEY(3)
        );
    }
    #endif
    for (uint8_t i = 0; i < NUM_KEYS; i++) {
        if (KEY_DELTA(i)) keyHook(i);
    }
    
    keypadDelta = 0;
}

// ======= MAIN ========
#define DELAY_MS 50

void setup()
{
    Serial.begin(115200);
    Serial.println("Serial Connected");
    keypadSetup();
}

void loop()
{
    keyHandler();
    delay(DELAY_MS);
}