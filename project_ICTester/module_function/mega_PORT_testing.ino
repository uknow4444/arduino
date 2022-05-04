void setup() {
  DDRL = 0xff;
  DDRC = 0xff;
  //portl, portc relay control port, input mode set
  PORTL=0;
  PORTC=0;
  PORTL=0xff;
  PORTC=0xff;

  
  PORTL = B00000011;
  PORTC = B11011110;
}

void loop() {
}
