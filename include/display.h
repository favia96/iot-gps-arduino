#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <U8x8lib.h>

//D20 (SDA) NANO
//D21 (SCL) NANO

//16 Spaces per Line

//display_write( X , Y ," text ");



U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

void display_write( int x , int y , char text[] )
{
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(x , y , text );
}

display_clear()
{
  display_write( 0 , 0 , "                " );

  display_write( 0 , 1 , "                " );

  display_write( 0 , 2 , "                " );

  display_write( 0 , 3 , "                " );

  display_write( 0 , 4 , "                " );

  display_write( 0 , 5 , "                " );

  display_write( 0 , 6 , "                " );

  display_write( 0 , 7 , "                " );

}


void display_developer(void)
{

  display_write( 0 , 0 , " =.IOT-GPS.= v1.0 " );

  display_write( 0 , 1 , "STATUS:         " );

  display_write( 0 , 2 , "BTN:     BT:    " );

  display_write( 0 , 3 , "RFI:     FP:    " );

  display_write( 0 , 4 , "PZO:            " );

  display_write( 0 , 5 , "P.    R    Y.   " );

  display_write( 0 , 6 , "GPS:            " );

  display_write( 0 , 7 , "SIG:            " );

}

void display_developer_without_status(void)
{
  display_write( 0 , 2 , "BTN:     BT:    " );

  display_write( 0 , 3 , "RFI:     FP:    " );

  display_write( 0 , 4 , "PZO:            " );

  display_write( 0 , 5 , "P.    R    Y.   " );

  display_write( 0 , 6 , "GPS:            " );

  display_write( 0 , 7 , "SIG:            " );
}



void setup_display(void)
{

  u8x8.begin();

  u8x8.setPowerSave(0);

  display_developer();

  //display_write( 0 , 1 , "STATUS: LOCKED  " );

}

void display_lock(void)
{

  display_developer_without_status();
  display_write( 0 , 1 , "STATUS: LOCKED  " );

}

void display_unlock(void)
{
  display_developer_without_status();
  display_write( 0 , 1 , "STATUS:UNLOCKED " );

}

void display_locked_attempt(int remaining_attempts )
{


  char number[1] ;
  itoa( remaining_attempts , number , 10);

  display_write( 0 , 0 , " =.IOT-GPS.= v1.0 " );

  display_write( 0 , 1 , "STATUS : LOCKED " );

  display_write( 0 , 2 , "                " );

  display_write( 0 , 3 , "  WRONG KEY!    " );

  display_write( 0 , 4 , "                " );

  display_write( 0 , 5 , "ATTEMPTS LEFT:  "); display_write( 14 , 5 ,  number );

  display_write( 0 , 6 , "                " );


  display_write( 0 , 7 ,  "               ");

}

void display_unlocked_attempt(void)
{

  display_write( 0 , 0 , " =.IOT-GPS.= v1.0 " );

  display_write( 0 , 1 , "STATUS:UNLOCKED " );

  display_write( 0 , 2 , "                " );

  display_write( 0 , 3 , "  WRONG KEY!    " );

  display_write( 0 , 4 , "                " );

  display_write( 0 , 5 , "                ");

  display_write( 0 , 6 , "                " );

  display_write( 0 , 7 ,  "               ");

}


void display_blocked()
{
  display_write( 0 , 0 , " =.IOT-GPS.= v1.0" );

  display_write( 0 , 1 , "STATUS :BLOCKED" );

  display_write( 0 , 2 , " ATTENTION !!! " );

  display_write( 0 , 3 , "DEVICE BLOCKED " );

  display_write( 0 , 4 , "               " );

  display_write( 0 , 5 , " CONNECT WITH  " );

  display_write( 0 , 6 , " BLUETOOTH OR  " );

  display_write( 0 , 7 , " FINGERPRINT   " );
}

void display_template(void)
{

  display_write( 0 , 0 , "" );

  display_write( 0 , 1 , "" );

  display_write( 0 , 2 , "" );

  display_write( 0 , 3 , "" );

  display_write( 0 , 4 , "" );

  display_write( 0 , 5 , "" );

  display_write( 0 , 6 , "" );

  display_write( 0 , 7 , "" );

}

//BUTTON

void display_button_status(int button_status)
{
  switch(button_status)
  {
    case 0 :
    {
      display_write(4,2,"0");
      break;
    }

    case 1 :
    {
      display_write(4,2,"1");
      break;
    }
    case 2 :
    {
      display_write(4,2,"2");
      break;
    }
    case 3 :
    {
      display_write(4,2,"3");
      break;
    }
    case 4 :
    {
      display_write(4,2,"4");
      break;
    }
  }

}

void display_refresh(bool locked)
{
  delay(1000);
  display_developer();
  if(locked)display_lock();
  else      display_unlock();
}

void display_ClickEvent(bool locked)
{
  display_clear();
  display_write( 0 , 4 , "  CLICK EVENT   ");
  display_refresh(locked);

}

void display_ShortHoldEvent(bool locked)
{
  display_clear();
  display_write( 0 , 4 , "SHORT HOLD EVENT");
  display_refresh(locked);
}

