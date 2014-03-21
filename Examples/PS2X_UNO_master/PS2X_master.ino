#include <PS2X_lib.h>  // Inclusion de la bibliothèque PS2X
#include <SPI.h> // Inclusion de la bibliothèque pour gérer la connexion SPI

/******************************************************************
* sélection des pins connecté au contrôleur
******************************************************************/
#define PS2_DAT        2  //14 Anciennes valeurs
#define PS2_CMD        7  //15
#define PS2_SEL        4  //16 - représente la ligne ATTENTION
#define PS2_CLK        8  //17

/******************************************************************
* sélection des modes du contrôleur:
*   - pressures = lecture analogique de la pression des boutons
*   - rumble    = vibration moteur
******************************************************************/
#define pressures   true
#define rumble      false
#define joystickTolerance 20 // sert à la calibration des 2 joysticks pour leur 2 axes
#define joystickCenter 127 // Ne pas toucher (cette valeur représente le centre des joysticks,
                           // ces derniers ne sont pas toujours à 127 en position central c'est pourquoi il y a joystickTolerance

/******************************************************************
* définition automatique des ID correspondant au composants, qui
* seront envoyé à la carte esclave afin qu'elle sache sur quel
* composant elle doit agir.
******************************************************************/
enum COMPOSANT {
    RELAIS_1, RELAIS_2, RELAIS_3, RELAIS_4,
    SERVO_1, SERVO_2, SERVO_3
};
/******************************************************************
* définition des pins correspondant au composants utilisés sur
* cette carte.
******************************************************************/
enum {
    PIN_MOTEUR_1 = 5, PIN_MOTEUR_2 = 6, PIN_MOTEUR_2 = 9,
    PIN_DIR_1 = 2, PIN_DIR_2 = 4, PIN_DIR_3 = 7
};

enum WHEELSCONTROLS {
    TANK_TYPE, REGULAR
};
enum JOYSTICKCOMMAND {
    NORMAL, INVERTED
};
enum MOTORDIR {
    CLOCKWISE = HIGH, ANTI_CLOCKWISE = LOW
};

/******************************************************************
* -wheelsControl: valeurs --> TANK_TYPE ou REGULAR, TANK_TYPE chaque
* joystick commande une roue. REGULAR en mode NORMAL le joystick
* gauche fait la direction et le droit le sens.
* -joystickCommand: valeurs --> NORMAL ou INVERTED, met les commandes
* en mode normal ou les inverves. Dans le cas de TANK_TYPE, la
* direction et l'assignation des moteurs est inversé. Dans le cas de
* REGULAR, seul le rôle des joystick est inversé (en NORMAL le
* joystick gauche contrôle la direction et le droit la vitesse)
* -motorDir_1...3: valeurs --> CLOCKWISE ou ANTI_CLOCKWISE, change
* le sens de rotation des moteurs
******************************************************************/
enum WHEELSCONTROLS wheelsControl = TANK_TYPE;
enum JOYSTICKCOMMAND joystickCommand = NORMAL;
enum MOTORDIR motorDir_1 = CLOCKWISE;
enum MOTORDIR motorDir_2 = CLOCKWISE;
enum MOTORDIR motorDir_3 = CLOCKWISE;

PS2X ps2x; // création d'une classe de type PS2X

/******************************************************************
* ATTENTION:
* pour le moment la bibliothèque ne supporte la connection des
* contrôleurs à chaud. Il faut donc soit redémarrer la carte
* arduino après avoir connecté le contrôleur, soit faire de
* nouveau appel à la fonction config_gamepad(...)
******************************************************************/

int error = 0; // variable stockant le code d'erreur, voir plus bas pour savoir à quelle erreur correspond tel code
byte type = 0; // variable stockant le type de contrôleur, voir plus bas pour savoir les types possibles
byte RX = joystickCenter; // variable servant à stocker la valeur de l'axe X du joystick droit
byte RY = joystickCenter; // variable servant à stocker la valeur de l'axe Y du joystick droit
byte LX = joystickCenter; // varaible servant à stocker la valeur de l'axe X du joystick gauche
byte LY = joystickCenter; // varaible servant à stocker la valeur de l'axe Y du joystick gauche


