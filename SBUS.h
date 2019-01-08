#ifndef SBUS_h
#define SBUS_h

#include "Arduino.h"

#define SBUS_FAILSAFE_INACTIVE 0
#define SBUS_FAILSAFE_ACTIVE   1
#define SBUS_STARTBYTE         0x0f
#define SBUS_ENDBYTE           0x00
#define SBUS2_ENDBYTE0         0x04
#define SBUS2_ENDBYTE1         0x14
#define SBUS2_ENDBYTE2         0x24
#define SBUS2_ENDBYTE3         0x34


class SBUS {
	public:
		SBUS(HardwareSerial & serial) : _serial (serial) {}
		void begin();
		void begin(bool useTimer);
		void process();
		int getChannel(int channel);
		int getNormalizedChannel(int channel);
		int getFailsafeStatus();
		int getFrameLoss();
		long getGoodFrames();
		long getLostFrames();
		long getDecoderErrorFrames();
		long long getLastTime();
	private:
		HardwareSerial & _serial;
		int _channels[18];
		int _failsafe;
		long _goodFrames;
		long _lostFrames;
		long _decoderErrorFrames;
		long long _lastGoodFrame;
};

#endif
