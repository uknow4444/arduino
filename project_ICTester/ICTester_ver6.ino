#include "U8glib.h"
int mode = 0;
int result[4];
int chip_num;

int result7404_test1;
int result7404_test2;

int mode21_state = 0; // mode21 상태 확인
int mode22_state = 0; // mode22 상태 확인
int mode22_select =0; // mode22 ic select 확인.
int mode23_state = 0; // mode23 상태 확인



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
#define testcode5 0x33
#define testcode6 0xCC

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
    draw(0, 22, "Insert IC");
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
  else if(mode == 13)
    mode13();
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
  if(chip_num == 0)mode = 13;
  else mode = 20;
}
void mode13(){
  testing_7402();
  mode = 20;
}
///////////////////////////////////////// mode2
void mode2(){
  if(!checkic()) mode=0; // ic가 없다면 초기화
  if(mode == 20){
    mode21_state = 0;
    mode22_state = 0;
    mode22_select = 0;
    mode20();
    mode=21;
  }
  else if(mode == 21){
    mode21();
  }
  else if(mode == 22){
    mode22();
  }
  else if(mode == 23){
    mode23();
  }
}

void mode20(){
  PORTC = 0;
  PORTL = 0;
  u8g.firstPage(); //OLED 출력 
  do {
    if(chip_num == 7400){
      draw(0, 22, "7400");
      draw(0, 44, "NAND GATE");
    }
    if(chip_num == 7402){
      draw(0, 22, "7402");
      draw(0, 44, "NOR GATE");
    }
    if(chip_num == 7404){
      draw(0, 22, "7404");
      draw(0, 44, "NOT GATE");
    }
    if(chip_num == 7408){
      draw(0, 22, "7408");
      draw(0, 44, "AND GATE");
    }
    if(chip_num == 7432){
      draw(0, 22, "7432");
      draw(0, 44, "OR GATE");
    }
    if(chip_num == 7486){
      draw(0, 22, "7486");
      draw(0, 44, "XOR GATE");
    }
    if(chip_num == 0){
      draw(0,22, "error!!");
      draw(88,22, ">>>");
    }
  } while( u8g.nextPage() );
}
void mode21(){
  /*
  if(Serial1.available())
    Serial.write(Serial1.read());
  if(Serial.available())
    Serial1.write(Serial.read());
  */
  if(mode21_state == 0)
    if(ADC_read(1)< 100) mode21_state = 1;
  if(mode21_state == 1){
    u8g.firstPage(); //OLED 출력 
    do{
      draw(0, 22, "Bluetooth");
      draw(0, 44, "Select IC");
      draw(88,22, ">>>");
    } while( u8g.nextPage() );
    delay(800);
    mode21_state = 2;
  }
  if(mode21_state == 2){
    if(ADC_read(0)> 900) mode21_state = 3;
    if(ADC_read(1)< 100) mode = 30;
  }
  if(mode21_state == 3){
    u8g.firstPage(); //OLED 출력 
    do{
      draw(0, 22, "Bluetooth");
      draw(0, 44, "Select IC");
      draw(88,44, ">>>");
    } while( u8g.nextPage() );
    delay(800);
    mode21_state = 4;
  }
  if(mode21_state == 4){
    if(ADC_read(0)< 100) mode21_state = 1;
    if(ADC_read(1)< 100) {
      mode = 22;
      mode22_state = 0;
    }
  }
}
void mode22(){
  if(mode22_state == 0){
    u8g.firstPage(); //OLED 출력 
    do{
      draw(0, 10, "select IC");
      draw(65,30, "^");
      draw(10, 40, "<<<");
      switch(mode22_select){
        case 0:draw(50,40,"7400");break;
        case 1:draw(50,40,"7402");break;
        case 2:draw(50,40,"7404");break;
        case 3:draw(50,40,"7408");break;
        case 4:draw(50,40,"7432");break;
        case 5:draw(50,40,"7486");break; 
      }
      draw(98,40, ">>>");
      draw(65, 55, "v");
    } while( u8g.nextPage() );
    delay(800);
    mode22_state = 1;
  }
  if(mode22_state == 1){
    if(ADC_read(1)>900){
      mode21_state = 3;
      mode = 21;
    }
    if(ADC_read(0)< 100){
      mode22_select--;
      if(mode22_select < 0) mode22_select = 5;
      mode22_state = 0;
    }
    if(ADC_read(0)> 900){
      mode22_select++;
      if(mode22_select > 5) mode22_select = 0;
      mode22_state = 0;
    }
    if(ADC_read(1)< 100){
      mode = 23;
      mode23_state = 0;
    }
  }
}
void mode23(){
  if(mode23_state == 0){
    switch(mode22_select){
      case 0:
      //7400
      default_test();
      break;
      case 1:
      //7402
      testing_7402();
      break;
      case 2:
      //7404
      testing_7404();
      break;
      case 3:
      //7408
      default_test();
      break;
      case 4:
      //7432
      default_test();
      break;
      case 5:
      //7486
      default_test();
      break;
    }
    u8g.firstPage(); //OLED 출력 
    do{
      draw(0, 10, "test");
      switch(mode22_select){
        case 0:
          draw(70,10,"7400");
          if(result[0] == result7400) draw(0,30,"1");
          else draw(0,50,"1");
          if(result[1] == result7400) draw(10,30,"2");
          else draw(10,50,"2");
          if(result[2] == result7400) draw(20,30,"3");
          else draw(20,50,"3");
          if(result[3] == result7400) draw(30,30,"4");
          else draw(30,50,"4");
          break;
        case 1:
          draw(70,10,"7402");
          if(result[0] == result7402) draw(0,30,"1");
          else draw(0,50,"1");
          if(result[1] == result7402) draw(10,30,"2");
          else draw(10,50,"2");
          if(result[2] == result7402) draw(20,30,"3");
          else draw(20,50,"3");
          if(result[3] == result7402) draw(30,30,"4");
          else draw(30,50,"4");
          break;
        case 2:
          draw(70,10,"7404");
          if(result[0] == result7404) draw(0,30,"1");
          else draw(0,50,"1");
          if(result[1] == result7404) draw(10,30,"2");
          else draw(10,50,"2");
          if(result7404_test1 == result7404) draw(20,30,"3");
          else draw(20,50,"3");
          if(result[2] == result7404) draw(30,30,"4");
          else draw(30,50,"4");
          if(result[3] == result7404) draw(40,30,"5");
          else draw(40,50,"5");
          if(result7404_test2 == result7404) draw(50,30,"6");
          else draw(50,50,"6");
          break;
        case 3:
          draw(70,10,"7408");
          if(result[0] == result7408) draw(0,30,"1");
          else draw(0,50,"1");
          if(result[1] == result7408) draw(10,30,"2");
          else draw(10,50,"2");
          if(result[2] == result7408) draw(20,30,"3");
          else draw(20,50,"3");
          if(result[3] == result7408) draw(30,30,"4");
          else draw(30,50,"4");
          break;
        case 4:
          draw(70,10,"7432");
          if(result[0] == result7432) draw(0,30,"1");
          else draw(0,50,"1");
          if(result[1] == result7432) draw(10,30,"2");
          else draw(10,50,"2");
          if(result[2] == result7432) draw(20,30,"3");
          else draw(20,50,"3");
          if(result[3] == result7432) draw(30,30,"4");
          else draw(30,50,"4");
          break;
        case 5:
          draw(70,10,"7486");
          if(result[0] == result7486) draw(0,30,"1");
          else draw(0,50,"1");
          if(result[1] == result7486) draw(10,30,"2");
          else draw(10,50,"2");
          if(result[2] == result7486) draw(20,30,"3");
          else draw(20,50,"3");
          if(result[3] == result7486) draw(30,30,"4");
          else draw(30,50,"4");
          break; 
      }
      draw(90,30,"good");
      draw(85,50,"error");
    } while( u8g.nextPage() );
    delay(800);
    mode23_state = 1;
  }
  if(mode23_state == 1){
    if(ADC_read(1) > 900){
      mode = 22;
    }
  }
}
///////////////////////////////////////// mode3
void mode3(){
  if(!checkic()) mode=0; // ic가 없다면 초기화
  if(mode == 30){
    mode30();
    mode = 31;
  }
  if(mode == 31)
    mode31();
}
void mode30(){
    u8g.firstPage(); //OLED 출력 
    do{
      draw(28, 22, "Bluetooth");
      draw(23, 44, "Connection");
    } while( u8g.nextPage() );
    delay(800);
}
void mode31(){
  if(ADC_read(1)>900){
    mode21_state = 1;
    mode = 21;
  }
}


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
    delay(20);
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
  /*
  Serial.println(result7400);
  Serial.print("1 : ");
  Serial.println(result[0]);
  Serial.print("2 : ");
  Serial.println(result[1]);
  Serial.print("3 : ");
  Serial.println(result[2]);
  Serial.print("4 : ");
  Serial.println(result[3]);
  */
}


void testing_7402(){
  chip_num = 0;
  for(int i = 0; i<4; i++)
    result[i] = 0;
  PORTL=0;
  PORTC=0;
  PORTL=0xff;
  PORTC=0xff;
  PORTL = 0B00000011;
  PORTC = 0B10111101;
  delay(20);
  //testing
  for(int i=0; i<13; i++){
    switch(test_sequence[i]){
      case '0' : PORTA = testcode0;break;
      case '1' : PORTA = testcode5;break;
      case '2' : PORTA = testcode6;break;
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
  if(result[0]== result7402){
    if(result[1] == result7402
    && result[2] == result7402
    && result[3] == result7402)
      chip_num = 7402; 
  }
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
  Serial1.begin(9600);
  // Hardware serial1 사용
  // tx1,rx1 = 18,19 port

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
  else
    mode3();
}




