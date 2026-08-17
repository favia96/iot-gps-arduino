//THIS IS THE MAIN FILE (THE CORE OF THE SYSTEM)

//HERE YOU CAN FIND THE MAIN FUNCTION THAT
//RUNS IN A LOOP TOGETHER WITH ALL THE INTERRUPTIONS THAT TRIGGER A REACTION IN REAL TIME

//V1.0 @ MAR 2019 --> IOT-GPS Project Started

//SYSTEM DEPENDENCIES
#include <Arduino.h>

//LOCAL DEPENDENCIES
//#include <interruptions.h>
#include <imu.h>
#include <rfid.h>
#include <touch_sensor.h>
#include <vibration_sensor.h>
#include <http.h>
#include <gps_sensor.h>
#include <display.h>
#include <serial.h>

bool developer_mode = true ;
//if (developer_mode)

int general_status = 0;

/*
Possible Status:
-----  BIKING MODE (LOCKED AND RUNNUNG) -------------------------------------------------------------------------------------------------
 0 -> Initial and Normal Locked Status in Riding Mode , sending Every X mins GPS and checking for IMU and Vibrations
 1 ->
 2 ->
 3 ->

 -----  PARKING MODE (LOCKED AND STOPPED) -------------------------------------------------------------------------------------------------
 4 ->
 5 ->
 6 ->
 7 ->

 -----  DRINKING MODE (UNLOCKED AND RUNNING) -------------------------------------------------------------------------------------------------


 -----  HOUSE MODE (UNLOCKED AND STOPED (MAYBE CHARGING)) -------------------------------------------------------------------------------------------------


*/


//CONFIGURATION FUNCTION
void setup()
{
  setup_serial(); //SERIAL COMMUNICATION SPEED /////////////////////////////

  if (developer_mode)
  {
    serial_welcome();
    setup_display();
  }

  //Apart from Initializing it will stop everything else
  //until you unlock for the First time
  setup_rfid();

  setup_imu();

  lock(); //We lock it now with the Given Key

  //TEMPORAL DEFAULTS UNTIL WE MIGRATE TO THE FINAL LOGIC
  general_status = 0;

  //Optionals

  //setup_internal_timer2();          //Internal Interruption
  //setup_button_interruption();      //External Interruption

  //////////////////////////////////////////////////////////////////////////////
}

// MAIN LOOP //////////////////////////////////////////////////////////////////////
void loop()
{
  //LOCKING FROM LONG_Press (Remove it Later!!!)
  if (locked) //Engaged to the Holder
  {
    //SPECIFIC FOR THE LOCKED MODE
    check_RFID(general_status);
    //COMMON BUT CHANGED BY STATUS
    check_vibration_sensor();
    check_touch_sensor(general_status);
    //check_gps_time();
  }

  else if (!locked) // Not Being Used
  {
    //SPECIFIC FOR THE UNLOCKED MODE

    //COMMON BUT CHANGED BY STATUS
    check_vibration_sensor();
    check_touch_sensor(general_status);

  }

  run_imu();

}
