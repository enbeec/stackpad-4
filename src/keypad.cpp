#include <keypad.h>

extern volatile uint32_t keypadState;
extern volatile uint32_t keypadDelta;
extern Adafruit_NeoKey_1x4 neokey;

// inlining this because it's potentially hot and very short
void keypadUpdate(int value, int offset) {
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
    attachInterrupt(NEOKEY_INTERRUPT_PIN, neokeyIR, LOW);

    Serial.println("Keypad setup complete!");
}