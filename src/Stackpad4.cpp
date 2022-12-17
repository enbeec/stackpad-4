#include "deps.h"
#include "input.h"

// globals
Input::Keypad keypad;

// void keyHook(int key) {
//     if (KEY(key)) {
//         // keydown
//     } else {
//         // keyup
//     }
// }

// ======= MAIN ========
#define DELAY_MS 50

void setup()
{
    Serial.begin(115200);
    Serial.println("Serial Connected");
    keypad.setup();
    // keypad.attach(keyHook);
}

void loop()
{
    keypad.handle();
    delay(DELAY_MS);
}