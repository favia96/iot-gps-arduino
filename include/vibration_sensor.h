
#ifndef VIBRATION_SENSOR_H
#define VIBRATION_SENSOR_H

#include <Arduino.h>
#include <display.h>
#include <rfid.h>
#include <serial.h>

bool developer_mode_vibration = true;
bool self_reset_mode = true ;

const int vibration_sensor = A6; // the piezo is connected to analog pin 6

const int vibration_threshold = 500;  // threshold value (from 0 to 1023)

int parking_alarm_seconds = 5 ; //to decide when the detected movement is a vibration or just some random punch
int theft_alarm_seconds = 20 ; //Someone is definitely moving your object

bool  previous_state = false ;
bool vibrating = false ;      // variable to store the value read from the sensor pin

bool locked_alarm = false ;

int alarm_status = 0;

unsigned long initial_time = 0 ;
unsigned long final_time = 0 ;
unsigned long partial_time = 0 ;

int reset_seconds = 10 ;
int count = 0 ;

/*
For the Display

Cases:

  0 No Vibration (OLED to 0)
  1 Vibration    (OLED to 1)
  2 Parking Alert (Less than 5 Seconds of Movement)
  3 Theft Alert  (More than 20 Seconds of Continuous Movement )
  4 Re-Enable Status before Restarting(Self Restarting Mode)

*/

bool vibration()
{
  if ( analogRead(vibration_sensor) >= vibration_threshold) {return true;}
  else {return false;}
}

//DECIDE WETHER OR NOT PASS AN ARGUMENT TO INDICATE THE LOCKED MODE OR MAKE IT INTERNALLY . FOR NOW I WILL MAKE IT INTERNALLY
void check_vibration_sensor()
{
  if (!key_saved) return;

  else //The Master Key was saved properly
  {
    if (!locked_alarm) //Alarm has not being Triggered
    {
      vibrating = vibration();

      if(vibrating) //Moving
      {

        if (vibrating != previous_state) // Run Just The First Time after Start Vibrating
        {
          initial_time = millis();
          alarm_status = 1 ;

          if(developer_mode_vibration)

          {
            serial_vibrating(alarm_status,0);
            display_vibrating(alarm_status,locked);
          }
          previous_state = vibrating;
          partial_time = millis();
          count=0;
        }

        else //Still Vibrating without stopping
        {
          final_time = millis();

          if (developer_mode_vibration)
          {
            if( alarm_status == 1 || alarm_status == 2 )
            {
              if (final_time > partial_time + 1000 ) { Serial.print(" * "); partial_time = millis(); count++; }

              if (count == 5)
              {
                //Serial.println();
                count=0;
              }
            }
          }

          if ( (final_time - initial_time) >  (parking_alarm_seconds *1000) &&
               (final_time - initial_time) <= (theft_alarm_seconds *1000)   &&
                alarm_status == 1 )
          {

            alarm_status = 2;

            if (developer_mode_vibration)
            {
              serial_vibrating(alarm_status,0);
              display_vibrating(alarm_status,locked);
            }

          }
          else if (final_time - initial_time > theft_alarm_seconds*1000 && alarm_status == 2 )
          {
            alarm_status = 3;

            if(developer_mode_vibration)
            {
              serial_vibrating(alarm_status,0);
              display_vibrating(alarm_status,locked);
            }

            locked_alarm = true;
            partial_time = millis();

          }
        }

      }
      else //Not Moving And Alarm Not Triggered
      {
        if(vibrating != previous_state) //Stopped Vibrating within the Threshold of Time before Triggering the  Alarm
        {

          final_time = millis();

          if (developer_mode_vibration)
          {
            if (alarm_status > 1)
            {
              display_developer();
              if(locked) display_lock();
              else display_unlock();
              alarm_status=0;
              display_vibrating(alarm_status,locked);
            }
            else
            {
              display_vibrating(alarm_status,locked);
            }
          }
          alarm_status = 0;
          previous_state = vibrating;

          if (developer_mode_vibration)
          {
            serial_vibrating( alarm_status , final_time - initial_time );
          }
        }
        display_vibrating(alarm_status,locked);
      }

    }//locked_alarm


    else //TIME RESET (After 10 Seconds)
    {
      alarm_status = 4 ;
      final_time = millis();

      if (self_reset_mode)
      {
        if(developer_mode_vibration )
        {
          display_vibrating(alarm_status,locked);
          serial_vibrating(alarm_status , 0);
        }

        delay(5000);

        alarm_status = 5;

        if(developer_mode_vibration)
        {
          display_vibrating(alarm_status,locked);
          serial_vibrating(alarm_status , 0);
        }

        delay(2000);

        alarm_status = 0 ;
        locked_alarm = false ;
        initial_time = millis();
        previous_state=false;
      }

    } //TIME RESET END

  }//key_saved
}

#endif
