#define F_CPU 4000000UL
#include <avr/io.h>
#include <util/delay.h>

unsigned char num[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f};

unsigned char shift[4] = {0x3f, 0x3f, 0x3f, 0x3f};




void segon()
{
	for(int i = 0 ; i <10 ; i++)
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

int main(void)
{
	DDRA = 0xff;
	DDRF = 0xff;

	while(1)
	{
		for(int i = 1; i<90;i++){
			shift[2] = num[i%10];
			shift[3] = num[(i/10)+1];
			shift[1] = num[((i%10)*((i/10)+1))/10];
			shift[0] = num[((i%10)*((i/10)+1))%10];
			segon();
		}
	}
}
