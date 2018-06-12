#include <avr/io.h>
#include <util/delay.h>
#include <timer.h>
#include <avr/eeprom.h>
#include "io.c"

//--------------------------------------
// LED Matrix Demo SynchSM
// Period: 100 ms
//--------------------------------------

#define SHIFT_REGISTER DDRB
#define SHIFT_PORT PORTB
#define DATA (1<<PB5)		//MOSI (SI)
#define LATCH (1<<PB4)		//SS   (RCK)
#define CLOCK (1<<PB7)		//SCK  (SCK)

enum Light_Tick {shift, setVars, checkPressed, updateLEDs} current;

unsigned char pattern = 0x00; // LED pattern - 0: LED off; 1: LED on
unsigned char row = 0x00; // Row(s) displaying pattern - 0: display pattern on row; 1: do NOT display pattern on row
unsigned char pattern1 = 0x00; // LED pattern - 0: LED off; 1: LED on
unsigned char row1 = 0xFE; // Row(s) displaying pattern - 0: display pattern on row; 1: do NOT display pattern on row
unsigned char pattern2 = 0x00; // LED pattern - 0: LED off; 1: LED on
unsigned char row2 = 0xFD; // Row(s) displaying pattern - 0: display pattern on row; 1: do NOT display pattern on row
unsigned char pattern3 = 0x00; // LED pattern - 0: LED off; 1: LED on
unsigned char row3 = 0xFB; // Row(s) displaying pattern - 0: display pattern on row; 1: do NOT display pattern on row
unsigned char pattern4 = 0x00; // LED pattern - 0: LED off; 1: LED on
unsigned char row4 = 0xF7; // Row(s) displaying pattern - 0: display pattern on row; 1: do NOT display pattern on row
unsigned char pattern5 = 0x00; // LED pattern - 0: LED off; 1: LED on
unsigned char row5 = 0xEF; // Row(s) displaying pattern - 0: display pattern on row; 1: do NOT display pattern on row

unsigned short light_count = 0;
unsigned short slowness = 200; //200
unsigned short counter = 200;
unsigned char mode = 0;
unsigned char reset = 1;

char did1 = 0;
char did2 = 0;
char did3 = 0;
char did4 = 0;
char did5 = 0;
char done1 = 0;
char done2 = 0;
char done3 = 0;
char done4 = 0;
char done5 = 0;

char secondPlayerPressed1 = 0;
char secondPlayerPressed2 = 0;
char secondPlayerPressed3 = 0;
char secondPlayerPressed4 = 0;
char secondPlayerPressed5 = 0;

char firstPlayerPressed1 = 0;
char firstPlayerPressed2 = 0;
char firstPlayerPressed3 = 0;
char firstPlayerPressed4 = 0;
char firstPlayerPressed5 = 0;

char sequence[225];
char time[225];

unsigned char size = 5;

unsigned char LCD_state = 1;

unsigned short score = 0;

uint16_t WordOfData = 0x00;

unsigned char tempA, tempB, tempC;

void init_IO(void){
	//Setup IO
	SHIFT_REGISTER |= (DATA | LATCH | CLOCK);	//Set control pins as outputs
	SHIFT_PORT &= ~(DATA | LATCH | CLOCK);		//Set control pins low
}

void init_SPI(void){
	//Setup SPI
	SPCR = (1<<SPE) | (1<<MSTR);	//Start SPI as Master
}

void spi_send(unsigned char byte){
	SPDR = byte;			//Shift in some data
	while(!(SPSR & (1<<SPIF)));	//Wait for SPI process to finish
}

int Light_Tick(int state) {
	spi_send(row); // Row(s) to display
	spi_send(pattern); // Pattern displaying pattern
	
	//Toggle latch to copy data to the storage register
	SHIFT_PORT |= LATCH;
	SHIFT_PORT &= ~LATCH;
	return state;
}

int Clear_Matrix(int state) {
	spi_send(0); // Row(s) to display
	spi_send(0); // Pattern displaying pattern
	
	//Toggle latch to copy data to the storage register
	SHIFT_PORT |= LATCH;
	SHIFT_PORT &= ~LATCH;
	return state;
}

