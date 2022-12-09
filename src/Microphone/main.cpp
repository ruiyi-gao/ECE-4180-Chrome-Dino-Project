#include "mbed.h"
#include "USBKeyboard.h"
#include "rtos.h"
//Adafruit MEMs SPW2430 microphone demo - LEDs display audio level
BusOut myleds(LED1,LED2,LED3,LED4);
USBKeyboard keyboard;
Serial pc(USBTX, USBRX);

class microphone
{
public :
    microphone(PinName pin);
    float read();
    operator float ();
private :
    AnalogIn _pin;
};
microphone::microphone (PinName pin):
    _pin(pin)
{
}
float microphone::read()
{
    return _pin.read();
}
inline microphone::operator float ()
{
    return _pin.read();
}

microphone mymicrophone(p16);

volatile float preValue=0;
volatile float currValue;
Mutex key;

void keyboardUpThread(void const *argument) {
     while (1) {
        currValue = mymicrophone.read()*1000;
        if ((currValue-preValue)>20) {
            key.lock();
            keyboard.keyCode(UP_ARROW);
            key.unlock();
            Thread::wait(200);
        }
        preValue = currValue;
    } 
}

int main()
{
    Thread thread1(keyboardUpThread);
    while(1) {
//read in, subtract 0.67 DC bias, take absolute value, and scale up .1Vpp to 15 for builtin LED display
        key.lock();
//        myleds = int(abs((mymicrophone - (0.67/3.3)))*500.0);
        pc.printf("%f\n",currValue);
        key.unlock();
//Use an 8kHz audio sample rate (phone quality audio);
//        Thread::wait(100);
    }
}