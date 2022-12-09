#include "mbed.h"
#include "USBKeyboard.h"
#include "DebounceIn.h"
 
DebounceIn pb_up(p13);
DebounceIn pb_down(p14);

//USBKeyboard
USBKeyboard keyboard;
 
int main(void) {  
    pb_up.mode(PullUp);
    pb_down.mode(PullUp);
    wait(0.1);
    while (1) {
        if (pb_up == 0) {
            keyboard.keyCode(UP_ARROW);
            wait(0.2);
        }
        // No delay here so we can continue to bow
        if (pb_down == 0) {
            keyboard.keyCode(DOWN_ARROW);
        }
    }
}