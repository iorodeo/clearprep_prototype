/*
Copyright (c) 2010 Jennifer Holt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

/*

/*
		re-wrote most of the code. optimized existing graphics functions and added some new ones
		I have not optimized the pixel drawing routine, I don't think it can get that much better, 
		this code only works with the small display(it's the one I have, so that's all I can test)
		if I ever get one of the larger displays, I'll write equivalent code for it

	TODO 	clean up, rename variables, organize, indent, comment
*/

#include <avr/pgmspace.h> 
#include <avr/io.h>
#include "rprintf.h"
#include <math.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/boot.h>


#define FOSC 16000000// Clock Speed
#define XOFF 0X13
#define XON 0x11
#define RX_BUFFER_SIZE 256	//max 256 bytes due to using char's as index, note this doesn't nee to be set very high if the host interface supports XON/XOFF

//128x64 definitions
#define EN 0	//PC0
#define RS 1	//PC1, D_I?
#define R_W 2	//PC2
#define RESET 3	//PC3
#define CS1 4	//PC4
#define CS2 5	//PC5
#define BL_EN 2	//PB2

//EEprom addresses
#define BPS 0
#define BACKLIGHT 1
#define SPLASH 2
#define REV 3
#define DEBUG 4
#define CRLF 5

//LCD control port states
// note PORTC is x x CS2 CS1   RESET RW DI EN
#define LCD_COMMAND2 0x18	//cs1,reset
#define LCD_COMMAND1 0x28	//cs2,reset
#define LCD_COMMAND12 0x38	//cs1,cs2,reset
#define LCD_DATA2 0x1A		//cs1,reset,DI
#define LCD_DATA1 0x2A		//cs2,reset,DI
#define LCD_DATA12 0x3A		//cs1,cs2,reset,DI
#define LCD_READ2 0x1E		//cs1,reset,rw,DI
#define LCD_READ1 0x2E		//cs2,reset,rw,DI
#define LCD_STATUS2 0x1C	//cs1,reset,rw
#define LCD_STATUS1 0x2C	//cs2,reset,rw
#define LCD_IDLE 0x08		//reset

//macro for clocking LCD
#define CLK_LCD PORTC |= (1 << EN);delay();PORTC &= ~(1 << EN);delay()

//macros for common LCD commands
#define set_x(x) write_command_LCD((0x40 | x), 0)
#define set_x_side(x,y) write_command_LCD((0x40 | x),y)
#define set_page(y) write_command_LCD((0xB8 | y),0)
#define display_on() write_command_LCD(0x3F,0)


//Define functions
//======================

//functions that write to flash
void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int uiAddress);

//initialization functions
void ioinit(void);     			//Initializes IO
void USART_Init( unsigned int ubrr);

//delays
void delay_ms(uint16_t x); 		//General purpose delay
void delay_us(uint8_t x);
void delay(void);			//display timing

//LCD interface functions
void LCD_reset(void);
void set_backlight(unsigned char dutycycle);
void set_data(char data);
void write_command_LCD(char byte, char chip);
void write_data_LCD(char byte, char chip);
char read_data_LCD(char chip);
char read_byte(char byte, char side);
void read_block(char x, char page, char length, char* buf);
void write_block(char x, char page, char length, char* buf);



//serial comms functions
void put_char(char byte);
void set_baud(char b);
char RX_get_byte(void);  //retrieves a bye from the serial port, blocks till data is available


//graphics functions
void clear_screen(char option);
void print_char(char txt);
void del_char(void);
void pixel(char S_R, char x, char y);
void line(char S_R, char x1, char y1, char x2, char y2);
void circle(char S_R, int x, int y, int r);
void draw_block(char x1, char y1, char x2, char y2, char data);
void box(char x1, char y1, char x2, char y2, char mode);
void bitblt(char x, char y, char width, char height, char mode, char* data);
void draw_sprite(char x, char y, char n, char mode);

//======================

//usart variables
char baud_rate = 6;//115200 by default
unsigned char RX_array[RX_BUFFER_SIZE];
volatile unsigned char RX_in = 0;
unsigned short RX_read = 0;
volatile unsigned char RX_bytes=0;	//keeps track of the # of bytes in the RX buffer
volatile unsigned char RX_pause=0;	//flags if RX has been suspended

//graphics variables
char reverse = 0;
unsigned char splash_screen = 1;
char buffer[256];			//buffer for image data in bitblt routines

//misc variables
unsigned char debug_msg = 0;		//debug messages, 0=off, 1=binary, 2=ascii
unsigned char BL_dutycycle = 100;



//sprite stuff
// note sprites all take SPRITE_SIZE bytes and there are NUM_SPRITES of them, but default I set these to 34 and 8 for a total of 272 bytes.
// this is sufficient for 8 16x16 sprites. these can be changed, but watch the total memory used. there is only 1k total on the ATmega168 

#define SPRITE_SIZE 34		//needs to be at least 22 to hold sparkfun logo
#define NUM_SPRITES 8		//make sure that the memory allocated to sprites (SPRITE_SIZE*NUM_SPRITES) doesn't over fill the device (it only has 1K for all variables)

//logo modified to use with bitblt, and placed in first sprite
char sprite[SPRITE_SIZE*NUM_SPRITES]={	
					0x0A,0x10,0x80,0xc0,0x40,0x0c,0x3e,0xfe,0xf2,	//array to hold sprites, each is SPRITE_BYTES long, each sprite has [width],[height],[bunch O' bytes for bitblk]
					0xe0,0xf0,0xe0,0xff,0x7f,0x3f,0x1f,		//user must make sure data is ok, code does not check
					0x1f,0x1f,0x1f,0x0f,0x07,0x03};

//font stuff

//font drawing variables
char x_offset = 0;	//upper left corner of character, x-coord
char y_offset = 0;	//upper left corner of character, y-coord
char font_mode=7;	//how font interacts with background. (bitblt option) 7 = overwrite and is default(no need to erase the background first)
char CR_LF = 1;		//should CR have an automatic LF
char font_bytes;	//# of bytes in a character (5 for default font)
char font_w;	//width of a character in pixels (5 for default font)
char font_h;	//height of a character in pixels (8 for default font)
char font_space;	//horizontal space to leave between characters
char aux_font=0;	//which font to use 0=default, 1=aux


//this include file stores all the revelant font data. you can make new ones with the bmp2header_font utility in the utilities folder
#include "font.h"			  			  

//this include file stores all the revelant aux font data. you can make new ones with the bmp2header_font utility in the utilities folder
#include "aux_font.h"			  			  

