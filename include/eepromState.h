#ifndef eepromState_h
#define eepromState_h

#include <EEPROM.h>
#include "setup.h"
#include "mqttserial.h"

#define EEPROM_CHK 0

#define EEPROM_HEATING  1
#define EEPROM_COOLING  2
#define EEPROM_SG1      3
#define EEPROM_SG2      4

void saveEEPROM(uint8_t registerNr, uint8_t state) {
  EEPROM.write(registerNr, state);
  EEPROM.commit();
}

void readEEPROM(){  
#ifdef PIN_RT_HEATING  
  digitalWrite(PIN_RT_HEATING,EEPROM.read(EEPROM_HEATING));
  mqttSerial.printf("Restoring previous heating state: %s",(EEPROM.read(EEPROM_HEATING) == SG_RELAY_INACTIVE_STATE)? "Off":"On" );  
#endif

#ifdef PIN_RT_COOLING
  digitalWrite(PIN_RT_COOLING,EEPROM.read(EEPROM_COOLING));
  mqttSerial.printf("Restoring previous cooling state: %s",(EEPROM.read(EEPROM_COOLING) == SG_RELAY_INACTIVE_STATE)? "Off":"On" );  
#endif

#ifdef SMART_GRID_ENABLED
  digitalWrite(PIN_SG1,EEPROM.read(EEPROM_SG1));
  mqttSerial.printf("Restoring previous SG1 state: %s",(EEPROM.read(EEPROM_SG1) == SG_RELAY_INACTIVE_STATE)? "Off":"On" );  

  digitalWrite(PIN_SG2,EEPROM.read(EEPROM_SG2));
  mqttSerial.printf("Restoring previous SG2 state: %s",(EEPROM.read(EEPROM_SG2) == SG_RELAY_INACTIVE_STATE)? "Off":"On" );  
#endif
}

void initEEPROM()
{
  uint8_t flags = 0;

#ifdef PIN_RT_HEATING
  flags |= 1 << EEPROM_HEATING;
#endif

#ifdef PIN_RT_COOLING
  flags |= 1 << EEPROM_COOLING;
#endif

#ifdef SMART_GRID_ENABLED
  flags |= 1 << EEPROM_SG1;
#endif

#ifdef SMART_GRID_ENABLED
  flags |= 1 << EEPROM_SG2;
#endif

  // if this needs to be initilized
  if(EEPROM.read(EEPROM_CHK) != flags)
  {
    mqttSerial.printf("EEPROM not initialized (%d)(%d). Initializing...", flags, EEPROM.read(EEPROM_CHK));
    EEPROM.write(EEPROM_CHK, flags);
    EEPROM.write(EEPROM_HEATING,  SG_RELAY_INACTIVE_STATE);
    EEPROM.write(EEPROM_COOLING,  SG_RELAY_INACTIVE_STATE);
    EEPROM.write(EEPROM_SG1,      SG_RELAY_INACTIVE_STATE);
    EEPROM.write(EEPROM_SG2,      SG_RELAY_INACTIVE_STATE);
    EEPROM.commit();
  }
}
#endif
