
#define F_CPU 4000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char font[3][8]={0b00000000,
	0b00110010,
	0b00000010,
	0b01111010,
	0b00010011,
	0b00100010,
	0b01010010,
	0b01001010,
	
	0b00011100,
	0b00100010,
	0b00011100,
	0b00000000,
	0b01111111,
	0b00010100,
	0b00100000,
	0b00111110,
	
	0b00001100,
	0b00000000,
	0b00111110,
	0b00011100,
	0b00100010,
	0b00011100,
	0b00001000,
	0b01111111
};// 0이 차, 1이 윤, 2가 호

int mode = 0;
unsigned char font_display[8];
int pos_font = 0;
int pos_shift = 0;

void font_reset(){
	for(int i =0; i<8;i++){
		font_display[i] = font[0][i];
	}
}

void mode0(){	
	for(int i =0; i<8;i++){
		font_display[i] = font[pos_font][i];
	}
	pos_font++;
	if(pos_font == 3){
		pos_font = 0;
	}
}

void mode1(){
	for(int i=0; i<8; i++){
		font_display[i] <<= 1;
		int shift = 7-pos_shift;
		unsigned font_sub = font[pos_font][i];
		font_display[i] |= (font_sub >> shift);
	}
	pos_shift++;
	if(pos_shift == 8){
		pos_font++;
		pos_shift =0;
		if(pos_font == 3)
			pos_font = 0;
	}
}

void mode2(){
	for(int i=0; i<8; i++){
		font_display[i] >>= 1;
		int shift = 7-pos_shift;
		unsigned font_sub = font[pos_font][i];
		font_display[i] |= (font_sub << shift);
	}
	pos_shift++;
	if(pos_shift == 8){
		pos_font++;
		pos_shift = 0;
		if(pos_font == 3)
			pos_font = 0;
	}
}

ISR(INT4_vect){
	if(mode == 2)
		mode = 1;
	else if(mode == 1)
		mode = 2;
	else
		mode =1;
	pos_font = 1;
	pos_shift =0;
	font_reset();
}
ISR(INT5_vect){
	if(mode == 3){
		mode =0;
		pos_font = 0;
	}
	else
		mode = 3;
}

int main(void)
{
	EIMSK = 0xf0; // INT4 - pe4, INT5 - pe5, INT6 - pe6, INT7 - pe7 enable set
	EICRB = 0xaf;
	SREG |= 0x80;
	
	DDRF = 0xFF;
	DDRB = 0xFF;
	
	PORTF = 0xFF;
	mode = 0;
	pos_shift = 0;
	pos_font = 0;
	
	while(1){
		if(mode == 0)
			mode0();
		else if (mode == 1)
			mode1();
		else if (mode == 2)
			mode2();
		for(int th =0; th<50; th++){
			int a =0;
			for(int i=7; i>-1; i--){
				PORTF = font_display[a++];
				PORTB = ~(1<<i);
				_delay_ms(1);
				PORTB = 0xff;
			}
		}
	}
}
