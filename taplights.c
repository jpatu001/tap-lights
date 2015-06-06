// PLAYER 1 Fully Works with correct timing
// Life Implemented, game ends when life==0;
// EEPROM WORKS.. I think
// Soft Reset button added

#include <avr/io.h>
#include "C:\Users\Paulo\Documents\Atmel Studio\io.c"
#include "C:\Users\Paulo\Documents\Atmel Studio\io.h"
#include "C:\Users\Paulo\Documents\Atmel Studio\timer.h"
#include "C:\Users\Paulo\Documents\Atmel Studio\scheduler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h>

// GLOBAL VARIABLES
unsigned char LED_counter;
unsigned char menu_counter;
unsigned char curr1;
unsigned char curr2;
unsigned char curr3;
unsigned char seed = 0; //Time
unsigned char r1; // Row 1 LED bank
unsigned char r2; // Row 2 LED bank
unsigned char r3; // Row 3 LED bank
unsigned char rowLED = 0; // Current Row to light an LED at
unsigned long int highScore = 20;

unsigned char gameOver = 0;
unsigned char onePlayer = 0;

// 1Player Variables
unsigned char req1 = 0;
unsigned char ack1 = 0;
unsigned long int player1Score;
unsigned char player1Life = 0;
unsigned char prev1;
unsigned char prev2;
unsigned char prev3;
unsigned char button1;
unsigned char button2;
unsigned char button3;

void transmit_data1(unsigned char data);
void transmit_data2(unsigned char data);
void transmit_data3(unsigned char data);

/*		//FOR BUTTON PRESS
		if(!(PINB & 0x80) //"ROW1"
		if(!(PINB & 0x40)) //"ROW2"
		if(!(PINB & 0x20) //"ROW3"
*/

enum menu_states {m_init, m_wait, m1p, m1p_game, m2p, m2p_game, mhs, game_over}menu_state;
void Menu_Tick();

enum LED_GEN_states {lg_init, outLED, lg_wait} lg_state;
void LED_GEN_Tick();

enum ONE_PLAYER_states {p1_init, p1_wait, game, p1_wait2, held} p1_state;
void ONE_PLAYER_Tick();

uint8_t memVal;

int main(void)
{
	//eeprom_write_byte(( uint8_t *) 46, 0x01); // Sets Default High Score to 1;
	memVal = eeprom_read_byte (( uint8_t *) 46);
	highScore = memVal;
	
	//INITITALIZATIONS
	// PORTS
	DDRA = 0x0F; PORTA = 0xF0; // Shifters
	DDRB = 0x0F; PORTB = 0xF0; // Shifters && 4 Buttons
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xCF; PORTD = 0x70; // LCD control lines
	seed = highScore;
	srand(seed);
	// VARIABLES
	r1 = 0x00;	r2  = 0x00;	r3  = 0x00;	//ROWS
	LED_counter = 0;
	menu_counter = 2;
	TimerSet(100);
	TimerOn();
	LCD_init();
	LCD_ClearScreen();
	
	lg_state = lg_init;
	p1_state = p1_init;
	menu_state = m_init;
	
	while(1)
	{	
		if(!(PINB & 0x10)) // Soft Reset
		{
			memVal = eeprom_read_byte (( uint8_t *) 46);
			highScore = memVal;
			
			//INITITALIZATIONS
			// PORTS
			DDRA = 0x0F; PORTA = 0xF0; // Shifters
			DDRB = 0x0F; PORTB = 0xF0; // Shifters && 4 Buttons
			DDRC = 0xFF; PORTC = 0x00; // LCD data lines
			DDRD = 0xCF; PORTD = 0x70; // LCD control lines
			seed = highScore;
			srand(seed);
			// VARIABLES
			r1 = 0x00;	r2  = 0x00;	r3  = 0x00;	//ROWS
			LED_counter = 0;
			menu_counter = 2;
			lg_state = lg_init;
			p1_state = p1_init;
			menu_state = m_init;
		}
		if(menu_counter==2) Menu_Tick();
		LED_GEN_Tick();
		ONE_PLAYER_Tick();
		if(LED_counter==3) LED_counter = 0; // Reset Counter
		if(menu_counter==2) menu_counter = 0; // Reset Menu Time
		LED_counter++;
		menu_counter++;
		while(!TimerFlag);
		TimerFlag = 0;
	}
	
	
	return 0;
}

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
		r1 = 0x00;
		r2 = 0x00;
		r3 = 0x00;
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
			if(player1Score > highScore){
				highScore = memVal = player1Score;
				eeprom_update_byte (( uint8_t *) 46 , memVal);
			}
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
		LCD_DisplayString(1, "Shitty Game v1.3");
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
		if( !(PINB & 0x80) && !(PINB & 0x40)  && !(PINB & 0x20) )
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