void display_LongHoldEvent(bool locked)
{
  display_clear();
  display_write( 0 , 4 , "LONG HOLD EVENT ");
  display_refresh(locked);
}

void display_StuckEvent(bool locked)
{
  display_clear();
  display_write( 0 , 4 , "  STUCK EVENT   ");
  display_refresh(locked);
}

void display_vibrating(int alarm_status,bool locked)
{
  /*
  Cases:
  0 No Vibration (OLED to 0)
  1 Vibration    (OLED to 1)
  2 Parking Alert (Less than 5 Seconds of Movement)
  3 Theft Alert  (More than 20 Seconds of Continuous Movement )
  4 Re-Enable Status before Restarting(Self Restarting Mode)
  */

  switch (alarm_status)
  {
    case 0 :
    {
      display_write( 4 , 4 , "0" );
      break;
    }

    case 1 :
    {
      display_write( 4 , 4 , "1" );
      break;
    }

    case 2 :
    {
      display_clear();

      display_write( 0 , 1 , "  ATTENTION     " );

      display_write( 0 , 2 , "   SOMEONE      " );

      display_write( 0 , 3 , " IS DEFINITELY  " );

      display_write( 0 , 4 , "     MOVING     " );

      display_write( 0 , 5 , "     YOUR       ");

      display_write( 0 , 6 , "   PRECIOUS     ");

      display_write( 0 , 7 , "    BICYCLE     ");

      break;
    }

    case 3 :
    {
      display_clear();

      display_write( 0 , 1 , " CONGRATULATIONS" );

      display_write( 0 , 2 , "   SOMEONE      " );

      display_write( 0 , 3 , "     JUST       " );

      display_write( 0 , 4 , "     STOLE      " );

      display_write( 0 , 5 , "     YOUR       ");

      display_write( 0 , 6 , "   PRECIOUS     ");

      display_write( 0 , 7 , "    BICYCLE     ");

      display_refresh(locked);

      break;
    }

    case 4 :
    {
      display_clear();

      display_write( 0 , 3 , "   RESTARTING   " );
      display_write( 0 , 4 , "   THE ALARM    " );
      display_write( 0 , 5 , " Wait 5 Seconds " );

      break;

    }

    case 5 :
    {
      display_clear();

      display_write( 0 , 3 , "     ALARM      " );
      display_write( 0 , 4 , "   RE-ENABLED   " );

      display_refresh(locked);
      break;
    }
  }

}

// FINISH ALL WHAT IS HAPPENNING HERE WITH THE Vibration




//SAVING THE MASTER KEY

void display_master_key_initialization()
{
  display_write( 0 , 0 , " =.IOT-GPS.= v1.0 " );

  display_write( 0 , 1 , "    WELCOME     ");

  display_write( 0 , 2 , "                ");

  display_write( 0 , 3 , "     PLEASE     " );

  display_write( 0 , 4 , "    PRESENT     " );

  display_write( 0 , 5 , "      YOUR      " );

  display_write( 0 , 6 , "     MASTER     " );

  display_write( 0 , 7 , "      KEY       ");

}

void display_key_detected()
{

  display_clear();

  display_write( 0 , 2 , "      KEY       " );

  display_write( 0 , 3 , "    DETECTED    " );

  display_write( 0 , 7 , ".....SAVING....." );

}

void display_key_saved()
{

  display_clear();

  display_write( 0 , 2 , "      KEY       " );

  display_write( 0 , 3 , "     SAVED      " );

  display_write( 0 , 7 , "                " );

  delay(2000);
}


// ------- IMU -----------------------------------------------------


void display_imu_start_calib()
{
  display_write( 0 , 0 , " =.IOT-GPS.= v1.0 " );

  display_write( 0 , 1 , "                 " );

  display_write( 0 , 2 , "     PRESS       " );

  display_write( 0 , 3 , "   THE BUTTON    " );

  display_write( 0 , 4 , "   TO START      " );

  display_write( 0 , 5 , "    THE IMU      ");

  display_write( 0 , 6 , "   CALIBRATION   ");

  display_write( 0 , 7 , "                ");
}

void display_imu_calibrating_gyro(int gyro_status)
{
  display_write( 0 , 0 , " =.IOT-GPS.= v1.0 " );

  display_write( 0 , 1 , "                " );

  display_write( 0 , 2 , "  CALIBRATING   " );

  display_write( 0 , 3 , "      GYRO      " );

  display_write( 0 , 4 , "                " );

  char gyro[1] ;
  itoa( gyro_status , gyro , 10);

  display_write( 0 , 5 , "               ");
  display_write( 7 , 5 , gyro );

  display_write( 0 , 6 , "       of      ");

  display_write( 0 , 7 , "       16      ");

}

void display_imu_pitch_roll(int pitch , int roll)
{
  //display_write( 0 , 5 , "P.    R    Y.   " );
  
  display_write( 2 , 5 , pitch );
  display_write( 7 , 5 , roll );

}


void display_imu_Calib( int x , int y , int z)
{


}

#endif
