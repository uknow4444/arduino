#include "U8glib.h"

U8GLIB_SH1106_128X64 u8g(10, 9, 12, 11, 13);
// SW SPI Com: SCK = 10, MOSI = 9, CS = 12, A0(DC) = 11, RST=13


void draw(char s[]) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 0, 22, s);
}

void setup(void) {
  u8g.firstPage();  
  do {
    draw("start");
  } while( u8g.nextPage() );
}

void loop(void) {

}
