#include <stdio.h>
#include "6190.h"
#include "ascii.h"

int switch_pins[8] = {SW0, SW1, SW2, SW3, SW4, SW5, SW6, SW7}; 

void setup(){

    timerSetup();   // Configure timer
    setupDisplay(); // Configure display
    eraseBuffer();  // Erase random data from display
    drawBuffer();   // Display contents of buffer on LED array

    for (int i = 0; i < 7; i++){
        pinSetup(switch_pins[i], GPIO_INPUT);
    }
    pinSetup(BTNF, GPIO_INPUT);

}


char switchToBinary(){
    /*
    Return: 
        char: 8-bit value. Bits 0-6 correspond to switch inputs, bit 7 is 0.
    */

    /////////////////////////////////////////
    // TO-DO: Complete switchToBinary()    // 
    /////////////////////////////////////////
    
    char input = 0;
    for(int i = 0; i < 8; i++)
        {
            input |= pinRead(switch_pins[i]) << i;
        }
    input &= ~(1 << 7);
    return input;
}


void drawAsciiString(char* string_to_draw){   
    /* Function that draws a string on the LED array using the mapping defined in ascii.h 
    by filling in a buffer.
    
    Argument: 
        char* string_to_draw: String to draw on the LED array.
    */

    /////////////////////////////////////////
    // TO-DO: Complete drawAsciiString()   // 
    /////////////////////////////////////////
    eraseBuffer();
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 4; j++)
            {
               if (*(string_to_draw+j) == 0)
                   break;
               int display = ascii[(int)*(string_to_draw+j)][i];
               screen_buffer[i] += display << (24 - 8*j);  
            };
    }
    drawBuffer();


}


void app_main(){
    setup();
    char message[100];
    char ascii_input = 0;
    
    int btnf_prev = -1;
    int i = 0;
    while(1){
        ascii_input = switchToBinary();
        
        int btnf = pinRead(BTNF);        // read BTNF to obtain the current state of the button

        if (btnf != btnf_prev && btnf == 0 && i < 100)
        {
            message[i] = ascii_input;
           
            
            if (message[i] == 0)
            {
                printf("Message: %s\n", message);
                drawAsciiString(message);
                i = 0;
            }
            else{
                i+=1;
            }
           
        }
       
        // if (btnf != btnf_prev && btnf == 0){  
        //     printf("Integer value: 0x%x, ", ascii_input);   // printing this value using %x formatter gives a hexadecimal value...
        //     printf("ASCII character: %c\n", ascii_input);   // ... printing the same value with %c formatter gives a character                  
        //     /* TO-DO: Put your print statements from the last part in here */
        // }                           
        
        btnf_prev = btnf;
        // Code to slow down loop so that "bouncing" associated with a single button press doesn't 
        // look like more than one button press. Write all of your code in above this point in the loop.
        int start = millis();           // Get "start" time stamp
        while(millis() - start < 50);   // Wait until current time stamp - "start" is greater than 50ms
    }
}
