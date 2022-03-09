#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg1;
struct can_frame canMsg2;
struct can_frame canMsg3;
MCP2515 mcp2515(10);


const int ignitionPin = A0;
const int ignitionInterrupt = 0;
const unsigned int pulsesPerRev = 1;

unsigned long lastPulseTime = 0;
unsigned long rpm = 0;
int rpm_int;
int rpm_to_disp;




void setup() {

  Serial.begin(9600);
  pinMode(ignitionPin, INPUT);
  attachInterrupt(ignitionInterrupt, &ignitionIsr, RISING);
  canMsg1.can_id  = 0x18DA10F1;
  canMsg1.can_dlc = 8;
  canMsg1.data[0] = 0x06;
  canMsg1.data[1] = 0x41;
  canMsg1.data[2] = 0x0D;
  canMsg1.data[3] = 0x00;
  canMsg1.data[4] = 0x00;
  canMsg1.data[5] = 0x00;
  canMsg1.data[6] = 0x00;
  canMsg1.data[7] = 0x00;

  canMsg2.can_id  = 0x18DA10F1;
  canMsg2.can_dlc = 8;
  canMsg2.data[0] = 0x02;
  canMsg2.data[1] = 0x41;
  canMsg2.data[2] = 0x04;
  canMsg2.data[3] = 0x00;
  canMsg2.data[4] = 0x00;
  canMsg2.data[5] = 0x00;
  canMsg2.data[6] = 0x00;
  canMsg2.data[7] = 0x00;

  canMsg3.can_id  = 0x18DA10F1;
  canMsg3.can_dlc = 8;
  canMsg3.data[0] = 0x03;
  canMsg3.data[1] = 0x41;
  canMsg3.data[2] = 0x05;
  canMsg3.data[3] = 0x61;
  canMsg3.data[4] = 0x00;
  canMsg3.data[5] = 0x00;
  canMsg3.data[6] = 0x00;
  canMsg3.data[7] = 0x00;

  SPI.begin();
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  
  Serial.println("Example: Write to CAN");
  
}

void ignitionIsr()
{
  unsigned long now = micros();
  unsigned long interval = now - lastPulseTime;
  if (interval > 5000)
  {
    rpm = 60000000UL / (interval * pulsesPerRev);
    lastPulseTime = now;
  }

}



void loop() {

  Serial.println("\n");

  noInterrupts();
  rpm_to_disp = int(rpm);

  interrupts();



  if ((rpm_to_disp > 0) && (rpm_to_disp < 10000)) {

    Serial.println(rpm_to_disp);
    mcp2515.sendMessage(&canMsg1);
    mcp2515.sendMessage(&canMsg2);
    mcp2515.sendMessage(&canMsg3);
    
    Serial.println("Messages sent");

    delay(100);

  
  }

}


/*
  START	ID		RTR	IDE	RESERVED	LENGTH	DATA
  1  	18DAF110x	Rx 	d 	8 		06	41 20 80 07 A0 11 00

  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	0D	0	0	0	0
  41	4	0	0	0	0
  41	5	61	0	0	0
  41	0C	0	0	0	0
  41	0D	0	0	0	0
  41	0E	80	0	0	0
  41	11	28	0	0	0
  41	4	0	0	0	0
  41	5	61	0	0	0
  41	0C	0	0	0	0
  41	0D	0	0	0	0
  41	0E	80	0	0	0


  ID FRAME DIAGNOSTIC REQUEST ECM: 18DA10F1x
  0	5	0	0	0	0	0

*/
