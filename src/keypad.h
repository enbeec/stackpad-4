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

// ======= KEYPAD INTERRUPTS ========
// G11 on MicroMod ATP board
#define NEOKEY_INTERRUPT_PIN 27

void keypadSetup();