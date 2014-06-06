////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <PS2X_lib.h>
#include <Servo.h>
#include <Motor.h>
#include <Relais.h>
#include <types.h>
#include "constantes.h"

PS2X manette; // création d'une classe de type manette

Motor moteurGauche(PWM_PIN_MOTOR_1, DIR_PIN_MOTOR_1);
Motor moteurDroit(PWM_PIN_MOTOR_2, DIR_PIN_MOTOR_2);
Motor moteur3(PWM_PIN_MOTOR_3, DIR_PIN_MOTOR_3), moteur4(PWM_PIN_MOTOR_4, DIR_PIN_MOTOR_4);
Relais relais1(PIN_RELAIS_1), relais2(PIN_RELAIS_2);
Servo servo1, servo2, servo3;

int error = 0; // variable stockant le code d'erreur, voir plus bas pour savoir à quelle erreur correspond tel code
byte type = 0; // variable stockant le type de contrôleur, voir plus bas pour savoir les types possibles
const byte acceleration = 8; // variable stockant l'accélération des moteurs contrôlant les roues du robot
byte angleServo2 = 0, angleServo3 = 0;

////////////////////////////////////////////////////////////
// fonction obligatoire d'arduino, elle est n'est effectué qu'une
// fois, à chaque démarrage/reset de la carte
////////////////////////////////////////////////////////////
void setup() { 
    Serial.begin(57600); // définition de la vitesse de transmission pour pouvoir communiquer avec le PC et avoir un retour sur la console

    // Cherche à synchroniser jusqu'à ce que la carte arduino est établi la communication
    do {
        // définition des pins et paramètre et vérification des erreurs: GamePad(clock, command, attention, data, Pressures?, Rumble?)
        error = manette.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
        manette.printCheckError(error); // Permet le retour d'éventuelles erreurs dans la console arduino
        type = manette.readType(); 
        manette.printCheckControllerType(type); // Permet d'avoir le retour du type de controller dans la console arduino
    } while (error != 0 && type == 0);

    servo1.attach(PIN_SERVO_1, 0, 179);
    servo2.attach(PIN_SERVO_2, 0, 179);
    servo3.attach(PIN_SERVO_3, 0, 179);
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

        tank_control_OT();

        /* Contrôle du 3eme moteur */
        if (manette.Button(PSB_PAD_UP)) {
            moteur3.setSpeed(manette.Analog(PSB_PAD_UP));
            moteur3.setDir(CLOCKWISE);
        }
        else if (manette.Button(PSB_PAD_DOWN)) {
            moteur3.setSpeed(manette.Analog(PSB_PAD_UP));
            moteur3.setDir(ANTI_CLOCKWISE);
        }
        else
            moteur3.stop();

        /* Contrôle du 4eme moteur */
        if (manette.Button(PSB_PAD_RIGHT)) {
            moteur4.setSpeed(manette.Analog(PSB_PAD_RIGHT));
            moteur4.setDir(CLOCKWISE);
        }
        else if (manette.Button(PSB_PAD_LEFT)) {
            moteur4.setSpeed(manette.Analog(PSB_PAD_LEFT));
            moteur4.setDir(ANTI_CLOCKWISE);
        }
        else
            moteur4.stop();

        /* Contrôle des servos*/
        /* Servo 1 */
        if (manette.Button(PSB_SQUARE))
            servo1.write(0);
        else if (manette.Button(PSB_CROSS))
            servo1.write(179);

        /* Servo 2 */
        if (manette.Button(PSB_R1)) {
            if (angleServo2 + 1 < 180)
                angleServo2++;
            servo2.write(angleServo2);
        }
        else if (manette.Button(PSB_R2)) {
            if (angleServo2 - 1 >= 0)
                angleServo2--;
            servo2.write(angleServo2);
        }

        /* Servo 3 */
        if (manette.Button(PSB_L1)) {
            if (angleServo3 + 1 < 180)
                angleServo3++;
            servo3.write(angleServo3);
        }
        else if (manette.Button(PSB_L2)) {
            if (angleServo3 - 1 >= 0)
                angleServo3--;
            servo3.write(angleServo3);
        }

        /* Contrôle des relais*/
        /* Relais 1 */
        if (manette.Button(PSB_TRIANGLE))
            relais1.setState(HIGH);
        else
            relais1.setState(LOW);

        /* Relais 2 */
        if (manette.Button(PSB_CIRCLE))
            relais2.setState(HIGH);
        else
            relais2.setState(LOW);

        /* Mise à jour des relais */
        relais1.update();
        relais2.update();

        /* Mise à jour du moteur 3 et 4*/
        moteur3.update();
        moteur4.update();

        delay(50);  // attend 50ms~
    }
}

void tank_control_OT() {
    /* Roue droite */
    if (manette.Analog(PSS_RY) == 255) {
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