ISR (SIG_USART_RECV)//USART Receive Interrupt
{
	cli();//Disable Interrupts

	RX_array[RX_in++] = UDR0;	//get recieved byte
	if (RX_in >= RX_BUFFER_SIZE) RX_in = 0;	//wrap RX_in	
	RX_bytes++;			//keep count of bytes recieved
	
	//test for buffer close to full
	if (RX_bytes > (unsigned char)(RX_BUFFER_SIZE *.75))
	{
		put_char(XOFF);		//send XOFF
		RX_pause=1;		//flag reception suspended
	}
	
	sei();//Enable Interrupts	
}

ISR (TIMER0_COMPA_vect) //
{
	unsigned char y;
	
	//cli();//Disable Interrupts
	
	TCCR0B = 0;
	TIMSK0 = 0;//disable timer ints
	
	TIFR0 = 0x02;//clear the interrupt
	TCNT0 = 0;
		
	y = PINB;
	if (y & (1<<BL_EN))//on
	{
		PORTB &= (~(1<<BL_EN));
		OCR0A = BL_dutycycle;
	}
	
	else//off
	{
		PORTB |= (1<<BL_EN);
		OCR0A = 100 - BL_dutycycle;
	}
	
	//PORTB |= (1<<BL_EN);
	
	TIMSK0 = 0x02;//enable OCR0A
	TCCR0B = 0x02;
	//sei();//Enable Interrupts
}