void Tick_First(){
	// Transitions
	switch (current) {
		case shift:
		break;
		case checkPressed:
		break;
		case updateLEDs:
		break;
		default:
		current = shift;
		break;

	}
	// Actions
	switch (current) {
		case shift:
			pattern = pattern1;
			row = row1;
			if(counter >= slowness){
				if(pattern == 0 || did1 == 1){
					pattern = pattern | 0x01;
					did1 = 0;
				}
				else{
					pattern <<= 1;
				}
				pattern1 = pattern;
				if((pattern1 & 0x80) == 0x80){
					if((~PINA & 0x20) == 0x20){
						firstPlayerPressed1 = 1;
					}
				}
			}
			if((pattern & 0x80) == 0x80){
				if((~PINA & 0x20) == 0x20){
					firstPlayerPressed1 = 1;
				}
			}
			Light_Tick(current);
		break;
		default:
		break;
	}
}
void Tick_Second(){
	// Transitions
	switch (current) {
		case shift:
		break;
		default:
		current = shift;
		break;

	}
	// Actions
	switch (current) {
		case shift:
		pattern = pattern2;
		row = row2;
		if(counter >= slowness){
			if(pattern == 0 || did2 == 1){
				pattern = pattern | 0x01;
				did2 = 0;
			}
			else{
				pattern <<= 1;
			}
			pattern2 = pattern;
			if((pattern2 & 0x80) == 0x80){
				if((~PINA & 0x40) == 0x40){
					firstPlayerPressed2 = 1;
				}
			}
		}
		if((pattern & 0x80) == 0x80){
			if((~PINA & 0x40) == 0x40){
				firstPlayerPressed2 = 1;
			}
		}
		Light_Tick(current);
		break;
		default:
		break;
	}
}
void Tick_Third(){
	pattern = pattern3;
	row = row3;
	// Transitions
	switch (current) {
		case shift:
		break;
		default:
		current = shift;
		break;

	}
	// Actions
	switch (current) {
		case shift:
		if(counter >= slowness){
			if(pattern == 0 || did3 == 1){
				pattern = pattern | 0x01;
				did3 = 0;
			}
			else{
				pattern <<= 1;
			}
			pattern3 = pattern;
			if((pattern3 & 0x80) == 0x80){
				if((~PINA & 0x80) == 0x80){
					firstPlayerPressed3 = 1;
				}
			}
		}
		if((pattern & 0x80) == 0x80){
			if((~PINA & 0x80) == 0x80){
				firstPlayerPressed3 = 1;
			}
		}
		Light_Tick(current);
		break;
		default:
		break;
	}
}
void Tick_Fourth(){
	pattern = pattern4;
	row = row4;
	// Transitions
	switch (current) {
		case shift:
		break;
		default:
		current = shift;
		break;

	}
	// Actions
	switch (current) {
		case shift:
		if(counter >= slowness){
			if(pattern == 0 || did4 == 1){
				pattern = pattern | 0x01;
				did4= 0;
			}
			else{
				pattern <<= 1;
			}
			pattern4 = pattern;
			if((pattern & 0x80) == 0x80){
				if((~PINC & 0x40) == 0x40){
					firstPlayerPressed4 = 1;
				}
			}
		}
		if((pattern4 & 0x80) == 0x80){
			if((~PINC & 0x40) == 0x40){
				firstPlayerPressed4 = 1;
			}
		}
		Light_Tick(current);
		break;
		default:
		break;
	}
}
void Tick_Fifth(){
	pattern = pattern5;
	row = row5;
	// Transitions
	switch (current) {
		case shift:
		break;
		default:
		current = shift;
		break;

	}
	// Actions
	switch (current) {
		case shift:
		if(counter >= slowness){
			if(pattern == 0 || did5 == 1){
				pattern = pattern | 0x01;
				did5 = 0;
			}
			else{
				pattern <<= 1;
			}
			pattern5 = pattern;
			if((pattern5 & 0x80) == 0x80){
				if((~PINC & 0x80) == 0x80){
					firstPlayerPressed5 = 1;
				}
			}
		}
		if((pattern & 0x80) == 0x80){
			if((~PINC & 0x80) == 0x80){
				firstPlayerPressed5 = 1;
			}
		}
		Light_Tick(current);
		break;
		default:
		break;
	}
}

