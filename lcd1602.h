/*
 * _1602.h
 *
 * Created: 06/10/2020 21:49:15
 *  Author: Alexey  Ivanov
 * 
 */ 


#ifndef LCD1602_H_
#define LCD1602_H_

#define F_CPU	16000000UL

#include <util/delay.h>
#include <avr/io.h>

/********************************************
Display Configuration
********************************************/
#define LCD1602_4BIT_INTERFACE  //if 8 bit interface remove definition
#define LCD1602_2_LINE_DISPLAY  //if 1 line display remove definition
#define LCD1602_5x8_FONT		//if 5x10 font remove definition

/****************************************************
DDRAM addresses. For 16 char display - visible till 0F

1st line:		00 01 02 03 04 ... 0F | 10 ... 26 27
2nd line:		40 41 42 43 44 ... 4F | 50 ... 66 67

Left Shift:	

1st line:		01 02 03 04 05 ... 10 | 11 ... 27 00
2nd line:		41 42 43 44 45 ... 50 | 51 ... 67 40

Right shift:

1st line:		27 00 01 02 03 ... 0E | 0F ... 25 26
2nd line:		67 40 41 42 43 ... 4E | 4F ... 65 66

****************************************************/

/************************************************************************/
/* PORT for command lines: RS, RW, E                                    */
/************************************************************************/
#define LCD1602_PORT_C		PORTB
#define LCD1602_DDR_C		DDRB

//command line declaration
#define LCD1602_RS			PINB0
#define LCD1602_RW			PINB1
#define LCD1602_E			PINB2


/************************************************************************/
/* PORT for Data bus                                                    */
/************************************************************************/
#define LCD1602_DDR_D		DDRD
#define LCD1602_PORT_D		PORTD


/************************************************************************/
/* For data bus all pins must follow on the same port
PinDataStart - first pin for data interface
for 8 bit interface pin corresponds to D0
fro 4 bit interface pin corresponds to D4                             */
/************************************************************************/
#define LCD1602_PinDataStart	PIND4

//Commands
#define LCD1602_CLR			0b00000001 //Clear Display and set address at 0
#define LCD1602_RET			0b00000010 //Set DDRAM address to 0 and return display from shifted position to original.

#define LCD1602_ENTRY_MODE	0b00000100 //Set cursor move direction and specify display shift
#define LCD1602_I_D			1	//1 - increment (right), 0 - decrement (left)
#define LCD1602_S			0	//1 - Shifts entire display according to I/D, 0 - no shift

#define LCD1602_DISP_CTRL	0b00001000 //Set display, cursor and blinking on/off
#define LCD1602_D_ON		2	//1 - Display ON, 0 - Display off
#define LCD1602_C_ON		1	//1 - Cursor displayed, 0 - Cursor not displayed
#define LCD1602_B_ON		0	//1 - Cursor blinking ON, 0 - cursor blinking off

#define LCD1602_SHIFT		0b00010000 //Moves cursor and shift display without changing DDRAM content
#define LCD1602_DC_SHIFT	3	//1 - Display shift, 0 - cursor shift
#define LCD1602_RL_SHIFT	2	//1 - Right shift, 0 - Left Shift

#define LCD1602_FUNC_SET	0b00100000 // Set interface, number of lines and character font
#define LCD1602_DATA_LEN	4	//1 - 8 bits data, 0 - 4 bits data
#define LCD1602_LINE_NUM	3	//1 - two lines, 0 - one line
#define LCD1602_FONT_SIZE	2	//1 - 5x10 dots, 0 - 5x8 dots

#define LCD1602_CGRAM_ADR	0b01000000 //set CGRAM address, for data input/output
#define LCD1602_DDRAM_ADR	0b10000000 //set DDRAM address, for data input/output

//States of RW Line
#define LCD1602_READ		1
#define LCD1602_WRITE		0

//States of RS line
#define LCD1602_IR			0
#define LCD1602_DR			1


/************************************************************************/
/* Display init, modify display config in LCD1602.H                     */
/************************************************************************/
void LCD1602_Init();

/************************************************************************/
/* Write a char to next cursor location                                 */
/************************************************************************/
void LCD1602_WriteChar(char cData);

/************************************************************************/
/* Clear Display                                                        */
/************************************************************************/
void LCD1602_ClearDisplay();

static void LCD1602_WriteCommand(char cRS, char cRW, char cCommand);

static void LCD1602_Write4Part(char cCommand);

/************************************************************************/
/* Software reset                                                       */
/************************************************************************/
void LCD1602_Reset();

/************************************************************************/
/* Write string to current position                                     */
/************************************************************************/
void LCD1602_WriteString(char* cString);

/************************************************************************/
/* Set DDRAM address                                                    */
/************************************************************************/
void LCD1602_SetDDRAMAddress(char cAdr);

/************************************************************************/
/* Send command Display control
	cDispOn:	1 - Display On;		0 - Display off
	cCursOn:	1 - Cursor On;		0 - Cursor  off
	cBlinkOn:	1 - Blinking On;	0 - Blinking off                    */
/************************************************************************/
void LCD1602_DisplayControl(char cDispOn, char cCursOn, char cBlinkOn);

/************************************************************************/
/* set DDRAM address to 0, also returns display from shifted position 
to original                                                             */
/************************************************************************/
void LCD1602_RetHome();

/************************************************************************/
/* Set entry mode
	cMoveDirection :	1 - Cursor moves to right; 0 - Cursor moves to left
	cShift:				1 - Display shify; 0 - No shift
                                                                     */
/************************************************************************/
void LCD1602_EntryModeSet(char cMoveDirection, char cShift);

/************************************************************************/
/* Display shift or cursor move
	cDCShift:	1 - Display shift, 0 - Cursor move
	cRightLeft:	1 - shift/move to the right; 0 - shift/move to ht left
                                                                     */
/************************************************************************/
void LCD1602_Shift(char cDCShift, char cRightLeft);

#endif /* LCD1602_H_ */