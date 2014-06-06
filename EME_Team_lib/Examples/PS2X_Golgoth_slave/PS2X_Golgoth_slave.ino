////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SPI.h>
#include <Servo.h>
#include <types.h>
#include <Relais.h>

#include "constantes.h"
#define SLAVE // voir l'effet dans communication.h
#include "communication.h"

Servo servo1, servo2, servo3;
Relais relais1(PIN_RELAIS_1), relais2(PIN_RELAIS_2), relais3(PIN_RELAIS_3), relais4(PIN_RELAIS_4);
//extern volatile boolean process_it;
//extern volatile byte buffer[2];
extern volatile byte IDs[7];
//int i = 0;

////////////////////////////////////////////////////////////
// fonction obligatoire d'arduino, elle est n'est effectué qu'une
// fois, à chaque démarrage/reset de la carte
////////////////////////////////////////////////////////////

void setup() { 
    Serial.begin(57600); // définition de la vitesse de transmission pour pouvoir communiquer avec le PC et avoir un retour sur la console

    servo1.attach(PIN_SERVO_1, 0, 179);
    servo2.attach(PIN_SERVO_2, 0, 179);
    servo3.attach(PIN_SERVO_3, 0, 179);

    initInterrupt()

    Serial.println("Initialized");
}


void loop() {
    // voir communication.h
    /*if (process_it) {
        Serial.println("Processing");
    	recevoir()
    	process_it = false;
    }*/
    servo1.write(IDs[SERVO_1 - 181]);
    servo2.write(IDs[SERVO_2 - 181]);
    servo3.write(IDs[SERVO_3 - 181]);
    relais1.setState(IDs[RELAIS_1 - 181]);
    relais1.update();
    relais2.setState(IDs[RELAIS_2 - 181]);
    relais2.update();
    relais3.setState(IDs[RELAIS_3 - 181]);
    relais3.update();
    relais4.setState(IDs[RELAIS_4 - 181]);
    relais4.update();
    /*for (i = 0; i < 7; i++) {
        Serial.print("Tab[");
        Serial.print(i, DEC);
        Serial.print("] = ");
        Serial.println(IDs[i], DEC);
    }*/

    /*
    relais1.setState(HIGH);
    relais1.update();
    relais2.setState(HIGH);
    relais2.update();

    relais3.setState(HIGH);
    relais3.update();
    relais4.setState(HIGH);
    relais4.update();*/

    delayMicroseconds(5);
}