/******************************************************************
* déclaration et définition d'une fonction permettant de
* simplifier l'envoi d'information à la carte esclave via le SPI
* args:
* -composant: cette valeur représente l'ID du composant concerné
* -valeur: valeur à envoyer, à noter que si la valeur n'est pas
* préciser, elle sera par défaut à 0.
******************************************************************/
void sendData(COMPOSANT const composant, byte const valeur = 0);

/******************************************************************
* déclaration et définition d'une fonction permettant de
* simplifier l'acquisition de la position du joystick, le centre
* représentant maintenant 0 et ses bords le maximum (255)
* arg:
* -joystickAxis: cette valeur représente l'ID du joystick concerné
* ainsi que l'axe voulu. Il faut utilisé les constantes de la
* bibliothèque PS2X (PSS_RX, PSS_RY, PSS_LX, PSS_LY)
* ATTENTION: cette fonction prend en compte 'joystickTolerance'
******************************************************************/
byte getCorrectedAxis(byte const joystickAxis);

/******************************************************************
* déclaration et définition d'une fonction permettant de contrôler
* les roues du robot, doit être appelé après read_gamepad()
******************************************************************/
void moveRobot();


/******************************************************************
* fonction obligatoire d'arduino, elle est n'est effectué qu'une
* fois, à chaque démarrage/reset de la carte
******************************************************************/
void setup() { 
    Serial.begin(57600); // définition de la vitesse de transmission pour pouvoir communiquer avec le PC et avoir un retour sur la console
    SPI.begin(); // Lancement du SPI (configuration automatique des pins par défaut d'arduino)
    SPI.setClockDivider(SPI_CLOCK_DIV8); // Création de l'horloge à (1/8ème?) de la fréquence de l'horloge système (16Mhz)

    pinMode(PIN_MOTEUR_1, OUTPUT);
    pinMode(PIN_MOTEUR_2, OUTPUT);
    pinMode(PIN_MOTEUR_3, OUTPUT);
    pinMode(PIN_DIR_1, OUTPUT);
    pinMode(PIN_DIR_2, OUTPUT);
    pinMode(PIN_DIR_3, OUTPUT);

    delay(300);  // inutile pour nous, mais pas dans le cas où le contrôleur utilisé est sans-fil (permet de lui laisser le temps de démarrer)

    // définition des pins et paramètre et vérification des erreurs: GamePad(clock, command, attention, data, Pressures?, Rumble?)
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    ps2x.printCheckError(error); // Permet le retour d'éventuelles erreurs dans la console arduino

    type = ps2x.readType(); 
    ps2x.printCheckControllerType(type); // Permet d'avoir le retour du type de controller dans la console arduino
}


void loop() {
    /******************************************************************
    * on doit lire la manette pour recevoir les nouvelles valeurs,
    * ainsi que pour définir les vibrations ps2x.read_gamepad(small motor
    * on/off,larger motor strenght from 0-255)
    * si rumble est a false, appeler simplement ps2x.read_gamepad();
    * il faudrait lire la manette au moins une fois toutes les secondes
    ******************************************************************/


    if(error == 1) // sort de la boucle principale
        return;

    if (type == 1 || type == 3) { //DualShock Controller (avec ou sans fil trouvée)
        ps2x.read_gamepad();

        /******************************************************************
        * changement de la boucle principale, il y a maintenant d'abord
        * un test pour savoir si un bouton de la manette à été changé au
        * lieu de tester tous les boutons même s'ils n'avaient pas changé
        * d'état. Les tests effectué sont réservé aux test qui ne requiert
        * pas de retour analogiques. Cela permet d'alléger la transmission
        * des données.
        ******************************************************************/


        delay(50);  // lit la manette toutes les 50ms~ (prendre en compte l'envoi d'une donnée qui ajoute du temps et qui prend au minimum 20ms)
    }
}


