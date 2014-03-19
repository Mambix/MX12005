#include <htc.h>
#include "timing.h"

#define segRefreshRATE	0xC180

char i;
char refreshLED;
char refreshMesurement;
char blinkLedDiv;
char pwmLED;
DWORD_VAL mesurement, tmpMesure;
DWORD_VAL avgMesure[8];

char avgPos;
unsigned int divMesure;

void interrupt isr(void)
{
	if (TMR0IF == 1)						// Recieve Timer0 Interrupt Flag bit reset				
	{
		TMR0IF = 0;						// Iterate ClockCounter until reaches seconds		
		//tmpMer.Hib.HH++;
	} 

	if (TMR1IF == 1)
	{
		TMR1IF = 0;
		TMR1 = segRefreshRATE;

		/**/
		divMesure++;
		if (divMesure == 250)
		{
			divMesure = 0;
			refreshMesurement = 1;
			TMR0 = 0;
		}
		//meritev.Val = 10;
		/**/
		
		refreshLED = 1;
	}
}
