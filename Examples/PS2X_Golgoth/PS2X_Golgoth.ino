////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <PS2X_lib.h>
#include <Servo.h>
#include "types.h"
#include "constantes.h"
#include "Robot.h"

PS2X ps2x; // création d'une classe de type PS2X

Robot robot;

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
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    PS2X::printCheckError(error); // Permet le retour d'éventuelles erreurs dans la console arduino

    type = ps2x.readType(); 
    PS2X::printCheckControllerType(type); // Permet d'avoir le retour du type de controller dans la console arduino

    robot.setup(PWM_PIN_MOTOR_1, DIR_PIN_MOTOR_1, PWM_PIN_MOTOR_2, DIR_PIN_MOTOR_2, PIN_SERVO_1, PIN_SERVO_2);
}


void loop() {

    ////////////////////////////////////////////////////////////
    // on doit lire la manette pour recevoir les nouvelles valeurs,
    // ainsi que pour définir les vibrations ps2x.read_gamepad(small motor
    // on/off,larger motor strenght from 0-255)
    // si rumble est a false, appeler simplement ps2x.read_gamepad();
    // il faudrait lire la manette au moins une fois toutes les secondes
    ////////////////////////////////////////////////////////////


    if(error == 1)
        delay(5000); // fait attendre le micro-contôleur pour évité qu'il tourne pour rien.

    else if (type == 1 || type == 3) { //DualShock Controller (avec ou sans fil trouvée)
        ps2x.read_gamepad(); // lit les données de la manette

        if (ps2x.Button(PSB_TRIANGLE))
            robot.setCommandType(NORMAL);
        else if (ps2x.Button(PSB_CIRCLE))
            robot.setCommandType(INVERTED);

        robot.processCommand(ps2x);
        //ps2x.printAllAxis();

        delay(50);  // attend 50ms~
    }
}
