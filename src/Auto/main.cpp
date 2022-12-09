#include "mbed.h"
#include "USBKeyboard.h"
#include "rtos.h"

Serial pc(USBTX, USBRX);
AnalogIn red1(p20);
AnalogIn green1(p19);
AnalogIn red2(p18);
AnalogIn green2(p17);
Timer session;

DigitalOut led1(p10);

volatile float delayTime;
volatile float RedValue1;
volatile float GreenValue1;
volatile float RedValue2;
volatile float GreenValue2;
volatile bool started = 0;
volatile int mode = 0; //white mode=0, black mode=1, no jump mode=3

USBKeyboard keyboard;
Mutex key;

void keyboardUpThread(void const *argument) {
    while (1) {
        RedValue1 = red1.read()*1000;
        GreenValue1 = green1.read()*1000;
        RedValue2 = red2.read()*1000;
        GreenValue2 = green2.read()*1000;
        
        if (mode == 0) {
            led1 = 0;
            if (((RedValue1+GreenValue1)<17 || (RedValue2+GreenValue2)<17)) {
                delayTime = 300-session.read()*3.5;
                if (delayTime>0) {
                    Thread::wait(delayTime);
                }
                key.lock();
                keyboard.keyCode(UP_ARROW);
                key.unlock();
                if (!started) {
                    session.start();
                    started = 1;
                }
//                Thread::wait(200);
            }
            
            if ((session.read()>=55.5 && session.read()<=68.5) or (session.read()>=98)){
                mode = 2;
            }
        } else if (mode == 1) {
            led1=1;
            if (((RedValue1+GreenValue1)>7 || (RedValue2+GreenValue2)>7)) {
                delayTime = 300-session.read()*3;
                if (delayTime>0) {
                    Thread::wait(delayTime);
                }
                key.lock();
                keyboard.keyCode(UP_ARROW);
                key.unlock();
//                Thread::wait(200);

                
                if ((session.read()>=68.5)){
                    mode = 2;
                }
            }
        } else if (mode ==2) {
            if (session.read()>=98.1) mode = 1;
            else if (session.read()>=68.65) mode = 0;
            else if (session.read()>=56) mode = 1;
        }
        
    }
}

int main() {
    Thread thread1(keyboardUpThread);
    while(1) {
        key.lock();
//        pc.printf("red1: %f,\ngreen1: %f,\n",RedValue1,GreenValue1);
//        pc.printf("red2: %f,\ngreen2: %f,\n",RedValue2,GreenValue2);
        pc.printf("time: %f, mode: %d\n",session.read(),mode);
        key.unlock();
    }
}
