#include <stdint.h>
#include <stdio.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"



// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 9/20/2016 
// Student names: Austin Gunter, Michael Glasser

uint32_t thou; 
uint32_t hund; 
uint32_t tens; 
uint32_t ones;


//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){
 uint32_t data;
	ADC0_PSSI_R = 0x0008;
	while((ADC0_RIS_R&0x08)==0){};
	data = ADC0_SSFIFO3_R&0xFFF;
	ADC0_ISC_R = 0x0008;
	return data;
}

uint32_t Convert(uint32_t input){
	if(input<=3287){
  return (input*57051+291971966)/1000000;
	}else{
		return (input*2483523-810478687)/100000;
	}
}

void ADC_Print(char Measurement[]){ uint32_t ADC_sample; uint32_t distance; uint32_t mods;
	uint32_t index = 0;
	ADC_sample = ADC_In();
	distance = Convert(ADC_sample);
	
	for(int i = 0; i < 9; i++){
		Measurement[i] = 0;
	}
	
	thou = (distance/1000) + 0x30;
		Measurement[index] = thou;
		index++;
		Measurement[index] = 0x2E;
		index++;
	mods = distance%1000;
	hund = (mods/100) + 0x30;
		Measurement[index] = hund;
		index++;
	mods %= 100;
	tens = (mods/10) + 0x30;
		Measurement[index] = tens;
		index++;		
	mods %= 10;
	ones = mods + 0x30;
		Measurement[index] = ones;
		index++;
		Measurement[index] = 0x20;
		index++;
		Measurement[index] = 0x63;
		index++;
		Measurement[index] = 0x6D;
//		index++;
//		Measurement[index] = 0x00;
		ST7735_SetCursor(0,4);
		ST7735_OutString(Measurement);
	
	
	
/*	
	ST7735_SetCursor(0,4);
	ST7735_OutChar(thou);
	ST7735_OutChar(0x2E);
	ST7735_OutChar(hund);
	ST7735_OutChar(tens);
	ST7735_OutChar(ones);
	ST7735_OutChar(0x20);
	ST7735_OutChar(0x63);
	ST7735_OutChar(0x6D);
*/
}

////////////////////////////////////////////////////////////////////// Other ADC Setup with SW trigger
// This initialization function sets up the ADC according to the
// following parameters.  Any parameters not explicitly listed
// below are not modified:
// Max sample rate: <=125,000 samples/second
// Sequencer 0 priority: 1st (highest)
// Sequencer 1 priority: 2nd
// Sequencer 2 priority: 3rd
// Sequencer 3 priority: 4th (lowest)
// SS3 triggering event: software trigger
// SS3 1st sample source: Ain9 (PE4)
// SS3 interrupts: enabled but not promoted to controller


void ADC0_InitSWTriggerSeq3_Ch9(void){ 
  SYSCTL_RCGCADC_R |= 0x0001;   // 7) activate ADC0 
                                  // 1) activate clock for Port E
  SYSCTL_RCGCGPIO_R |= 0x10;
  while((SYSCTL_PRGPIO_R&0x10) != 0x10){};
  GPIO_PORTE_DIR_R &= ~0x10;      // 2) make PE4 input
  GPIO_PORTE_AFSEL_R |= 0x10;     // 3) enable alternate function on PE4
  GPIO_PORTE_DEN_R &= ~0x10;      // 4) disable digital I/O on PE4
  GPIO_PORTE_AMSEL_R |= 0x10;     // 5) enable analog functionality on PE4
    
//  while((SYSCTL_PRADC_R&0x0001) != 0x0001){};    // good code, but not yet implemented in simulator


  ADC0_PC_R &= ~0xF;              // 7) clear max sample rate field
  ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F;       // 11) clear SS3 field
  ADC0_SSMUX3_R += 9;             //    set channel
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;           // 13) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;         // 14) enable sample sequencer 3
}


//------------ADC0_InSeq3------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC0_InSeq3(void){  uint32_t result;
  ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
    // if you have an A0-A3 revision number, you need to add an 8 usec wait here
  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
  return result;
}







