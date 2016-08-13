#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "GPIOClass.h"
#include  "stepper.h"
using namespace std;

void sig_handler(int sig);

bool ctrl_c_pressed = false;

int StepPins[4] = {5,6,13,19};

//int Seq[8][4] =     {{1,0,0,1},
//                    {1,0,0,0},
//                    {1,1,0,0},
//                    {0,1,0,0},
//                    {0,1,1,0},
//                    {0,0,1,0},
//                    {0,0,1,1},
//                    {0,0,0,1}};

int Seq[4][4] =     {{1,0,1,0},
                    {0,1,1,0},
                    {0,1,0,1},
                    {1,0,0,1}};
   

uint StepCount = 4;
int StepDir = 1;

long WaitTime = 20000;

int StepCounter;


int main (void)
{
    const int stepsPerRevolution = 100;  // change this to fit the number of steps per revolution
                                  // for your motor.  In this case the motor is 7.5 degrees per step

    // initialize the stepper library:
    Stepper myStepper(stepsPerRevolution, 5,6,13,19);  
    myStepper.setSpeed(200);
    
//    while(true)
//    {
//        for (int i=0; i<4; i++)
//        {
//            myStepper.stepMotor(i);
//            usleep(20000);
//        }
//        
//    }
    int counter =0;
    while(counter<5)
    {
        myStepper.step(2 * (stepsPerRevolution)); // make 4 revolutions
        sleep(2);
        myStepper.step(- (2*stepsPerRevolution));    // make 1 counter revolutions
        sleep(2);
        counter++;
    }
    
//    StepCounter = 0;
//    int xpin;
//    
//    struct sigaction sig_struct;
//    sig_struct.sa_handler = sig_handler;
//    sig_struct.sa_flags = 0;
//    sigemptyset(&sig_struct.sa_mask);
//
//    if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
//        cout << "Problem with sigaction" << endl;
//        exit(1);
//    }
//
//    string inputstate;
//    // TODO : create a pointer array for the gpio
//    GPIOClass* gpio[4];
//    gpio[0] = new GPIOClass("5");
//    gpio[1] = new GPIOClass("6");
//    gpio[2] = new GPIOClass("13");
//    gpio[3] = new GPIOClass("19");
//    
//    for(int i = 0; i < 4; i++)
//    {
//        gpio[i]->export_gpio();
//        gpio[i]->setdir_gpio("out");
//        gpio[i]->setval_gpio("0");
//    }
//
//    cout << " GPIO pins exported" << endl;
//    
//    cout << " Set GPIO pin directions" << endl;
//    
//    while(true)
//    {
//        for(int i = 0; i < 4; i++)
//        {
//        gpio[i]->setval_gpio("0");
//        }
//        for(int pin = 0; pin <4; pin++)
//        {
//            xpin = StepPins[pin];
//            if(Seq[StepCounter][pin]!=0)
//            {
//                cout << "Enable GPIO " << xpin << endl;
//                gpio[pin]->setval_gpio("1");
//            }
//            else
//            {
//                cout << "Disable GPIO " << xpin << endl;
//                gpio[pin]->setval_gpio("0");
//            }
//        }
//        StepCounter += StepDir;
//        if (StepCounter>=StepCount)
//        {
//            StepCounter = 0;
//        }
//        if (StepCounter<0)
//        {
//            StepCounter = StepCount+StepDir;
//        }
//
//        usleep(WaitTime);
//    }
    return 0;
}

void sig_handler(int sig)
{
    write(0,"nCtrl^C pressed in sig handlern",32);
    ctrl_c_pressed = true;
}