int main (void)
{
	char x, y, z, temp, q, p, i;	//variables used for loops, buffering of command bytes, counters etc.
	uint16_t count, count2;		//counters for long stuff that may go over 256
	
	ioinit(); 			//Setup IO pins and defaults

	set_baud(6);			//115200
	rprintf_devopen(put_char); // init rprintf 
	put_char(XON);

	//check for existing preset values==============================================================
	temp = EEPROM_read((unsigned int)BPS);
	
	if ((temp < 1) | (temp > 6))//BPS will only be 1-6
	{
		cli();//Disable Interrupts
		
		EEPROM_write((unsigned int) BPS, 6);
		EEPROM_write((unsigned int) BACKLIGHT, 100);
		EEPROM_write((unsigned int) SPLASH, 1);
		EEPROM_write((unsigned int) REV, 0);
		EEPROM_write((unsigned int) DEBUG, 0);
		EEPROM_write((unsigned int) CRLF, 1);

		sei();//Enable Interrupts
		
		BL_dutycycle = 100;
		baud_rate = 6;
		splash_screen = 1;
		reverse = 0;
		debug_msg = 0;
		CR_LF = 1;
	}
	
	else
	{
		baud_rate = temp;
		BL_dutycycle = EEPROM_read((unsigned int)BACKLIGHT);
		splash_screen = EEPROM_read((unsigned int)SPLASH);
		reverse = EEPROM_read((unsigned int)REV);
		debug_msg = EEPROM_read((unsigned int)DEBUG);
		CR_LF = EEPROM_read((unsigned int)CRLF);
	}
	


//		baud_rate = 6;			//these are the default values, usually these should get read from EEPROM, if that doesn't work for you
//		BL_dutycycle = 100;		//uncomment these and set them to whatever you want.
//		baud_rate = 6;
//		splash_screen = 1;
//		reverse = 0;
//		debug_msg = 0;
//		CR_LF = 1;

	//Set up the default font===========================================
	font_w = pgm_read_byte(&text_array[0]);	
	font_h = pgm_read_byte(&text_array[1]);	
	font_space = pgm_read_byte(&text_array[2]);
	font_bytes = font_h/8;	//8 pixels/byte
	if (font_h % 8 != 0)
		font_bytes++;	//partial rows count too
	font_bytes *= font_w;	//need font_w stacks of rows	

	//Reset the display=================================================

	LCD_reset();

	set_backlight(BL_dutycycle);
	
	delay_ms(500);

	
	//Logo==========================================================
	if (splash_screen == 1)
	{
		draw_sprite(56,24,0,4);	//XOR'ing the sprite works in normal and reverse modes
	}

	cli();
	RX_in = 0;	//clear RX input buffer
	RX_read = 0;
	RX_bytes = 0;
	sei();	

	delay_ms(1000);	//display the splash screen for a second
	clear_screen(0);	//clear the screen
	
	if (RX_in > 0)	//revert to 115200
	{
		print_char('1');
		print_char('1');
		print_char('5');
		print_char('2');
		print_char('0');
		print_char('0');
		
		baud_rate = 6;
		set_baud(6);//115200
		
		cli();
		
		EEPROM_write((unsigned int) BPS, 6);
		
		sei();//Enable Interrupts
	}
	
	else (set_baud(baud_rate));
	
	delay_ms(1000);
	clear_screen(0);
	
	//flush any received chars

	cli();
	RX_read=0;
	RX_in=0;
	RX_bytes=0;
	sei();

	//main loop===================================================
	//main loop waits for characters on the serial port, and either prints them, or interprets them as a command
	while(1)
	{
			x = RX_get_byte();	
			switch(x)		//switch based on received character
			{
			 case 8:			//backspace
				del_char();
				if (debug_msg==2)
					put_char(x);	//if ASCII debug, echo characters typed
			 break;
			
			 case 13:			//CR, reset to start of row

				x_offset = x_offset % (font_w+font_space);
				if (debug_msg==2)	
					put_char(x);	//if ASCII debug, echo characters typed

				if (CR_LF==1)
				{
					if (debug_msg==2)	
						put_char(0x0A);	//if ASCII debug, echo LF as well

					if (y_offset > (64-font_h-font_h))
						y_offset = y_offset % font_h;	//this makes sure that the line restarted at the top will overlap the old one
					else 
						y_offset += font_h;
				}
			 break;

			 case 10:			//LF move to next row

				if (y_offset > (64-font_h-font_h))
					y_offset = y_offset % font_h;	//this makes sure that the line restarted at the top will overlap the old one
				else 
					y_offset += font_h;
				if (debug_msg==2)
					put_char(x);	//if ASCII debug, echo characters typed
			 break;
			//--------------------------------------------------------------------------------------------------------------------
			 case 124:					//received character indicated a coming command
				y = RX_get_byte();			//get char from buffer

				switch (debug_msg)		//output for debugging or monitoring purposes
				{
				 case 1:			//print binary debug msg
					put_char(y);
				 break;
				 case 2:			//print ASCII debug msg
					rprintf("Entered Command %d\n",y);
				 break;
				 default:
				 break;
				}
	
				switch(y)				//switch to which command was sent
				{
				 case 0:	//clear screen
					clear_screen(0);
					x_offset=0;
					y_offset=0;
				 break;	
				//************************************************************************************************************
				 case 1:	//set debug level
								
					debug_msg = RX_get_byte();		//get char from buffer

					if (debug_msg >2) debug_msg=2;
					cli();
					EEPROM_write((unsigned int) DEBUG, debug_msg);
					sei();
					
				 break;	
				//************************************************************************************************************			
				 case 2:	//set backlight duty cycle
					//wait for byte
					BL_dutycycle = RX_get_byte();		//get char from buffer
					
					set_backlight(BL_dutycycle);
					
					cli();
					EEPROM_write((unsigned int) BACKLIGHT, BL_dutycycle);
					sei();
				 break;	
				//************************************************************************************************************
				 case 3:	//draw circle
					//need 4 bytes, RX_read already pointing at the first
				
						//wait for byte (x coord of circle)
					y = RX_get_byte();	//get char from buffer
						
						//wait for byte (y coord of circle)
					z = RX_get_byte();	//get char from buffer

						//wait for byte (r of circle)
					temp = RX_get_byte();	//store it and increment RX_read
 				
						//wait for byte (S_R of circle)
					q = RX_get_byte();	//store it and increment RX_read

					circle(q, y, z, temp);
				 break;
				//************************************************************************************************************
				 case 4:	//toggles CR_LF
					if (CR_LF==1)
						CR_LF=0;
					else
						CR_LF=1;
					cli();
					EEPROM_write((unsigned int) CRLF, CR_LF);
					sei();

				 break;
				//************************************************************************************************************
				 case 5:	//erase block
					//need 4 bytes, RX_read already pointing at the first
					
						//wait for byte (x coord of corner1)
					y = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary
						
						//wait for byte (y coord of corner1)
					z = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

						//wait for byte (x coord of corner2)
					temp = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

						//wait for byte (y coord of corner2)
					q = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

					p=0;				//do erase by drawing a block filled with 0x00
					if (reverse==1)			//if display is reversed, draw a block filled with 0xFF
						p=0xff;
					draw_block(y ,z , temp, q, p);
				 break;
				//************************************************************************************************************
				 case 6:	//LCD reset
					LCD_reset();
				 break;	
				//************************************************************************************************************
				 case 7:	//change baud rate

					y = RX_get_byte();	//store it and increment RX_read
										
					if ((y > 0) && (y < 7)) baud_rate = y;

					
					set_baud(baud_rate);
					
					cli();
					EEPROM_write((unsigned int) BPS, baud_rate);
					sei();
				 break;	
				//************************************************************************************************************
				 case 8:	//toggle aux font
				
					if (aux_font==1)	//if using aux font, set back to default
					{
						aux_font=0;
						font_w = pgm_read_byte(&text_array[0]);	
						font_h = pgm_read_byte(&text_array[1]);	
						font_space = pgm_read_byte(&text_array[2]);
						font_bytes = font_h/8;	//8 pixels/byte
						if (font_h % 8 != 0)
							font_bytes++;	//partial rows count too
						font_bytes *= font_w;	//need font_w stacks of rows	
					}
					else			//if using default font, set to aux
					{
						aux_font=1;
						font_w = pgm_read_byte(&aux_text_array[0]);	
						font_h = pgm_read_byte(&aux_text_array[1]);	
						font_space = pgm_read_byte(&aux_text_array[2]);
						font_bytes = font_h/8;	//8 pixels/byte
						if (font_h % 8 != 0)
							font_bytes++;	//partial rows count too
						font_bytes *= font_w;	//need font_w stacks of rows	
					}
				 break;	
				//************************************************************************************************************
				 case 10:	//set font mode

						//wait for byte (x coord of lower left)
					y = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary
						
					if (y<0 || y > 7)
						y=7;
					font_mode=y;
				 break;	
				//************************************************************************************************************
				 case 11:	//draw_sprite
						
						//wait for byte (x coord)
					y = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary
						
						//wait for byte (y coord)
					z = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

						//wait for byte (sprite #)
					temp = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

						//wait for byte (mode)
					q = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary
					
					draw_sprite(y,z,temp,q);
					
				 break;	
				//************************************************************************************************************
				 case 12:	//line
					//need 5 bytes, RX_read already pointing at the first
					
						//wait for byte (x coord of start)
					y = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary
						
						//wait for byte (y coord of start)
					z = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

						//wait for byte (x coord of end)
					temp = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

						//wait for byte (y coord of end)
					q = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

						//wait for byte (S_R)
					p = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

					line(p, y ,z , temp, q);
				 break;
				//************************************************************************************************************
				 case 13:	//upload sprite
						//send sprite # first 0..(NUM_SPRITES-1), then sprite data
						//sprite format is (char) width (char) height (SPRITE_SIZE-2 char's) data
						//sprite data is in native bitblt format, ie rows of bytes representing 8-pixel high vertical stripes

										//wait for byte (sprite#)
					y = RX_get_byte();					//store it and increment RX_read
 										//wrap if necessary
					
					if (y>=NUM_SPRITES)
						y=(NUM_SPRITES-1);					//coerce to valid sprite					
					
					//calculate offset to sprite		

					count = y * (uint16_t)SPRITE_SIZE;				//offset to sprite data

					 for (count2=0;count2 < (uint16_t)SPRITE_SIZE ;count2++)	//loop for sprite bytes
					 {
										//wait for byte
						sprite[count++] = RX_get_byte();			//store it and increment RX_read
	 									//wrap if necessary
					 }			
				 break;	
				//************************************************************************************************************
 				 case 15:	//draw box
					//need 5 bytes, RX_read already pointing at the first
					
						//wait for byte (x coord of corner1)
					y = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary
						
						//wait for byte (y coord of corner1)
					z = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

						//wait for byte (x coord of corner2)
					temp = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

						//wait for byte (y coord of corner2)
					q = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary
					p = RX_get_byte();	//store it and increment RX_read

					box(y ,z , temp, q, p);
				 break;
				//************************************************************************************************************
				 case 16:		//set pixel
					//need 3 bytes, RX_read already pointing at the first
					
						//wait for byte (x coord of pixel)
					z = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

						//wait for byte (y coord of pixel)d
					temp = RX_get_byte();
 					

						//wait for byte (S_R of pixel)
					q = RX_get_byte();
 					
					
					pixel(q, z, temp);		//draw the pixel
				 break;
				//************************************************************************************************************
				// ASCII 17 is XON so we can't use it for a command
				 case 18:	//draw filled box
					//need 5 bytes, RX_read already pointing at the first
					
						//wait for byte (x coord of corner1)
					y = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary
						
						//wait for byte (y coord of corner1)
					z = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

						//wait for byte (x coord of corner2)
					temp = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

						//wait for byte (y coord of corner2)
					q = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary
					
						//wait for byte (fill)
					p = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary


					draw_block(y ,z , temp, q, p);
				 break;
				//************************************************************************************************************
				// ASCII 19 is XOFF so we can't use it for a command
				 case 20:	//reverse mode
					reverse ^= 1;
					clear_screen(1);
					cli();
					EEPROM_write((unsigned int) REV, reverse);
					sei();
				 break;	
				//************************************************************************************************************
				 case 21:	//toggle splash
					splash_screen ^= 1;
					cli();
					EEPROM_write((unsigned int) SPLASH, splash_screen);
					sei();
				 break;	
				//************************************************************************************************************
				 case 22:		//bitblt
						//wait for byte (x coord)
					y = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary
						
						//wait for byte (y coord)
					z = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

						//wait for byte (mode)
					temp = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

						//wait for byte (width)
					q = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary
					
						//wait for byte (height)
					p = RX_get_byte();	//store it and increment RX_read
 						//wrap if necessary

					bitblt(y, z, q, p, temp, NULL);	//calling bitblt with a NULL pointer tells it to take data from the serial port
									//note bitblt will wait till it gets all the data it wants based of the size of the image
									//if the data is interrupted or a byte gets lost, the whole program will hang
					
				 break;
				//************************************************************************************************************
				 case 24:		//set x_offset
							//wait for byte
					x_offset = RX_get_byte();		//get new x offset
							
					if (x_offset > 159) x_offset = 159;	//coerce x offset to valid range
				 break;
				//************************************************************************************************************
				 case 25:		//set y_offset
							//wait for byte
					y_offset = RX_get_byte();		//get new y offset
							

					if (y_offset > 127) y_offset = 127;	//coerce y offset to valid range		
				 break;	
				//************************************************************************************************************
 				 case 26:		//write command
							//wait for byte(data)
					y = RX_get_byte();		
							

							//wait for byte(chip # {1,2,3})
					z = RX_get_byte();	
							

					write_command_LCD(y,z);			//write command
				 break;	
				//************************************************************************************************************
				 case 27:		//write data
							//wait for byte(data)
					y = RX_get_byte();		
							

							//wait for byte(chip # {1,2,3})
					z = RX_get_byte();	
							

					write_data_LCD(y,z);			//write data
	
				//************************************************************************************************************
				 case 28:		//write block
							//wait for byte(x)
					y = RX_get_byte();		
							

							//wait for byte(page)
					z = RX_get_byte();		
							

							//wait for byte(length)
					temp = RX_get_byte();		
							
					
					//coerce length, there are only 128 pixels across the whole screen
					if (temp>128)
						temp=128;

					for(q=0;q<temp;q++)
					{
								//wait for byte(data)
						buffer[q] = RX_get_byte();	//store data	
								
					}
					write_block(y,z,temp,buffer);			//do block write
				 break;	
				//************************************************************************************************************
				 case 29:		//read data
							//wait for byte(chip # {1,2})
					y = RX_get_byte();		
							
					
					z = read_data_LCD(y);
					if(debug_msg==2)
						rprintf("0x%.2x\n",z);
					else
						put_char(z);
				 break;	
				//************************************************************************************************************
				 case 30:		//read byte
							//wait for byte(x)
					y = RX_get_byte();		
							
					
							//wait for byte(side)
					z = RX_get_byte();		
							
			
					temp= read_byte(y,z);
					
					if(debug_msg==2)
						rprintf("0x%.2x\n",temp);
					else
						put_char(temp);
				 break;	
				//************************************************************************************************************
				 case 31:		//read block
							//wait for byte(x)
					y = RX_get_byte();		
							

							//wait for byte(page)
					z = RX_get_byte();		
							

							//wait for byte(length)
					temp = RX_get_byte();		
							
					
					//coerce length
					if (temp>128)
						temp=128;

					read_block(y,z,temp,buffer);		//do block read
					
					for(z=0;z<temp;z++)
						if(debug_msg==2)
							rprintf("0x%.2x\n",buffer[z]);
						else
							put_char(buffer[z]);			
				 break;	
				//************************************************************************************************************
				 case 32:		//set y reg
							//wait for byte
					y = RX_get_byte();		//get new y offset
							
					
					set_x(y);
				 break;	
				//************************************************************************************************************
				 case 33:		//set page
							//wait for byte
					y = RX_get_byte();		//get new y offset
							
					
					set_page(y);
				 break;	
				//************************************************************************************************************
				}//end switch for which command	

				switch (debug_msg)
				{
				 case 1:	//print binary debug msg
					put_char(0);
				 break;
				 case 2:	//print ASCII debug msg
					rprintf("Exited Command, RX_buffer=%d\n\n",RX_bytes);
				 break;
				 default:
				 break;
				}
			 break;
			//--------------------------------------------------------------------------------------------------------------------
			 default:		//nothing special, print character to screen
				if (debug_msg==2)
					put_char(x);	//if ASCII debug, echo characters typed
				print_char(x);
			//--------------------------------------------------------------------------------------------------------------------
			}//end switch for backspace/command/normal char
	}//end while
}//end main()

