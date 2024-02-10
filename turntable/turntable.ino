
/*
 based on "Stepper Motor Control" by Tom Igoe

 This little fellow waits for commands on the USB line
 and turns a NEMA17 accordingly.
 
 "ident"        - identify yourself
 "start"        - Todo: spin continously
 "stop"         - Todo: stop spinning
 "turn <angle>" - turn stepper by angle
 
 */

#include <Stepper.h>

#define MAX_ANGLE 360

/* PIN connectors */
const unsigned char GPIO_YEL = 5u;
const unsigned char GPIO_GRE = 4u;
const unsigned char GPIO_WHI = 12u;
const unsigned char GPIO_BLU = 13u;

// change this to fit the number of steps per revolution
const int stepsPerRevolution = 200;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, GPIO_BLU, GPIO_WHI, GPIO_GRE, GPIO_YEL);

/* commands */
const char tag_TURN[]  = "turn";
const char tag_STOP[]  = "stop";
const char tag_START[] = "start";
const char tag_IDENT[] = "ident";

/* command string max lenght */
const uint8_t L_MAX_CMD = 255u;


char serialInBuffer[L_MAX_CMD];
uint8_t idx;

void setup() {
  //pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  
  memset((void*)serialInBuffer, 0x00, sizeof(serialInBuffer));
  idx = 0;
  
  // set the speed at 60 rpm
  myStepper.setSpeed(60);

  // initialize the serial port:
  Serial.begin(115200);  
  Serial.write("\nReady to Run\n");

}


void setStepperIdle() {
  digitalWrite(GPIO_YEL, LOW);
  digitalWrite(GPIO_GRE, LOW);
  digitalWrite(GPIO_WHI, LOW);
  digitalWrite(GPIO_BLU, LOW);
}


void turnPlate(int16_t angle)
{
  myStepper.step(angle);
  setStepperIdle();
}


void decodeCmd (const char* cmd)
{
  /*
    turn [-MAX_ANGLE, MAX_ANGLE]
    start
    stop
   */
  if (cmd)
  {
    int16_t angle = -181;
    char *substr = nullptr;

    substr = strtok((char*)cmd, " ,.\n");

    if (substr)
    {
      /* TURN command */
      if ( 0 == strncmp(substr, tag_TURN, L_MAX_CMD) ) {
        substr = strtok(nullptr, "\n");
  
        if (substr){
                    
          angle = atof(substr);
          if ( (-MAX_ANGLE > angle) || (MAX_ANGLE < angle) ) {
            Serial.write("invalid angle: "); 
          }
          else {            
            // convert angle into motorsteps
            int steps = int(angle/1.8);
            turnPlate(steps);
            Serial.write("turning: ");
          }
          Serial.print(angle, DEC);
          Serial.write(" --\n");
        }
  
      }
      /* START command */
      else if ( 0 == strncmp(substr, tag_START, L_MAX_CMD) ) {
        
        Serial.write("start spinning\n");
        
        // ToDo
      }
      /* STOP command */
      else if ( 0 == strncmp(substr, tag_STOP, L_MAX_CMD) ) {
        
        Serial.write("stopping\n");
        
        // ToDo
      }
      /* IDENT command */
      else if ( 0 == strncmp(substr, tag_IDENT, L_MAX_CMD) ) {
        
        Serial.write("0815\n");
      }
      else
      {
        Serial.write("invalid cmd: ");
        Serial.write(cmd);
        Serial.write("\n");
      }
    }

  }
}


void loop() {
  // init 
  //digitalWrite(LED_BUILTIN, LOW);
  //delay(500);
  //digitalWrite(LED_BUILTIN, HIGH);
  
  // better for serial interface reading
  delay(500);


  /* get command from serial line */
  while(Serial.available()){
    serialInBuffer[idx] = Serial.read();
    idx++;
  } /* while */

  // Is there a new command?
  if (3 < idx) 
  {
    //Serial.write(serialInBuffer);
    decodeCmd(serialInBuffer);
    
    memset((void*)serialInBuffer, 0x00, sizeof(serialInBuffer));
    idx = 0;
  }
  
}
