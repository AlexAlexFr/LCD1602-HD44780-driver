/*
 * _1602.c
 *
 * Created: 06/10/2020 21:48:58
 *  Author: Alexey Ivanov
 * Driver for LCD with HD44780 controller
 * v1.0
 */ 

#include "lcd1602.h"

void LCD1602_Init()
{
/*********************************
	Mandatory Initilisation
**********************************/
	LCD1602_DDR_C |= (1<<LCD1602_RS)|(1<<LCD1602_RW)|(1<<LCD1602_E);

	
#ifdef LCD1602_4BIT_INTERFACE	
	LCD1602_DDR_D |= 0x0F << LCD1602_PinDataStart;
#else
	LCD1602_DDR_D |= 0xFF;
#endif

	//Set chip enable LOW
	LCD1602_PORT_C &=~(1<<LCD1602_E);
	_delay_ms(50);

#ifdef LCD1602_2_LINE_DISPLAY
	#define LCD1602_LINES (1<<LCD1602_LINE_NUM)
#else
	#define LCD1602_LINES 0
#endif

#ifdef LCD1602_5x8_FONT
	#define LCD1602_FONT	0
#else
	#define LCD1602_FONT	(1<<LCD1602_FONT_SIZE)
#endif
		
#ifdef LCD1602_4BIT_INTERFACE	
	//first time FuncSet send twice
	LCD1602_PORT_C &= ~(1<<LCD1602_RS);
	LCD1602_PORT_C &= ~(1<<LCD1602_RW);
		
	LCD1602_Write4Part(0b00000010<<LCD1602_PinDataStart);
	
	LCD1602_WriteCommand(LCD1602_IR, LCD1602_WRITE, LCD1602_FUNC_SET|LCD1602_LINES|LCD1602_FONT);
#else
	
	LCD1602_WriteCommand(LCD1602_IR, LCD1602_WRITE, LCD1602_FUNC_SET|(1<<LCD1602_DATA_LEN)|LCD1602_LINES|LCD1602_FONT);
#endif

/********************************
End of Mandatory Initialisation
*******************************/

	//Dipslay ON, Cursor ON, Blinking	
	//LCD1602_WriteCommand(LCD1602_IR, LCD1602_WRITE, LCD1602_DISP_CTRL|(1<<LCD1602_D_ON)|(1<<LCD1602_C_ON)|(1<<LCD1602_B_ON));
	
	LCD1602_WriteCommand(LCD1602_IR, LCD1602_WRITE, LCD1602_DISP_CTRL|(1<<LCD1602_D_ON)|0|0);
	LCD1602_WriteCommand(LCD1602_IR, LCD1602_WRITE, LCD1602_ENTRY_MODE|(1<<LCD1602_I_D));
		

}

void LCD1602_Reset()
{
	_delay_ms(20);
	
	LCD1602_PORT_C &= ~(1<<LCD1602_RS);
	LCD1602_PORT_C &= ~(1<<LCD1602_RW);
		
	LCD1602_Write4Part(0b0011<<LCD1602_PinDataStart);
	
	_delay_ms(5);
	
	LCD1602_Write4Part(0b0011<<LCD1602_PinDataStart);
	
	_delay_us(150);
	
	LCD1602_Write4Part(0b0011<<LCD1602_PinDataStart);
	
	_delay_ms(1);
	
	LCD1602_Init();
	
}

static void LCD1602_WriteCommand(char cRS, char cRW, char cCommand)
{
		//set RS line
		if (cRS==0)
		{
			LCD1602_PORT_C &= ~(1<<LCD1602_RS);
		}
		else
		{
			LCD1602_PORT_C |= (1<<LCD1602_RS);
		}
		
		//Set R/W line
		if (cRW == 0)
		{
			LCD1602_PORT_C &= ~(1<<LCD1602_RW);
		}
		else
		{
			LCD1602_PORT_C |= (1<<LCD1602_RW);
		}
		
#ifdef	LCD1602_4BIT_INTERFACE
		//Take High part of command
		char cCommandPart = cCommand & 0xF0;
		//Shift to right
		cCommandPart = cCommandPart >> 4;
		//Shift starting from PinDataStart
		cCommandPart = cCommandPart << LCD1602_PinDataStart;
		
		LCD1602_Write4Part(cCommandPart);
		
		cCommandPart = cCommand & 0x0F;
		cCommandPart = cCommandPart << LCD1602_PinDataStart;
		
		LCD1602_Write4Part(cCommandPart);

#else
	
	LCD1602_PORT_D = cCommand;
	_delay_us(1);
	

	LCD1602_PORT_C |= (1<<LCD1602_E);
	_delay_us(1);
	
	LCD1602_PORT_C &= ~(1<<LCD1602_E);
	_delay_ms(2);
	
#endif		

}

static void LCD1602_Write4Part(char cCommand)
{
	LCD1602_PORT_D = cCommand;
	
		_delay_us(1);
		

		LCD1602_PORT_C |= (1<<LCD1602_E);
		_delay_us(1);
		
		LCD1602_PORT_C &= ~(1<<LCD1602_E);
		_delay_ms(2);
	
}

void LCD1602_WriteChar(char cData)
{
	LCD1602_WriteCommand(LCD1602_DR,LCD1602_WRITE, cData);
}

void LCD1602_ClearDisplay()
{
	LCD1602_WriteCommand(LCD1602_IR, LCD1602_WRITE,LCD1602_CLR);
	
	_delay_ms(2);
}

void LCD1602_WriteString(char* cString)
{
	char cCount = 0;
	while (cString[cCount] != 0)
	{
		LCD1602_WriteCommand(LCD1602_DR,LCD1602_WRITE, cString[cCount]);
		cCount++;
	}
}

void LCD1602_SetDDRAMAddress(char cAdr)
{
	LCD1602_WriteCommand(LCD1602_IR, LCD1602_WRITE,LCD1602_DDRAM_ADR|cAdr);
}

void LCD1602_DisplayControl(char cDispOn, char cCursOn, char cBlinkOn)
{
	LCD1602_WriteCommand(LCD1602_IR, LCD1602_WRITE, LCD1602_DISP_CTRL|(cDispOn<<LCD1602_D_ON)|(cCursOn<<LCD1602_C_ON)|(cBlinkOn<<LCD1602_B_ON));
}

void LCD1602_RetHome()
{
	LCD1602_WriteCommand(LCD1602_IR, LCD1602_WRITE, LCD1602_RET);
}

void LCD1602_EntryModeSet(char cMoveDirection, char cShift)
{
	LCD1602_WriteCommand(LCD1602_IR, LCD1602_WRITE, LCD1602_ENTRY_MODE|(cMoveDirection<<LCD1602_I_D)|(cShift<<LCD1602_S));
}

void LCD1602_Shift(char cDCShift, char cRightLeft)
{
	LCD1602_WriteCommand(LCD1602_IR, LCD1602_WRITE, LCD1602_SHIFT|(cDCShift<<LCD1602_DC_SHIFT)|(cRightLeft<<LCD1602_RL_SHIFT));
}
