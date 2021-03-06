#include "adc.h"
#include "Lcd.h"
#include "string.h"
#include "dcmotor.h"
#include <LPC214x.H>                       /* LPC214x definitions */
#define buzzer (1<<21)

/**
*******************************************************************************
  Function Name :init_adc0()

  Description :	Initialises the ADC0  

  Input :	None

  Output :	None

  Note : 
*******************************************************************************
*/	
void init_adc0(void)
{
	PINSEL1 = (PINSEL1 & ~(3 << 18)) | (1 << 18);//AD0.4	PIN 25
	PINSEL1 = (PINSEL1 & ~(3 << 28)) | (1 << 28);//AD0.3	PIN 28
	PINSEL1 = (PINSEL1 & ~(3 << 26)) | (1 << 26); //AD0.2.		  29
	PINSEL1 = (PINSEL1 & ~(3 << 24)) | (1 << 24);//AD0.1			30
	PINSEL0 |= (3<< 8);	//AD0.6     P0.4
}

/**
*******************************************************************************
  Function Name :init_adc1()

  Description :	Initialises the ADC1  

  Input :	None

  Output :	None

  Note : 
*******************************************************************************
*/	
void init_adc1(void)
{

}


/**
*******************************************************************************
	Function Name : adc_read()

	Description   :

	Input         : adc number,channel

	Output        : 10 bit AD value

	Note          :
*******************************************************************************
*/

unsigned short adc_read(unsigned char adc_num, unsigned char ch)
{
  unsigned int i=0;
  
  switch(adc_num)
  {
    case ADC0:
      AD0CR = 0x00200D00 | (1<<ch);    // select channel
      AD0CR |= 0x01000000; // Start A/D Conversion
    
      do
      {
        i = AD0GDR; // Read A/D Data Register
      } while ((i & 0x80000000) == 0); // Wait for end of A/D Conversion
      break;
    
    case ADC1:
      AD1CR = 0x00200D00 | (1<<ch); // select channel
      AD1CR |= 0x09000000; // Start A/D Conversion
      do
      {
        i = AD1GDR; // Read A/D Data Register
      } while ((i & 0x90000000) == 0); // Wait for end of A/D Conversion
      break;
  }
 
  return (i >> 6) & 0x03FF; // bit 6:15 is 10 bit AD value
 
}









