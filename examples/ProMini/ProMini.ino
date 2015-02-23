// The Arduino Pro Mini is a simple & cheap board, ideal for leaving inside of
// your model. It only has a single Serial port, and the default SBUS code
// doesn't run on it. You can get it to work by not using the timer, but that
// means you can't use delay in the loop() function anymore.

#include <SBUS.h>
SBUS sbus(Serial);

void setup()
{
  sbus.begin(false);
}

void loop()
{
  sbus.process();
  int value = sbus.getChannel(4);
  analogWrite(3, value / 8);
}
