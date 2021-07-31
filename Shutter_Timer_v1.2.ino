
//Douwe Krooshof July 2021, building on Cameradactyl's shutter tester code.
//This is shutter tester for horizontally travelling focal plane shutters.
//The tester uses two lasers and sensors to measure the speed of the curtains
//and the relative exposure across the frame.

int SensorDist = 25;                  // milimeter distance between the laser beams at the shutter plane.

// set-up of variables for timing
long StartTimeCurt1; 
long StopTimeCurt1;
long StartTimeCurt2; 
long StopTimeCurt2;

// set-up of logical parameters
int Fired = 0;
int RisingFlagSensor1 = 0;  
int RisingFlagSensor2 = 0;
int FallingFlagSensor1 = 0; 
int FallingFlagSensor2 = 0;

void setup() {                                                 
  Serial.begin(9600);
  Serial.print("Starting");  
  Serial.println();   
  // interrupts change logical parameters when voltage of the sensors changes in either direction.                                   
  attachInterrupt(digitalPinToInterrupt(2), CLOCKSENSOR1, CHANGE);   
  attachInterrupt(digitalPinToInterrupt(3), CLOCKSENSOR2, CHANGE);
}

void loop() { 
  //curtain 1                                                
  if(RisingFlagSensor1 == 1){        // Curtain 1 opening the shutter, sensor 1 exposed to laser 1, voltage goes up.               
    StartTimeCurt1 = micros();       // set the variable StartTimeCurt1 to current microseconds 
    RisingFlagSensor1 = 0;           // reset the RisingFlagSensor1 flag to 0, so that this function isnt called again until the shutter actually fires
  }
  if(RisingFlagSensor2 == 1){        // Curtain 1 still opening the shutter, sensor 2 exposed to laser 2, voltage goes up.
    StopTimeCurt1 = micros();        // set the variable Stop to current microseconds
    RisingFlagSensor2 = 0;           // reset the RisingFlagSensor2 to 0
  }
  
  //curtain 2
  if(FallingFlagSensor1 == 1){       // Curtain 2 closing the shutter, sensor 1 covered, voltage goes down.               
    StartTimeCurt2 = micros();       // set the variable StartTimeCurt2 to current microseconds
    FallingFlagSensor1 = 0;          // reset the FallingFlagSensor1 flag to 0
  }                      
  if(FallingFlagSensor2 == 1){       // Curtain 2 still closing the shutter, sensor 2 covered, voltage goes down.
    StopTimeCurt2 = micros();        // set the variable StopTimeCurt2 to current microseconds
    FallingFlagSensor2 = 0;          // reset the FallingFlagSensor2 to 0.
    Fired = 1;                       // set the fired flag to 1, triggering calculation and display.
  }

  
  if(Fired == 1){                    // if the flag Fired = 1, print this information to the serial monitor"
    // Curtain travel times:
    long TravelTimeCurt1 = (StopTimeCurt1 - StartTimeCurt1);     
    long TravelTimeCurt2 = (StopTimeCurt2 - StartTimeCurt2);     

    // Curtain speed in meter per second
    float Curt1Speed = (float)(SensorDist*1000)/TravelTimeCurt1;
    float Curt2Speed = (float)(SensorDist*1000)/TravelTimeCurt2;

    // Time for which the shutter is open at sensor 1 and 2 in microseconds
    long OpenTimeAtSensor1 = (StartTimeCurt2 - StartTimeCurt1);
    long OpenTimeAtSensor2 = (StopTimeCurt2 - StopTimeCurt1);

    // Time for which the shutter is open at sensor 1 and 2 in seconds
    float Sens1SS = (float)OpenTimeAtSensor1/1000000;
    float Sens2SS = (float)OpenTimeAtSensor2/1000000;

    // Inverse of the opening time in seconds for display
    float InvSens1SS = 1/Sens1SS; 
    float InvSens2SS = 1/Sens2SS;

    Serial.print("Shutter speed at sensor 1: 1/");
    Serial.println(InvSens1SS);             

    Serial.print("Shutter speed at sensor 2: 1/");
    Serial.println(InvSens2SS);             

    Serial.print("First curtain speed (m/s): ");
    Serial.println(Curt1Speed);

    Serial.print("Second curtain speed (m/s): ");
    Serial.println(Curt2Speed);

    // reset
    StartTimeCurt1 = 0; 
    StopTimeCurt1 = 0;
    StartTimeCurt2 = 0; 
    StopTimeCurt2 = 0;
    Serial.print("Shutter cycle completed!");
    Serial.println();   
    Serial.println();                       
    Fired = 0;                          
  } 
}

void CLOCKSENSOR1(){                     // interrupt function for sensor 1 on digital pin 2
  if(digitalRead(2) == HIGH){
    RisingFlagSensor1 = 1; 
  }         
  if(digitalRead(2) == LOW){        
    FallingFlagSensor1 = 1;
  }
}

void CLOCKSENSOR2(){                     // interrupt function for sensor 2 on digital pin 3
  if(digitalRead(3) == HIGH){
    RisingFlagSensor2 = 1;               
  }
  if(digitalRead(3) == LOW){        
    FallingFlagSensor2 =1;
  }
}