void ioinit (void)
{
	
    //1 = output, 0 = input
   
	DDRB = 0b00000000; //PB0 and PB1 are inputs by default
	DDRD = 0b00000000; //PD2-PD7 are also inputs.  Ports B and D are the data bus.
	
	PORTB |= (1<<BL_EN);//Backlight off
	DDRB |= (1<<BL_EN);//set PB2 as output
	
	
	DDRC = ((1<<EN) | (1<<RS) | (1<<R_W) | (1<<RESET) | (1<<CS1) | (1<<CS2));
	PORTC = LCD_IDLE;
		
	//Init timer 2
    	TCCR2B = (1<<CS21); //Set Prescaler to 8. CS21=1

	//Set up Timer 0
	TCCR0A = 0x02;//CTC mode
	//TCCR0B = 0x02;
	TIMSK0 = 0x02;//enable OCR0A
	//OCR0B = 255 - BL_dutycycle;
	
	//OCR0A = 255 - (100 - BL_dutycycle);
	OCR0A = BL_dutycycle;

}

//General short delays
void delay_ms(uint16_t x)
{
	for (; x > 0 ; x--)
    {
        delay_us(250);
        delay_us(250);
        delay_us(250);
        delay_us(250);
    }
	
}

//General short delays
void delay_us(uint8_t x)
{
	char temp;
	
	if (x == 0) temp = 1;
	else temp = x;
	
	TIFR2 |= 0x01;			//Clear any interrupt flags on Timer2
    
    TCNT2 = 256 - temp; 		//256 - 125 = 131 : Preload timer 2 for x clicks. Should be 1us per click

	while(!(TIFR2 & 0x01));
	
	if (x == 0) return;		//this is for display timing	
	
	//The prescaler doesn't allow for a setting of 16, just 8 or 32. So, we do this twice.
	TIFR2 |= 0x01;
    
    TCNT2 = 256 - temp; 		//256 - 125 = 131 : Preload timer 2 for x clicks. Should be 1us per click

	while(!(TIFR2 & 0x01));
	
}

