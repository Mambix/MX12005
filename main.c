#include <htc.h>
#include <eeprom_routines.h>
#include "display.h"
#include "timing.h"

#define _XTAL_FREQ      32000000
#define ShowModeDelay	1000

#define refVOLTAGE		4

// Define Keys
#define KEY1		PORTAbits.RA0
#define trisKEY1	TRISAbits.TRISA0
#define pullKEY1	WPUA0
#define KEY2		PORTAbits.RA1
#define trisKEY2	TRISAbits.TRISA1
#define pullKEY2	WPUA1

__CONFIG(FOSC_INTOSC & WDTE_OFF & PWRTE_OFF & MCLRE_ON & CP_OFF & CPD_OFF & BOREN_ON & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
__CONFIG(WRT_OFF & PLLEN_ON & STVREN_OFF & /*BORV_25 &*/ LVP_OFF);

eeprom unsigned char ee_datavalues[1] = {0x12};

extern char refreshLED;
extern char refreshMesurement;
extern unsigned int muxLED;
extern unsigned char blink;
extern DWORD_VAL mesurement;
extern unsigned int divMesure;
extern unsigned char ShowMode;

extern DWORD_VAL avgMesure[8];
extern char avgPos;
extern unsigned long atmPressure;

unsigned char muxDIV;

main()
{
	ADCON0 = 0b00001000; // Channel AN2
	ADCON1 = 0b10110011; // Right jutified; fRC clock
#if refVOLTAGE == 4
	FVRCON = 0b10000011; // Fixed voltage set to 4.096
#elif refVOLTAGE == 2
	FVRCON = 0b10000010; // Fixed voltage set to 2.048
#elif refVOLTAGE == 1
	FVRCON = 0b10000001; // Fixed voltage set to 1.024
#else
	FVRCON = 0b00000000; // Fixed voltage OFF; set to supply voltage
#endif
	ADON = 1;
	ANSELA = ANSELB = ANSELC = 0;

	// Internal 32MHz XTAL
	OSCCON = 0b11110000;

	//WDT setup
	WDTCON = 0;

	TRISA = ANSELA = 0b00000100; //RA2 analog input
	PORTA = 0;

	trisKEY1 = 1;
	KEY1 = 0;
	trisKEY2 = 1;
	KEY2 = 0;
	pullKEY1 = 1;
	pullKEY2 = 1;	

	muxLED = 0;
	refreshLED = 0;

	ShowMode = eeprom_read(0x00);

	initDisplay();
	divMesure = 0;
	ShowModeCountdown = 0;

	for (avgPos = 0; avgPos<8; avgPos++)
		avgMesure[avgPos].Val = 0;
	avgPos = 0;

	// capacitive sensing
	CPSON = 0;
	T0XCS = 0;

	// initialize timer 0;
	TMR0=0;
	OPTION_REG = 0b00111000;	// no prescale	
	TMR0IE = 1;
	//TMR0CS = 0;
	
	// intialize timer 1
	T1CON = 0b01000000; // enable timer interrupt  F/1
	
	// 16 bit
	TMR1 = 0;
	TMR1ON=1;
   	TMR1IF=0;
   	TMR1IE=1;
	PEIE=1;

/* PIN INTERUPTS */
	INTF = 0;
	INTE = 0;
	IOCIF = 0;
	IOCIE = 0;
	IOCAP = 0b00000000;
	IOCAN = 0b00000000;
/**/
	__delay_ms(500);

	GIE = 1;			// enable global interrupts

	while(1)
	{
		CLRWDT();
		
		if ( refreshLED == 1 )
		{
			refreshLED = 0;

			if (refreshMesurement == 1)
			{
				refreshMesurement = 0xffffffff;

				GIE = 0;
				
				// Start ADC conversion
				ADON = 1;
				ADGO = 1;
				while ((ADGO==1) && (refreshMesurement>4) ) 
				{
					refreshMesurement--;
				}
				ADGO = 0;
				ADON = 0;
				
				// Conversion done
				// Get results
				mesurement.Val = 0;
				mesurement.byte.HB = ADRESH;
				mesurement.byte.LB = ADRESL;
				
				GIE=1;
				refreshMesurement = 0;
			}

			refreshSegment();

			if (blink==1)
				muxLED = (++muxLED % 0x80);
			else
				muxLED = (++muxLED % 4);
		}
/**/
		if (KEY1 == 0)
		{
			__delay_ms(50);
			if (KEY1 == 0)
			{
				while (KEY1 == 0) {}
				ShowMode++;
				ShowMode &= 0x77;
				ShowModeCountdown = ShowModeDelay;
				eeprom_write(0x00, ShowMode);
			}
		}
		
		if (KEY2 == 0)
		{
			__delay_ms(50);
			if (KEY2 == 0)
			{
				while (KEY2 == 0) {}
				ShowMode += 0x10;
				ShowMode &= 0x77;
				ShowModeCountdown = ShowModeDelay;
				eeprom_write(0x00, ShowMode);
			}
		}
/**/
		
	}
}