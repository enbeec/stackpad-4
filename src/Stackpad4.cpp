#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_NeoKey_1x4.h>
#include <seesaw_neopixel.h>

// ======= KEYPAD CONSTANTS ========
/* Stackpad4 is a keypad for (at its simplest) manipulating a 4bit stack.
 * The neokey 1x4 is arranged MSB-to-LSB starting with 0.
 */
#define K_8 0
#define K_4 1
#define K_2 2
#define K_1 3
// #define K_PUSH 4
// #define K_POP 5

// layout
#define NUM_KEYS 4
#define NUM_NEOKEYS 4

// offsets
#define NEOKEY_OFFSET 0 // start
#define AUXKEY_OFFSET 4 // add num previous (NUM_NEOKEYS)
// #define FOOKEY_OFFSET 6 // etc...

// ======= KEYPAD GLOBALS ========
Adafruit_NeoKey_1x4 neokey;

// ======= KEYPAD INTERRUPTS ========
// G11 on MicroMod ATP board
#define NEOKEY_INTERRUPT_PIN 27

// we only need 6 bits but 32 is faster on Teensy
volatile uint32_t keypadState = 0;
volatile uint32_t keypadDelta = 0;

// getters
#define KEY(i) bitRead(keypadState, i)
#define KEY_DELTA(i) bitRead(keypadDelta, i)

// inlining this because it's potentially hot and very short
static inline void keypadUpdate(int value, int offset) {
    // start by masking off the rightmost bits after offset
    uint32_t mask = 0xffff << offset;
    uint32_t updated = (value << offset) & mask;

    // invert the mask when ORing in the old state to merge
    mask = ~mask;
    updated |= keypadState & mask;

    // store results
    keypadDelta = keypadState ^ updated;
    keypadState = updated;
}

void neokeyIR() {
    keypadUpdate(neokey.read(), NEOKEY_OFFSET);
}

inline void keypadIRSetup() {
    attachInterrupt(NEOKEY_INTERRUPT_PIN, neokeyIR, FALLING);
}

// ======= KEYPAD HANDLER =======
#define PIX_OFF 0x000000
#define PIX_PURPLE  0xAA00AA

void keyHook(int key) {
    if (KEY(key)) {
        neokey.pixels.setPixelColor(key, PIX_PURPLE);
    } else {
        neokey.pixels.setPixelColor(key, PIX_OFF);
    }
}

inline void keyHandler() {
    for (uint8_t i = 0; i < NUM_KEYS; i++) {
        if (KEY_DELTA(i)) keyHook(i);
    }
    
    if (neokey.pixels.canShow()) neokey.pixels.show();
    
    keypadDelta = 0;
}

// ====== KEYPAD SETUP =======
void keypadSetup() {
    Serial.println("Setting up keypad...");
    if (! neokey.begin(NEOKEY_1X4_ADDR)) {
        Serial.println("Failed to start NeoKey -- aborting");
        // loop forever
        while(1) delay(3000);
    } else {
        Serial.println("NeoKey 1x4 Connected");
    }
    
    Serial.println("Setting up keypad interrupt routines.");
    keypadIRSetup();

    Serial.println("Keypad setup complete!");
}

// ======= MAIN ========
int delayMs = 20;

void setup()
{
    Serial.begin(115200);
    Serial.println("Serial Connected");
    keypadSetup();
}

void loop()
{
    keyHandler();
    delay(delayMs);
}