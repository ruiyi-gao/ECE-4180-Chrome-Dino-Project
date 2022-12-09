#include "mbed.h"
#include "HCSR04.h"
#include "USBKeyboard.h"
#include "rtos.h"41
USBKeyboard keyboard;
volatile int baseline = -1;
volatile float distance;

Serial pc(USBTX, USBRX);
HCSR04 sensor(p21, p22);
DigitalOut led1(led1);
DigitalOut led2(led2);
DigitalOut led3(led3);
DigitalOut led4(led4);
Mutex key;
Timer downTime;

void keyboardUpThread(void const *argument) {
     while (1) {
        key.lock();
        if (distance<100) {
            downTime.start();
            while(downTime.read()<=0.2)  keyboard.keyCode(DOWN_ARROW);
            downTime.reset();
        }
        else if (distance>200) {
            keyboard.keyCode(UP_ARROW);
            Thread::wait(200);
        }
        key.unlock();
    }  
}

int main()
{
//    sensor.setRanges(10, 300);
    Thread thread1(keyboardUpThread);
    while (1) {
        key.lock();
        sensor.startMeasurement();
        while(!sensor.isNewDataReady()) {
            // wait for new data
            // waiting time depends on the distance
        }
        distance = sensor.getDistance_mm();
        pc.printf("%f\n",distance);
        key.unlock();
    }
}