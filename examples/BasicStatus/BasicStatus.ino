#include <SBUS.h>
SBUS sbus(Serial3);

void setup()
{
  sbus.begin();
  Serial.begin(115200);
  Serial.println("SBUS Status");
}

// this is timer2, which triggers ever 1ms and processes the incoming SBUS datastream
ISR(TIMER2_COMPA_vect)
{
  sbus.process();
}

void loop()
{
  delay(300);
  printSBUSStatus();
}

void printSBUSStatus()
{
  Serial.print("Ch1  ");
  Serial.println(sbus.getNormalizedChannel(1));
  Serial.print("Ch2  ");
  Serial.println(sbus.getNormalizedChannel(2));
  Serial.print("Ch3  ");
  Serial.println(sbus.getNormalizedChannel(3));
  Serial.print("Ch4  ");
  Serial.println(sbus.getNormalizedChannel(4));
  Serial.print("Ch5  ");
  Serial.println(sbus.getNormalizedChannel(5));
  Serial.print("Ch6  ");
  Serial.println(sbus.getNormalizedChannel(6));
  Serial.print("Ch7  ");
  Serial.println(sbus.getNormalizedChannel(7));
  Serial.print("Ch8  ");
  Serial.println(sbus.getNormalizedChannel(8));
  Serial.print("Ch9  ");
  Serial.println(sbus.getNormalizedChannel(9));
  Serial.print("Ch10 ");
  Serial.println(sbus.getNormalizedChannel(10));
  Serial.print("Ch11 ");
  Serial.println(sbus.getNormalizedChannel(11));
  Serial.print("Ch12 ");
  Serial.println(sbus.getNormalizedChannel(12));
  Serial.print("Ch13 ");
  Serial.println(sbus.getNormalizedChannel(13));
  Serial.print("Ch14 ");
  Serial.println(sbus.getNormalizedChannel(14));
  Serial.print("Ch15 ");
  Serial.println(sbus.getNormalizedChannel(15));
  Serial.print("Ch16 ");
  Serial.println(sbus.getNormalizedChannel(16));
  Serial.println();
  Serial.print("Failsafe: ");
  if (sbus.getFailsafeStatus() == SBUS_FAILSAFE_ACTIVE) {
    Serial.println("Active");
  }
  if (sbus.getFailsafeStatus() == SBUS_FAILSAFE_INACTIVE) {
    Serial.println("Not Active");
  }

  Serial.print("Data loss on connection: ");
  Serial.print(sbus.getFrameLoss());
  Serial.println("%");

  Serial.print("Frames: ");
  Serial.print(sbus.getGoodFrames());
  Serial.print(" / ");
  Serial.print(sbus.getLostFrames());
  Serial.print(" / ");
  Serial.println(sbus.getDecoderErrorFrames());

  Serial.print("Time diff: ");
  Serial.println(millis() - sbus.getLastTime());
}
