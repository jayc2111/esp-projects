#ifndef MH_Z19C_H
#define MH_Z19C_H

#include <SoftwareSerial.h>

class co2_sensor {

public:
  co2_sensor(uint8_t rx, uint8_t tx);
  ~co2_sensor();

  void    readCo2();
  uint8_t checksum();

private:
  const static uint8_t CO2_CMD_LEN = 9u;
  const char read_cmd[CO2_CMD_LEN] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

  SoftwareSerial mySerial;
  char read_buf[CO2_CMD_LEN];
};

#endif /* MH_Z19C_H */