void USART_Init( unsigned int ubrr)
{
	// Set baud rate 
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	// Enable receiver and transmitter 
	UCSR0A = (1<<U2X0);
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);	//Enable Interrupts on receive character
	
	UCSR0C = (1<<UCSZ00)|(1<<UCSZ01);
	sei();
}

char RX_get_byte(void)	//retrieves a character from the RX_buffer and does book-keeping
{
	char x;

	while(RX_bytes==0);				//wait for data to be available
	x = RX_array[RX_read++];			//get char from buffer and increment read pointer
	if(RX_read >= RX_BUFFER_SIZE) RX_read = 0;	//if the read pointer reaches the end of the buffer, wrap back to the beginning
	cli();						//interrupts must be disabled when changing RX_bytes, since it can be chabged here and in the ISR
	RX_bytes--;
	sei();
	
	//check to see if we need to re-enable reception 

	if (RX_pause==1)		//if USART reception is suspended
		if (RX_bytes < (unsigned char)(RX_BUFFER_SIZE *.25) )		//if RX_buffer is suitably empty
		{
			put_char(XON);	//resume reception
			RX_pause=0;	//flag it
		}	
	return x;
}

void put_char(char byte)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = byte;
}

//delay for display timing
void delay(void)
{
	char y;
	
	for(y = 0; y < 30; y++)
	{
		asm volatile ("nop");
		
	}	
}

//set data port
void set_data(char data)
{
	//PORTB
	//DB0 = PB0
	//DB1 = PB1
	
	PORTB &= 0xFC;
	
	//PORTD
	//DB2 = PD2
	//DB3 = PD3
	//DB4 = PD4
	//DB5 = PD5
	//DB6 = PD6
	//DB7 = PD7
	
	PORTD &= 0x03;
	
	PORTB |= (data & 0x03);
	PORTD |= (data & 0xFC);

}

void clear_screen(char option)
{
	char x, y;

	if (option==0)	//actual erase of screen
	{		
		
		for (x = 0; x < 8; x++)
		{
			//set x address (page)
			set_page(x);
			
			//Set y address to zero
			set_x(0x00);
			
			//y address increments after every write
			//write data
			//need to set data bus to outputs
			DDRB |= 0x03; //PB0 and PB1 are outs
			DDRD |= 0xFC; //PD2-PD7 are also outs.  Ports B and D are the data bus.
		
			if (reverse == 1) set_data(0xFF);	//since data never changes when clearing the screen, set it once and do a bunch of clocks
			else set_data(0);

			PORTC = LCD_DATA12;			//write data to both chips
			for (y = 0; y < 64; y++)		//clock for all 64 horizontal positions
			{
				delay();
				CLK_LCD;
			}
			PORTC = LCD_IDLE;

			//set the data bus back to inputs
			DDRB &= 0xFC;	//PB0 and PB1 as inputs
			DDRD = 0;
			x_offset = 0;
			y_offset = 0;
		}//end page loop normal clear
	}//end normal clear
	else	//reverse mode switch, do a logical inversion of the screen
	{
		for(y=0;y<8;y++)	//loop for each page
		{
			read_block(0,y,128,buffer);	//read in display data
			for (x=0;x<128;x++)
				buffer[x]=~buffer[x];	//invert the data
			write_block(0,y,128,buffer);	//write data back to screen
		}
	}//end if(option)
}


//prints a character to the screen
//at x_offset, y_offset(top/left corner of character). Automatically augments offsets for next write
void print_char(char txt)
{

    // x_offset counts pixels from the left side of the screen
    // y_offset counts pixels from the top of the screen

    short text_array_offset, j;
    char k;
	text_array_offset = (txt - 32) * font_bytes+3;	// txt-32 is the ascii offset to 'space', font_bytes is the # of bytes/character, and 3 for font width,height,space which are stores at the beginning of the array

	//get data for character
	k=128;			//put in upper half of buffer, lower half is used by bitblt
	if (aux_font==0)	//using default font?
	{
		if (reverse == 1)
		{	
			for (j=text_array_offset; j < text_array_offset + font_bytes; j++) //loop for one character worth of bytes 
				buffer[k++] = ~pgm_read_byte(&text_array[j]);
		}
		else
		{
			for (j=text_array_offset; j < text_array_offset + font_bytes; j++) //loop for one character worth of bytes 
				buffer[k++] = pgm_read_byte(&text_array[j]);
		}
	}
	else	//using aux font
	{

		if (reverse == 1)
		{	
			for (j=text_array_offset; j < text_array_offset + font_bytes; j++) //loop for one character worth of bytes 
				buffer[k++] = ~pgm_read_byte(&aux_text_array[j]);
		}
		else
		{
			for (j=text_array_offset; j < text_array_offset + font_bytes; j++) //loop for one character worth of bytes 
				buffer[k++] = pgm_read_byte(&aux_text_array[j]);
		}
	}
	//bitblt it
	
	bitblt(x_offset, y_offset, font_w, font_h, font_mode, buffer+128);
	
	x_offset+=font_w+font_space;
	
//check x offset and do necessary wrapping
	
    if ((x_offset + font_w + font_space) > 127)
	{
		x_offset = x_offset % (font_w+font_space);	//this makes sure text on the next line will line up with the previous line
		if (y_offset > (64-font_h-font_h))
			y_offset = y_offset % font_h;	//this makes sure that the line restarted at the top will overlap the old one
		else 
			y_offset += font_h;
	}
}

//write_command_LCD
void write_command_LCD(char byte, char chip)
{
	//byte is the command to write, chip determines which chips get written to 1=1,2=2, anything else=both

	DDRB |= 0x03; //PB0 and PB1 are outs
	DDRD |= 0xFC; //PD2-PD7 are also outs.  Ports B and D are the data bus.
	
	if (chip==1) 
		PORTC = LCD_COMMAND1;
	else if (chip == 2)
		PORTC = LCD_COMMAND2;
	     else
		PORTC = LCD_COMMAND12;
 		
	set_data(byte);
	delay();

	CLK_LCD;
	PORTC = LCD_IDLE;	
	delay();

	DDRB &= 0xFC;	//PB0 and PB1 as inputs
	DDRD = 0;
}
 
