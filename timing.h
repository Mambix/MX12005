#if defined(__GNUC__)
#define __EXTENSION __extension__
#else
#define __EXTENSION
#endif


#if !defined(__PACKED)
    #define __PACKED
#endif


typedef unsigned char           BYTE;                           /* 8-bit unsigned  */
typedef unsigned short int      WORD;                           /* 16-bit unsigned */
typedef unsigned long           DWORD; 
typedef unsigned short long 	UINT24;

typedef union
{
	WORD Val;
	struct
	{
		BYTE   HB;
		BYTE   LB;
	} byte;
} WORD_VAL;

typedef union
{
    DWORD Val;	
	struct
	{
		WORD LW;
		WORD HW;
	} word;
    struct
    {
        BYTE LB;
        BYTE HB;
        BYTE UB;
        BYTE MB;
    } byte;
} DWORD_VAL;

extern char refreshLED;
extern char refreshMesurement;
extern DWORD_VAL mesurement;
extern unsigned int divMesure;
extern DWORD_VAL avgMesure[8];
extern char avgPos;