void sendData(COMPOSANT const composant, byte const valeur) {
    digitalWrite(SS, LOW); // Met la ligne SS à l'état bas

    SPI.transfer(composant);
    SPI.transfer(valeur);

    digitalWrite(SS, HIGH); // Met la ligne SS à l'état haut

    /******************************************************************
    * par rapport à l'architecture du programme actuel, ce dernier
    * envoi constamment les données concernant les joysticks et les
    * 2 boutons dédié au relais et servo, le delay(20) est donc la
    * pour laisser le temps à l'esclave de les interpréter.
    * AMELIORATON POSSIBLE: n'envoyer les données que si une action
    * sur la manette à eu lieu afin de moins encombré la liaision SPI
    ******************************************************************/
    delay(20);
}


byte getCorrectedAxis(byte const joystickAxis) {
    byte axis = ps2x.Analog(joystickAxis);
    if (axis < joystickCenter + joystickTolerance && axis > joystickCenter + joystickTolerance)
        return 0;
    if (axis > joystickCenter + joystickTolerance) {
        return map(axis, joystickCenter + joystickTolerance, 255, 0, 255);
    }
    return map(axis, joystickCenter - joystickTolerance, 0, 0, 255);
}


void moveRobot() {
    switch (wheelsControl) {
        case TANK_TYPE:
            LY = getCorrectedAxis(PSS_LY);
            LX = getCorrectedAxis(PSS_LX);
            if (ps2x.Analog(PSS_LY) < joystickCenter)
                motorDir_1 = CLOCKWISE;
            else
                motorDir_1 = ANTI_CLOCKWISE;
            if (ps2x.Analog(PSS_LX) > joystickCenter)
                motorDir_2 = CLOCKWISE;
            else
                motorDir_2 = ANTI_CLOCKWISE;
            switch (joystickCommand) {
                case NORMAL:
                    digitalWrite(PIN_MOTEUR_1, motorDir_1);
                    digitalWrite(PIN_MOTEUR_2, motorDir_2);
                    analogWrite(PIN_MOTEUR_1, LY);
                    analogWrite(PIN_MOTEUR_2, LX);
                    break;
                case INVERTED:
                    // inversion des directions
                    if (motorDir_1 == CLOCKWISE)
                        motorDir_1 = ANTI_CLOCKWISE;
                    else
                        motorDir_1 = CLOCKWISE;
                    if (motorDir_2 == CLOCKWISE)
                        motorDir_2 = ANTI_CLOCKWISE;
                    else
                        motorDir_2 = CLOCKWISE;
                    // inversion des directions correspondant aux moteurs
                    digitalWrite(PIN_MOTEUR_1, motorDir_2);
                    digitalWrite(PIN_MOTEUR_2, motorDir_1);
                    // inversion des joysticks correspondant aux moteurs
                    analogWrite(PIN_MOTEUR_1, LX);
                    analogWrite(PIN_MOTEUR_2, LY);
                    break;
            }
            break;
        case REGULAR:
            switch (joystickCommand) {
                case NORMAL: {
                        byte speed = getCorrectedAxis(PSS_RY);
                        if (ps2x.Analog(PSS_RY) < joystickCenter) {
                            motorDir_1 = CLOCKWISE;
                            motorDir_2 = CLOCKWISE;
                        }
                        else {
                            motorDir_1 = ANTI_CLOCKWISE;
                            motorDir_2 = ANTI_CLOCKWISE;
                        }
                        byte turning = getCorrectedAxis(PSS_LX);
                        boolean turningRight = (ps2x.Analog(PSS_LX) > joystickCenter);
                    }
                    break;
                case INVERTED:
                    break;
            }
            break;
    }
}