#ifndef HTTP_H
#define HTTP_H

#include <Arduino.h>

//variables
int8_t answer;

//function to sending ATcommand and waiting for expected answer (i.e. OK)
int8_t sendATcommand(char* ATcommand, char* expected_answer1,
        unsigned int timeout)
{

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( Serial.available() > 0) Serial.read();   // Clean the input buffer

    Serial.println(ATcommand);    // Send the AT command

    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{

        if(Serial.available() != 0){
            response[x] = Serial.read();
            x++;
            // check if the desired answer is in the response of the module
            if (strstr(response, expected_answer1) != NULL)
            {
                answer = 1;
            }
        }
        // Waits for the asnwer with time out
    }while((answer == 0) && ((millis() - previous) < timeout));

    return answer;
}

void http_init() // Initializes HTTP service
{
   answer = sendATcommand("AT+HTTPINIT", "OK", 10000);

}

void http_term() // Terminate HTTP service
{
  sendATcommand("AT+HTTPTERM", "OK", 5000);

}

void http_post()
{

}

#endif
