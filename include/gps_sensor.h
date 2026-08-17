#ifndef GPS_SENSOR_H
#define GPS_SENSOR_H

#include <Arduino.h>
#include <http.h>

// Product name: GPS/GPRS/GSM Module V3.0
// # Product SKU : TEL0051
// # Version     : 1.2


// # Description:
// # The sketch for driving the gps mode via the Arduino board

// # Steps:
// #        1. Turn the S1 switch to the Prog(right side)
// #        2. Turn the S2 switch to the Arduino side(left side)
// #        3. Set the UART select switch to middle one.
// #        4. Upload the sketch to the Arduino board
// #        5. Turn the S1 switch to the comm(left side)
// #        6. RST the board

// #        If you get 'inf' values, go outdoors and wait until it is connected.
// #        wiki link- http://www.dfrobot.com/wiki/index.php/GPS/GPRS/GSM_Module_V3.0_(SKU:TEL0051)


/*
 *  GPRS+GPS Quadband Module (SIM908)
 *
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L.
 *  http://www.libelium.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  a
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 *  Version:           2.0
 *  Design:            David Gascón
 *  Implementation:    Alejandro Gallego & Marcos Martinez
 */

bool developer_mode_gps = false ;
//if (developer_mode_interruptions ==true)

//variables
unsigned long previousMillis = 0;
const long interval = 120; // Every X Seconds

//int8_t answer;
int counter;
long previous;
float lat, lon;
char RMC_str[100];

double Datatransfer(char *data_buf,char num)//convert the data to the float type
{                                           //*data_buf：the data array
  double temp=0.0;                           //the number of the right of a decimal point
  unsigned char i,j;
  if(data_buf[0]=='-')
  {
    i=1;
    //process the data array
    while(data_buf[i]!='.')
      temp=temp*10+(data_buf[i++]-0x30);
    for(j=0;j<num;j++)
      temp=temp*10+(data_buf[++i]-0x30);
    //convert the int type to the float type
    for(j=0;j<num;j++)
      temp=temp/10;
    //convert to the negative numbe
    temp=0-temp;
  }
  else//for the positive number
  {
    i=0;
    while(data_buf[i]!='.')
      temp=temp*10+(data_buf[i++]-0x30);
    for(j=0;j<num;j++)
      temp=temp*10+(data_buf[++i]-0x30);
    for(j=0;j<num;j++)
      temp=temp/10 ;
  }
  return temp;
}

double decimalgps(double rawdata) //convert raw coord data to decimal google maps friendly
{
  int degrees = (int)(rawdata / 100);
  double minutes = rawdata - (degrees*100);
  double mindecimal = minutes / 60.0;
  double total = degrees + mindecimal;
  return total;
}

void comma(char num)//read buffer until get a certain num of ','
{
  char val;
  char count=0;//count the number of ','

  while(1)
  {
    if(Serial.available())
    {
      val = Serial.read();
      if(val==',')
        count++; //increases the "," counted
    }
    if(count==num)//if the command is right, run return
      return;
  }
}

float latitude()//get float decimal latitude
{
  char i;
  char lat[10]={
    '0','0','0','0','0','0','0','0','0','0'
  };

  comma(3); //read until 3 commas
  while(1)
  {
    if(Serial.available())
    {
        lat[i] = Serial.read();
        i++;
    }
    if(i==10)
    {
      i=0;
      double newlat =  Datatransfer(lat,6);
      float corrected = decimalgps(newlat); //convert to decimal
      return corrected;
    }
  }

}

float longitude()//get float decimal longitude
{
  char i;
  char lon[10]={
    '0','0','0','0','0','0','0','0','0','0'
  };

  comma(2); //read after latitude until 2 commas
  while(1)
  {
    if(Serial.available())
    {
      lon[i] = Serial.read();
      i++;
    }
    if(i==11)
    {
      i=0;
      double newlon = Datatransfer(lon,5);
      float corrected = decimalgps(newlon); //convert to decimal
      return corrected;
    }
  }
}

void setup_gps_sensor() //initialize and power up gps sensor, waiting for 2d status at least
{
    pinMode(3,OUTPUT);//The default digital driver pins for the GSM and GPS mode
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
    Serial.begin(9600);
    delay(2000);

    //GSM output timing
    digitalWrite(5,HIGH);
    delay(1500);
    digitalWrite(5,LOW);

    delay(2000);

    digitalWrite(3,LOW);//Enable GSM mode
    delay(3000);
    digitalWrite(4,HIGH);//Disable GPS mode

    Serial.println("Starting...");

    uint8_t answer=0;

    // checks if the module is started
    do
    {
        answer = sendATcommand("AT", "OK", 2000);
    } while(answer == 0);
    //GPS ready
    delay(5000);

    sendATcommand("AT+CGPSIPR=9600","OK", 2000); //resent baudrate to 9600s
    sendATcommand("AT+CGPSPWR=1", "OK", 2000); //turn on GPS power supply
    sendATcommand("AT+CGPSRST=0", "OK", 2000); //reset in cold mode

    // waits for fix GPS
    while( (sendATcommand("AT+CGPSSTATUS?", "2D Fix", 5000) ||
            sendATcommand("AT+CGPSSTATUS?", "3D Fix", 5000)) == 0 );
}

void off_gps_sensor() //turn off GPS power supply
{
    sendATcommand("AT+CGPSPWR=0","OK",2000);
}

void read_gps() //get gps coordinates in 2 float values (and print on serial monitor)
{
  while( Serial.available() > 0) Serial.read();
  delay(1000);

  // request RMC string
  sendATcommand("AT+CGPSINF=32", "AT+CGPSINF=32\r\n\r\n", 2000);

  // this get the coordinates
  lat = latitude(); //return the float latitude corrected in decimal for google maps
  lon = longitude(); //return the float longitude corrected in decimal for google maps

  Serial.println("*************************************************");
  Serial.println("coordinates:");
  Serial.print(lat,5); //  approximation to 5 decimal digits
  Serial.print(", ");
  Serial.println(lon,5);
  Serial.println("*************************************************");

}

void check_gps_time(int status) //still to develop correctly
{
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= (interval*1000)) //From Seconds to Miliseconds = (interval*1000)
  {
    if (developer_mode_gps == true)
    {
      setup_gps_sensor(); //initialize the gps sensor on
      read_gps(); //print on serial monitor the coordinates



      //code to sent to server..




      Serial.println( " GPS Default Signal Sent" );
      off_gps_sensor(); //turn off gps
    }
    previousMillis = currentMillis;
  }
}

#endif
