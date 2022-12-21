/* Simple test program that blinks the LED on and off. */

#include "mbed.h"

#define DELAY_TIME 500ms

int main() {
    DigitalOut led(USER_LED);
    led = false;
    while (true) {
        led = !led;
        ThisThread::sleep_for(DELAY_TIME);
    }
}