#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#if ARDUINO > 22
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include <SPI.h>

enum ID
{
	SERVO_1, SERVO_2, SERVO_3,
	RELAIS_1, RELAIS_2, RELAIS_3, RELAIS_4
};

#define envoyer(id, valeur) {		\
	for (byte i = 0; i < 3; ++i) {						\
		SPI.transfer(0);								\
	}													\
	SPI.transfer(id);									\
	SPI.transfer(valeur);								\
}

#endif