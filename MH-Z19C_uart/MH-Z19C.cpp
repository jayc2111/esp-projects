#include "MH-Z19C.hpp"


co2_sensor::co2_sensor(uint8_t rx, uint8_t tx): 
    mySerial(rx, tx)
{
    mySerial.begin(9600);
}


co2_sensor::~co2_sensor() {   
}


void co2_sensor::readCo2() {   
  uint16_t co2Value = 0u;

  memset(read_buf, 0, sizeof(read_buf));
  mySerial.write(read_cmd, sizeof(read_cmd));
  mySerial.flush();

  while (mySerial.available() < CO2_CMD_LEN) {
//    Serial.print('+');
    delay(10);
  }

  int cnt = mySerial.readBytes(read_buf, sizeof(read_buf));
  
//  Serial.println("");
//  for (int i = 0; i < CO2_CMD_LEN; i++) {
//    Serial.print(read_buf[i], HEX);Serial.print(" ");
//  }
//  Serial.println("");

  if (read_buf[8] == checksum()) {
    co2Value = (read_buf[2]<<8) + read_buf[3];
    Serial.print("CO2: ");Serial.println(co2Value, DEC);
  } else {
    Serial.println("invalid checksum");
  }
}

uint8_t co2_sensor::checksum() {
    uint8_t chksum = 0;
    char* buf_ptr = read_buf;
    
    // sum the following bytes
    for(char i = 7; i--;)
      chksum += *(++buf_ptr);
    
    // invert and add "1"   
    chksum = ~chksum + 0x01;
    //Serial.print("chksum = ");Serial.println(chksum, HEX);
    
    return chksum;
}
