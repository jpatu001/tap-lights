#include <avr/io.h>
#include "C:\Users\Paulo\Documents\Atmel Studio\io.c"
#include "C:\Users\Paulo\Documents\Atmel Studio\io.h"
#include "C:\Users\Paulo\Documents\Atmel Studio\timer.h"
#include "C:\Users\Paulo\Documents\Atmel Studio\scheduler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// GLOBAL VARIABLES
unsigned char curr1;
unsigned char curr2;
unsigned char curr3;
unsigned char seed = 0; //Time
unsigned char r1; // Row 1 LED bank
unsigned char r2; // Row 2 LED bank
unsigned char r3; // Row 3 LED bank
unsigned char rowLED = 0; // Current Row to light an LED at
unsigned long int highScore = 100;

unsigned char gameOver = 0;
unsigned char onePlayer = 0;

unsigned char req1 = 0;
unsigned char ack1 = 0;
unsigned long int player1Score;


void transmit_data1(unsigned char data);
void transmit_data2(unsigned char data);
void transmit_data3(unsigned char data);

/*		//FOR BUTTON PRESS
		if(!(PINB & 0x80) //"ROW1"
		if(!(PINB & 0x40)) //"ROW2"
		if(!(PINB & 0x20) //"ROW3"
*/

enum menu_states {m_init, m_wait, m1p, m1p_game, m2p, m2p_game, mhs, game_over}menu_state;
void Menu_Tick()
{
	switch(menu_state) // Actions
	{
		case m_init:
		rowLED = 0;
		req1 = 0;
		ack1 = 0;
		gameOver = 0;
		onePlayer = 0;
		player1Score = 0;
		transmit_data1(0x00);
		transmit_data2(0x00);
		transmit_data3(0x00);
		break;
		
		case m_wait:
		break;
		
		case m1p:
		break;
		
		case m1p_game:
		break;
		
		case m2p:
		break;
		
		case m2p_game:
		break;
		
		case mhs:
		break;
		
		case game_over:
		if(onePlayer)
		{
			if(player1Score > highScore) highScore = player1Score;
			char HS[20] = "Score: ";
			char temp[10];
			itoa(player1Score, temp, 10);
			strcat(HS, temp);
			LCD_DisplayString(1, HS);
		}
		break;
	}
	switch(menu_state) // Transitions
	{
		case m_init:
		menu_state = m_wait;
		LCD_DisplayString(1, "SHITTY GAME v1.0");
		break;
		
		case m_wait:
		if(!(PINB & 0x80)){
			char HS[20] = "High Score: ";
			char temp[10];
			itoa(highScore, temp, 10);
			strcat(HS, temp);
			LCD_DisplayString(1, HS);
			menu_state = mhs;
		}
		if(!(PINB & 0x20)){
			LCD_DisplayString(1, "1 Player Mode");
			menu_state = m1p;
		}
		else menu_state = m_wait;
		break;
		
		case mhs:
		if(!(PINB & 0x80)) //"ROW1"
		{
			LCD_DisplayString(1, "2 Player Mode");
			menu_state = m2p;
		}
		if(!(PINB & 0x20)) //"ROW3"
		{
			LCD_DisplayString(1, "1 Player Mode");
			menu_state = m1p;
		}
		break;
		
		case m1p:
		if(!(PINB & 0x80)) //"ROW1"
		{
			char HS[20] = "High Score: ";
			char temp[10];
			itoa(highScore, temp, 10);
			strcat(HS, temp);
			LCD_DisplayString(1, HS);
			menu_state = mhs;
		}
		if(!(PINB & 0x20)) //"ROW3"
		{
			LCD_DisplayString(1, "2 Player Mode");
			menu_state = m2p;
		}
		if(!(PINB & 0x40))
		{
			LCD_DisplayString(1, "Game!");
			req1 = 1;
			onePlayer = 1;
			menu_state = m1p_game;
		}
		else {}
		break;
		
		case m1p_game:
		if( !(PINB & 0x80) && !(PINB & 0x40)  && !(PINB & 0x20) )
		{
			LCD_DisplayString(1, "Quitting");
			req1 = 0;
			menu_state = game_over;
		}
		if(ack1 == 1)
		{
			menu_state = game_over;
		}
		else{}
		break;
		
		case m2p:
		if(!(PINB & 0x80)) //"ROW1"
		{
			LCD_DisplayString(1, "1 Player Mode");
			menu_state = m1p;
		}
		if(!(PINB & 0x20)) //"ROW3"
		{
			char HS[20] = "High Score: ";
			char temp[10];
			itoa(highScore, temp, 10);
			strcat(HS, temp);
			LCD_DisplayString(1, HS);
			menu_state = mhs;
		}
		// M2PGAME SUPP
		else {}
		break;
		
		case m2p_game:
		break;
		
		case game_over:
		if( !(PINB & 0x80) || !(PINB & 0x40)  || !(PINB & 0x20) )
		{
			req1 = 0;
			menu_state = m_init;
		}
		break;
		
		default:
		menu_state = m_init;
		break;
		
	}
}
enum LED_GEN_states {lg_init, outLED, lg_wait} lg_state;
void LED_GEN_Tick();

