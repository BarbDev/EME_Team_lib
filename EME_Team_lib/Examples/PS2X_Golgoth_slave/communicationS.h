#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

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

////////////////////////////////////////////////////////////
// ATTENTION: les macros (#define fonction()) doivent avoir
// leurs lignes qui finissent par un anti-slash si elle sont
// écrites sur plusieurs lignes. Jusqu'à l'avant dernière
// ligne, donc la dernière ligne n'a pas d'anti-slash
////////////////////////////////////////////////////////////

// see Gammon arduino site
volatile byte IDs[7] = {0};

#define initInterrupt() {								\
	SPCR |= _BV(SPE);									\
	SPI.attachInterrupt();								\
}

////////////////////////////////////////////////////////////
// Objets définits dans le fichier .ino
////////////////////////////////////////////////////////////
extern Servo servo1, servo2, servo3;
extern Relais relais1, relais2, relais3, relais4;

////////////////////////////////////////////////////////////
// Fonction appelée lors d'une interruption
////////////////////////////////////////////////////////////
ISR (SPI_STC_vect) {
    volatile byte data = SPDR; // stocke la valeur envoyé par le maître
    static byte dataCount = 0, id = 0;

    if (dataCount == 0) { // Si encore aucune donnée n'a été reçu
    	id = data;
    	dataCount++;
    	if (id <= ID_START && id >= ID_END) { // Si l'ID reçu ne fait pas parti des ID définits (voir enum ID)
    		dataCount = 0; // On remet à zéro le compte des données reçu
    		return; // On sort de la fonction ISR
    	}
    }
    else if (dataCount > 0) { // Si il y a déjà eu une valeur de reçu
    	id -= 181; // On soustrait 181 à l'ID afin d'avoir le bon index pour le tableau
    	IDs[id] = data; // On met à jour le composant du tableau
    	dataCount = 0;
    }

    //Serial.println(data, DEC);
}

#endif // !COMMUNICATIONS_H