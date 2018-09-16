#include <EnableInterrupt.h>

#define SERIAL_PORT_SPEED 57600
#define RC_NUM_CHANNELS  6

#define RC_CH1  0
#define RC_CH2  1
#define RC_CH3  2
#define RC_CH4  3
#define RC_CH5  4
#define RC_CH6  5

//CONTROLLER RECEIVER INPUT PIN CONNECTION NUMBERS IN ARDUINO BOARD

#define RC_CH1_INPUT  A0
#define RC_CH2_INPUT  A1
#define RC_CH3_INPUT  A2
#define RC_CH4_INPUT  A3
#define RC_CH5_INPUT  A4
#define RC_CH6_INPUT  A5

//MOTOR SPEED AND DIRECTION PIN CONNECTION NUMBERS IN ARDUINO BOARD

#define FrontRightMotorPWM  9
#define FrontRightMotorDIR  8

#define FrontLeftMotorPWM  5
#define FrontLeftMotorDIR  4

#define RearRightMotorPWM  6
#define RearRightMotorDIR  7

#define RearLeftMotorPWM  3
#define RearLeftMotorDIR  2

//Actuator and switch pins
#define actuatordir  13
#define actuatorpwm  10
#define switchpin  0

#define LEDdir  12
#define LEDpwm  11

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
void calc_ch5() { calc_input(RC_CH5, RC_CH5_INPUT); }
void calc_ch6() { calc_input(RC_CH6, RC_CH6_INPUT); }

bool shootingposition;
bool switchposition;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_PORT_SPEED);

  pinMode(RC_CH1_INPUT, INPUT);
  pinMode(RC_CH2_INPUT, INPUT);
  pinMode(RC_CH3_INPUT, INPUT);
  pinMode(RC_CH4_INPUT, INPUT);
  pinMode(RC_CH5_INPUT, INPUT);
  pinMode(RC_CH6_INPUT, INPUT);

  enableInterrupt(RC_CH1_INPUT, calc_ch1, CHANGE);
  enableInterrupt(RC_CH2_INPUT, calc_ch2, CHANGE);
  enableInterrupt(RC_CH3_INPUT, calc_ch3, CHANGE);
  enableInterrupt(RC_CH4_INPUT, calc_ch4, CHANGE);
  enableInterrupt(RC_CH5_INPUT, calc_ch5, CHANGE);
  enableInterrupt(RC_CH6_INPUT, calc_ch6, CHANGE);

  pinMode(FrontRightMotorPWM, OUTPUT);
  pinMode(FrontRightMotorDIR, OUTPUT);
  pinMode(FrontLeftMotorPWM, OUTPUT);
  pinMode(FrontLeftMotorDIR, OUTPUT);
  pinMode(RearRightMotorPWM, OUTPUT);
  pinMode(RearRightMotorDIR, OUTPUT);
  pinMode(RearLeftMotorPWM, OUTPUT);
  pinMode(RearLeftMotorDIR, OUTPUT);

  analogWrite(FrontRightMotorPWM, 0);
  analogWrite(FrontLeftMotorPWM, 0);
  analogWrite(RearRightMotorPWM, 0);
  analogWrite(RearLeftMotorPWM, 0);
  
  pinMode(actuatordir, OUTPUT);
  pinMode(actuatorpwm, OUTPUT);
  pinMode(switchpin, INPUT);
  pinMode(LEDdir, OUTPUT);
  pinMode(LEDpwm, OUTPUT);
  digitalWrite(actuatordir, HIGH);
  shootingposition = false; 
}

//Bot Movement Function

int pwmchannel1 = 0;
int pwmchannel2 = 0;
int pwmchannel3 = 0;
int pwmchannel4 = 0;