void updateScore(){
	if(firstPlayerPressed1 == 1){
		score += 1;
		firstPlayerPressed1 = 0;
	}
	if(firstPlayerPressed2 == 1){
		score += 1;
		firstPlayerPressed2 = 0;
	}
	if(firstPlayerPressed3 == 1){
		score += 1;
		firstPlayerPressed3 = 0;
	}
	if(firstPlayerPressed4 == 1){
		score += 1;
		firstPlayerPressed4 = 0;
	}
	if(firstPlayerPressed5 == 1){
		score += 1;
		firstPlayerPressed5 = 0;
	}
}

void updateLights(){
	switch (current) {
		case shift:
		break;
		case setVars:
		break;
		default:
		current = shift;
		break;

	}
	// Actions
	switch (current) {
		case shift:
			counter++;
			
			for(int c = 0; c < size; c++){
				if(time[c] == light_count){
					if(sequence[c] == 1){
						Tick_First();
						_delay_ms(1);
					}
					if(sequence[c] == 2){
						Tick_Second();
						_delay_ms(1);
					}
					if(sequence[c] == 3){
						Tick_Third();
						_delay_ms(1);
					}
					if(sequence[c] == 4){
						Tick_Fourth();
						_delay_ms(1);
					}
					if(sequence[c] == 5){
						Tick_Fifth();
						_delay_ms(1);
					}
				}
				
			if(time[c] == light_count - 1 || time[c] == light_count - 2 || time[c] == light_count - 3 || time[c] == light_count - 4 || time[c] == light_count - 5 || time[c] == light_count - 6 || time[c] == light_count - 7 || time[c] == light_count - 8){
				if(sequence[c] == 1){
					if(did1 == 1){
						did1 = 0;
					}
					if(done1 != 1){
						Tick_First();
						_delay_ms(1);
					}
					did1 = 1;
					done1 = 1;
				}
				if(sequence[c] == 2){
					if(did2 == 1){
						did2 = 0;
					}
					if(done2 != 1){
						Tick_Second();
						_delay_ms(1);
					}
					did2 = 1;
					done2 = 1;
				}
				if(sequence[c] == 3){
					if(did3 == 1){
						did3 = 0;
					}
					if(done3 != 1){
						Tick_Third();
						_delay_ms(1);
					}
					did3 = 1;
					done3 = 1;
				}
				if(sequence[c] == 4){
					if(did4 == 1){
						did4 = 0;
					}
					if(done4 != 1){
						Tick_Fourth();
						_delay_ms(1);
					}
					did4 = 1;
					done4 = 1;
				}
				if(sequence[c] == 5){
					if(did5 == 1){
						did5 = 0;
					}
					if(done5 != 1){
						Tick_Fifth();
						_delay_ms(1);
					}
					did5 = 1;
					done5 = 1;
				}
			}
		}
		current = setVars;
		break;
		
		case setVars:
			done1 = 0;
			done2 = 0;
			done3 = 0;
			done4 = 0;
			done5 = 0;
		
			if(counter >= slowness){
				counter = 0;
				slowness -= 10;
				if(slowness <= 25){
					slowness = 25;
				}
				light_count++;
				updateScore();
			}
			current = shift;
			break;
		default:
		break;
	}
}

void InitGame(){
	Clear_Matrix(current);
	if(LCD_state == 1){
		LCD_ClearScreen();
		LCD_DisplayString(1, "   Welcome to    Keyboard Hero!");
		LCD_state++;
	}
	delay_ms(200);
	if(LCD_state == 2){
		tempC = ~PINC & 0xC0;
		tempA = ~PINA & 0xE0;
		LCD_ClearScreen();
		LCD_DisplayString(1, " Would you like to play 1 or 2P?");
		while(tempA != 0x20 && tempA != 0x40){
			tempC = ~PINC & 0xC0;
			tempA = ~PINA & 0xE0;
		}
		if(tempA == 0x20){
			LCD_ClearScreen();
			LCD_DisplayString(1, " 1 Player Mode!");
			mode = 1;
		}
		else if(tempA == 0x40){
			LCD_ClearScreen();
			LCD_DisplayString(1, " 2 Player Mode!");
			mode = 2;
		}
		delay_ms(100);
		LCD_ClearScreen();
		LCD_DisplayString(1, "     READY");
		delay_ms(200);
		LCD_ClearScreen();
		LCD_DisplayString(1, "     START");
		delay_ms(200);
		LCD_ClearScreen();
	}
}

