uint8_t abcd[9][8] = {
  { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00}, // U+0031 (1)
  { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00}, // U+0032 (2)
  { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00}, // U+0033 (3)
  { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00}, // U+0034 (4)
  { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00}, // U+0035 (5)
  { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00}, // U+0036 (6)
  { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00}, // U+0037 (7)
  { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00}, // U+0038 (8)
  { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00}, // U+0039 (9)
}; // 1~9까지의 폰트 저장.

uint8_t display_buffer[4][8] = {0,};

void setup() {
  DDRB = 0xff;
  //PB0 = DIN (53)
  //PB1 = CLK (52)
  //PB2 = CS (51)
  PORTB = 0x00; // Clear
  write_Max7219(0x09, 0x00); // Decode Mode - No decode for digits
  write_Max7219(0x0a, 0x01); // Intensity - 3/32
  write_Max7219(0x0b, 0x07); // Scan Limit - All Output Port Enable
  write_Max7219(0x0c, 0x01); // Shutdown - Normal Operation
  write_Max7219(0x0f, 0x00); // Display Test
  for(uint8_t k=0; k<4;k++) write_Max7219(0,0); //4번 매트릭스 셧다운 해제를 위한 3번의 통신 진행
  delay(30);
}

void loop() {
  for (uint8_t i = 1 ; i < 9 ; i++) {
    PORTB &= ~(1 << 2); // CS Low
    for (uint8_t CD = 0; CD < 4; CD++) {
      write_byte(1, i);// MSB First, 순방향
      write_byte(0, abcd[CD][i - 1]); //LSB First 역방향, dot matrix랑 폰트의 MSB가 반대방향임.
    }
    PORTB |= (1 << 2); // CS High
    delay(30);
  }
}
void write_Max7219(uint8_t address, uint8_t data) {
  PORTB &= ~(1 << 2); // CS Low
  write_byte(1, address); // MSB First, 순방향
  write_byte(1, data); // MSB First, 순방향
  PORTB |= (1 << 2); // CS High
}

void write_byte(uint8_t flag, uint8_t data) {
  for (uint8_t i = 0; i < 8; i++) {
    if (flag) { // MSB First
      if (!!(data & ( 1 << 7 - i))) PORTB |= 1;
      else PORTB &= ~1;
    }
    else { // LSB First
      if (!!(data & ( 1 << i))) PORTB |= 1;
      else PORTB &= ~1;
    }
    PORTB |= (1 << 1); // CLK High
    PORTB &= ~(1 << 1); // CLK Low
  }
}
