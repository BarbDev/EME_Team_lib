////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <PS2X_lib.h>
#include <SPI.h>
#include <Motor.h>
#include <types.h>
#include "constantes.h"
#include "communication.h"

PS2X manette; // création d'une classe de type manette

Motor moteurGauche(PWM_PIN_MOTOR_1, DIR_PIN_MOTOR_1);
Motor moteurDroit(PWM_PIN_MOTOR_2, DIR_PIN_MOTOR_2);

////////////////////////////////////////////////////////////
// ATTENTION:
// pour le moment la bibliothèque ne supporte la connection des
// contrôleurs à chaud. Il faut donc soit redémarrer la carte
// arduino après avoir connecté le contrôleur, soit faire de
// nouveau appel à la fonction config_gamepad(...)
////////////////////////////////////////////////////////////

int error = 0; // variable stockant le code d'erreur, voir plus bas pour savoir à quelle erreur correspond tel code
byte type = 0; // variable stockant le type de contrôleur, voir plus bas pour savoir les types possibles

////////////////////////////////////////////////////////////
// fonction obligatoire d'arduino, elle est n'est effectué qu'une
// fois, à chaque démarrage/reset de la carte
////////////////////////////////////////////////////////////
void setup() { 
    Serial.begin(57600); // définition de la vitesse de transmission pour pouvoir communiquer avec le PC et avoir un retour sur la console

    // définition des pins et paramètre et vérification des erreurs: GamePad(clock, command, attention, data, Pressures?, Rumble?)
    error = manette.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    manette.printCheckError(error); // Permet le retour d'éventuelles erreurs dans la console arduino

    type = manette.readType(); 
    manette.printCheckControllerType(type); // Permet d'avoir le retour du type de controller dans la console arduino

    SPI.begin();
}


void loop() {

    ////////////////////////////////////////////////////////////
    // on doit lire la manette pour recevoir les nouvelles valeurs,
    // ainsi que pour définir les vibrations manette.read_gamepad(small motor
    // on/off,larger motor strenght from 0-255)
    // si rumble est a false, appeler simplement manette.read_gamepad();
    // il faudrait lire la manette au moins une fois toutes les secondes
    ////////////////////////////////////////////////////////////


    if(error == 1)
        delay(5000); // fait attendre le micro-contôleur pour évité qu'il tourne pour rien.

    else if (type == 1 || type == 3) { //DualShock Controller (avec ou sans fil trouvée)
        manette.read_gamepad(); // lit les données de la manette

        if (manette.Analog(PSS_RY) == 255) {
            moteurDroit.incrementSpeed(5);
            moteurDroit.setDir(CLOCKWISE);
        }
        else if (manette.Analog(PSS_RY) == 0) {
            moteurDroit.incrementSpeed(5);
            moteurDroit.setDir(ANTI_CLOCKWISE);
        }
        else
            moteurDroit.stop();

        if (manette.Analog(PSS_LY) == 255) {
            moteurGauche.incrementSpeed(5);
            moteurGauche.setDir(CLOCKWISE);
        }
        else if (manette.Analog(PSS_LY) == 0) {
            moteurGauche.incrementSpeed(5);
            moteurGauche.setDir(ANTI_CLOCKWISE);
        }
        else
            moteurGauche.stop();

        if (manette.Button(PSB_L1))
            envoyer(SERVO_1, 0)
        else if (manette.Button(PSB_L2))
            envoyer(SERVO_1, 179)

        if (manette.Button(PSB_R1))
            envoyer(SERVO_2, 0)
        else if (manette.Button(PSB_R2))
            envoyer(SERVO_2, 179)

        moteurGauche.update();
        moteurDroit.update();

        delay(50);  // attend 50ms~
    }
}
