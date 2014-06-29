////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SPI.h>
#include <Servo.h>
#include <types.h>
#include <Relais.h>

#include "constantes.h"
#include "communicationS.h"

Servo servo1, servo2, servo3;
Relais relais1(PIN_RELAIS_1), relais2(PIN_RELAIS_2), relais3(PIN_RELAIS_3), relais4(PIN_RELAIS_4);
extern volatile byte IDs[7]; // Définit dans le fichier communicationS.h

////////////////////////////////////////////////////////////
// fonction obligatoire d'arduino, elle est n'est effectué qu'une
// fois, à chaque démarrage/reset de la carte
////////////////////////////////////////////////////////////

void setup() { 
    Serial.begin(57600); // définition de la vitesse de transmission pour pouvoir communiquer avec le PC et avoir un retour sur la console

    /* Attachement des servos à leurs pins correspondantes et définition de l'angle minimal et maximal */
    servo1.attach(PIN_SERVO_1, 0, 179);
    servo2.attach(PIN_SERVO_2, 0, 179);
    servo3.attach(PIN_SERVO_3, 0, 179);

    initInterrupt()
}

////////////////////////////////////////////////////////////
// Le programme va constamment mettre à jour les composants
// toutes les 5µs.
// Il faut trouver un moyen de soulager le microcontroleur
// en supprimant toutes les mises à jours inutiles.
////////////////////////////////////////////////////////////
void loop() {
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

    delayMicroseconds(5);
}
