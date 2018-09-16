//Code for the Chaos Bot
//Written for the Arduino Uno using the Polulu Dual VNH5019 Motor Driver Shield
//https://www.pololu.com/product/2502
//Stick Direction: Ch1 = Left Stick L/R, Ch2 = Left Stick F/B, Ch3 = Right Stick L/R ,Ch4 = Right Stick F/B


#include <EnableInterrupt.h>

#define SERIAL_PORT_SPEED 57600
#define RC_NUM_CHANNELS  6

#define RC_CH1  0
#define RC_CH2  1
#define RC_CH3  2
#define RC_CH4  3


//CONTROLLER RECEIVER INPUT PIN CONNECTION NUMBERS IN ARDUINO BOARD

#define RC_CH1_INPUT A0
#define RC_CH2_INPUT A1
#define RC_CH3_INPUT A2
#define RC_CH4_INPUT A3


//MOTOR SPEED AND DIRECTION PIN CONNECTION NUMBERS IN ARDUINO BOARD

#define RightMotorPWM 10
#define RightMotorF  7
#define RightMotorB 8

#define LeftMotorPWM 9
#define LeftMotorF  2
#define LeftMotorB  4

#define solenoidpin  13

bool solenoidpushed;

int LeftStickFB = 0;
int LeftStickLR = 0;

int RightStickFB = 0;
int RightStickLR = 0;


uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();
}

void calc_input(uint8_t channel, uint8_t input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    rc_start[channel] = micros();
  } else {
    uint16_t rc_compare = (uint16_t)(micros() - rc_start[channel]);
    rc_shared[channel] = rc_compare;
  }
}

void calc_ch1() { calc_input(RC_CH1, RC_CH1_INPUT); }
void calc_ch2() { calc_input(RC_CH2, RC_CH2_INPUT); }
void calc_ch3() { calc_input(RC_CH3, RC_CH3_INPUT); }
void calc_ch4() { calc_input(RC_CH4, RC_CH4_INPUT); }


void setup() {
  Serial.begin(SERIAL_PORT_SPEED);

  pinMode(RC_CH1_INPUT, INPUT);
  pinMode(RC_CH2_INPUT, INPUT);
  pinMode(RC_CH3_INPUT, INPUT);
  pinMode(RC_CH4_INPUT, INPUT);
  

  enableInterrupt(RC_CH1_INPUT, calc_ch1, CHANGE);
  enableInterrupt(RC_CH2_INPUT, calc_ch2, CHANGE);
  enableInterrupt(RC_CH3_INPUT, calc_ch3, CHANGE);
  enableInterrupt(RC_CH4_INPUT, calc_ch4, CHANGE);
 

  pinMode(RightMotorPWM, OUTPUT);
  pinMode(RightMotorF, OUTPUT);
  pinMode(RightMotorB, OUTPUT);
  
  pinMode(LeftMotorPWM, OUTPUT);
  pinMode(LeftMotorF, OUTPUT);
  pinMode(LeftMotorB, OUTPUT);

  digitalWrite(LeftMotorF, HIGH);
  digitalWrite(RightMotorF, HIGH);
  digitalWrite(LeftMotorB, LOW);
  digitalWrite(RightMotorB, LOW);
  analogWrite(LeftMotorPWM, 0);
  analogWrite(RightMotorPWM, 0);

  pinMode(solenoidpin, OUTPUT);
  digitalWrite(solenoidpin, LOW);
  bool solenoidpushed = true;
 
}

//BOT MOVEMENT FUNCTIONS


void MovementForward(int PwmVal){
     //Serial.print("Forward"); //Debugging only
     //Serial.print(PwmVal); //Debugging only
     //Serial.print("\t");
     
     
    /* digitalWrite(LeftMotorF, HIGH);
     digitalWrite(RightMotorF, HIGH);
     digitalWrite(LeftMotorB, LOW);
     digitalWrite(RightMotorB, LOW);
     analogWrite(LeftMotorPWM, PwmVal);
     analogWrite(RightMotorPWM, PwmVal);*/

     digitalWrite(LeftMotorF, LOW);
     digitalWrite(RightMotorF, LOW);
     digitalWrite(LeftMotorB, HIGH);
     digitalWrite(RightMotorB, HIGH);
     analogWrite(LeftMotorPWM, PwmVal);
     analogWrite(RightMotorPWM, PwmVal);
  
}

