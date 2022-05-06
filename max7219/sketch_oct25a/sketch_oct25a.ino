uint8_t abcd[6][8] = {
  {0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00}, // U+0054 (T)
  {0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00}, // U+0065 (e)
  {0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00}, // U+0073 (s)
  {0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00} // U+0074 (t)
};


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
  delay(300);
  

  for (uint8_t i = 1 ; i < 9 ; i++) {
  PORTB &= ~(1 << 2); // CS Low
    for (uint8_t CD = 0; CD < 4; CD++) {
      write_byte(1, i);
      write_byte(1, 0);
    }
    PORTB |= (1 << 2); // CS High
    delay(30);
  }
}

void loop() {
  for (uint8_t i = 1 ; i < 9 ; i++) {
    PORTB &= ~(1 << 2); // CS Low
    for (uint8_t CD = 0; CD < 4; CD++) {
      write_byte(1, i);
      write_byte(0, abcd[CD][i - 1]);
    }
    PORTB |= (1 << 2); // CS High
    delay(30);
  }
}
void write_Max7219(uint8_t address, uint8_t data) {
  PORTB &= ~(1 << 2); // CS Low
  delay(30);
  write_byte(1, address);
  write_byte(1, data);
  PORTB |= (1 << 2); // CS High
}

void write_byte(uint8_t flag, uint8_t data) {
  for (uint8_t i = 0; i < 8; i++) {
    if (flag) {
      if (!!(data & ( 1 << 7 - i))) PORTB |= 1;
      else PORTB &= ~1;
    }
    else {
      if (!!(data & ( 1 << i))) PORTB |= 1;
      else PORTB &= ~1;
    }
    PORTB |= (1 << 1); // CLK High
    PORTB &= ~(1 << 1); // CLK Low
  }
}
