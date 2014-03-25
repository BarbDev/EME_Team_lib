#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#if ARDUINO > 22
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include <SPI.h>
#include "constantes.h"

volatile byte buffer[2] = {0};

enum ID
{
	SERVO_1, SERVO_2, SERVO_3,
	RELAIS_1, RELAIS_2, RELAIS_3, RELAIS_4
};

////////////////////////////////////////////////////////////
// ATTENTION: les macros (#define fonction()) doivent avoir
// leurs lignes qui finissent par un anti-slash si elle sont
// écrites sur plusieurs lignes. Jusqu'à l'avant dernière
// ligne, donc la dernière ligne n'a pas d'anti-slash
////////////////////////////////////////////////////////////

#define initInterrupt() {								\
	SPI.begin();										\
	SPI.attachInterrupt();								\
}

#define envoyer(id, valeur) {		\
	for (byte i = 0; i < 3; ++i) {						\
		SPI.transfer(0);								\
	}													\
	SPI.transfer(id);									\
	SPI.transfer(valeur);								\
}

#define recevoir() {									\
	switch(buffer[0]) {									\
		case SERVO_1:									\
			servo1.write(buffer[1]);					\
			break;										\
		case SERVO_2:									\
			break;										\
		case SERVO_3:									\
			break;										\
		case RELAIS_1:									\
			break;										\
		case RELAIS_2:									\
			break;										\
		case RELAIS_3:									\
			break;										\
		case RELAIS_4:									\
			break;										\
	}													\
}

#ifdef SLAVE
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
	    	recevoir();
	    }
	}
#endif

#endif