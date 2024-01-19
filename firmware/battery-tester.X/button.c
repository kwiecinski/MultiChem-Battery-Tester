/**
  * MIT License
  * 
  * Copyright (c) 2020 Taras Kornuta (Leon11t)
  * 
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  * 
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  * 
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
**/


#include <stdio.h>
#include "main.h"
#include "LCD-KS0108/IO_Macros.h"
#include "interrupts.h"
#include "button.h"

#define BTN_PROCESS_DEF        (10)
#define BTN_DEBOUNCE_DEF       (20)
#define BTN_LONG_PRESS_DEF     (1000)

#define BTN_DEBOUNCE_TIME      (this.debounce_time_ms / this.process_time_ms)
#define BTN_LONG_PRESS_TIME(x) (this.instance[x].long_press_time_ms / this.process_time_ms)

#define BUTTON_COUNT    5

#define SW_RIGHT_GetValue 
#define SW_LEFT_GetValue


/////////// Functions for reading button state ////////////////

uint8_t ButtonRight(void)
{
   return DigitalRead(RIGHT_BTN);
}

uint8_t ButtonLeft(void)
{
   return DigitalRead(LEFT_BTN);
}

uint8_t ButtonUp(void)
{
   return DigitalRead(UP_BTN);
}

uint8_t ButtonDown(void)
{
   return DigitalRead(DOWN_BTN);
}

uint8_t ButtonOK(void)
{
   return DigitalRead(OK_BTN);
}

/////////// Button definitions ////////////////
btn_instance_t instance[BUTTON_COUNT] = 
{
  {
      .port_read = ButtonRight,
      .long_press_time_ms = 1200,
  },
  {
      .long_press_time_ms = 1200,
      .port_read = ButtonLeft,
  },
  {
      .long_press_time_ms = 1200,
      .port_read = ButtonUp,
  }, 
  {
      .long_press_time_ms = 1200,
      .port_read = ButtonDown,
  },  
  {
      .long_press_time_ms = 1200,
      .port_read = ButtonOK,
  },
};

      
/////////// Callback functions for button pressing ////////////////





/////////// Button instance configuration parameters ////////////////

static btn_init_t this = 
{
   .process_time_ms = 10,        // time of update rate
   .debounce_time_ms = 20,       
   .instance = &instance[0],
};




void Button_Init(void)
{
    PinMode(LEFT_BTN, Input);
    PinMode(RIGHT_BTN, Input);
    PinMode(UP_BTN, Input);
    PinMode(DOWN_BTN, Input);
    PinMode(OK_BTN, Input);
      
 // init internal variables, set defaults
  
  this.process_time_ms = (this.process_time_ms) ? this.process_time_ms  : BTN_PROCESS_DEF;
  this.debounce_time_ms = (this.debounce_time_ms) ? this.debounce_time_ms : BTN_DEBOUNCE_DEF;
  this.long_press_def_ms = (this.long_press_def_ms) ? this.long_press_def_ms : BTN_LONG_PRESS_DEF; 

  for(uint8_t i = 0; i < BUTTON_COUNT; i++)
  {
    this.instance[i].state.locked = 0;
    this.instance[i].state.prew = BTN_STATE_NONE;
    this.instance[i].long_count = 0;
    this.instance[i].lock_count = 0;
    this.instance[i].long_press_time_ms = (this.instance[i].long_press_time_ms) ? this.instance[i].long_press_time_ms : this.long_press_def_ms;
  }
}


void Button_Update(void)
{
  uint8_t key = 0;
  
  
  if(button_counter>=10)
  {
    while(key < BUTTON_COUNT)
    {
      btn_state_t now_pressed = BTN_STATE_NONE;
      
      now_pressed = ((this.instance[key].port_read)()==0 ? BTN_STATE_SHORT : BTN_STATE_NONE);

      if (BTN_STATE_SHORT == now_pressed)                 // Button pressed   
      {
        if (!this.instance[key].state.locked)
        {
          if (++this.instance[key].lock_count >= BTN_DEBOUNCE_TIME) // Debounce time
          {
            this.instance[key].state.locked = 1;
          }
        }

        if ((this.instance[key].state.locked) && 
            (BTN_STATE_SHORT == this.instance[key].state.prew))
        {
          if (++this.instance[key].long_count >= BTN_LONG_PRESS_TIME(key))
          {
            this.instance[key].state.act = BTN_STATE_LONG;
            if (NULL != this.long_press) this.long_press(key);
          }
        }
        this.instance[key].state.prew = now_pressed;
      }
      else if (BTN_STATE_NONE == now_pressed)            // Button released
      {
        if (this.instance[key].state.locked)
        {
          if (this.instance[key].lock_count)
          { 
            this.instance[key].lock_count--;
          }
          else
          {
            this.instance[key].state.locked = 0;
            if (this.instance[key].long_count < BTN_LONG_PRESS_TIME(key))
            {
              if (0 == this.instance[key].lock_count)
              {
                this.instance[key].state.act = BTN_STATE_SHORT;
                if (NULL != this.short_release) this.short_release(key);
              }
            }
            else
            {
              if (BTN_STATE_LONG == this.instance[key].state.act)
              {
                if (NULL != this.long_release) this.long_release(key);
              }
            }
          }
        }
        else
        {
          this.instance[key].long_count = 0;
        }
      }
      key++;
    }
      button_counter=0;
  }
}

btn_state_t Button_EventGet(uint8_t key)
{
  if (key >= BUTTON_COUNT) return 0;
  return this.instance[key].state.act;
 
}

void Button_Event_Reset(void)
{
    this.instance[0].state.act = 0;
    this.instance[1].state.act = 0;
    this.instance[2].state.act = 0;
    this.instance[3].state.act = 0;
    this.instance[4].state.act = 0;

 
}

