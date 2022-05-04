#include "U8glib.h"
int mode = 0;
int result[4];
int chip_num;

int result7404_test1;
int result7404_test2;



#define result7400 0B0001110111110101
#define result7402 0B0001000001010001
#define result7404 0B01010101
#define result7408 0B0000001000001010
#define result7432 0B0000111110101110
#define result7486 0B0000110110100100

#define testcode0 0x00
#define testcode1 0x55
#define testcode2 0xAA
#define testcode3 0xFF //test sequence 0 1232 1020 3130
#define testcode4 0x88 // 7404 first test

char test_sequence[13] = {'0','1','2','3','2','1','0','2','0','3','1','3','0'};

/*///////////////////////////////////////////
                  ADC
///////////////////////////////////////////*/

#define ADC0_set 0B01000000
#define ADC1_set 0B01000001
#define ADC2_set 0B01000010
#define ADC3_set 0B01000011
#define ADC4_set 0B01000100
#define ADC5_set 0B01000101
#define ADC6_set 0B01000110
#define ADC7_set 0B01000111

int ADC_read(int set){
  int adc_value;
  switch(set){
    case 0 : ADMUX = ADC0_set; break;
    case 1 : ADMUX = ADC1_set; break;
    case 2 : ADMUX = ADC2_set; break;
    case 3 : ADMUX = ADC3_set; break;
    case 4 : ADMUX = ADC4_set; break;
    case 5 : ADMUX = ADC5_set; break;
    case 6 : ADMUX = ADC6_set; break;
    case 7 : ADMUX = ADC7_set; break;
  }
  ADCSRA |=(1<<ADSC);
  while((ADCSRA & (1<<ADIF)) == 0);
  adc_value = ((int)ADCL)|((int)ADCH<<8);
  return adc_value;
}


/*///////////////////////////////////////////
                  OLED
///////////////////////////////////////////*/

U8GLIB_SH1106_128X64 u8g(10, 9, 12, 11, 13);
// SW SPI Com: SCK = 10, MOSI = 9, CS = 12, A0(DC) = 11, RST=13
void draw(int x, int y, char s[]) {
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( x, y, s);
}



///////////////////////////////////////// mode0
// checking ic
void mode0(){
  if(mode==0){
    mode00();
    mode = 1;
  }
  else if(mode==1)
    mode01();
}
void mode00(){
  u8g.firstPage(); //OLED 출력 
  do {
    draw(0, 22, "Waiting IC");
  } while( u8g.nextPage() );
}
void mode01(){
  if(checkic())
    mode = 10;
}

bool checkic(){
  if(ADC_read(3)>100)
    return true;
  else
    return false;
}

///////////////////////////////////////// mode1
void mode1(){
  if(!checkic()) mode=0; // ic가 없다면 초기화
  if(mode == 10){
    mode10();
    mode=11;
  }
  else if(mode == 11)
    mode11();
  else if(mode == 12)
    mode12();
}

void mode10(){
  u8g.firstPage(); //OLED 출력 
  do {
    draw(0, 22, "IC testing...");
  } while( u8g.nextPage() );
  delay(1000);
}
void mode11(){
  testing_7404();
  if(chip_num == 0)mode = 12;
  else mode = 20;
}
void mode12(){
  default_test();
  mode = 20;
}
///////////////////////////////////////// mode2
void mode2(){
  if(!checkic()) mode=0; // ic가 없다면 초기화
  if(mode == 20){
    mode20();
    mode=21;
  }
}

void mode20(){
  u8g.firstPage(); //OLED 출력 
  do {
    if(chip_num == 7400)
      draw(0, 22, "7400");
    if(chip_num == 7404)
      draw(0, 22, "7404");
    if(chip_num == 7408)
      draw(0, 22, "7408");
    if(chip_num == 7432)
      draw(0, 22, "7432");
    if(chip_num == 7486)
      draw(0, 22, "7486");
  } while( u8g.nextPage() );
}
///////////////////////////////////////// mode3



///////////////////////////////////////// testing function


void result_reset(){
  chip_num = 0;
  for(int i = 0; i<4; i++)
    result[i] = 0;
}