void displayScore(){
	Clear_Matrix(current);
	WordOfData = eeprom_read_word((uint16_t*)46);
	int hundreds = 0, tens = 0, ones = 0;
	
	if(WordOfData < score){
		LCD_ClearScreen();
		LCD_DisplayString(1, "Congratulations! New High Score");
		delay_ms(200);
		LCD_DisplayString(1, "High score: ");
		
		if(score > 99){
			hundreds = score / 100;
		}
		if(score > 9){
			tens = (score - (hundreds * 100)) / 10;
		}
		if(score > 0){
			ones = (score - (hundreds * 100) - (tens * 10));
		}
		
		if(hundreds > 0){
			LCD_WriteData( hundreds + '0');
		}
		if(tens > 0){
			LCD_WriteData( tens + '0');
		}
		LCD_WriteData( ones + '0');
		
		eeprom_write_word ((uint16_t*)46, score);
	}
	else{
		LCD_ClearScreen();
		LCD_DisplayString(1, "Your score: ");
		if(score > 99){
			hundreds = score / 100;
		}
		if(score > 9){
			tens = (score - (hundreds * 100)) / 10;
		}
		if(score > 0){
			ones = (score - (hundreds * 100) - (tens * 10));
		}
		
		if(hundreds > 0){
 			LCD_WriteData( hundreds + '0');
		}
		if(tens > 0){
			LCD_WriteData( tens + '0');
		}
		LCD_WriteData( ones + '0');
		
		delay_ms(500);
		LCD_DisplayString(1, "High score: ");
		
		if(WordOfData > 99){
			hundreds = WordOfData / 100;
		}
		if(WordOfData > 9){
			tens = (WordOfData - (hundreds * 100)) / 10;
		}
		if(WordOfData > 0){
			ones = (WordOfData - (hundreds * 100) - (tens * 10));
		}
		
		if(hundreds > 0){
			LCD_WriteData( hundreds + '0');
		}
		if(tens > 0){
			LCD_WriteData( tens + '0');
		}
		LCD_WriteData( ones + '0');
	}
}

void onePlayer(){
	sequence[0] = 1;
	sequence[1] = 2;
	sequence[2] = 3;
	sequence[3] = 4;
	sequence[4] = 5;
	sequence[5] = 3;
	sequence[6] = 4;
	sequence[7] = 4;
	sequence[8] = 1;
	sequence[9] = 1;
	sequence[10] = 2;
	sequence[11] = 5;
	sequence[12] = 3;
	sequence[13] = 1;
	sequence[14] = 4;
	sequence[15] = 1;
	sequence[16] = 2;
	sequence[17] = 3;
	sequence[18] = 4;
	sequence[19] = 5;
	sequence[20] = 1;
	
	time[0] = 1;
	time[1] = 2;
	time[2] = 3;
	time[3] = 4;
	time[4] = 5;
	time[5] = 8;
	time[6] = 10;
	time[7] = 11;
	time[8] = 12;
	time[9] = 13;
	time[10] = 16;
	time[11] = 16;
	time[12] = 17;
	time[13] = 19;
	time[14] = 20;
	time[15] = 21;
	time[16] = 26;
	time[17] = 29;
	time[18] = 30;
	time[19] = 34;
	time[20] = 35;
	
	size = 21;
}
void twoPlayer(){
	if(size < 224 && light_count < 35){
		if((~PINA & 0x01) == 0x01){
			secondPlayerPressed1 = 1;
		}
		else if(secondPlayerPressed1 == 1){
			time[size] = light_count;
			sequence[size] = 1;
			size++;
			secondPlayerPressed1 = 0;
		}
		if((~PINA & 0x02) == 0x02){
			secondPlayerPressed2 = 1;
		}
		else if(secondPlayerPressed2 == 1){
			time[size] = light_count;
			sequence[size] = 2;
			size++;
			secondPlayerPressed2 = 0;
		}
		if((~PINA & 0x04) == 0x04){
			secondPlayerPressed3 = 1;
		}
		else if(secondPlayerPressed3 == 1){
			time[size] = light_count;
			sequence[size] = 3;
			size++;
			secondPlayerPressed3 = 0;
		}
		if((~PINA & 0x08) == 0x08){
			secondPlayerPressed4 = 1;
		}
		else if(secondPlayerPressed4 == 1){
			time[size] = light_count;
			sequence[size] = 4;
			size++;
			secondPlayerPressed4 = 0;
		}
		if((~PINA & 0x10) == 0x10){
			secondPlayerPressed5 = 1;
		}
		else if(secondPlayerPressed5 == 1){
			time[size] = light_count;
			sequence[size] = 5;
			size++;
			secondPlayerPressed5 = 0;
		}
	}
}

