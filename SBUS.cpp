#include "SBUS.h"
#include <Arduino.h>

void SBUS::begin() {
	SBUS::begin(true);
}

void SBUS::begin(bool useTimer) {
	if (useTimer) {
		noInterrupts();
		TCCR2A  = 0;
		TCCR2B  = 0;
		TCNT2   = 0;
		OCR2A   = 249;
		TCCR2A |= (1 << WGM21);
		TCCR2B |= (1 << CS22);
		TIMSK2 |= (1 << OCIE2A);
		interrupts();
	}

	for (byte i = 0; i<18; i++) {
		_channels[i]      = 0;
	}

	_goodFrames         = 0;
	_lostFrames         = 0;
	_decoderErrorFrames = 0;
	_failsafe           = SBUS_FAILSAFE_INACTIVE;

	_serial.begin(100000, SERIAL_8E2);
}

void SBUS::process() {
	static byte buffer[25];
	static byte buffer_index = 0;

	while (_serial.available()) {
		byte rx = _serial.read();
		if (buffer_index == 0 && rx != SBUS_STARTBYTE) {
			//incorrect start byte, out of sync
			_decoderErrorFrames++;
			continue;
		}

		buffer[buffer_index++] = rx;

		if (buffer_index == 25) {
			buffer_index = 0;
			if (buffer[24] != SBUS_ENDBYTE) {
				//incorrect end byte, out of sync
				_decoderErrorFrames++;
				continue;
			}
			_goodFrames++;

			_channels[0]  = ((buffer[1]    |buffer[2]<<8)                 & 0x07FF);
			_channels[1]  = ((buffer[2]>>3 |buffer[3]<<5)                 & 0x07FF);
			_channels[2]  = ((buffer[3]>>6 |buffer[4]<<2 |buffer[5]<<10)  & 0x07FF);
			_channels[3]  = ((buffer[5]>>1 |buffer[6]<<7)                 & 0x07FF);
			_channels[4]  = ((buffer[6]>>4 |buffer[7]<<4)                 & 0x07FF);
			_channels[5]  = ((buffer[7]>>7 |buffer[8]<<1 |buffer[9]<<9)   & 0x07FF);
			_channels[6]  = ((buffer[9]>>2 |buffer[10]<<6)                & 0x07FF);
			_channels[7]  = ((buffer[10]>>5|buffer[11]<<3)                & 0x07FF);
			_channels[8]  = ((buffer[12]   |buffer[13]<<8)                & 0x07FF);
			_channels[9]  = ((buffer[13]>>3|buffer[14]<<5)                & 0x07FF);
			_channels[10] = ((buffer[14]>>6|buffer[15]<<2|buffer[16]<<10) & 0x07FF);
			_channels[11] = ((buffer[16]>>1|buffer[17]<<7)                & 0x07FF);
			_channels[12] = ((buffer[17]>>4|buffer[18]<<4)                & 0x07FF);
			_channels[13] = ((buffer[18]>>7|buffer[19]<<1|buffer[20]<<9)  & 0x07FF);
			_channels[14] = ((buffer[20]>>2|buffer[21]<<6)                & 0x07FF);
			_channels[15] = ((buffer[21]>>5|buffer[22]<<3)                & 0x07FF);

			((buffer[23])      & 0x0001) ? _channels[16] = 2047: _channels[16] = 0;
			((buffer[23] >> 1) & 0x0001) ? _channels[17] = 2047: _channels[17] = 0;

			if ((buffer[23] >> 3) & 0x0001) {
				_failsafe = SBUS_FAILSAFE_ACTIVE;
			} else {
				_failsafe = SBUS_FAILSAFE_INACTIVE;
			}

			if ((buffer[23] >> 2) & 0x0001) {
				_lostFrames++;
			}

			_lastGoodFrame = millis();
		}
	}
}

int SBUS::getChannel(int channel) {
	if (channel < 1 or channel > 18) {
		return 0;
	} else {
		return _channels[channel - 1];
	}
}

int SBUS::getNormalizedChannel(int channel) {
	if (channel < 1 or channel > 18) {
		return 0;
	} else {
		return (int) lround(_channels[channel - 1] / 9.92) - 100; //9.92 or 10.24?
	}
}

int SBUS::getFailsafeStatus() {
	return _failsafe;
}

int SBUS::getFrameLoss() {
	return (int) ((_lostFrames + _decoderErrorFrames) * 100 / (_goodFrames + _lostFrames + _decoderErrorFrames));
}

long SBUS::getGoodFrames() {
	return _goodFrames;
}

long SBUS::getLostFrames() {
	return _lostFrames;
}

long SBUS::getDecoderErrorFrames() {
	return _decoderErrorFrames;
}

long long SBUS::getLastTime() {
	return _lastGoodFrame;
}