enum ONE_PLAYER_states {p1_init, p1_wait, game, p1_wait2} p1_state;
void ONE_PLAYER_Tick()
{	
	switch(p1_state) // Transitions
	{
		case p1_init:
		p1_state = p1_wait;
		break;
		
		case p1_wait:
		if(req1) p1_state = game;
		else{}
		break;
		
		case game:
		//if((curr1 && (PINB & 0x80)) || (curr2 && (PINB & 0x40)) || (curr3 && (PINB & 0x20))) LCD_DisplayString(1, "Missed it Bitch!");
		//if((curr1 && (PINB & 0x80))) //LCD_DisplayString(1, "Missed row 1!");
		//if((curr2 && (PINB & 0x40))) //LCD_DisplayString(1, "Missed row 2!");
		//if((curr3 && (PINB & 0x20))) //LCD_DisplayString(1, "Missed row 3!");
		
		if((curr1 && !(PINB & 0x80))){
			LCD_DisplayString(1, "Got(Row1)!");
			player1Score++;
		}
		if((curr2 && !(PINB & 0x40))){
			LCD_DisplayString(1, "Got(Row2)!");
			player1Score++;
		}
		if((curr3 && !(PINB & 0x20))){
			LCD_DisplayString(1, "Got(Row3)!");
			player1Score++;
		}
		if(!(PINB & 0x20) && !(PINB & 0x40) && !(PINB & 0x80))
		{
			ack1 = 1;
			p1_state = p1_wait2;
		}

		case p1_wait2:
		if (req1==0)
		{
			ack1 = 0;
			p1_state = p1_init;
		}
		break;
		
		default:
		p1_state = p1_init;
	}
	
	switch(p1_state) // Actions
	{
		case p1_init:
		break;
		
		case p1_wait:
		break;
		
		case game:
		break;
		
		case p1_wait2:
		break;
	}
}

int main(void)
{
	//INITITALIZATIONS
	// PORTS
	DDRA = 0xFF; PORTA = 0x00; // Shifters
	DDRB = 0x0F; PORTB = 0xF0; // Shifters && 3 Buttons
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xCF; PORTD = 0x70; // LCD control lines
	srand(seed);
	// VARIABLES
	r1 = 0x00;	r2  = 0x00;	r3  = 0x00;	//ROWS

	TimerOn();
	TimerSet(300);
	LCD_init();
	LCD_ClearScreen();
	
	lg_state = lg_init;
	menu_state = m_init;
	
	while(1)
	{	// Scheduler code
		Menu_Tick();
		LED_GEN_Tick();
		ONE_PLAYER_Tick();
		
		while(!TimerFlag);
		TimerFlag = 0;
	}
	
	
	return 0;
}

/*		//FOR BUTTON PRESS
		if((curr1 & 0x01) && !(PINB & 0x80)) LCD_DisplayString(1, "ROW1");
		if((curr2 & 0x01) && !(PINB & 0x40)) LCD_DisplayString(1, "ROW2");
		if((curr3 & 0x01) && !(PINB & 0x20)) LCD_DisplayString(1, "ROW3");
*/

void LED_GEN_Tick()
{
	switch(lg_state)
	{
		case lg_init:
		lg_state = lg_wait;
		break;
		
		case lg_wait:
		if (req1)
		{
			lg_state = outLED;
		}
		break;
		
		case outLED:
		if(ack1==0 && req1==0)
		{
			lg_state = lg_init;
		}
		break;
		
		default:
		lg_state = lg_init;
		break;
	}
	switch(lg_state)
	{
		case lg_init:
		curr1 = 0x00;	curr2 = 0x00;	curr3 = 0x00; //CURRENT
		rowLED = 0;
		transmit_data1(r1);	transmit_data2(r2);	transmit_data3(r3);
		break;
		
		case lg_wait:
		break;
		
		case outLED:
		curr1 = (r1 & 0x01);	curr2 = (r2 & 0x01);	curr3 = (r3 &0x01); //Get Current end of register
		rowLED = rand() % 10;	// RANDOMIZER LED
		if(rowLED == 0)	r1 |= 0x80;
		if(rowLED == 2)	r2 |= 0x80;
		if(rowLED == 3)	r3 |= 0x80;
		else {}
		transmit_data1(r1);	transmit_data2(r2);	transmit_data3(r3);			// Transmit to Shift register
		r1 = r1 >> 1;	r2 = r2 >> 1;	r3 = r3 >> 1;						// SHIFT DATA
		break;
	}
}

void transmit_data1(unsigned char data) {
	int i;
	for (i = 0; i < 9 ; ++i) {
		PORTA = 0x04;
		PORTA |= ((data >> i) & 0x01);
		PORTA |= 0x02;
	}
	PORTA = 0x00;
}

void transmit_data2(unsigned char data) {
	int i;
	for (i = 0; i < 9 ; ++i) {
		PORTB = 0x04;
		PORTB |= ((data >> i) & 0x01);
		PORTB |= 0x02;
	}
	PORTB = 0x00;
}

void transmit_data3(unsigned char data) {
	int i;
	for (i = 0; i < 9 ; ++i) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTD = 0x04;
		// set SER = next bit of data to be sent.
		PORTD |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTD |= 0x02;
	}
	// se  RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
	//PORTD |= 0x02;
	// clears all lines in preparation of a new transmission
	PORTD = 0x00;
}
