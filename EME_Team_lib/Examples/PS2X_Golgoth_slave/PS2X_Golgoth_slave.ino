////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SPI.h>
#include <Servo.h>
#include <types.h>
#include <Relais.h>

#include "constantes.h"
#include "communication.h"

#define SLAVE // voir l'effet dans communication.h
Servo servo1, servo2, servo3;
Relais relais1(PIN_RELAIS_1), relais2(PIN_RELAIS_2), relais3(PIN_RELAIS_3), relais4(PIN_RELAIS_4);

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
}


void loop() {
    // voir communication.h et mettre ses fonctions dans le switch
}