void MovementReverse(int PwmVal){
     //Serial.print("Reverse"); //Debugging only
     //Serial.print(PwmVal); //Debugging only
     //Serial.print("\t");
     
     
    /* digitalWrite(LeftMotorF, LOW);
     digitalWrite(RightMotorF, LOW);
     digitalWrite(LeftMotorB, HIGH);
     digitalWrite(RightMotorB, HIGH);
     analogWrite(LeftMotorPWM, PwmVal);
     analogWrite(RightMotorPWM, PwmVal);*/

     digitalWrite(LeftMotorF, HIGH);
     digitalWrite(RightMotorF, HIGH);
     digitalWrite(LeftMotorB, LOW);
     digitalWrite(RightMotorB, LOW);
     analogWrite(LeftMotorPWM, PwmVal);
     analogWrite(RightMotorPWM, PwmVal);
  
}

void MovementLeftSpin(int PwmVal){
     //Serial.print("Left"); //Debugging only
     //Serial.print(PwmVal); //Debugging only
     //Serial.print("\t");
     
     
     digitalWrite(LeftMotorF, LOW);
     digitalWrite(RightMotorF, HIGH);
     digitalWrite(LeftMotorB, HIGH);
     digitalWrite(RightMotorB, LOW);
     analogWrite(LeftMotorPWM, PwmVal);
     analogWrite(RightMotorPWM, PwmVal);
}

void MovementRightSpin(int PwmVal){
     //Serial.print("Right "); //Debugging only
     //Serial.print(PwmVal); //Debugging only
     //Serial.print("\t");
     
     
     digitalWrite(LeftMotorF, HIGH);
     digitalWrite(RightMotorF, LOW);
     digitalWrite(LeftMotorB, LOW);
     digitalWrite(RightMotorB, HIGH);
     analogWrite(LeftMotorPWM, PwmVal);
     analogWrite(RightMotorPWM, PwmVal);
}

void Stationary(void){
     //Serial.print("Right "); //Debugging only
     //Serial.print(PwmVal); //Debugging only
     //Serial.print("\t");
     
     
     digitalWrite(LeftMotorF, HIGH);
     digitalWrite(RightMotorF, HIGH);
     digitalWrite(LeftMotorB, LOW);
     digitalWrite(RightMotorB, LOW);
     analogWrite(LeftMotorPWM, 0);
     analogWrite(RightMotorPWM, 0);
}

void ForwardLeftTurn(int PwmValL, int PwmValR){
     /*digitalWrite(LeftMotorF, HIGH);
     digitalWrite(RightMotorF, HIGH);
     digitalWrite(LeftMotorB, LOW);
     digitalWrite(RightMotorB, LOW);
     analogWrite(LeftMotorPWM, PwmValL);
     analogWrite(RightMotorPWM, PwmValR);  */
     digitalWrite(LeftMotorF, LOW);
     digitalWrite(RightMotorF, LOW);
     digitalWrite(LeftMotorB, HIGH);
     digitalWrite(RightMotorB, HIGH);
     analogWrite(LeftMotorPWM, PwmValR);
     analogWrite(RightMotorPWM, PwmValL);
}

void ForwardRightTurn(int PwmValL, int PwmValR){
     /*digitalWrite(LeftMotorF, HIGH);
     digitalWrite(RightMotorF, HIGH);
     digitalWrite(LeftMotorB, LOW);
     digitalWrite(RightMotorB, LOW);
     analogWrite(LeftMotorPWM, PwmValL);
     analogWrite(RightMotorPWM, PwmValR);  */
     digitalWrite(LeftMotorF, LOW);
     digitalWrite(RightMotorF, LOW);
     digitalWrite(LeftMotorB, HIGH);
     digitalWrite(RightMotorB, HIGH);
     analogWrite(LeftMotorPWM, PwmValR);
     analogWrite(RightMotorPWM, PwmValL);
}

void BackwardLeftTurn(int PwmValL, int PwmValR){
     /*digitalWrite(LeftMotorF, LOW);
     digitalWrite(RightMotorF, LOW);
     digitalWrite(LeftMotorB, HIGH);
     digitalWrite(RightMotorB, HIGH);
     analogWrite(LeftMotorPWM, PwmValL);
     analogWrite(RightMotorPWM, PwmValR);*/
     digitalWrite(LeftMotorF, HIGH);
     digitalWrite(RightMotorF, HIGH);
     digitalWrite(LeftMotorB, LOW);
     digitalWrite(RightMotorB, LOW);
     analogWrite(LeftMotorPWM, PwmValR);
     analogWrite(RightMotorPWM, PwmValL);  
}