void ONE_PLAYER_Tick()
{
	switch(p1_state) // Transitions
	{
		case p1_init:
		player1Life = 5;
		prev1 = 0x00; prev2 = 0x00; prev3 = 0x00;
		button1 = 0x00; button2 = 0x00; button3 = 0x00;
		p1_state = p1_wait;
		break;
		
		case p1_wait:
		if(req1){
			LCD_DisplayString(1, "Life: ");
			LCD_WriteData(player1Life + '0');
			p1_state = game;
		}
		else{}
		break;
		
		case game:
		if(player1Life == 0)
		{
			ack1 = 1;
			p1_state = p1_wait2;
		}
		
		if((curr1 && !(PINB & 0x80))){
			player1Score++;
			p1_state = held;
			button1++;
		}
		if((curr2 && !(PINB & 0x40))){
			player1Score++;
			p1_state = held;
			button2++;
		}
		if((curr3 && !(PINB & 0x20))){
			player1Score++;
			p1_state = held;
			button3++;
		}
		if(!(PINB & 0x20) && !(PINB & 0x40) && !(PINB & 0x80))
		{
			ack1 = 1;
			p1_state = p1_wait2;
		}
		// Missed buttons
		
		if(!(curr1 & 0x01) && (prev1 & 0x01) && (PINB & 0x80) && (button1==0) && LED_counter>=3){ //LCD_DisplayString(1, "Missed row 1!");
			LCD_DisplayString(1, "Life: ");
			if(player1Life > 0) player1Life--;
			LCD_WriteData('0' + player1Life);
		}
		
		if(!(curr2 & 0x01) && (prev2 & 0x01) && (PINB & 0x40) && (button2==0) && LED_counter>=3){ //LCD_DisplayString(1, "Missed row 2!");
			LCD_DisplayString(1, "Life: ");
			if(player1Life > 0) player1Life--;
			LCD_WriteData('0' + player1Life);
		}
		if(!(curr3 & 0x01) && (prev3 & 0x01) && (PINB & 0x20) && (button3==0) && LED_counter>=3){ //LCD_DisplayString(1, "Missed row 3!");
			LCD_DisplayString(1, "Life: ");
			if(player1Life > 0) player1Life--;
			LCD_WriteData('0' + player1Life);
		}
		
		if(prev1!=curr1 || prev2!=curr2 || prev3!=curr3) // Reset button if it changed
		{
			button1 = 0x00;
			button2 = 0x00;
			button3 = 0x00;
		}
		prev1 = curr1; prev2 = curr2; prev3 = curr3;
		break;

		case p1_wait2:
		if (req1==0)
		{
			ack1 = 0;
			p1_state = p1_init;
		}
		break;
		
		case held:
		if(!(PINB & 0x20) || !(PINB & 0x40) || !(PINB & 0x80)) p1_state = held;
		if(!(PINB & 0x20) && !(PINB & 0x40) && !(PINB & 0x80))
		{
			ack1 = 1;
			p1_state = p1_wait2;
		}
		if((PINB & 0x20) && (PINB & 0x40) && (PINB & 0x80)) p1_state = game;
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
		
		case held:
		break;
	}
}

void LED_GEN_Tick()
{
	switch(lg_state)
	{
		case lg_init:
		lg_state = lg_wait;
		break;
		
		case lg_wait:
		if (req1 && LED_counter==3)
		{
			lg_state = outLED;
		}
		break;
		
		case outLED:
		if(ack1==0 && req1==0)
		{
			lg_state = lg_init;
		}
		lg_state = lg_wait;
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
	unsigned char tmp = PINB & 0xF0;
	tmp = tmp | 0x04;
	for (i = 0; i < 9 ; ++i) {
		PORTB = tmp; // PORTB = 0x04;
		PORTB |= ((data >> i) & 0x01);
		PORTB |= 0x02;
	}
	tmp = tmp & 0xF0;
	PORTB = tmp;
}

void transmit_data3(unsigned char data) {
	int i;
	for (i = 0; i < 9 ; ++i) {
		PORTD = 0x04;
		PORTD |= ((data >> i) & 0x01);
		PORTD |= 0x02;
	}
	PORTD = 0x00;
}
