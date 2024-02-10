/*
 This little fellow waits for commands on the USB line
 and turns a NEMA17 accordingly.
 
 "ident"        - identify yourself
 "start"        - spin continously
 "stop"         - stop spinning
 "turn <angle>" - turn stepper by angle
 
 */

#define LED_BUILTIN  0
#define PWM_PIN     12 /*  GPIO12 is D6 on NodeMCU*/

/* commands */
const char tag_TURN[]  = "turn";
const char tag_STOP[]  = "stop";
const char tag_START[] = "start";
const char tag_IDENT[] = "ident";

/* command string max lenght */
const uint8_t L_MAX_CMD = 255u;

char serialInBuffer[L_MAX_CMD];
uint8_t serialInBuffer_idx;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  memset((void*)serialInBuffer, 0x00, sizeof(serialInBuffer));
  serialInBuffer_idx = 0;

  pinMode(PWM_PIN, OUTPUT);
  
  // initialize the serial port
  Serial.begin(115200);  
  Serial.write("\nReady to Run\n");
}


void turnPlate(uint16_t pos)
{
   digitalWrite(PWM_PIN, HIGH);
   delay(pos*10);
   digitalWrite(PWM_PIN, LOW);
}


void decodeCmd (const char* cmd)
{
  /*
    turn [-180, 180]
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
  
        if (substr)
          angle = atoi(substr);
  
        if ( (-180 > angle) || (180 < angle) ) {
          Serial.write("invalid angle: "); 
        }
        else {
          turnPlate(angle);
          Serial.write("turning: ");
        }
        Serial.print(angle, DEC);
        Serial.write(" --\n");
  
      }
      /* START command */
      else if ( 0 == strncmp(substr, tag_START, L_MAX_CMD) ) {
        digitalWrite(PWM_PIN, HIGH);
        Serial.write("start spinning\n");
      }
      /* STOP command */
      else if ( 0 == strncmp(substr, tag_STOP, L_MAX_CMD) ) {
        digitalWrite(PWM_PIN, LOW);
        Serial.write("stopping\n");
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
    } /* substring not empty */
  }
}


void loop() {
  
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);


  /* get command from serial line */
  while(Serial.available()){
    serialInBuffer[serialInBuffer_idx] = Serial.read();
    serialInBuffer_idx++;
  } /* while */
  
  // Is there a new command?
  if (3 < serialInBuffer_idx) 
  {
    //Serial.write(serialInBuffer);
    decodeCmd(serialInBuffer);
    
    memset((void*)serialInBuffer, 0x00, sizeof(serialInBuffer));
    serialInBuffer_idx = 0;
  }
  
  
}