void gamePlay(){
	reset = 0;	
	LCD_state = 1;
	init_IO();
	init_SPI();
	TimerSet(1);
	TimerOn();
	// Initializes the LCD display
	LCD_init();
	
	current = shift;
	
	InitGame();

	while(1)
	{
		tempA = ~PINA & 0xFF;
		tempC = ~PINC & 0xC3;
			
		//Pull LATCH low (Important: this is necessary to start the SPI transfer!)
		SHIFT_PORT &= ~LATCH;
		
		if((tempC & 0x01) == 0x01){
			reset = 1;
		}
		
		if(reset == 1){
			break;
		}
		
		if(mode == 1){
			onePlayer();
			if(light_count < 45){
				updateLights();
			}
			else{
				displayScore();
				while(1){
					if((~PINC & 0x01) == 0x01){
						reset = 1;
						break;
					}
				}
			}
		}
		if(mode == 2){
			if(light_count < 45){
				twoPlayer();
				updateLights();
			}
			else{
				displayScore();
				while(1){
					if((~PINC & 0x01) == 0x01){
						reset = 1;
						break;
					}
				}
			}
		}
		
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	//Player 2 buttons + 3 of Player 1 Buttons
	DDRC = 0x02; PORTC = 0xC1;	//Last 2 Player 1 Buttons + Reset Button + Speaker Output
	DDRB = 0x03; PORTB = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	while(reset == 1){
		WordOfData = eeprom_read_word((uint16_t*)46);	
		
		if(WordOfData > 0x200){
			eeprom_write_word ((uint16_t*)46, 0);
		}
			
		pattern = 0x00; // LED pattern - 0: LED off; 1: LED on
		row = 0x00; // Row(s) displaying pattern - 0: display pattern on row; 1: do NOT display pattern on row
		pattern1 = 0x00; // LED pattern - 0: LED off; 1: LED on
		row1 = 0xFE; // Row(s) displaying pattern - 0: display pattern on row; 1: do NOT display pattern on row
		pattern2 = 0x00; // LED pattern - 0: LED off; 1: LED on
		row2 = 0xFD; // Row(s) displaying pattern - 0: display pattern on row; 1: do NOT display pattern on row
		pattern3 = 0x00; // LED pattern - 0: LED off; 1: LED on
		row3 = 0xFB; // Row(s) displaying pattern - 0: display pattern on row; 1: do NOT display pattern on row
		pattern4 = 0x00; // LED pattern - 0: LED off; 1: LED on
		row4 = 0xF7; // Row(s) displaying pattern - 0: display pattern on row; 1: do NOT display pattern on row
		pattern5 = 0x00; // LED pattern - 0: LED off; 1: LED on
		row5 = 0xEF; // Row(s) displaying pattern - 0: display pattern on row; 1: do NOT display pattern on row

		light_count = 0;
		slowness = 200; //200
		counter = 200;
		mode = 0;

		did1 = 0;
		did2 = 0;
		did3 = 0;
		did4 = 0;
		did5 = 0;
		done1 = 0;
		done2 = 0;
		done3 = 0;
		done4 = 0;
		done5 = 0;
		
		score = 0;
		
		secondPlayerPressed1 = 0;
		secondPlayerPressed2 = 0;
		secondPlayerPressed3 = 0;
		secondPlayerPressed4 = 0;
		secondPlayerPressed5 = 0;
		
		firstPlayerPressed1 = 0;
		firstPlayerPressed2 = 0;
		firstPlayerPressed3 = 0;
		firstPlayerPressed4 = 0;
		firstPlayerPressed5 = 0;
		
		size = 5;
		
		for(char i = 0; i < 225; i++){
			time[i] = 0;
			sequence[i] = 0;
		}
		
		gamePlay();
	}
}