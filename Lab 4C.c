// ADC
// Buffer


#include <stdint.h>
#include "ST7735.h"
#include <string.h>


void Parse_Buffer(char Buffer[]){
	
char* temperature = strstr(Buffer, "temp");
	
if(temperature){
	temperature += 6;
	ST7735_SetCursor(0, 0);
	ST7735_OutString("Temp = ");
	for(int i = 0; i < 5; i++){
		ST7735_OutChar(temperature[i]);
	}
	ST7735_OutString(" C");	
}





}
