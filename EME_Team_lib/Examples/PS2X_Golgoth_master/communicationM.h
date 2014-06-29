#ifndef COMMUNICATIONM_H
#define COMMUNICATIONM_H

#if ARDUINO > 22
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include <SPI.h>
#include "constantes.h"

////////////////////////////////////////////////////////////
// Enumération permettant de simplifier l'identification
// des éléments qui sont sur la 2ème carte.
// Amélioration possible: arrêter l'utilisation d'enum et
// envoyer seulement un seul octet sur lequel chaque bit
// correspond à un état.
////////////////////////////////////////////////////////////
enum ID {
	ID_START = 180, SERVO_1, SERVO_2, SERVO_3,
	RELAIS_1, RELAIS_2, RELAIS_3, RELAIS_4, ID_END
};

////////////////////////////////////////////////////////////
// ATTENTION: les macros (#define fonction()) doivent avoir
// leurs lignes qui finissent par un anti-slash si elle sont
// écrites sur plusieurs lignes. Jusqu'à l'avant dernière
// ligne, donc la dernière ligne n'a pas d'anti-slash
////////////////////////////////////////////////////////////

#define sendWithDelay(valeur) {							\
	SPI.transfer(valeur);								\
	delayMicroseconds(100);								\
}
#define envoyer(id, valeur) {							\
	digitalWrite(SS, LOW);								\
	for (byte i = 0; i < 3; ++i) {						\
		SPI.transfer(0);								\
	}													\
	SPI.transfer(id);									\
	SPI.transfer(valeur);								\
	digitalWrite(SS, HIGH);								\
}

#define envoyerAvecDelay(id, valeur) {					\
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
#define envoiDirecte(id, valeur) {						\
	digitalWrite(SS, LOW);								\
	sendWithDelay(id);									\
	sendWithDelay(valeur);								\
	digitalWrite(SS, HIGH);								\
}

#endif // !COMMUNICATIONM_H