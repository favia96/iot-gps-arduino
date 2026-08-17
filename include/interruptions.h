#ifndef INTERRUPTIONS_H
#define INTERRUPTIONS_H
/*

#include <Arduino.h>

bool developer_mode_interruptions = true ;
//if (developer_mode_interruptions ==true)

///VARIABLES DECLARATION/////////////////////////

// FOR THE ALARM_INTERRUPTION() ///////////////////
const byte button = 3 ;
volatile byte state = LOW;
/////////////////////////////////////////////////

//FOR INTERNAL INTERRUPTIONS
boolean toggle2 = 0;

unsigned long interruption_nr = 0;


//---------CONFIGURING THE INTERRUPTIONS (SETUPS & INTERRUPTION KERNELS) -----------------------------------------------------------

//EXTERNAL INTERRUPTIONS

//FOR THE ALARM_INTERRUPTION() ////////////////////////////

void button_interruption()
{
  delay(200); //Not Recommended inside and ISR but needed in case of delaying in the release of the button

  state = !state;

  if (state == LOW)
  {
    if (developer_mode_interruptions ==true)Serial.print("ALARM Interruption --> OFF  \n");
  }

  else if (state == HIGH)
  {
    if (developer_mode_interruptions ==true) Serial.print("Interruption --> ON  \n");
  }

}

setup_button_interruption()
{
  pinMode(button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button), button_interruption , FALLING);
}


// THIS IS THE INTERNAL INTERRUIPTION OPTION , IS GOOD BECAUSE IT'S NOT DISTURBED BY ANYRHING < BUT IS DANGEROUS BECAUSE IT MAY INTERUPT THE SLEEPING MODE    ////////////////////////////////////////////////////////

//INTERNAL INTERRUPTIONS (Handled by the Arduino's Timers)

//timer setup for timer2.
//For arduinos with ATMEL 328

//timer2 will be interrupted at 8kHz

//storage variables

void setup_internal_timer2()
{
  cli();//stop interrupts (Internal Interruptions (Timer-based))

  //set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 255;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  sei();//allow interrupts
}

ISR(TIMER2_COMPA_vect)
{
  //timer2 interrupt 8kHz
  if (developer_mode_interruptions ==true && interruption_nr >= 500000) { Serial.println("\n Internal Interruption Nr : "); Serial.print(interruption_nr); interruption_nr=0;}
  interruption_nr++;

}
*/

#endif
