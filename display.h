#define mux1000		PORTBbits.RB4
#define mux100		PORTBbits.RB5
#define mux10		PORTBbits.RB6 
#define mux1		PORTBbits.RB7 
#define ledPort		PORTC
#define ledDP1		PORTCbits.RC7
#define ledDP2		PORTAbits.RA4

#define tris1000	TRISBbits.TRISB4
#define tris100		TRISBbits.TRISB5
#define tris10		TRISBbits.TRISB6
#define tris1		TRISBbits.TRISB7
#define trisPort	TRISC
#define trisDP2		TRISAbits.TRISA4

extern unsigned int muxLED;
extern unsigned char blink;
extern unsigned char ShowMode;
extern unsigned long atmPressure;
extern unsigned long ShowModeCountdown;

extern void refreshSegment(void);
extern void displayOff(void);
extern void initDisplay(void);
extern unsigned long calcPressure(unsigned long adc);

