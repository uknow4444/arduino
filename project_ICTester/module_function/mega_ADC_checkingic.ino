#define ADC0_set 0B01000000
#define ADC1_set 0B01000001
#define ADC2_set 0B01000010
#define ADC3_set 0B01000011
#define ADC4_set 0B01000100
#define ADC5_set 0B01000101
#define ADC6_set 0B01000110
#define ADC7_set 0B01000111


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
  
}

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

void loop() {
   Serial.print("checking : ");
   Serial.println(ADC_read(7));
   delay(500);
}
