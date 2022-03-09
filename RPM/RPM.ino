
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
  
 
  
}

void ignitionIsr()
{
  unsigned long now = micros();
  unsigned long interval = now - lastPulseTime;
  if (interval > 5000)
  {
     rpm = 60000000UL/(interval * pulsesPerRev);
     lastPulseTime = now;
  }  
  
}

    

void loop() {
  
  
  Serial.write("\n"); 
  
  noInterrupts();
  rpm_to_disp=int(rpm);
  
  interrupts();
  

   
  if ((rpm_to_disp>0) && (rpm_to_disp<10000)){

   Serial.println(rpm_to_disp);
    
}

}