void testing_7404(){
  result_reset();
  result7404_test1=0;
  result7404_test2=0;
  //relay setting
  PORTL=0;
  PORTC=0;
  PORTL=0xff;
  PORTC=0xff;
  PORTL=0;
  PORTC=0;
  for(int i = 0; i<4; i++){
    PORTA = testcode4; // 7404 first test
    delay(5);
    result7404_test1 <<=1;
    result7404_test2 <<=1;
    if(ADC_read(5)>720){
      result7404_test1 |= 1;
    }
    if(ADC_read(7)>720){
      result7404_test2 |= 1;
    }
    PORTA = testcode0; // 7404 first test
    delay(5);
    result7404_test1 <<=1;
    result7404_test2 <<=1;
    if(ADC_read(5)>720){
      result7404_test1 |= 1;
    }
    if(ADC_read(7)>720){
      result7404_test2 |= 1;
    }
  }
   if((result7404_test1 == result7404) && 
  (result7404_test2 == result7404)){
    PORTL=0;
    PORTC=0;
    PORTL=0xff;
    PORTC=0xff;
    PORTL = 0B00000011;
    PORTC = 0B11011110;
    for(int i=0; i<4; i++){
      PORTA = testcode1;
      delay(5);
      for(int j=0; j<4; j++){
        if(ADC_read(j+4)>720){
          result[j] <<=1;
          result[j] |= 1;
        }
        else{ 
          result[j] <<=1;
        }
      }
      PORTA = testcode0;
      delay(5);
      for(int j=0; j<4; j++){
        if(ADC_read(j+4)>720){
          result[j] <<=1;
          result[j] |= 1;
        }
        else{ 
          result[j] <<=1;
        }
      }
    }
  }
  if(result[0]== result7404&&
  result[1]== result7404&&
  result[2]== result7404&&
  result[3]== result7404)
    chip_num = 7404;
}

void default_test(){
  result_reset();
  //relay setting
  PORTL=0;
  PORTC=0;
  PORTL=0xff;
  PORTC=0xff;
  PORTL=0;
  PORTC=0;
  //testing
  for(int i=0; i<13; i++){
    switch(test_sequence[i]){
      case '0' : PORTA = testcode0;break;
      case '1' : PORTA = testcode1;break;
      case '2' : PORTA = testcode2;break;
      case '3' : PORTA = testcode3;break;
    }
    delay(5); // charging wait
    for(int k = 0; k<4; k++){
      if(ADC_read(k+4)>720){
        result[k] <<=1;
        result[k] |= 1;
      }
      else{ 
        result[k] <<=1;
      }
    }
  }
  checking_num();
}

void checking_num(){
  if(result[0]== result7400){
    if(result[1] == result7400
    && result[2] == result7400
    && result[3] == result7400)
      chip_num = 7400; 
  }
  else if(result[0]== result7408){
    if(result[1] == result7408
    && result[2] == result7408
    && result[3] == result7408)
      chip_num = 7408; 
  }
  else if(result[0]== result7432){
    if(result[1] == result7432
    && result[2] == result7432
    && result[3] == result7432)
      chip_num = 7432; 
  }
  else if(result[0]== result7486){
    if(result[1] == result7486
    && result[2] == result7486
    && result[3] == result7486)
      chip_num = 7486; 
  }
  Serial.println(result7400);
  Serial.print("1 : ");
  Serial.println(result[0]);
  Serial.print("2 : ");
  Serial.println(result[1]);
  Serial.print("3 : ");
  Serial.println(result[2]);
  Serial.print("4 : ");
  Serial.println(result[3]);
}


//////////////////////////// setting
void setup() {
  ADMUX = B01000000;
  // 01 : AVCC 사용
  // 0 : ADCL부터 저장.
  // 000000 = ADC0
  ADCSRA = B10000111;
  // 분주비 128
  // ADC Enable
  ADCSRB = 0x00;
  
  
  Serial.begin(9600);
  
  DDRL = 0xff; // relay control 2
  DDRC = 0xff; // relay control 1
  DDRA = 0xff; // input data control
  PORTL=0;
  PORTC=0;
  PORTA=0;
  chip_num = 0;
  for(int i = 0; i<4; i++)
    result[i] = 0;
  result7404_test1= 0;
  result7404_test2= 0;

  
  u8g.firstPage(); //OLED 출력 
  do {
    draw(0, 22, "start");
  } while( u8g.nextPage() );
  delay(1000); // 1초 대기
  mode = 0;
}


void loop() {
  if(mode < 10) // mode 0
    mode0();
  else if(mode < 20) // mode 1
    mode1();
  else if(mode < 30)
    mode2();
}
