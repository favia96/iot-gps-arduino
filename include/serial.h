#ifndef SERIAL_H
#define SERIAL_H

//THIS IS THE HEADER TO MANAGE EVERYTHING RELATED WITH THE SERIAL COMMUNICATION PC <---> ARDUINO

#include <Arduino.h>

void setup_serial()
{
  Serial.begin(38400);
}

void serial_welcome()
{
  Serial.println ( " \n\n - Welcome to the =.IOT GPS.= Project  --- Developer Mode Active - " );
}

void serial_lock()
{
  Serial.println("\n============================");
  Serial.println(  "       --- LOCKING ---      ");
  Serial.println(  "============================\n");
}

void serial_unlock()
{
  Serial.println("\n============================");
  Serial.println(  "      --- UNLOCKING ---     ");
  Serial.println(  "============================\n");
}


void serial_locked_attempt(int remaining_attempts)
{
  Serial.println("\n==============================================");
  Serial.print  (  "   --- Unknown Key ---  REMAINING ATTEMPTS :  ");
  Serial.println( remaining_attempts );
  Serial.println(  "==============================================\n");
}

void serial_unlocked_attempt()
{
  Serial.println("\n===========================");
  Serial.println  ("    --- Unknown Key ---  ");
  Serial.println(  "===========================\n");
}

void serial_blocked()
{
  Serial.println("\n===========================");
  Serial.println  ("    --- BLOCKED ---  ");
  Serial.println("=============================\n");
}


void serial_button_status(int button_status)
{
  switch(button_status)
  {
    case 1:
    {
      Serial.print("\n\n|<- BUTTON TOUCHED! ->|");
      break;
    }
    case 2:
    {
      Serial.print ("|<- SHORT HOLD! ->|");
      break;
    }
    case 3:
    {
      Serial.print ("|<- LONG HOLD! ->|");
      break;
    }
    case 4 :
    {
      Serial.print ("|<- STUCKED! Release to Continue ! ->|");
      break;
    }

    case 10 :
    {
      Serial.print (" * ");
      break;
    }

    default:
    {
      Serial.print ("|<- RELEASED AFTER ");
      Serial.print( button_status / 1000 );
      Serial.print(" Seconds ->|");
      break;
    }

  }

}

void serial_ClickEvent()
{
  Serial.println("\n\n============================");
  Serial.println    ("     --- Click Event ---    ");
  Serial.println(    "============================\n");
}

void serial_ShortHoldEvent()
{
  Serial.println("\n\n============================");
  Serial.println  (  "  --- Short Hold Event ---  ");
  Serial.println(    "============================\n");
}


void serial_LongHoldEvent()
{
  Serial.println("\n\n===============================");
  Serial.println(    "    --- Long Hold Event ---  ");
  Serial.println(    "===============================\n");
}

void serial_StuckEvent()
{
  Serial.println("\n\n==============================");
  Serial.println  (  "  --- Stucked , Ignoring ---  ");
  Serial.println(    "==============================");
}

//---- PIEZO SENSOR ----------------------------

void serial_vibrating()
{
  Serial.println("\n============================");
  Serial.println(  "     --- VIBRATING ---      ");
  Serial.println(  "============================");
}


//SAVING THE MASTER KEY

void serial_master_key_initialization()
{
  Serial.println("\n============================");
  Serial.println(  "  Please Unlock the System  ");
  Serial.println(  "   with your Desired Key    ");
  Serial.println(  "============================");

}

void serial_key_detected()
{

  Serial.println("\n=============================");
  Serial.println(  " --- MASTER KEY DETECTED --- ");
  Serial.print  (  "=============================   ");

  for (int j = 0; j < 7; j++)
  {
    Serial.print("  *  ");
    delay(200);
  }
  Serial.println(" SAVED ! ");
}

//ALARM


void serial_vibrating(int alarm_status , unsigned long time)
{
  /*
  Cases:
    0 No Vibration (OLED to 0)
    1 Vibration    (OLED to 1)
    2 Vibration After Locking (5 seconds Waiting)
    3 Locking Alarm Triggered (more than 5 second of continuous vibration )
    4 Someone is moving the object (more than 1 minute of continuos vibration (Stealing Alarm)

  */

  switch (alarm_status)
  {
    case 0 :
    {
      Serial.println("\n=====================================");
      Serial.println(  "  ---   NOT VIBRATING ANYMORE   --- ");
      Serial.print(    "   VIBRATION TIME :  "); Serial.print(time/1000);Serial.println(" Seconds");
      Serial.println(  "=====================================");
      break;
    }

    case 1 :
    {
      Serial.println("\n=============================");
      Serial.println("       --- VIBRATING ---       ");
      Serial.print(    "=============================      ");
      break;
    }

    case 2 :
    {
      Serial.println("\n\n====================================================");
      Serial.println(    "  --- Hey,Someone Is Definitely Moving your Object ---");
      Serial.println(    "  ---                 PARKING ALARM              ---");
      Serial.println(    "====================================================");
      break;
    }

    case 3 :
    {
      Serial.println("\n=========================================");
      Serial.println(  "  --- SOMEONE IS GETTING A NEW Object ---  ");
      Serial.println(  "  ---          THEFT ALARM          ---  ");
      Serial.println(  "=========================================");
      break;
    }

    case 4 :
    {
      Serial.println("\n===================================================");
      Serial.println(  ". . . RE-Enabling the Alarm , wait 10 Seconds . . .");
      Serial.println(  "===================================================");
      break;
    }

    case 5:

    {
      Serial.println("\n=========================================");
      Serial.println(  "        --- ALARM RE-ENABLED ---         ");
      Serial.println(  "=========================================");
      break;
    }
  }

}




// ------------------- -IMU- -----------------------

void serial_imu_start_calib()
{
  Serial.println("\n====================================");
  Serial.println(  "---       IMU CALIBRATION        --- ");
  Serial.println(  "--- PRESS THE BUTTON TO CONTINUE --- ");
  Serial.println(  "====================================");
}

void serial_imu_calibrating_gyro(int gyro_status)
{
  if (gyro_status == 1 )
  {
    Serial.println("\n=========================================");
    Serial.println(  "         --- CALIBRATING GYRO ---        ");
    Serial.println(  "A Total of 16 Measurements will be taken ");
    Serial.println(  "=========================================\n\n");
  }

  Serial.print(gyro_status);
  Serial.print("  ");

  if ( (gyro_status > 1) && (gyro_status % 4 == 0) ) Serial.print("\n\n");

  else if (gyro_status < 16 ) Serial.print( " ->  " );

  if(gyro_status == 16)
  {
    Serial.println("\n======================================");
    Serial.println(  "     --- CALIBRATION  FINISHED ---        ");
    Serial.println(  "======================================\n\n");
    delay(2000);
  }

}


#endif
