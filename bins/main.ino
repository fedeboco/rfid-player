#include "Arduino.h"
#include "cardSystem.h"

CardSystem sys;

void setup()
{
  sys.init();
}

void loop()
{
  sys.updateSystem();
}
