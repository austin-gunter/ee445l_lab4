// ADC
// Buffer


#include <stdint.h>
#include "ST7735.h"
#include <string.h>


void Parse_Buffer(char Buffer[]){
	
char* temperature = strstr(Buffer, "temp");		// look for "temp" in the buffer, return a pointer to the start of it
	
if(temperature){
	temperature += 6;				// Increment index by 6 so we start looking at the actual temp numbers
	ST7735_SetCursor(0, 0);
	ST7735_OutString("Temp = ");
	for(int i = 0; i < 5; i++){
		ST7735_OutChar(temperature[i]);					// Print out temp
	}
	ST7735_OutString(" C");	
}





}
