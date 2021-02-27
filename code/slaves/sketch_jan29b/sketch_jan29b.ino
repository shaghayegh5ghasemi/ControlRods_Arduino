#include <SPI.h>
bool process;
char buff [50];
volatile byte indx;

void setup() {
  Serial.begin(9600);
  pinMode(MISO, OUTPUT); 
  SPCR |= _BV(SPE);
  process = false;
  SPI.attachInterrupt();
  pinMode(2,OUTPUT);
}

ISR (SPI_STC_vect)
{
  byte c = SPDR;
  if(c==115){
    Serial.println("safe");
    digitalWrite(2,LOW);
  }
  else{
    Serial.println("emergency");
    digitalWrite(2,HIGH);
    delay(500);
  }
  delay(200);
}

void loop() {
}
