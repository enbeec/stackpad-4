#include "deps.h"
#include "input.h"

// globals
Input::Keypad keypad;

void keyHook(Input::Key key) {
    const bool state = bitRead(keypad.getState(), static_cast<int>(key));
    if (state)
        Serial.printf("%i down\n", key);
    else
        Serial.printf("%i up\n", key);
}

// ======= MAIN ========
#define DELAY_MS 50

void setup()
{
    Serial.begin(115200);
    Serial.println("Serial Connected");
    keypad.setup();
    keypad.attach(keyHook);
}

void loop()
{
    noInterrupts();
    keypad.handle();
    interrupts();
    delay(DELAY_MS);
}
