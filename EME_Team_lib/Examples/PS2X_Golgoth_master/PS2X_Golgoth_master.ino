////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <PS2X_lib.h>
#include <SPI.h>
#include <Motor.h>
#include <types.h>
#include "constantes.h"
#include "communicationM.h"

PS2X manette; // création d'une classe de type manette

Motor moteurGauche(PWM_PIN_MOTOR_1, DIR_PIN_MOTOR_1);
Motor moteurDroit(PWM_PIN_MOTOR_2, DIR_PIN_MOTOR_2);
Motor moteur3(PWM_PIN_MOTOR_3, DIR_PIN_MOTOR_3), moteur4(PWM_PIN_MOTOR_4, DIR_PIN_MOTOR_4);
byte angleServo2 = 0, angleServo3 = 0;

int error = 0; // variable stockant le code d'erreur, voir plus bas pour savoir à quelle erreur correspond tel code
byte type = 0; // variable stockant le type de contrôleur, voir plus bas pour savoir les types possibles

const byte acceleration = 8; // variable représentant l'incrémentation de la vitesse des moteurs

////////////////////////////////////////////////////////////
// fonction obligatoire d'arduino, elle est effectué qu'une
// fois, à chaque démarrage/reset de la carte
////////////////////////////////////////////////////////////
void setup() {
    Serial.begin(57600); // définition de la vitesse de transmission pour pouvoir communiquer avec le PC et avoir un retour sur la console
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV128); // ralenti le l'horloge du micro-contrôleur

    // Cherche à synchroniser jusqu'à ce que la carte arduino est établi la communication
    do {
        // définition des pins et paramètre et vérification des erreurs: GamePad(clock, command, attention, data, Pressures?, Rumble?)
        error = manette.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
        manette.printCheckError(error); // Permet le retour d'éventuelles erreurs dans la console arduino
        type = manette.readType(); 
        manette.printCheckControllerType(type); // Permet d'avoir le retour du type de controller dans la console arduino
    } while (error != 0 && type == 0);
}


void loop() {

    ////////////////////////////////////////////////////////////
    // on doit lire la manette pour recevoir les nouvelles valeurs,
    // ainsi que pour définir les vibrations manette.read_gamepad(small motor
    // on/off,larger motor strenght from 0-255)
    // si rumble est à false, appeler simplement manette.read_gamepad();
    // il faudrait lire la manette au moins une fois toutes les secondes
    ////////////////////////////////////////////////////////////


    if(error == 1)
        delay(5000); // Fait attendre le micro-contôleur pour évité qu'il tourne pour rien.
                     // Au lieu de ça, appeler manette.config_gamepad afin d'essayer de se resynchroniser ?

    else if (type == 1 || type == 3) { //DualShock Controller (avec ou sans fil, trouvée)
        manette.read_gamepad(); // lit les données de la manette

        /* Fonctions concernant le déplacement du robot (déclarée plus bas) */
        tank_control_OT();
        //regular_control_OT();

        /* Contrôle du 3eme moteur */
        if (manette.Button(PSB_PAD_UP)) {
            const byte vitesse = manette.Analog(PSAB_PAD_UP);
            moteur3.setSpeed(vitesse);
            moteur3.setDir(CLOCKWISE);
        }
        else if (manette.Button(PSB_PAD_DOWN)) {
            const byte vitesse = manette.Analog(PSAB_PAD_DOWN);
            moteur3.setSpeed(vitesse);
            moteur3.setDir(ANTI_CLOCKWISE);
        }
        else
            moteur3.stop();

        /* Contrôle du 4eme moteur */
        if (manette.Button(PSB_PAD_RIGHT)) {
            const byte vitesse = manette.Analog(PSAB_PAD_RIGHT);
            moteur4.setSpeed(vitesse);
            moteur4.setDir(CLOCKWISE);
        }
        else if (manette.Button(PSB_PAD_LEFT)) {
            const byte vitesse = manette.Analog(PSAB_PAD_LEFT);
            moteur4.setSpeed(vitesse);
            moteur4.setDir(ANTI_CLOCKWISE);
        }
        else
            moteur4.stop();

        /* Mise à jour du moteur 3 et 4*/
        moteur3.update();
        moteur4.update();

        /* Contrôle des servos*/
        /* Servo 1 */
        if (manette.ButtonPressed(PSB_SQUARE))
            envoiDirecte(SERVO_1, 0)
        else if (manette.ButtonReleased(PSB_CROSS))
            envoiDirecte(SERVO_1, 179)

        /* Servo 2 */
        if (manette.ButtonPressed(PSB_R1)) {
            if (angleServo2 + 1 < 180)
                angleServo2++;
            envoiDirecte(SERVO_2, angleServo2)
        }
        else if (manette.ButtonReleased(PSB_R2)) {
            if (angleServo2 - 1 >= 0)
                angleServo2--;
            envoiDirecte(SERVO_2, angleServo2)
        }

        /* Servo 3 */
        if (manette.Button(PSB_L1)) {
            if (angleServo3 + 1 < 180)
                angleServo3++;
            envoiDirecte(SERVO_3, angleServo3)
        }
        else if (manette.Button(PSB_L2)) {
            if (angleServo3 - 1 >= 0)
                angleServo3--;
            envoiDirecte(SERVO_3, angleServo3)
        }

        /* Contrôle des relais*/
        /* Relais 1 */
        if (manette.ButtonPressed(PSB_TRIANGLE))
            envoiDirecte(RELAIS_1, HIGH)
        else if (manette.ButtonReleased(PSB_TRIANGLE))
            envoiDirecte(RELAIS_1, LOW)

        /* Relais 2 */
        if (manette.ButtonPressed(PSB_CIRCLE))
            envoiDirecte(RELAIS_2, HIGH)
        else if (manette.ButtonReleased(PSB_CIRCLE))
            envoiDirecte(RELAIS_2, LOW)

        /* Relais 3 */
        if (manette.ButtonPressed(PSB_SELECT))
            envoiDirecte(RELAIS_3, HIGH)
        else if (manette.ButtonReleased(PSB_SELECT))
            envoiDirecte(RELAIS_3, LOW)

        /* Relais 4 */
        if (manette.ButtonPressed(PSB_START))
            envoiDirecte(RELAIS_4, HIGH)
        else if (manette.ButtonReleased(PSB_START))
            envoiDirecte(RELAIS_4, LOW)

        delay(50);  // attend 50ms~ Amélioration: diminuer au maximum cette valeur pour une meilleur
                    // réaction du robot par rapport aux commandes.
    }
}