void botmovement(){
  
  if( rc_values[RC_CH2] > 1470 && rc_values[RC_CH2] < 1520 && rc_values[RC_CH1] > 1470 && rc_values[RC_CH1] < 1520 && rc_values[RC_CH4] > 1470 && rc_values[RC_CH4] < 1520){
    //MIGHT NEED TO MAKE PWNCHANNELS 0 HERE OR IN THE BEGINNING OF THE FUNCTION
    analogWrite(FrontRightMotorPWM, 0);
    analogWrite(FrontLeftMotorPWM, 0);
    analogWrite(RearRightMotorPWM, 0);
    analogWrite(RearLeftMotorPWM, 0);
    }

//Spinning Left
    else if( rc_values[RC_CH2] > 1470 && rc_values[RC_CH2] < 1520 && rc_values[RC_CH1] > 1470 && rc_values[RC_CH1] < 1520 && rc_values[RC_CH4] < 1470){
    pwmchannel4 = map(rc_values[RC_CH4], 1470, 1160, 0, 255);
    digitalWrite(FrontRightMotorDIR, HIGH);
    digitalWrite(FrontLeftMotorDIR, LOW);
    digitalWrite(RearRightMotorDIR, HIGH);
    digitalWrite(RearLeftMotorDIR, LOW);
    analogWrite(FrontRightMotorPWM, pwmchannel4);
    analogWrite(FrontLeftMotorPWM, pwmchannel4);
    analogWrite(RearRightMotorPWM, pwmchannel4);
    analogWrite(RearLeftMotorPWM, pwmchannel4);
    }

    //Spinning Right
    else if( rc_values[RC_CH2] > 1470 && rc_values[RC_CH2] < 1520 && rc_values[RC_CH1] > 1470 && rc_values[RC_CH1] < 1520 && rc_values[RC_CH4] > 1520){
    pwmchannel4 = map(rc_values[RC_CH4], 1520, 1870, 0, 255);
    digitalWrite(FrontRightMotorDIR, LOW);
    digitalWrite(FrontLeftMotorDIR, HIGH);
    digitalWrite(RearRightMotorDIR, LOW);
    digitalWrite(RearLeftMotorDIR, HIGH);
    analogWrite(FrontRightMotorPWM, pwmchannel4);
    analogWrite(FrontLeftMotorPWM, pwmchannel4);
    analogWrite(RearRightMotorPWM, pwmchannel4);
    analogWrite(RearLeftMotorPWM, pwmchannel4);
    }
 
 //Forward ; all motors spin forward

  else if(rc_values[RC_CH2] > 1520 && rc_values[RC_CH1] > 1350 && rc_values[RC_CH1] < 1650 && rc_values[RC_CH4] > 1470 && rc_values[RC_CH4] < 1520){
    pwmchannel2 = map(rc_values[RC_CH2], 1520, 1790, 0, 255);
    //pwmchannel2 = (RC_CH2/1996)*255;
    digitalWrite(FrontRightMotorDIR, HIGH);
    digitalWrite(FrontLeftMotorDIR, HIGH);
    digitalWrite(RearRightMotorDIR, HIGH);
    digitalWrite(RearLeftMotorDIR, HIGH);
    analogWrite(FrontRightMotorPWM, pwmchannel2);
    analogWrite(FrontLeftMotorPWM, pwmchannel2);
    analogWrite(RearRightMotorPWM, pwmchannel2);
    analogWrite(RearLeftMotorPWM, pwmchannel2);
    
    }

//Backward
//(rc_values[RC_CH2] < 1500))
  else if (rc_values[RC_CH2] < 1470 && rc_values[RC_CH1] > 1350 && rc_values[RC_CH1] < 1650 && rc_values[RC_CH4] > 1470 && rc_values[RC_CH4] < 1520){
    pwmchannel2 = map(rc_values[RC_CH2], 1470, 1170, 0, 255);
    //pwmchannel2 = (RC_CH2 - 1470) * (255 - 0) / (1000 - 1470) + 0;
    digitalWrite(FrontRightMotorDIR, LOW);
    digitalWrite(FrontLeftMotorDIR, LOW);
    digitalWrite(RearRightMotorDIR, LOW);
    digitalWrite(RearLeftMotorDIR, LOW);
    analogWrite(FrontRightMotorPWM, pwmchannel2);
    analogWrite(FrontLeftMotorPWM, pwmchannel2);
    analogWrite(RearRightMotorPWM, pwmchannel2);
    analogWrite(RearLeftMotorPWM, pwmchannel2);
    
    }

//Turning Left Forward
  else if (rc_values[RC_CH2] > 1520 && rc_values[RC_CH1] > 1350 && rc_values[RC_CH1] < 1650 && rc_values[RC_CH4] < 1470){
    pwmchannel2 = map(rc_values[RC_CH2], 1520, 1790, 0, 255);
    pwmchannel4 = map(rc_values[RC_CH4], 1470, 1160, 0, 255);
    //pwmchannel2 = (RC_CH2 - 1470) * (255 - 0) / (1000 - 1470) + 0;
    digitalWrite(FrontRightMotorDIR, HIGH);
    digitalWrite(FrontLeftMotorDIR, HIGH);
    digitalWrite(RearRightMotorDIR, HIGH);
    digitalWrite(RearLeftMotorDIR, HIGH);
    analogWrite(FrontRightMotorPWM, pwmchannel2);
    analogWrite(FrontLeftMotorPWM, pwmchannel2);
    analogWrite(RearRightMotorPWM, pwmchannel4);
    analogWrite(RearLeftMotorPWM, 0);
    
    }

//Turning Right Forward
  else if (rc_values[RC_CH2] > 1520 && rc_values[RC_CH1] > 1350 && rc_values[RC_CH1] < 1650 && rc_values[RC_CH4] > 1520){
    pwmchannel2 = map(rc_values[RC_CH2], 1520, 1790, 0, 255);
    pwmchannel4 = map(rc_values[RC_CH4], 1520, 1870, 0, 255);
    //pwmchannel2 = (RC_CH2 - 1470) * (255 - 0) / (1000 - 1470) + 0;
    digitalWrite(FrontRightMotorDIR, HIGH);
    digitalWrite(FrontLeftMotorDIR, HIGH);
    digitalWrite(RearRightMotorDIR, HIGH);
    digitalWrite(RearLeftMotorDIR, HIGH);
    analogWrite(FrontRightMotorPWM, pwmchannel2);
    analogWrite(FrontLeftMotorPWM, pwmchannel2);
    analogWrite(RearRightMotorPWM, 0);
    analogWrite(RearLeftMotorPWM, pwmchannel4);
    
    }

//Turning Left Backward
  else if (rc_values[RC_CH2] < 1470 && rc_values[RC_CH1] > 1350 && rc_values[RC_CH1] < 1650 && rc_values[RC_CH4] < 1470){
    pwmchannel2 = map(rc_values[RC_CH2], 1470, 1170, 0, 255);
    pwmchannel4 = map(rc_values[RC_CH4], 1470, 1160, 0, 255);
    //pwmchannel2 = (RC_CH2 - 1470) * (255 - 0) / (1000 - 1470) + 0;
    digitalWrite(FrontRightMotorDIR, LOW);
    digitalWrite(FrontLeftMotorDIR, LOW);
    digitalWrite(RearRightMotorDIR, LOW);
    digitalWrite(RearLeftMotorDIR, LOW);
    analogWrite(FrontRightMotorPWM, pwmchannel2);
    analogWrite(FrontLeftMotorPWM, pwmchannel2);
    analogWrite(RearRightMotorPWM, pwmchannel4);
    analogWrite(RearLeftMotorPWM, 0);
    
    }

//Turning Right Backward
  else if (rc_values[RC_CH2] < 1470 && rc_values[RC_CH1] > 1350 && rc_values[RC_CH1] < 1650 && rc_values[RC_CH4] > 1520){
    pwmchannel2 = map(rc_values[RC_CH2], 1470, 1170, 0, 255);
    pwmchannel4 = map(rc_values[RC_CH4], 1520, 1870, 0, 255);
    //pwmchannel2 = (RC_CH2 - 1470) * (255 - 0) / (1000 - 1470) + 0;
    digitalWrite(FrontRightMotorDIR, LOW);
    digitalWrite(FrontLeftMotorDIR, LOW);
    digitalWrite(RearRightMotorDIR, LOW);
    digitalWrite(RearLeftMotorDIR, LOW);
    analogWrite(FrontRightMotorPWM, pwmchannel2);
    analogWrite(FrontLeftMotorPWM, pwmchannel2);
    analogWrite(RearRightMotorPWM, 0);
    analogWrite(RearLeftMotorPWM, pwmchannel4);
    
    }

//Strafe Left
  else if(rc_values[RC_CH1] < 1470 && rc_values[RC_CH2] > 1350 && rc_values[RC_CH2] < 1650){
    pwmchannel1 = map(rc_values[RC_CH1], 1470, 1170, 0, 255);
    digitalWrite(FrontRightMotorDIR, HIGH);
    digitalWrite(FrontLeftMotorDIR, LOW);
    digitalWrite(RearRightMotorDIR, LOW);
    digitalWrite(RearLeftMotorDIR, HIGH);
    analogWrite(FrontRightMotorPWM, pwmchannel1);
    analogWrite(FrontLeftMotorPWM, pwmchannel1);
    analogWrite(RearRightMotorPWM, pwmchannel1);
    analogWrite(RearLeftMotorPWM, pwmchannel1);
    
    }

//Strafe Right
  else if (rc_values[RC_CH1] > 1520 && rc_values[RC_CH2] > 1350 && rc_values[RC_CH2] < 1650){
    
    pwmchannel1 = map(rc_values[RC_CH1], 1520, 1900, 0, 255);
    digitalWrite(FrontRightMotorDIR, LOW);
    digitalWrite(FrontLeftMotorDIR, HIGH);
    digitalWrite(RearRightMotorDIR, HIGH);
    digitalWrite(RearLeftMotorDIR, LOW);
    analogWrite(FrontRightMotorPWM, pwmchannel1);
    analogWrite(FrontLeftMotorPWM, pwmchannel1);
    analogWrite(RearRightMotorPWM, pwmchannel1);
    analogWrite(RearLeftMotorPWM, pwmchannel1);
    }

//Forward-Left Diagonal
  else if(rc_values[RC_CH2] > 1650 && rc_values[RC_CH1] < 1350){
    
    //pwmchannel1 = map(RC_CH1, 1500, 1996, 0, 255);
    pwmchannel2 = map(rc_values[RC_CH2], 1650, 1790, 0, 255);
    //digitalWrite(FrontRightMotorDIR, HIGH);
    digitalWrite(FrontRightMotorDIR, HIGH);
    digitalWrite(RearLeftMotorDIR, HIGH);
    //digitalWrite(RearLeftMotorDIR, HIGH);
    analogWrite(FrontRightMotorPWM, pwmchannel2);
    analogWrite(FrontLeftMotorPWM, 0);
    analogWrite(RearRightMotorPWM, 0);
    analogWrite(RearLeftMotorPWM, pwmchannel2);
    }

//Forward-Right Diagonal
  else if(rc_values[RC_CH2] > 1650 && rc_values[RC_CH1] > 1650){
    
    //pwmchannel1 = map(RC_CH1, 1996, 1500, 0, 255);
    pwmchannel2 = map(rc_values[RC_CH2], 1650, 1790, 0, 255);
    digitalWrite(FrontLeftMotorDIR, HIGH);
    //digitalWrite(FrontLeftMotorDIR, LOW);
    //digitalWrite(RearRightMotorDIR, LOW);
    digitalWrite(RearRightMotorDIR, HIGH);
    analogWrite(FrontRightMotorPWM, 0);
    analogWrite(FrontLeftMotorPWM, pwmchannel2);
    analogWrite(RearRightMotorPWM, pwmchannel2);
    analogWrite(RearLeftMotorPWM, 0);
    }

//Backward-Left Diagonal
  else if(rc_values[RC_CH2] < 1350 && rc_values[RC_CH1] < 1350){
    //pwmchannel1 = map(RC_CH1, 1500, 1000, 0, 255);
    pwmchannel2 = map(rc_values[RC_CH2], 1350, 1170, 0, 255);
    digitalWrite(FrontRightMotorDIR, LOW);
    //digitalWrite(FrontLeftMotorDIR, LOW);
    //digitalWrite(RearRightMotorDIR, LOW);
    digitalWrite(RearLeftMotorDIR, LOW);
    analogWrite(FrontRightMotorPWM, 0);
    analogWrite(FrontLeftMotorPWM, pwmchannel2);
    analogWrite(RearRightMotorPWM, pwmchannel2);
    analogWrite(RearLeftMotorPWM, 0);
    
    }

//Backward-Right Diagonal
//(rc_values[RC_CH2] < 1495 && rc_values[RC_CH1] > 1510)
  else {
    
    //pwmchannel1 = map(RC_CH1, 1500, 1000, 0, 255);
    pwmchannel2 = map(rc_values[RC_CH2], 1350, 1170, 0, 255);
    digitalWrite(FrontRightMotorDIR, LOW);
    //digitalWrite(FrontLeftMotorDIR, LOW);
    //digitalWrite(RearRightMotorDIR, LOW);
    digitalWrite(RearLeftMotorDIR, LOW);
    analogWrite(FrontRightMotorPWM, pwmchannel2);
    analogWrite(FrontLeftMotorPWM, 0);
    analogWrite(RearRightMotorPWM, 0);
    analogWrite(RearLeftMotorPWM, pwmchannel2);
    }
   /*IF WE WANT REGULAR TURNING LIKE NORMAL CAR WHEELS, THEN DIAGONAL COMMANDS WILL ONLY BE SATISFIED WITH A CHECK LIKE
   THIS FOR //Forward-Left Diagonal
   else if(rc_values[RC_CH2] > 1900 && rc_values[RC_CH1] < 1100)
   THIS IS ESSENTIALLY CHECKING IF THE CONTROLLER STICK IS AT THE UPPER-LEFT MOST CORNER*/
}

