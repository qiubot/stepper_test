/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "stepper.h"
#include <unistd.h>
#include <string>
#include <sstream>
#include <iostream>


/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */
Stepper::Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2,
                                      int motor_pin_3, int motor_pin_4)
{
  this->step_number = 0;    // which step the motor is on
  this->speed = 0;      // motor speed
  this->last_step_time = 0; // time stamp in us of the last step taken
  this->number_of_steps = number_of_steps; // total number of steps for this motor


  // setup the pins on the microcontroller:
  stringstream line;
  string temp;
  line << motor_pin_1;
  line >> temp;
  gpio[0] = new GPIOClass(temp);
  line.clear();
  line << motor_pin_2;
  line >> temp;
  gpio[1] = new GPIOClass(temp);
  line.clear();
  line << motor_pin_3;
  line >> temp;
  gpio[2] = new GPIOClass(temp);
  line.clear();
  line << motor_pin_4;
  line >> temp;
  gpio[3] = new GPIOClass(temp);
  for(int i = 0; i < 4; i++)
    {
        gpio[i]->export_gpio();
        gpio[i]->setdir_gpio("out");
        gpio[i]->setval_gpio("0");
    }
  

  // pin_count is used by the stepMotor() method:
  this->pin_count = 4;
}


/*
 * Sets the speed in revs per minute
 */
void Stepper::setSpeed(long whatSpeed)
{
    if(whatSpeed > 0)
    {
        this->step_delay = 60L * 1000L * 1000L / this->number_of_steps / whatSpeed;
        this->speed = whatSpeed;
    }
    else if (whatSpeed == 0)
    {
        this->step_delay = 1000000;
        this->speed = whatSpeed;
    }
}

/*
 * Moves the motor steps_to_move steps.  If the number is negative,
 * the motor moves in the reverse direction.
 */
void Stepper::step(int steps_to_move)
{
  int steps_left = steps_to_move > 0 ? steps_to_move : -steps_to_move;  // how many steps to take;

  // decrement the number of steps, moving one step each time:
  while (steps_left > 0)
  {
      if (this->speed == 0)
      {
          break;
      }
      // increment or decrement the step number,
      // depending on direction:
      if (steps_to_move > 0)
      {
        this->step_number++;
        if (this->step_number == this->number_of_steps) {
          this->step_number = 0;
        }
      }
      else
      {
        if (this->step_number == 0) {
          this->step_number = this->number_of_steps;
        }
        this->step_number--;
      }
      // decrement the steps left:
      steps_left--;
      // step the motor to step number 0, 1, ..., {3 or 10}
      stepMotor(this->step_number % 4);
      usleep(this->step_delay);
  }
}

void Stepper::stepForever()
{

  // decrement the number of steps, moving one step each time:
  while (true)
  {
      // increment or decrement the step number,
      // depending on direction:
      if (this->speed == 0)
      {
          break;
      }
      if (this->speed > 0)
      {
        this->step_number++;
        if (this->step_number == this->number_of_steps) {
          this->step_number = 0;
        }
      }
      else
      {
        if (this->step_number == 0) {
          this->step_number = this->number_of_steps;
        }
        this->step_number--;
      }
      // decrement the steps left:
      // step the motor to step number 0, 1, ..., {3 or 10}
      stepMotor(this->step_number % 4);
      usleep(this->step_delay);
  }
}

/*
 * Moves the motor forward or backwards.
 */
void Stepper::stepMotor(int thisStep)
{
      for (int i=0; i < this->pin_count; i++)
      {
          if(four_phase_seq[thisStep][i]!=0)
            {
                gpio[i]->setval_gpio("1");
            }
            else
            {
                gpio[i]->setval_gpio("0");
            }
      }
}
