/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   stepper.h
 * Author: caochao
 *
 * Created on August 12, 2016, 4:49 PM
 */

#ifndef STEPPER_H
#define STEPPER_H

#include <thread>
#include "GPIOClass.h"

class Stepper {
  public:
    // constructors:
    
    Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2,
                                 int motor_pin_3, int motor_pin_4);
    ~Stepper(void);
    // speed setter method:
    void setSpeed(double whatSpeed); //0 is for brake


    // mover method:
    void step(int number_of_steps);
    
    void disable();

    thread getThread();
     
  private:
   
    void stepMotor(int this_step);
    void stepForever();
    
    unsigned long step_delay; // delay between steps, in ms, based on speed
    int number_of_steps;      // total number of steps this motor can take
    int pin_count;            // how many pins are in use.
    int step_number;          // which step the motor is on
    double speed;             // speed of the step motor; 0 is for brake, positive for clockwise and negative counter-clockwise
    bool motor_enabled;

    GPIOClass* gpio[4];       // four GPIOClass
    
    unsigned long last_step_time; // time stamp in us of when the last step was taken
    
};

const int four_phase_seq[4][4] = {{1,0,1,0},
                                {0,1,1,0},
                                {0,1,0,1},
                                {1,0,0,1}};

#endif /* STEPPER_H */

