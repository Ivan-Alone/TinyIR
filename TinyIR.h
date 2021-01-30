#ifndef __TinyIR__
#define __TinyIR__
	#include <Arduino.h>

	volatile uint32_t __TinyIR_latestCallTime = 0;

	volatile uint32_t __TinyIR_bakedCode = 0;
	volatile bool __TinyIR_codeReadyStatus = false;
	volatile uint8_t __TinyIR_RX_Counter = 0;

	#ifndef TinyIR_DISABLE_REPEAT
	  volatile uint32_t latestCode = 0;
	#endif

	#define __TinyIR_compare(data,result,fault) ((data <= result + fault) && (data >= result - fault))

	bool __TinyIR_isRealCode(uint32_t code) {
	  return (uint8_t)(code >> 0x18) == (uint8_t)(code >> 0x10) && ((uint8_t)(code >> 0x08) ^ (uint8_t)code) == 0xFF;
	}

	void __TinyIR_interruptCallback() {
	  if (digitalRead(1)) {
		__TinyIR_latestCallTime = micros();
	  } else {
		uint16_t test = micros() - __TinyIR_latestCallTime;

		if (!(test > 5000)) {
		  if (__TinyIR_compare(test, 4500, 200)) {
			__TinyIR_bakedCode = 0;
			__TinyIR_codeReadyStatus = false;
			__TinyIR_RX_Counter = 0;
		  } else {
			#ifdef TinyIR_NO_ACCURATE_DECODING
			  __TinyIR_bakedCode = (__TinyIR_bakedCode << 1) | __TinyIR_compare(test, 1640, 200);
			#else
			  uint8_t result = __TinyIR_compare(test, 1640, 200);
			  if (result || __TinyIR_compare(test, 500, 200)) {
				__TinyIR_bakedCode = (__TinyIR_bakedCode << 1) | result;
			  }
			#endif
			
			if (__TinyIR_RX_Counter >= 0x1F) {
			  #ifndef TinyIR_DISABLE_REPEAT
				if (__TinyIR_bakedCode == 0 || __TinyIR_bakedCode == 1) {
				  __TinyIR_bakedCode = latestCode;
				}
			  #endif
			  if (__TinyIR_isRealCode(__TinyIR_bakedCode)) {
				__TinyIR_codeReadyStatus = true;
				#ifndef TinyIR_DISABLE_REPEAT
				  latestCode = __TinyIR_bakedCode;
				#endif
			  }
			}
		  }
		  __TinyIR_RX_Counter++;
		}
	  }
	}

	bool IR_CheckCode(uint32_t code) {
	  if (__TinyIR_codeReadyStatus && __TinyIR_bakedCode == code) {
		__TinyIR_codeReadyStatus = false;
		return true;
	  }
	  return false;
	}

	void IR_Init() {
	  attachInterrupt(0, __TinyIR_interruptCallback, CHANGE);
	}

#endif