//Actuator Shooting Mechanism
void shootingmechanism(){
  switchposition = digitalRead(switchpin);
 
  if(rc_values[RC_CH3] < 1040 && !shootingposition){
    digitalWrite(actuatordir, HIGH);
    analogWrite(actuatorpwm, 255);
  }

  if(switchposition){
    analogWrite(actuatorpwm, 0);
    digitalWrite(LEDdir, HIGH);
    analogWrite(LEDpwm, 255);
    shootingposition = true; 
  }
  
  if(rc_values[RC_CH3] > 1060 && shootingposition){
    //turn on the motor for a certain amout of time until it touches the end and lets the string shoot the ball
    //make sure the actuator goes back to the fully extended position
    digitalWrite(actuatordir, HIGH);
    analogWrite(actuatorpwm, 255);
    //The motor needs to run until the string releases before shootingposition is set to false
    //We can either use timer library or a dirty method with a for loop
    //for(int i = 0; i < 1000000000000; i++){
    //  digitalWrite(actuatorpin, HIGH);
    //  analogWrite(actuatorpin, 255);
    //}
    delay(2000);
    digitalWrite(LEDdir, HIGH);
    analogWrite(LEDpwm, 0);
    shootingposition = false;
  }

  if(rc_values[RC_CH3] > 1060 && !shootingposition){
    //turn on the motor in opposite direction for a certain amout of time until it touches the end and lets the string shoot the ball
    //make sure the actuator goes back to the fully extended position
    digitalWrite(actuatordir, LOW);
    analogWrite(actuatorpwm, 255);
  }

}

void loop() {

  rc_read_values();

  //Print controller channel values
  Serial.print("CH1:"); Serial.print(rc_values[RC_CH1]); Serial.print("\t");
  Serial.print("CH2:"); Serial.print(rc_values[RC_CH2]); Serial.print("\t");
  Serial.print("CH3:"); Serial.print(rc_values[RC_CH3]); Serial.print("\t");
  Serial.print("CH4:"); Serial.print(rc_values[RC_CH4]); Serial.print("\t");
  Serial.print("CH5:"); Serial.print(rc_values[RC_CH5]); Serial.print("\t");
  Serial.print("CH6:"); Serial.println(rc_values[RC_CH6]);

  Serial.println(switchposition);

  botmovement();
  shootingmechanism();

}
