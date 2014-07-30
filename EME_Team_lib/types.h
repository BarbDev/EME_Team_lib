#ifndef TYPES_H
#define TYPES_H

////////////////////////////////////////////////////////////
/// Définition des différents types de contrôle du robot
////////////////////////////////////////////////////////////
enum WHEELSCONTROLS {
    TANK_TYPE, ///<  Chaque joystick commande une roue, en mode NORMAL le joystick droit
    		   ///<  commande la roue droite et le gauche la roue gauche
    		   ///< \see JOYSTICKCOMMAND

    REGULAR    ///<  En mode NORMAL le joystick gauche fait la direction et le droit le sens
    		   ///< \see JOYSTICKCOMMAND
};

////////////////////////////////////////////////////////////
/// Définition des différents types de commandes du robot
////////////////////////////////////////////////////////////
enum JOYSTICKCOMMAND {
    NORMAL,		///<  Commandes des roues normales
    INVERTED 	///<  Commandes des roues inversées
};

////////////////////////////////////////////////////////////
/// Définition des différents sens de rotation d'un moteur
/// Attention, ces derniers peuvent être inversé selon la
/// configuration de la carte arduino ou selon le cablage
/// du moteur.
////////////////////////////////////////////////////////////
enum MOTORDIR {
    CLOCKWISE = 1,		///<  Sens horaire @hideinitializer
    ANTI_CLOCKWISE = 0	///<  Sens anti-horaire @hideinitializer
};

#endif
