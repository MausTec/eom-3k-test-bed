#ifndef __voltage_sense_h
#define __voltage_sense_h

namespace VoltageSense {
  void disableTestRails();
  
  void enable12V();

  void disable12V();

  void enableResistance12V();
  
  void enableResistance5V();
  
  void enableResistance3V3();

  float measureResistance();

  float readVcc(int pin, float maxV = 15.0);
  float read12V();
  float read5V();
  float read3V3();
}

#endif
