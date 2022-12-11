#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_NeoKey_1x4.h>
#include <seesaw_neopixel.h>

int delayMs = 20;

// ======= NEOKEY 1x4 =========
Adafruit_NeoKey_1x4 neokey;
void neokeySetup(int addr) {
    if (! neokey.begin(addr)) {
        Serial.println("Failed to start NeoKey");
        while(1) delay(delayMs);
    } else {
        Serial.println("NeoKey 1x4 Connected");
    }
}

void handleKeys() {
    uint8_t bits = neokey.read();
    
    for (uint16_t i = 0; i < 4; i++) {
        if (bitRead(bits, i)) neokey.pixels.setPixelColor(i, 0xAA00AA);
        else neokey.pixels.setPixelColor(i, 0x000000);
    }

    neokey.pixels.show();
}

// ======= MAIN ========
void setup()
{
    Serial.begin(115200);
    Serial.println("Serial Connected");
    neokeySetup(0x30);
}

void loop()
{
    handleKeys();
    delay(delayMs);
}