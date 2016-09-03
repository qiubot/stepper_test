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

int initial_speed = 100;

int main (void)
{
    struct sigaction sig_struct;
    sig_struct.sa_handler = sig_handler;
    sig_struct.sa_flags = 0;
    sigemptyset(&sig_struct.sa_mask);

    if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
        cout << "Problem with sigaction" << endl;
        exit(1);
    }
    
    const int stepsPerRevolution = 100;  // change this to fit the number of steps per revolution
                                  // for your motor.  In this case the motor is 7.5 degrees per step

    // initialize the stepper library:
    Stepper* stepperMotors[4];
    thread* thd[4];
    
    stepperMotors[0] = new Stepper(stepsPerRevolution, 5,6,13,19);
    stepperMotors[1] = new Stepper(stepsPerRevolution, 4,17,27,22);
    stepperMotors[2] = new Stepper(stepsPerRevolution, 26,16,20,21);
    stepperMotors[3] = new Stepper(stepsPerRevolution, 23,24,25,12);
    
    for (int i=0; i<4; i++)
    {
        thd[i] = new thread(stepperMotors[i]->getThread());
    }
    
  
    //cout << "Thread has started" << endl;
    
    while(true)
    {
        if(ctrl_c_pressed)
        {
            cout << "Ctrl^C Pressed" << endl;
            cout << "destroy steppers" << endl;
            for (int i=0; i<4; i++)
            {
                stepperMotors[i]->disable();
                thd[i]->join();
            }
            delete [] stepperMotors;
            delete [] thd;
            break;
        }
        else
        {
            //initial_speed = initial_speed;
            for (int i=0; i<4; i++)
            {
                stepperMotors[i]->setSpeed(initial_speed);
            }
            cout << "Now the speed is " << initial_speed << endl;
            sleep(3);
        }
    }

    cout << "Exiting....." << endl;
    
    return 0;
}

void sig_handler(int sig)
{
    write(0,"nCtrl^C pressed in sig handlern",32);
    ctrl_c_pressed = true;
}