////////////////////////////////////////////////////////////
// Déclaration et définition de fonctions.
// Il faut savoir que dans le fichier principale .ino,
// arduino s'occupe lui même de déclarer les fonctions.
// Cela permet de ne pas avoir à le faire au début du fichier
// rendant le code plus claire et permettant l'appel des
// fonctions avant leur déclaration.
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Fonction assignant chaque joystick à une roue du robot.
// La vitesse est incrémenté au cours du temps. L'accélération
// peut être modifié par la variable 'acceleration' déclarée
// au début du fichier.
////////////////////////////////////////////////////////////
void tank_control_OT() {
    /* Roue droite */
    if (manette.Analog(PSS_RY) == 255) { // Si le joystick droit est complétement vers le haut
        moteurDroit.incrementSpeed(acceleration);
        moteurDroit.setDir(CLOCKWISE);
    }
    else if (manette.Analog(PSS_RY) == 0) {
        moteurDroit.incrementSpeed(acceleration);
        moteurDroit.setDir(ANTI_CLOCKWISE);
    }
    else
        moteurDroit.stop();

    /* Roue gauche */
    if (manette.Analog(PSS_LY) == 255) {
        moteurGauche.incrementSpeed(acceleration);
        moteurGauche.setDir(CLOCKWISE);
    }
    else if (manette.Analog(PSS_LY) == 0) {
        moteurGauche.incrementSpeed(acceleration);
        moteurGauche.setDir(ANTI_CLOCKWISE);
    }
    else
        moteurGauche.stop();

    /* Mise à jour des moteurs */
    moteurGauche.update();
    moteurDroit.update();
}

////////////////////////////////////////////////////////////
// Fonction assignant le joystick droit à l'accélération du
// robot et le joystick gauche à la direction du robot.
// La vitesse est incrémenté au cours du temps. L'accélération
// peut être modifié par la variable 'acceleration' déclarée
// au début du fichier.
////////////////////////////////////////////////////////////
void regular_control_OT() {
    /* Contrôle de la vitesse de déplacement avec le joystick droit*/
    if (manette.Analog(PSS_RY) == 255) {
        moteurDroit.incrementSpeed(acceleration);
        moteurDroit.setDir(CLOCKWISE);
        moteurGauche.incrementSpeed(acceleration);
        moteurGauche.setDir(CLOCKWISE);
    }
    else if (manette.Analog(PSS_RY) == 0) {
        moteurDroit.incrementSpeed(acceleration);
        moteurDroit.setDir(ANTI_CLOCKWISE);
        moteurGauche.incrementSpeed(acceleration);
        moteurGauche.setDir(ANTI_CLOCKWISE);
    }
    else {
        moteurDroit.stop();
        moteurGauche.stop();
    }

    /* Contrôle de la direction avec le joystick gauche */
    if (manette.Analog(PSS_LX) == 0) // On veut tourner à gauche
        moteurGauche.stop();
    else if (manette.Analog(PSS_LX) == 255) // On veut tourner à droite
        moteurDroit.stop();
        
    moteurGauche.update();
    moteurDroit.update();
}