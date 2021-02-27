#include <SPI.h>
#include <Servo.h>

const int tempPin = A0;
const int Slave1_SS = 15;
const int Slave2_SS = 16;
int Vin;  // Variable to read the value from Arduino A0
float Temperature; // variable that receives the converted voltage
char TransferedData;
int mystatus=0;
int servodegree;
Servo myservo1;
Servo myservo2;
Servo myservo3;

void setup (void) {
  //spi setup
   SPI.begin ();
   Serial.begin(9600);
   pinMode(Slave1_SS, OUTPUT);
   pinMode(Slave2_SS, OUTPUT);
   digitalWrite(Slave1_SS, HIGH);
   digitalWrite(Slave2_SS, HIGH);
   //spi setup
   //read temp at start
    Vin = analogRead(tempPin);
    servodegree=180;
    Temperature = (500.0 * Vin)/1023; // Convert the read value into a voltage
    Temperature/=10;
    //servo
   myservo1.attach(2);
   myservo1.write(180);
   myservo2.attach(3);
   myservo2.write(180);
   myservo3.attach(4);
   myservo3.write(180);
  //buzeer
   pinMode(0,OUTPUT);
}


void checkemergency(){
  if(Temperature>120){
    mystatus=1;
    digitalWrite(0,HIGH);
    servodegree=0;
    myservo1.write(0);
    myservo2.write(0);
    myservo3.write(0);
  }
  else{
    if(Temperature>60){
      digitalWrite(0,LOW);
      servodegree=50;
      myservo1.write(50);
      myservo2.write(50);
      myservo3.write(50);
      mystatus=0;
    }
    else{
      digitalWrite(0,LOW);
      servodegree=180;
      myservo1.write(180);
      myservo2.write(180);
      myservo3.write(180);
      mystatus=0;
    }
  }
}

void loop (void) {
  if(servodegree>0){
      Temperature+=servodegree/10;
      checkemergency();
      //-----------------------------send status---------------------------------------
    /**** First Slave Data ****/   
   digitalWrite(Slave1_SS, LOW);
   if(mystatus==1){
    for (const char * p = "emergency\r" ; TransferedData = *p; p++) {
      SPI.transfer(TransferedData);
      Serial.println(TransferedData);
    }
   }
   else{
    for (const char * p = "safe\r" ; TransferedData = *p; p++) {
      SPI.transfer(TransferedData);
      Serial.println(TransferedData);
    }
   }
   digitalWrite(Slave1_SS, HIGH);
   delay(20);
/**** Second Slave Data ****/
   digitalWrite(Slave2_SS, LOW);
   if(mystatus==1){
    for (const char * p = "emergency\r" ; TransferedData = *p; p++) {
      SPI.transfer(TransferedData);
      Serial.println(TransferedData);
    }
   }
   else{
    for (const char * p = "safe\r" ; TransferedData = *p; p++) {
      SPI.transfer(TransferedData);
      Serial.println(TransferedData);
    }
   }
   digitalWrite(Slave2_SS, HIGH);
   delay(20);
   //-----------------------------send status---------------------------------------
   }
  if(servodegree==0){
      delay(2000);
      Temperature=0;
      servodegree=180;
      myservo1.write(180);
      myservo2.write(180);
      myservo3.write(180);
    }
   Serial.println(Temperature);
   delay(200);
}
