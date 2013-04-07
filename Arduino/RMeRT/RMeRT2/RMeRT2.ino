#include <Servo.h>

#include <CAN.h>
#include <SPI.h>
#include "Wire.h"
#include "SRF02.h"

#define BUS_SPEED 250

SRF02 srf02[1] = { 
  SRF02(0x70, SRF02_CENTIMETERS)
};

//global variable used to determine whether loop should
//be in Tx or Rx mode.
int state;
//global variable used to toggle pin
int pin;

void setup() {                

  Serial.begin(9600);
  Wire.begin();
  SRF02::setInterval(100);
  
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  
  // initialize CAN bus class
  // this class initializes SPI communications with MCP2515
  CAN.begin();
  CAN.baudConfig(BUS_SPEED);
  CAN.setMode(NORMAL);  // set to "NORMAL" for standard com

  Serial.print("Hello World");
  state = 1;
  pin = LOW;  
}

void loop() {
  
  byte length,rx_status,i;
  unsigned long frame_id;
  byte frame_data[8];
  unsigned int usValue;
  byte usHigh;
  byte usLow;
  byte usHigh_real;
  byte usLow_real;
  unsigned int ir1Value;
  byte ir1High;
  byte ir1Low;
  unsigned int ir2Value;
  byte ir2High;
  byte ir2Low;
  
  
  //signal iterations on pin 8
  digitalWrite(8,pin);  
  
  switch(state) {
    case 0: //Tx
//      Serial.print("Preparing data for the CAN");
      
      //signal state on pin 7;
      digitalWrite(7,LOW);
      
      usValue = getUltrasonicMeasurement();
      usLow = (usValue << 1) & 0xFF;
      usHigh = (usValue >> 6) & 0xFF;
      
      usLow_real = usValue & 0xFF;
      usHigh_real = usValue >> 8 & 0xFF;
                
      ir1Value = getInfraredMeasurement1();
      ir1High = ir1Value & 0xFF;
      ir1Low = (ir1Value << 2) & 0xFF;
            
      ir2Value = getInfraredMeasurement2();
      ir2High = ir2Value & 0xFF;
      ir2Low = (ir2Value << 2) & 0xFF;
      
      frame_data[0] = usHigh_real;
      frame_data[1] = usLow_real;
      frame_data[2] = ir2High;
      frame_data[3] = ir2Low;
      frame_data[4] = ir1High;
      frame_data[5] = ir1Low;
      frame_data[6] = usHigh;
      frame_data[7] = usLow;
  
      frame_id = 0x05A58888;
  
      length = 8;
  
      CAN.load_ff_0(length,frame_id,frame_data);
      CAN.send_0();
      
      delay(10);
      state = 1;
      
      break;
    case 1: //Rx
    
      //signal state on pin 7
      digitalWrite(7,HIGH);    
      //clear receive buffers, just in case.
      frame_data[0] = 0x00;
      frame_data[1] = 0x00;
      frame_data[2] = 0x00;
      frame_data[3] = 0x00;
      frame_data[4] = 0x00;
      frame_data[5] = 0x00;
      frame_data[6] = 0x00;
      frame_data[7] = 0x00;
  
      frame_id = 0x0000;
  
      length = 0;
      
      rx_status = CAN.readStatus();
      
      if (rx_status & 0x40 == 0x40) {
       CAN.readDATA_ff_0(&length,frame_data,&frame_id);
//       Serial.println("Frame ID");
//       Serial.println(frame_id);
      } else if (rx_status & 0x80 == 0x80) {
       CAN.readDATA_ff_1(&length,frame_data,&frame_id);
       //state = 0;
//       Serial.println("80");
      }
      
      
      
       if( frame_id == 0x05A4EEEE) //84192205 )
      {
//        Serial.println("Received a request for information");
        
        state = 0;
      }
      
//      for (i=0;i<8;i++) {
//        Serial.print(" / ");
//        Serial.print(frame_data[i],HEX);
//      }

//      Serial.println();
      break;      
  }
  
//  if (pin == LOW) {
//    pin = HIGH;
//  } else {
//    pin = LOW;
//  }  
        
  //delay(500);

}

int getUltrasonicMeasurement()
{
  SRF02::update();
  return srf02[0].read();
}

unsigned int getInfraredMeasurement1()
{
  unsigned int distance, reading, i;
  
  for(i=0;i<100;i++)
  {
    reading += analogRead(3);
  }
  reading /= 1000;
  
  distance = (0x3f - reading)/2;
   
  return distance;  
}

unsigned int getInfraredMeasurement2()
{
  unsigned int distance, reading, i;
  
  for(i=0;i<100;i++)
  {
    reading += analogRead(2);
  }
  reading /= 1000;
  
  distance = (0x3f - reading)/2;

  return distance;
}