void BackwardRightTurn(int PwmValL, int PwmValR){
     /*digitalWrite(LeftMotorF, LOW);
     digitalWrite(RightMotorF, LOW);
     digitalWrite(LeftMotorB, HIGH);
     digitalWrite(RightMotorB, HIGH);
     analogWrite(LeftMotorPWM, PwmValL);
     analogWrite(RightMotorPWM, PwmValR); */

      digitalWrite(LeftMotorF, HIGH);
     digitalWrite(RightMotorF, HIGH);
     digitalWrite(LeftMotorB, LOW);
     digitalWrite(RightMotorB, LOW);
     analogWrite(LeftMotorPWM, PwmValR);
     analogWrite(RightMotorPWM, PwmValL);  
}

void shootingmechanism(){
  //Pull Solenoid
  if(rc_values[RC_CH3] < 1100 && solenoidpushed){
    digitalWrite(solenoidpin, HIGH);
    delay(200);
    digitalWrite(solenoidpin, LOW);
    //t.pulse(solenoidpin, 5000, LOW);
    solenoidpushed = false;
  }

  //Push solenoid
  if(rc_values[RC_CH3] > 1100 && !solenoidpushed){
    digitalWrite(solenoidpin, HIGH);
    delay(200);
    digitalWrite(solenoidpin, LOW);
    //t.pulse(solenoidpin, 5000, HIGH);
    solenoidpushed = true;
  }
}

void loop() {
  rc_read_values();

  Serial.print("CH1:"); Serial.print(rc_values[RC_CH1]); Serial.print("\t");
  Serial.print("CH2:"); Serial.print(rc_values[RC_CH2]); Serial.print("\t");
  Serial.print("CH3:"); Serial.print(rc_values[RC_CH3]); Serial.print("\t");
  Serial.print("CH4:"); Serial.println(rc_values[RC_CH4]); 

  //Moves values of controller's position to dedicated variables
  //LeftStickFB = rc_values[RC_CH1];
  //LeftStickLR = rc_values[RC_CH2];

  RightStickFB = rc_values[RC_CH2];
  RightStickLR = rc_values[RC_CH1];

  if(rc_values[RC_CH1]==0 && rc_values[RC_CH2]==0 && rc_values[RC_CH3]==0 && rc_values[RC_CH4]==0){
    Stationary();
  }
  
  //Forward Movement
  else if(RightStickFB > 1540 && RightStickLR < 1650 && RightStickLR > 1350 ){
    MovementForward(map(RightStickFB, 1540, 2000, 0, 170));
  }

  //Forward Left Turn
  else if(RightStickFB > 1540 && RightStickLR < 1350 ){
    ForwardLeftTurn(map(RightStickFB, 1540, 2000, 0, 64), map(RightStickLR, 1350, 1180, 0, 128));
  }

  //Forward Right Turn
  else if(RightStickFB > 1540 && RightStickLR > 1650 ){
    ForwardRightTurn(map(RightStickLR, 1650, 1900, 0, 128), map(RightStickFB, 1540, 2000, 0, 64));
  }

  //Backward Left Turn
  else if(RightStickFB < 1470 && RightStickLR < 1350 ){
    BackwardLeftTurn(map(RightStickFB, 1470, 1000, 0, 64), map(RightStickLR, 1350, 1180, 0, 128));
  }

  //Backward Right Turn
  else if(RightStickFB < 1470 && RightStickLR  > 1650){
    BackwardRightTurn(map(RightStickLR, 1650, 1900, 0, 128), map(RightStickFB, 1470, 1000, 0, 64));
  }

  //Reverse Movement
  else if(RightStickFB < 1470 && RightStickLR < 1650 && RightStickLR > 1350){
    MovementReverse(map(RightStickFB, 1470, 1000, 0, 170));
    
  }

  //Left Spinning
  else if(RightStickLR < 1470 && RightStickFB < 1650 && RightStickFB > 1350){
    MovementLeftSpin(map(RightStickLR, 1470, 1180, 0, 128));
  }

  //Right Spinning
  else if(RightStickLR > 1540 && RightStickFB < 1650 && RightStickFB > 1350){
    MovementRightSpin(map(RightStickLR, 1540, 1900, 0, 128));
  }

  //stationary
  else if(RightStickLR < 1540 && RightStickLR > 1470 && RightStickFB < 1540 && RightStickFB > 1470){
      Stationary(); 
  }

  shootingmechanism();

  //Stationary (does not work properly)
 /* if((RightStickLR < 1510 || RightStickLR > 1490) && (RightStickFB < 1510 || RightStickFB > 1490)){
    
     analogWrite(LeftMotorPWM, 0);
     analogWrite(RightMotorPWM, 0);
     digitalWrite(LeftMotorF, LOW);
     digitalWrite(RightMotorF, LOW);
     digitalWrite(LeftMotorB, LOW);
     digitalWrite(RightMotorB, LOW);
    
  }

*/

     
}
