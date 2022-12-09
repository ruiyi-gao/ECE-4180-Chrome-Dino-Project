#include "mbed.h"
#include "HCSR04.h"
#include "USBKeyboard.h"
#include "rtos.h"
#include "DebounceIn.h"

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

class Nav_Switch
{
public:
    Nav_Switch(PinName up,PinName down,PinName left,PinName right,PinName fire);
    int read();
//boolean functions to test each switch
    bool up();
    bool down();
    bool left();
    bool right();
    bool fire();
//automatic read on RHS
    operator int ();
//index to any switch array style
    bool operator[](int index) {
        return _pins[index];
    };
private:
    BusIn _pins;

};

Nav_Switch::Nav_Switch (PinName up,PinName down,PinName left,PinName right,PinName fire):
    _pins(up, down, left, right, fire)
{
    _pins.mode(PullUp); //needed if pullups not on board or a bare nav switch is used - delete otherwise
    wait(0.001); //delays just a bit for pullups to pull inputs high
}
inline bool Nav_Switch::up()
{
    return !(_pins[0]);
}
inline bool Nav_Switch::down()
{
    return !(_pins[1]);
}
inline bool Nav_Switch::left()
{
    return !(_pins[2]);
}
inline bool Nav_Switch::right()
{
    return !(_pins[3]);
}
inline bool Nav_Switch::fire()
{
    return !(_pins[4]);
}
inline int Nav_Switch::read()
{
    return _pins.read();
}
inline Nav_Switch::operator int ()
{
    return _pins.read();
}

Mutex key;
Timer downTime;
Serial pc(USBTX, USBRX);

USBKeyboard keyboard; //USB Keyboard Library
HCSR04 sensor(p21, p22); //Lidar Sensor
microphone mymicrophone(p16);
Nav_Switch myNav( p9, p6, p7, p5, p8);
DebounceIn pb_up(p13);
DebounceIn pb_down(p14);

// General Variables
volatile bool jump=0;
volatile bool crouch=0;

// Lidar Variables
volatile float lidar_distance=150;

// Microphone Variables
volatile float mic_pre_value=0;
volatile float mic_curr_value;
volatile float mic_diff=0;

// Nav Switch Variables
volatile uint8_t left_click = 0;
volatile bool nav_up = 0;
volatile bool nav_down = 0;

// Push Button Variables
volatile bool pb_up_pushed = 0;
volatile bool pb_down_pushed = 0;


void keyboardUpThread(void const *argument) {
     while (1) {
        // Choose Corresponding Actions
        if (lidar_distance>200 || mic_diff>20 || nav_up==1 || pb_up_pushed==1) {
            jump = 1;
        } else if (lidar_distance < 100 || nav_down==1 || pb_down_pushed==1) {
            crouch =  1;
        } else {
            jump = 0;
            crouch = 0;
        }
        
        // Perform the Action using Keyboard
        key.lock();
        if (jump == 1) {
            keyboard.keyCode(UP_ARROW);
            Thread::wait(200);
        } else if (crouch == 1) {
            downTime.start();
            while(downTime.read()<=0.2)  keyboard.keyCode(DOWN_ARROW);
            downTime.reset();
        }
        key.unlock();
    }  
}

void lidarThread(void const *argument) {
    while (1) {
        key.lock();
        sensor.startMeasurement();
        while(!sensor.isNewDataReady()) {
            // wait for new data
        }
        lidar_distance = sensor.getDistance_mm();
        key.unlock();
    }
}

void microphoneThread(void const *argument) {
    while (1) {
        mic_curr_value = mymicrophone.read()*1000;
        mic_diff = mic_curr_value - mic_pre_value;
        mic_pre_value = mic_curr_value;
        Thread::wait(200);
    }
}

void navSwitchThread(void const *argument) {
    while (1) {
        if (myNav.up()) nav_up = 1;
        else if (!myNav.up()) nav_up = 0;
        if (myNav.down()) nav_down = 1;
        else if (!myNav.down()) nav_down = 0;
        
        //check mouse left button click
        if (myNav.fire()) left_click = 1;
        if (!myNav.fire()) left_click = 0;
    }
}

int main()
{
    Thread thread1(keyboardUpThread);
//    Thread thread2(lidarThread);
//    Thread thread3(microphoneThread);
    Thread thread4(navSwitchThread);
    
    pb_up.mode(PullUp);
    pb_down.mode(PullUp);
    wait(0.1);
    
    while (1) {
        if (pb_up == 0) pb_up_pushed = 1;
        else if (pb_up == 1) pb_up_pushed = 0;
        if (pb_down == 0) pb_down_pushed = 1;
        else if (pb_down == 1) pb_down_pushed = 0;
        key.lock();
        pc.printf("%d\n",mymicrophone.read());
        key.unlock();
    }
}