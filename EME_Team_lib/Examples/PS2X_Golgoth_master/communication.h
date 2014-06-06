#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#if ARDUINO > 22
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include <SPI.h>
#include "constantes.h"

enum ID {
	ID_START = 180, SERVO_1, SERVO_2, SERVO_3,
	RELAIS_1, RELAIS_2, RELAIS_3, RELAIS_4, ID_END
};
#define ERROR_VALUE 181
#define CORRECTLY_RECEIVED 182

////////////////////////////////////////////////////////////
// ATTENTION: les macros (#define fonction()) doivent avoir
// leurs lignes qui finissent par un anti-slash si elle sont
// écrites sur plusieurs lignes. Jusqu'à l'avant dernière
// ligne, donc la dernière ligne n'a pas d'anti-slash
////////////////////////////////////////////////////////////

#ifdef MASTER

	#define sendWithDelay(valeur) {		\
		SPI.transfer(valeur);								\
		delayMicroseconds(100);								\
	}
	#define envoyer(id, valeur) {		\
		digitalWrite(SS, LOW);								\
		for (byte i = 0; i < 3; ++i) {						\
			SPI.transfer(0);								\
		}													\
		SPI.transfer(id);									\
		SPI.transfer(valeur);								\
		digitalWrite(SS, HIGH);								\
	}

	#define envoyerAvecDelay(id, valeur) {		\
		digitalWrite(SS, LOW);								\
		for (byte i = 0; i < 3; ++i) {						\
			SPI.transfer(0);								\
			delay(5);										\
		}													\
		SPI.transfer(id);									\
		delay(5);											\
		SPI.transfer(valeur);								\
		delay(5);											\
		digitalWrite(SS, HIGH);								\
	}
	#define envoiDirecte(id, valeur) {			\
		digitalWrite(SS, LOW);								\
		sendWithDelay(id);									\
		sendWithDelay(valeur);								\
		digitalWrite(SS, HIGH);								\
	}
#endif

// see Gammon arduino site
#ifdef SLAVE
	volatile boolean process_it = false;
	volatile byte buffer[2] = {0};

	#define initInterrupt() {								\
		SPCR |= _BV(SPE);									\
		SPI.attachInterrupt();								\
	}

	extern Servo servo1, servo2, servo3;
	extern Relais relais1, relais2, relais3, relais4;

	#define recevoir() {									\
		switch(buffer[0]) {									\
			case SERVO_1:									\
				servo1.write(buffer[1]);					\
				break;										\
			case SERVO_2:									\
				servo2.write(buffer[1]);					\
				break;										\
			case SERVO_3:									\
				servo3.write(buffer[1]);					\
				break;										\
			case RELAIS_1:									\
				relais1.setState(buffer[1]);				\
				relais1.update();							\
				break;										\
			case RELAIS_2:									\
				relais2.setState(buffer[1]);				\
				relais2.update();							\
				break;										\
			case RELAIS_3:									\
				relais3.setState(buffer[1]);				\
				relais3.update();							\
				break;										\
			case RELAIS_4:									\
				relais4.setState(buffer[1]);				\
				relais4.update();							\
				break;										\
		}													\
	}

	ISR (SPI_STC_vect) {
	    volatile byte data = SPDR; // stocke la valeur envoyé par le maître
	    volatile static byte lowerStateCount = 0, dataCount = 0;
	    
	    if (data == 0 && lowerStateCount != 3) {
	    	lowerStateCount++;
	    }
	    else if (lowerStateCount == 3 && dataCount < 2) {
	    	buffer[dataCount] = data;
	    	dataCount++;
	    }
	    if (dataCount == 2) {
	    	lowerStateCount = 0;
	    	dataCount = 0;
	    	process_it = true;
	    }
	    
	    //Serial.println(data, DEC);
	}
#endif

#endif