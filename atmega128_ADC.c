#define F_CPU 16000000UL
#define sbi(PORTX,bitX) PORTX|=(1<<bitX)
#define cbi(PORTX,bitX) PORTX&=~(1<<bitX)

#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<stdio.h>


char string1[16] = "Voltage : ";
int value1 = 2;
int value2 = 47;
unsigned int adc_value = 1023;
char buf[32] = {' ',};



void LCD_command(char command)
{
	PORTA = (command&0xF0);	// send High data
	cbi(PORTA,0); 			// RS=0
	//cbi(PORTA,1);			// RW=0
	sbi(PORTA,2);			// Enable
	_delay_us(1);
	cbi(PORTA,2);			// Disable

	PORTA = (command&0x0F)<<4;// send Low data
	cbi(PORTA,0); 			// RS=0
	//cbi(PORTA,1);			// RW=0
	sbi(PORTA,2);			// Enable
	_delay_us(1);
	cbi(PORTA,2);			// Disable
}



void LCD_data(char data)
{
	_delay_us(100);

	PORTA = (data&0xF0);	// send High data
	sbi(PORTA,0); 			// RS=1
	//cbi(PORTA,1);			// RW=0
	sbi(PORTA,2);			// Enable
	_delay_us(1);
	cbi(PORTA,2);			// Disable

	PORTA = (data&0x0F)<<4;	// send Low data
	sbi(PORTA,0); 			// RS=1
	//cbi(PORTA,1);			// RW=0
	sbi(PORTA,2);			// Enable
	_delay_us(1);
	cbi(PORTA,2);			// Disable
}



void LCD_init(void)
{
	_delay_ms(50);

	LCD_command(0x28);		// DL=0(4bit) N=1(2Line) F=0(5x7)
	_delay_ms(2);			// [function set] 0b00101000
	// 4:(DL) 1이면 8bit모드, 0이면 4bit모드
	// 3:(N) 0이면 1줄짜리, 1이면 2줄짜리
	// 2:(F) 0이면 5x8dots, 1이면 5x11dots

	LCD_command(0x0C);		// LCD ON, Cursor X, Blink X
	_delay_ms(2);			// [display on/off control] 0b00001100
	// 2:(D) 1이면 display on, 0이면 off
	// 1:(C) 1이면 cursor on, 0이면 off
	// 0:(B) 1이면 cursor blink, 0이면 off

	LCD_command(0x06);		// Entry Mode
	_delay_ms(2);			// [entry mode set] 0b00000110
	// 1:(I/D) 1이면 오른쪽으로, 0이면 왼쪽
	// 0:(SH) CGRAM 사용관련
	
	LCD_command(0x01);		// LCD Clear
	_delay_ms(2);
}



void LCD_string(char line, char *string)
{
	LCD_command(0x80+((line-1)*0x40));
	while(*string)
	LCD_data(*string++);
}






int main()
{
	DDRA = 0xFF;
	PORTA = 0x00;
	LCD_init();
	
	DDRF = 0x00;
	ADMUX = 0b00000000;
	ADCSRA |= (1<<ADEN);
	//ADCSRA |= (1<<ADFR);
	//ADCSRA |= (1<<ADIE); // ADC활성화
	//while((ADCSRA & (1<<ADIF)) == 0);
	//SREG = 0x80;
	//ADCSRA |= (1<<ADSC);
	float sub_adc;
	while(1)
	{
		ADCSRA |= (1<<ADSC);
		while((ADCSRA & (1<<ADIF)) == 0);
		adc_value = ((int)ADCH<<8)|((int)ADCL);
		sub_adc = 2.5/1024 * adc_value;
		value1 = sub_adc;
		value2 = ((int)(sub_adc*100))%100;
		sprintf(buf, "%s%d.%dV",string1, value1,value2);
		LCD_string(1,buf);
		sprintf(buf, "      %d",adc_value);
		LCD_string(2,buf);
		_delay_ms(1000);
		LCD_command(0x01);		// LCD Clear
		_delay_ms(2);
	}
}
