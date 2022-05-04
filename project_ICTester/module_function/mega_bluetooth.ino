#include <SoftwareSerial.h>

// Hardware serial1 사용
// tx1,rx1 = 18,19 port
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("Start");
}

void loop() {
  if(Serial1.available())
    Serial.write(Serial1.read());
  if(Serial.available())
    Serial1.write(Serial.read());    
}
