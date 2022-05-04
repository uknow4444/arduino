#define F_CPU 4000000UL
#include <avr/io.h>
#include<avr/interrupt.h>
#include<util/atomic.h>
#include <util/delay.h>

unsigned char num[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f};

unsigned char shift[4] = {0x3f, 0x3f, 0x3f, 0x3f};




void segon(int time)
{
	for(int i = 0 ; i <time ; i++)
		{
			PORTF = 0;
			PORTA = 0;
			PORTF = shift[3];
			PORTA = 0xfe;
			_delay_ms(2);

			PORTF = 0;
			PORTA = 0;
			PORTF = shift[2];
			PORTA = 0xfd;
			_delay_ms(2);

			PORTF = 0;
			PORTA = 0;
			PORTA = 0xfb;
			PORTF = shift[1];
			_delay_ms(2);

			PORTF = 0;
			PORTA = 0;
			PORTA = 0xf7;
			PORTF = shift[0];
			_delay_ms(2);

		}
}

ISR(INT4_vect){
	for(int i=0; i<5;i++){
		PORTC = 0x80;
		segon(25);
		PORTC = 0x00;
		segon(25);
	}
}

int main(void)
{
	DDRA = 0xff;
	DDRF = 0xff;
	DDRC = 0xff;
	PORTC = 0x01;
	
	DDRE = 0x00;
	
	EICRB = 0x0b00000000;
	EIMSK = 0x10;
	SREG = 0x80;
	
	int n = 71;

	while(1)
	{
		for(int i = 0; i<n;i++){
			shift[0] = num[i%10];
			shift[1] = num[(((i%1000)%100)/10)];
			shift[2] = num[(i%1000)/100];
			shift[3] = num[i/1000];
			segon(1);
		}

		for(int i = n; i<10000;i++){
			shift[0] = num[i%10];
			shift[1] = num[(((i%1000)%100)/10)];
			shift[2] = num[(i%1000)/100];
			shift[3] = num[i/1000];
			segon(1);
		}
	}
}
