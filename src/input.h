#ifndef INPUT_H
#define INPUT_H
#include "deps.h"

#define NEOKEY_INTERRUPT_PIN 27 // G11 on MicroMod ATP board

namespace Input {
    /* enumerated top to bottom, left to right */
    enum Key { K_8, K_4, K_2, K_1 };

    /* switch(key) -- visited by Keypad::handle() */
    typedef void (*Hook)(Key key);

    // TODO: enforce singleton (https://www.oreilly.com/library/view/c-cookbook/0596007612/ch08s10.html)
    class Keypad {
        public:
            void setup();
            void attach(Input::Hook newHook);
            void detach();
            void handle();
            uint32_t getState();

        private:
            const static int NEOKEY_START;

            static Adafruit_NeoKey_1x4 neokey;
            static void neokeyIR(); // interrupt routine
            static void neokeySetup(uint8_t neokeyAddr);
            
            const static int NUM_KEYS;

            /* bitwise flags */
            static volatile uint32_t state; // which keys are pressed
            static volatile uint32_t delta; // which keys have changed since last handle

            /* called once per IR */
            static void update(int value, int offset);
            
            Input::Hook hook;

    };
}

#endif /* INPUT_H */
