    //This Arduino UNO Sketch requires the Seeed CAN-BUS Shield Libraries
    //https://github.com/yexiaobo-seeedstudio/CAN_BUS_Shield
    #include <SPI.h>
    #include <mcp_can.h>



    INT32U canId = 0x000;

    //2024   4   65   12   18   248   185   147
     
long timelast;
        unsigned char rndCoolantTemp=80;
        unsigned char rndRPM=34;
        unsigned char rndSpeed=100;
        unsigned char rndIAT=0;
        unsigned char rndMAF=0;
        unsigned char rndAmbientAirTemp=20;
        unsigned char rndCAT1Temp=255;
    unsigned char len = 0;
    unsigned char buf[8];
    char str[20];

    String BuildMessage="";
    int MSGIdentifier=0;

    void setup()
    {
        Serial.begin(38400);

    START_INIT:

        if(CAN_OK == CAN.begin(CAN_500KBPS))
        {
            Serial.println("CAN BUS Shield init ok!");
        }
        else
        {
            Serial.println("CAN BUS Shield init fail");
            Serial.println("Init CAN BUS Shield again");
            delay(100);
            goto START_INIT;
        }
    }


    void loop()
    {
       if ((millis()-timelast)>5000)
 {
 
 timelast=millis(); 
      rndCoolantTemp=random(1,200);
      rndRPM=random(1,155);
      rndSpeed=random(0,255);
      rndIAT=random(0,255);
      rndMAF=random(0,255);
      rndAmbientAirTemp=random(0,200);
      rndCAT1Temp=random(1,55);
 }
        //GENERAL ROUTINE
        unsigned char SupportedPID[8] =       {255,255,255,255,255,255,255,255};
        unsigned char MilCleared[7] =         {4, 65, 63, 34, 224, 185, 147};
       
        //SENSORS
        unsigned char CoolantTemp[7] =        {4, 65, 5, rndCoolantTemp, 0, 185, 147}; 
        unsigned char rpm[7] =                {4, 65, 12, rndRPM, 224, 185, 147};
        unsigned char vspeed[7] =             {4, 65, 13, rndSpeed, 224, 185, 147};
        unsigned char IATSensor[7] =          {4, 65, 15, rndIAT, 0, 185, 147};
        unsigned char MAFSensor[7] =          {4, 65, 16, rndMAF, 0, 185, 147};
        unsigned char AmbientAirTemp[7] =     {4, 65, 70, rndAmbientAirTemp, 0, 185, 147};
        unsigned char CAT1Temp[7] =           {4, 65, 60, rndCAT1Temp, 224, 185, 147};
        unsigned char CAT2Temp[7] =           {4, 65, 61, rndCAT1Temp, 224, 185, 147};
        unsigned char CAT3Temp[7] =           {4, 65, 62, rndCAT1Temp, 224, 185, 147};
        unsigned char CAT4Temp[7] =           {4, 65, 63, rndCAT1Temp, 224, 185, 147};
       
        if(CAN_MSGAVAIL == CAN.checkReceive()) 
        {
         
          CAN.readMsgBuf(&len, buf);
            canId = CAN.getCanId();
            Serial.print("<");Serial.print(canId);Serial.print(",");

            for(int i = 0; i<len; i++)
            { 
              BuildMessage = BuildMessage + buf[i] + ",";
            }
            Serial.println(BuildMessage);
           
            //Check wich message was received.
            if(BuildMessage=="2,1,0,0,0,0,0,0,") {CAN.sendMsgBuf(0x7E8, 0, 8, SupportedPID);}
            if(BuildMessage=="2,1,1,0,0,0,0,0,") {CAN.sendMsgBuf(0x7E8, 0, 7, MilCleared);}
           
            //SEND SENSOR STATUSES
            if(BuildMessage=="2,1,5,0,0,0,0,0,") {CAN.sendMsgBuf(0x7E8, 0, 7, CoolantTemp);}
            if(BuildMessage=="2,1,12,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, rpm);}
            if(BuildMessage=="2,1,13,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, vspeed);}
            if(BuildMessage=="2,1,15,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, IATSensor);}
            if(BuildMessage=="2,1,16,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, MAFSensor);}
            if(BuildMessage=="2,1,70,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, AmbientAirTemp);}
            if(BuildMessage=="2,1,60,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, CAT1Temp);}
            if(BuildMessage=="2,1,61,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, CAT2Temp);}
            if(BuildMessage=="2,1,62,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, CAT3Temp);}
            if(BuildMessage=="2,1,63,0,0,0,0,0,"){CAN.sendMsgBuf(0x7E8, 0, 7, CAT4Temp);}

            BuildMessage="";

           

        }
       

    }