//write_data_LCD
void write_data_LCD(char byte, char chip)
{
	//byte is the data to write, chip determines which chips get written to 1=1,2=2 anything else=both
	
	DDRB |= 0x03; //PB0 and PB1 are outs
	DDRD |= 0xFC; //PD2-PD7 are also outs.  Ports B and D are the data bus.	

	if (chip==1) 
		PORTC = LCD_DATA1;
	else if (chip == 2)
		PORTC = LCD_DATA2;
	     else
		PORTC = LCD_DATA12;

	set_data(byte);
	delay();

	CLK_LCD;
	PORTC = LCD_IDLE;	
	delay();
	
	DDRB &= 0xFC;	//PB0 and PB1 as inputs
	DDRD = 0;
}


//this writes a block of contiguous bytes to a single page
//automatically takes care of crossing from one chip to the next
void write_block(char x, char page, char length, char* buf)
{
	char side;
	char i, s, n1, n2, o;

	side=1;
	o=0;			//offset in data
	set_page(page);		//set proper page
	if (x>63)		//account for x being larger than 63 
	{
		x-= 64;
		side=2;
	}

	s = (64-x);		//distance to edge

	if (length>s)		
	{
		n1=s;		//how much to read in the first loop
		n2=length-s;	//how much to read in the second loop
	}
	else
	{
		n1=length;
		n2=0;
	}

	set_x(x);

	DDRB |= 0x03; //PB0 and PB1 are outs
	DDRD |= 0xFC; //PD2-PD7 are also outs.  Ports B and D are the data bus.	

	//write the data
	if  (side==1) 					//select proper chip
		PORTC = LCD_DATA1;	
	else
		PORTC = LCD_DATA2;


	for(i=0;i<n1;i++)
	{
		set_data(buf[o++]);			//put data on bus
		delay();
		CLK_LCD;				//clock it in
	}

	PORTC = LCD_IDLE;	
	delay();

	if(n2>0)
	{
		set_x(0);

		DDRB |= 0x03; //PB0 and PB1 are outs
		DDRD |= 0xFC; //PD2-PD7 are also outs.  Ports B and D are the data bus.	

		if  (side==1) 				//select proper chip(opposite of before, since we ran over)
			PORTC = LCD_DATA2;	
		else
			PORTC = LCD_DATA1;


		for(i=0;i<n2;i++)
		{
			set_data(buf[o++]);		//put data on bus
			delay();
			CLK_LCD;			//clock it in
		}
	
		PORTC = LCD_IDLE;	
		delay();

	}//if n2>0

	DDRB &= 0xFC;	//PB0 and PB1 as inputs
	DDRD = 0;	
}

//read_data_LCD
//this incorporates a dummy read, which is necessary for reading after changing the LCD command port, but messes up successive reads if set_x is not called
//for reading a block of continuous data, use read_block
char read_data_LCD(char chip)
{
	char data1,data2;
	//chip determines which chip to read from
		
	if  (chip==1) 				//select proper chip
		PORTC = LCD_READ1;	
	else
		PORTC = LCD_READ2;
	
	delay();				//dummy clk
	PORTC |= (1 << EN);
	delay();
	PORTC &= ~(1 << EN);
	delay();

	PORTC |= (1 << EN);
	delay();

	data1 = PINB;				//read data
	data1 &= 0x03;
	
	data2 = PIND;
	data2 &= 0xFC;
	
	data1 |= data2;
	
	PORTC &= ~(1 << EN);
	delay();

	PORTC = LCD_IDLE;	
	delay();
	return data1;
}

//reads [length] display bytes from page [page] starting at horizontal value [x] and puts the values in buf
//automatically accounts for crossing chips 
void read_block(char x, char page, char length, char* buf)
{
	char data1,data2,side;
	char c,s, n1, n2, o;

	side=1;
	o=0;				//offset in data
	set_page(page);			//set proper page
	if (x>63)			//account for x being larger than 63 
	{
		x-= 64;
		side=2;
	}

	s = (64-x);			//distance to edge

	if (length>s)		
	{
		n1=s;			//how much to read in the first loop
		n2=length-s;		//how much to read in the second loop
	}
	else
	{
		n1=length;
		n2=0;
	}

	set_x(x);			//set x	
				
	if  (side==1) 			//select proper chip
		PORTC = LCD_READ1;	
	else
		PORTC = LCD_READ2;

	delay();			//dummy clk
	PORTC |= (1 << EN);
	delay();
	PORTC &= ~(1 << EN);
	delay();

	for(c=0;c < n1;c++)		//loop for all the bytes	
		{
			PORTC |= (1 << EN);
			delay();

			data1 = PINB;	//read data
			data1 &= 0x03;
	
			data2 = PIND;
			data2 &= 0xFC;
	
			data1 |= data2;
			buf[o++] = data1;

			PORTC &= ~(1 << EN);
			delay();

		}

	PORTC = LCD_IDLE;	
	delay();	

	if (n2>0)
	{	
		set_x(0);			// we got here because we overran the previous side. always start @ 0
		if  (side==1) 			//select proper chip, note this is opposite of normal because this read catches overrun from previous side
			PORTC = LCD_READ2;	
		else
			PORTC = LCD_READ1;
		
		delay();			//dummy clk
		PORTC |= (1 << EN);
		delay();
		PORTC &= ~(1 << EN);
		delay();

		for(c=0;c < n2;c++)		//loop for all the bytes	
			{
				PORTC |= (1 << EN);
				delay();

				data1 = PINB;	//read data
				data1 &= 0x03;
	
				data2 = PIND;
				data2 &= 0xFC;
	
				data1 |= data2;
				buf[o++] = data1;

				PORTC &= ~(1 << EN);
				delay();

			}

		PORTC = LCD_IDLE;	
		delay();	
	}//if n2>0
}

