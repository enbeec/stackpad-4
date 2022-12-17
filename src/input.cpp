#include "input.h"
#include "deps.h"

inline void nullHook(Input::Key key){};

using namespace Input;
Adafruit_NeoKey_1x4 Keypad::neokey;

const int Keypad::NEOKEY_START = Input::Key::K_8;
const int Keypad::NUM_KEYS = Input::Key::K_1 + 1;

void Keypad::neokeyIR()
{
    update(neokey.read(), NEOKEY_START);
}

void Keypad::neokeySetup(uint8_t neokeyAddr)
{
    if (!neokey.begin(neokeyAddr))
    {
        Serial.println("Failed to start NeoKey 1x4 -- aborting");
        // loop forever
        while (1)
            delay(3000);
    }
    Serial.println("NeoKey 1x4 Connected");
}

volatile uint32_t Keypad::state = 0;
volatile uint32_t Keypad::delta = 0;

void Keypad::update(int value, int offset)
{
    // start by masking off the rightmost bits after offset
    uint32_t mask = 0xffff << offset;
    uint32_t updated = (value << offset) & mask;

    // invert the mask when ORing in the old state to merge
    mask = ~mask;
    updated |= state & mask;

    // store results
    delta = state ^ updated;
    state = updated;
}

void Keypad::setup()
{
    Serial.println("Setting up keypad...");

    Serial.println("Setting up NeoKey 1x4...");
    neokeySetup(NEOKEY_1X4_ADDR);

    Serial.println("Setting up keypad interrupt routines.");
    attachInterrupt(NEOKEY_INTERRUPT_PIN, neokeyIR, LOW);

    Serial.println("Keypad setup complete!");
};

void Keypad::attach(Input::Hook newHook)
{
    hook = newHook;
};

void Keypad::detach()
{
    hook = *nullHook;
};

void Keypad::handle()
{
    for (uint8_t i = 0; i < NUM_KEYS; i++)
    {
        if (bitRead(delta, i))
            hook(static_cast<Input::Key>(i));
    }

    delta = 0;
};
