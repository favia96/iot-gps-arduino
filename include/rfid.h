
#ifndef RFID_H
#define RFID_H

#include<Arduino.h>
#include<SPI.h>
#include<MFRC522.h>
#include<display.h>
#include<serial.h>

#define SS_PIN 53
#define RST_PIN 49

/*
RC522 MODULE    MEGA(2560)
SDA             D53
SCK             D52
MOSI            D51
MISO            D50
IRQ             N/A
GND             GND
RST             D49
3.3V            3.3V
*/
//REMEMBER BEFORE CONNECTING THAT IS 3.3 V !!!!

bool developer_mode_rfid = true ;
bool key_debugger_mode = true ;
bool block_bypass_mode = true;
bool short_waiting_mode = true ;


bool locked = true ;//Try to Start with False for safety reasons

int max_attempts = 4;
int refresh = 1000; //Time to read the Token Again
int remaining_attempts = max_attempts ;

bool key_saved = false;
bool key_validated = false;
byte nuidPICC[4];

bool master_key = false;

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

//Card  UID : C0D4EB25    DEC -> (192 212 235 37)
//Token UID : 87D3B91C    DEC -> (135 211 185 28)

//This is the stored UID (Master Key (Card))

bool save_key()
{
  if(developer_mode_rfid)
  {
    display_key_detected();
    serial_key_detected();
    display_key_saved();
  }

  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++)
  {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }

  return true;
}

void setup_rfid()
{
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  MFRC522::MIFARE_Key key;

  if (developer_mode_rfid)
  {
    display_master_key_initialization();
    serial_master_key_initialization();
  }

  while (1) //Hang The Program until the Master Key is Detected
  {
    if(rfid.PICC_IsNewCardPresent())
    {
      if(!key_saved) //Save Key for the First Time
      {
        if (save_key())
        {
          key_saved = true;
        }
      }
    }
    if (key_saved) break;
  }
}

bool lock()
{
  locked = true ;

  if (developer_mode_rfid)
  {
     serial_lock();
    display_lock();
  }

return locked;
}

bool unlock()
{
  locked = false ;
  remaining_attempts = max_attempts ;

  if (developer_mode_rfid)
  {
     serial_unlock();
    display_unlock();
  }

  //Here unlocking Routine

  return locked;

}

void attempt() //WE WILL IGNORE RFID WHILE THE DEVICE IS UNLOCKED
{
    if (developer_mode_rfid)
    {
       serial_locked_attempt(remaining_attempts);
      display_locked_attempt(remaining_attempts);
    }
    remaining_attempts--;
}

void blocked()
{
  if (developer_mode_rfid)
  {
    serial_blocked();
    display_blocked();
  }

  //Here implement the unlocking just by fingerprint or bluetooth

  if (block_bypass_mode) //For Developing Purposes , change flag before release
  {
    Serial.println("Released ..(Bypass Mode is Enabled) ");
    if (short_waiting_mode)  delay( refresh * max_attempts / 2 );
    else                     delay( refresh * max_attempts * 2 );
    remaining_attempts = max_attempts - 1 ; // TO KNOW THAT I WAS PREVIOUSLY BLOCKED
  }
}

bool validate_key()
{
  if (rfid.uid.uidByte[0] == nuidPICC[0] ||
      rfid.uid.uidByte[1] == nuidPICC[1] ||
      rfid.uid.uidByte[2] == nuidPICC[2] ||
      rfid.uid.uidByte[3] == nuidPICC[3]   )
      {
        if (developer_mode_rfid && key_debugger_mode)
        {
          Serial.println();
          Serial.print(" Correct Key ");
        }
        return true;
      }
  else
      {
        if (developer_mode_rfid && key_debugger_mode)
        {
          Serial.println();
          Serial.print(" Incorrect  Key ");
        }
        return false;
      }


}

void check_RFID(int status)
{
  if(!rfid.PICC_IsNewCardPresent())return;

  else if(!rfid.PICC_ReadCardSerial())return;

  else //Card Detected
  {

    if(!key_saved) //Key was not saved before for the First Time
    {
      return;
    }
    else //Key Already Saved
    {
      if (validate_key()) key_validated = true;
      else                key_validated = false;

      if (key_validated)//Correct Key
      {
        if (locked) ////WE WILL IGNORE RFID WHILE THE DEVICE IS UNLOCKED
        {
          if (remaining_attempts >= 0)
          {
            unlock();
          }
        }
      }

      else //Invalid Key
      {
        if (remaining_attempts > 0)
        {
          attempt();
        }
        else
        {
          blocked();
        }
      }
    }
  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
  }

}


#endif