//does a bit transfer from data to display memory
//if NULL is passed as data, bitblt assumes the data is to come from the serial port, and will take it from there
//bitblt will not return until it gets all the bytes it wants
//x,y is upper left corner of image in pixels
//bitblt counts coordinates in the standard fashion. ie (0,0) is upper left, +x it to the right +y is down
//width is width in pixels
//height is height in pixels
//mode determines how the bits in the image combine with the bits already present on the display 
//mode 0=AND, 1=NAND, 2=OR, 3=NOR, 4=XOR, 5=NXOR, 6=fill(used for block erase, data[0] sets fill byte), 7=copy(overwrites background)
void bitblt(char x, char y, char width, char height, char mode, char* data)
{
	unsigned char row, column, shift, shift2, mask1, mask2, n, n2, temp, RX_temp;
	int offset, offset2;

	//make sure legal dimensions
	if ((height<1) || (height >64))
	{
		for(row=0;row<height;row++)			//loop for all the data that's coming
			for(column=0;column<width;column++)
				RX_get_byte();			//read it, but do nothing
		return;
	}
	if ((width<1) || (width > 128))
	{
		for(row=0;row<height;row++)			//loop for all the data that's coming
			for(column=0;column<width;column++)
				RX_get_byte();			//read it, but do nothing
		return;	
	}
	shift = y % 8;				//calculate how much to shift the data bytes to line them up with the pages
	shift2 = 8 - shift;
	n = ((height-1+shift)/8)+1;		//number of pages(rows) the image occupies(need to loop through all of these, each gets pixels changed)
	n2 = height/8;				//number of rows in image
	if (height % 8 != 0)
		n2++;
	
	mask1 =	0xFF << shift;			//mask1 needs to have 0's for each pixel in the top row NOT occupied by new image data	
	mask2 = 0xFF >> (8-( (height+y) % 8) );	//mask2 needs to have 0's for each pixel in the bottom row NOT occupied by new image data
	
	if ( ((height+y) % 8) == 0)
		mask2=0xff;			//special case of exact fit in last row
		
		offset=0;			//start at the beginning of data
		offset2=-width;			//offset2 points a the previous row

	for(row = 0;row < n;row++)		//loop through all rows
	{
		//if NULL was passed for data, take it from the serial port. 
		//it is necessary to have 2 rows of data, current and previous to do bitblt
		//since 0<width<128 (display is only 128 wide), we can use the second 128 bytes in buffer to hold the previous row

		read_block(x,(row+(y/8)),width,buffer);				//read the row in(background image data)

			for(column=0;column<width;column++)			//loop for columns
			{
				if (data!=NULL)					//decide if data comes from ram or serial			
					temp = ( (data[offset++] << shift) | (data[offset2++] >> shift2) );	//data from ram
				else
				{	
					if (row<n2)					//only read the number of rows in the image
					{
						//we want bitblt to be fast, so inline RX_get_byte code to avoid the overhead of a function call
						while(RX_bytes==0);							//wait for data to be available
						RX_temp = RX_array[RX_read++];				//get char from buffer and increment read pointer
						if(RX_read >= RX_BUFFER_SIZE) RX_read = 0;	//if the read pointer reaches the end of the buffer, wrap back to the beginning
						cli();										//interrupts must be disabled when changing RX_bytes, since it can be chabged here and in the ISR
						RX_bytes--;
						sei();
						//check to see if we need to re-enable reception 

						if (RX_pause==1)		//if USART reception is suspended
							if (RX_bytes < (unsigned char)(RX_BUFFER_SIZE *.25) )		//if RX_buffer is suitably empty
							{
								put_char(XON);	//resume reception
								RX_pause=0;	//flag it
							}	
					}
					temp = ( (RX_temp << shift) | (buffer[128+column] >> shift2) );
					buffer[128+column] = RX_temp;					//store most recent byte for future use
				}
				
				if (mode==6)		//fill is a special case, just use data[0]
					temp = ( (data[0] << shift) | (data[0] >> shift2) );				

				if (row == 0)		//some special treatment for first and last rows
				{
					//if this is the first row, we need to mask off the blank pixels at the top of the row(these pix have random data)
					temp &= mask1; //mask1 has shift blank pixels starting from LSB (LSB is the top of the stripe) 
				}
				else if (row == (n-1))
				{
					//if this is the last row, we need to maks off the blank pixels at the bottom of the image
					temp &= mask2; //mask2 has blank pixels starting at MSB (MSB is the bottom of the stripe)
				}
								
				//combine image data with background
				switch(mode)	
				{
				 case 0:
				 case 1:
					buffer[column] &= temp;			//AND it with buffer
				 break;
				 case 2:
				 case 3:
					buffer[column] |= temp;			//OR it with buffer
				 break;
				 case 4:
				 case 5:
					buffer[column] ^= temp;			//XOR it with buffer
				 break;
				 case 6:	//for copy and fill, we don't want the background bits to interfere with the image, so we clear them
				 case 7:
				 default:
					
					if (row == 0)
						buffer[column] &= ~mask1;	//first row, clear bottom bits of the background
					else if (row == (n-1))
						buffer[column] &= ~mask2;	//last row, clear top bits of background
					else
						buffer[column] = 0;		//middle row, clear all of the background
					
					buffer[column] |= temp;			//OR it with buffer
				 break;
				}
			
				if ((mode % 2 == 1) && (mode < 6))
					buffer[column] = ~buffer[column];	//if we wanted an inverted operation, do it
			}//end column loop
		write_block(x,(row+(y/8)),width,buffer);	//write new row to display
	}//row loop
}

//mapping to Cartesian coordinates, (0,0) is in the lower left corner, (127,63) is in the upper right
void pixel(char S_R, char x, char y)
{
	static char temp_page, temp_side, temp_x = 0, temp_data1 = 0, temp_data2 = 0;
	
	//don't try to print something outside of our range
	if (x > 127) return;
	if (y > 63) return;
	
	y=63-y;	

	if (reverse == 1) S_R ^= 1;
	
	if (x >= 64) temp_side = 2, temp_x = x - 64;
	else temp_side = 1, temp_x = x;
		
	temp_page = 7 - (y >> 3);
		
	//data = (1 << (y - ((7 - temp_page) * 8)));
	temp_data1 = (1 << (7 - (y - ((7 - temp_page) * 8))));
		
	set_page(temp_page);
				
	//need to read the existing byte here, then or it with the new byte
	temp_data2 = read_byte(temp_x, temp_side);
		
	if (S_R == 0)
	{
		temp_data1 = ~temp_data1;
		temp_data1 &= temp_data2;
	}
	else temp_data1 |= temp_data2;
		
	set_x(temp_x);//reset this...
	write_data_LCD(temp_data1, temp_side);
}


//draws (S_R = 1) or erases (S_R = 0) a line from x0, y0 to x1, y1 using Bresenham's line algorithm.
void line(char S_R, char x0, char y0, char x1, char y1)
{
	
	unsigned char steep;
	unsigned char swp;

    steep = (abs(y1 - y0) > abs(x1 - x0));
    if (steep) 
	{
		swp=x0;
		x0=y0;
		y0=swp;
		swp=x1;
		x1=y1;
		y1=swp;
	}

    if (x0 > x1) 
	{
       		swp=x0;
		x0=x1;
		x1=swp;
		swp=y0;
		y0=y1;
		y1=swp;
	}

	int deltax = x1 - x0;
	int deltay = abs(y1 - y0);
	int error = deltax / 2;
	int ystep;
	int y = y0;
	int x;

        if (y0 < y1) ystep = 1; else ystep = -1;
        for (x = x0; x <= x1; ++x)
	{
            if (steep) pixel(S_R,y,x); else pixel(S_R,x,y);

            error = error - deltay;
		if (error < 0) 
		{
			y = y + ystep;
			error = error + deltax;
		}
        }
}


