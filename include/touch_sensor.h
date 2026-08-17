/* 4-Way Button:  Click, Double-Click, Press+Hold, and Press+Long-Hold

Output events from a single push-button.
  1) Click:  rapid press and release
  2) Double-Click:  two clicks in quick succession
  3) Press and Hold:  holding the button down
  4) Long Press and Hold:  holding the button for a long time */


#ifndef TOUCH_SENSOR_H
#define TOUCH_SENSOR_H

#include <Arduino.h>
#include <display.h>
#include <rfid.h>

bool developer_mode_touch = true ;
//if (developer_mode_touch ==true)

int touch_sensor = A3; //Analog Pin for the Connection of the Button

//=================================================
//Stablishing a threshold Value for the Touch Sensor Sensibility

int analogValue = 0;
int touch_threshold = 100; // from 0 to 255

//=================================================
//  MULTI-CLICK:  One Button, Multiple Events

int button_status = 0;


//Stablished times for the button_status
int short_hold_time = 3 * 1000 ;
int long_hold_time  = 7 * 1000 ;
int stuck_time      = 10 * 1000 ;

//=================================================
//BUTTON SETUP

void setup_touch_sensor()
{
  pinMode(touch_sensor, INPUT);
}

//=================================================
// Events to trigger

void ClickEvent()
{
  if (!locked)
  {

    if (developer_mode_touch)
    {
       serial_ClickEvent();
      display_ClickEvent(locked);
    }

    lock();
  }

}

void ShortHoldEvent()
{
  if (locked)
  {
    if (developer_mode_touch)
    {
       serial_ShortHoldEvent();
      display_ShortHoldEvent(locked);
    }

  }


}

void LongHoldEvent()
{
  if (locked)
  {
    if (developer_mode_touch)
    {
       serial_LongHoldEvent();
      display_LongHoldEvent(locked);
    }
  }

}

void StuckEvent()
{
  if (locked)
  {
    if (developer_mode_touch)
    {
      serial_StuckEvent();
      display_StuckEvent(locked);
    }
  }
}

int checkButton()
{
  analogValue = analogRead(touch_sensor);

  /* ---- STATUS :

  0 - NO BUTTON PRESSED Detected
  1 - SINGLE CLICK Detected
  2 - HOLD EVENT Detected
  3 - LONG HOLD DETECTED
  4 - PRESSED MORE THAN 10 SECONDS WILL BREAK THE LOOP AND RETURN A LONG PRESSED EVENT AND NOT ALLOW TO HAVE THE BUTTON PRESSED AGAIN UNTIL A BUTTON RELEASE HAS BEEN DETECTED
  5 - BUTTON STILL BEING PRESSED BY THE USER
  */

  if (analogValue < touch_threshold) //Button Not Touched
  {
    button_status=0;

    if(developer_mode_touch)
    {
      display_button_status(button_status);
    }
    return button_status;
  }

  else //Button Touched
  {
    button_status = 1;

    if(developer_mode_touch)
    {
      serial_button_status(button_status);
      display_button_status(button_status);
    }

    long initial_time = millis();
    long printing_time = millis();

    while(1)
    {
      analogValue = analogRead(touch_sensor); //Reading the State of the Sensor

      if (analogValue  > touch_threshold) //Button is still Pressed
      {
        long current_time = millis() - initial_time ;

        if (millis() - printing_time > 1000 && developer_mode_touch)
        {
          serial_button_status(10);
          printing_time = millis();
        }


        if( current_time > short_hold_time && button_status == 1)
        {
          button_status = 2 ;

          if(developer_mode_touch)
          {
            serial_button_status(button_status);
            display_button_status(button_status);
          }

        }
        else if( current_time > long_hold_time && button_status == 2)
        {
          button_status = 3 ;

          if(developer_mode_touch)
          {
            serial_button_status(button_status);
            display_button_status(button_status);
          }
        }
        else if(current_time > stuck_time && button_status == 3)
        {
          button_status = 4;

          if(developer_mode_touch)
          {
            serial_button_status(button_status);
            display_button_status(button_status);
          }

          while (1) //Stuck here until the User release the Button
          {
            analogValue = analogRead(touch_sensor);
            if (analogValue < touch_threshold) break;
          }
        }
      }
      else //Button Released
      {
        if(developer_mode_touch) //Printing Release Time
        {
          long final_time = millis() - initial_time ;

          serial_button_status(final_time);
          display_button_status(final_time);
        }
        return button_status;
      } //Button Released END
    } //While
  }//Button Trigger
}

void check_touch_sensor(int general_status)
{
   // Get button event and act accordingly
   int pressed_status = checkButton();

   switch(pressed_status)
   {
     case 1:
     {
       ClickEvent();
       break;
     }
     case 2:
     {
       ShortHoldEvent();
       break;
     }
     case 3:
     {
       LongHoldEvent();
       break;
     }
     case 4:
     {
       StuckEvent();
       break;
     }
   }
}

#endif
