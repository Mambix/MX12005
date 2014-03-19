/*

Functions for displaying results

ShowMode (x = don't care)
xxxxx000: ADC Value
xxxxx001: ADC Value x2
xxxxx010: ADC Value x4
xxxxx011: ADC value x5
xxxxx100: ADC Value (round to closest5)
xxxxx101: ADC Value x2 (round to closest5)
xxxxx110: ADC Value x4 (round to closest5)
xxxxx111: ADC value x5 (round to closest5)
x000xxxx: no DP
x001xxxx: DP at seg1000
x010xxxx: DP at seg100
x011xxxx: DP at seg10
x100xxxx: no DP2
x101xxxx: DP2 at seg1000
x110xxxx: DP2 at seg100
x111xxxx: DP2 at seg10
*/

#include <htc.h>
#include "display.h"
#include "timing.h"

unsigned char seg1000, seg100, seg10, seg1;
unsigned int muxLED;
unsigned char blink;
unsigned char ShowMode;
unsigned long ShowModeCountdown;

#define dispMINUS	16
#define dispOFF		17

/**/
unsigned char ledCif[] = { 
	0x00111111, //0
	0b00000110, //1
	0b01011011, //2
	0b01001111, //3
	0b01100110, //4
	0b01101101, //5
	0b01111101, //6
	0b00000111, //7
	0b01111111, //8
	0b01101111, //9
	0b01000000, //A
	0b00000000, //B
	0b01110110, //C
	0b01110110, //D
	0b01110110, //E
	0b01110110, //F
	0b01000000, //16 => -
	0b00000000, //17 => OFF
	0b00000000, //18 => OFF
	0b01110110, //19 => H
	0b01110110, //20 => H
 };

signed char round5[] = { 
	0, -1, -2, 2, 1
 };


void initDisplay()
{
	trisPort = 0;
	trisDP2 = tris1000 = tris100 = tris10 = tris1 = 0;
	ledPort = 0;
	ledDP2 = mux1000 = mux100 = mux10 = mux1 = 0;
	ledCif[0] = 0b00111111;
}

void displayOff()
{
	ledPort = 0;
	mux1 = mux10 = mux100 = mux1000 = 0;
}

void refreshSegment()
{	
	unsigned long tmpInt;

	seg1000 = seg100 = seg10 = seg1 = dispOFF;

	// Decide how to show results
	switch (ShowMode & 0x07)
	{
		case 0:
		case 4:
			tmpInt = mesurement.Val;									
			break;
		case 1:
		case 5:
			tmpInt = mesurement.Val;
			tmpInt *= 2;
			break;
		case 2:
		case 6:
			tmpInt = mesurement.Val;
			tmpInt *= 4;
			break;
		case 3:
		case 7:
			tmpInt = mesurement.Val;
			tmpInt *= 5000;
			tmpInt /= 1024;
			break;
	}
	if (ShowMode & 0x04 == 0x04)
		tmpInt += round5[tmpInt % 5];		

	if (ShowModeCountdown>0)
	{
		ShowModeCountdown--;

		ledDP1 = ledDP2 = 1;

		seg1000 = seg1 = dispOFF;
		tmpInt = ShowMode;
		seg10 = tmpInt % 16;
		tmpInt /= 16;
		seg100 = tmpInt % 10;
	}
	else
	{
		seg1 = tmpInt % 10;
		tmpInt /= 10;
		seg10 = tmpInt % 10;
		tmpInt /= 10;
		seg100 = tmpInt % 10;
		tmpInt /= 10;
		seg1000 = tmpInt % 10;

		ledDP1 = ledDP2 = 0;
	}

	

	switch (muxLED % 4)
	{
		case 0:
			mux1 = 0;
			ledPort = ledCif[seg1000];
			switch (ShowMode & 0x70)
			{
				case 0x10:
					ledDP1 = 1;
					break;
				case 0x50:
					ledDP2 = 1;
					break;
				default:
					break;
			}
			mux1000 = 1;
			break;
		case 1:
			mux1000 = 0;
			ledPort = ledCif[seg100];
			switch (ShowMode & 0x70)
			{
				case 0x20:
					ledDP1 = 1;
					break;
				case 0x60:
					ledDP2 = 1;
					break;
				default:
					break;
			}
			mux100 = 1;
			break;
		case 2:
			mux100 = 0;			
			ledPort = ledCif[seg10];
			switch (ShowMode & 0x70)
			{
				case 0x30:
					ledDP1 = 1;
					break;
				case 0x70:
					ledDP2 = 1;
					break;
				default:
					break;
			}
			mux10 = 1;
			break;
		case 3:
			mux10 = 0;
			ledPort = ledCif[seg1];
			mux1 = 1;
			break;
		default:
			displayOff();
			break;
	}
}