char read_byte(char byte, char side)
{
	set_x(byte);
	return read_data_LCD(side);
}


//draws (S_R = 1) or erases (S_R = 0) a circle at x, y with radius r, using midpoint circle algorithm
void circle(char S_R, int x0, int y0, int r)
{
	
	int f = 1 - r;
	int ddF_x = 1;
	int ddF_y = -2 * r;
	int x = 0;
	int y = r;
 
  pixel(S_R, x0, y0 + r);
  pixel(S_R, x0, y0 - r);
  pixel(S_R, x0 + r, y0);
  pixel(S_R, x0 - r, y0);
 
  while(x < y)
  {
    if(f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;    
    pixel(S_R, x0 + x, y0 + y);
    pixel(S_R, x0 - x, y0 + y);
    pixel(S_R, x0 + x, y0 - y);
    pixel(S_R, x0 - x, y0 - y);
    pixel(S_R, x0 + y, y0 + x);
    pixel(S_R, x0 - y, y0 + x);
    pixel(S_R, x0 + y, y0 - x);
    pixel(S_R, x0 - y, y0 - x);
  }
}


void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
/* Wait for completion of previous write */
while(EECR & (1<<EEPE))
;
/* Set up address and Data Registers */
EEAR = uiAddress;
EEDR = ucData;
/* Write logical one to EEMPE */
EECR |= (1<<EEMPE);
/* Start eeprom write by setting EEPE */
EECR |= (1<<EEPE);
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
/* Wait for completion of previous write */
while(EECR & (1<<EEPE))
;
/* Set up address register */
EEAR = uiAddress;
/* Start eeprom read by writing EERE */
EECR |= (1<<EERE);
/* Return data from Data Register */
return EEDR;
}

void set_backlight(unsigned char dutycycle)
{
	//Set up Timer 0
	TCCR0A = 0x02;//CTC mode
	//TCCR0B = 0x02;
	//TIMSK0 = 0x02;//enable OCR0A
	//OCR0B = 255 - BL_dutycycle;
	
	//OCR0A = 255 - (100 - BL_dutycycle);
	//OCR0A = dutycycle;
	
	//SREG |= 0x80;
	
	
	if(BL_dutycycle >= 100)
	{
		TCCR0B = 0;
		TIMSK0 = 0;//disable timer ints
		
		//Backlight on
		PORTB &= (~(1<<BL_EN));
	}
	else if (BL_dutycycle == 0)
	{
		TCCR0B = 0;
		TIMSK0 = 0;//disable timer ints
		
		//Backlight off
		PORTB |= (1<<BL_EN);
	}
	
	
	else
	{
		TCCR0B = 0;
		TIMSK0 = 0;//disable timer ints
		
		OCR0A = 100 - BL_dutycycle;
		
		TIMSK0 = 0x02;//enable match on A
		TCCR0B = 0x02;
		
		SREG |= 0x80;
		
	}

}


//Deletes a full character space previous to the current location (backspace)
void del_char()
{
	char f;

	if (x_offset <= font_w)					//if previous char wouldn't have fit
	{			
		x_offset = (128 - (font_w+1) - ((128-x_offset) % (font_w+1)) );			
				
		if (y_offset < font_h) 	
			y_offset = (64 - font_h - ((64-y_offset) % font_h) );	//if we run off the top of the screen
		else
			y_offset-=font_h;
	}
	
	else x_offset -= (font_w+1);					// back x_offset up by the font width + 1 pixel space btwn characters
	
	f=0;
	if (reverse==1)
		f=0xff;	

	draw_block(x_offset, y_offset, x_offset+font_w, y_offset+font_h-1,f);	//erase the block
}


//draws a block on the screen. Block is described
//by a diagonal line from x, y1 to x2, y2
//block is filled with byte data (describes a vertical row of 8 pixels, use 0x00 to clear the block, 0xFF to fill it, etc)
void draw_block(char x1, char y1, char x2, char y2, char data)
{
	static signed int width = 0, height = 0,x,y;
	char n;
	n=data;
	if (x1>x2)
	{
		width=x1-x2;
		x=x2;
	}
	else
	{
		width=x2-x1;
		x=x1;
	}
	if (y1>y2)
	{
		height=y1-y2;
		y=y2;
	}	
	else
	{
		height=y2-y1;
		y=y1;
	}
	
	bitblt(x, y, width, height, 6, &n);	//use erase mode of bitblt

}

//draws a box. The box is described

//by a diagonal line from x, y1 to x2, y2
void box(char x1, char y1, char x2, char y2, char mode)
{
	
	if (mode==1)
	{
		line(1, x2, y2, x2, y1);
		line(1, x1, y2, x2, y2);
		line(1, x1, y2, x1, y1);
		line(1, x1, y1, x2, y1);
	}
	else
	{
		line(0, x2, y2, x2, y1);
		line(0, x1, y2, x2, y2);
		line(0, x1, y2, x1, y1);
		line(0, x1, y1, x2, y1);
	}
}

void draw_sprite(char x, char y, char n, char mode)	//draws nth sprite at (x,y) using mode
{
	uint16_t o;
	o=n*SPRITE_SIZE;				//offset to sprite
	bitblt(x, y, sprite[o], sprite[o+1], mode, sprite + 2 + o);
}

void set_baud(char b)
{
	if (b == 1) USART_Init( 1000000/2400-1);//4800
	else if (b == 2) USART_Init( 1000000/4800-1);//9600
	else if (b == 3) USART_Init( 1000000/9600-1);//19200
	else if (b == 4) USART_Init( 1000000/19200-1);//38400
	else if (b == 5) USART_Init( 1000000/28800-1);//57600
	else if (b == 6) USART_Init( 1000000/57600-1);//115200
	
}

void LCD_reset(void)
{
	PORTC = 0x00;
	delay_ms(60);
	PORTC = LCD_IDLE;
	
	clear_screen(0);
	set_page(0);
			
	set_x(0);
			
	display_on();
	
	//set display start line to 0, command 0xC0
	write_command_LCD(0xC0,0);
	
	x_offset = 0;

	set_page(0);
}
