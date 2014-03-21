#ifndef ROBOT_H
#define ROBOT_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#if ARDUINO > 22
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include <PS2X_lib.h>
#include <Servo.h>
#include "types.h"
#include "Motor.h"

class Robot {
public:
	////////////////////////////////////////////////////////////
    /// \brief Initialise les pin en sorti, et les membres de la classe.
    ///
    /// \param pinMotor1 Numéro de pin correspondant à la PWM du moteur 1
    /// \param pinDir1 Numéro de pin correspondant à la DIRECTION du moteur 1
    /// \param pinMotor2 Numéro de pin correspondant à la PWM du moteur 2
    /// \param pinDir2 Numéro de pin correspondant à la DIRECTION du moteur 2
    /// \param pinServ1 Numéro de pin correspondant au servomoteur 1
    /// \param pinServo2 Numéro de pin correspondant au servomoteur 2
    ///
    ////////////////////////////////////////////////////////////
	void setup(byte const pinMotor1, byte const pinDir1, byte const pinMotor2, byte const pinDir2, byte const pinServ1, byte const pinServo2);

	////////////////////////////////////////////////////////////
    /// \brief Permet de choisir le mode de contrôle
    ///
    /// \param control Indique le mode de contrôle (TANK_TYPE ou REGULAR)
    ///
    ////////////////////////////////////////////////////////////
	void setControlType(WHEELSCONTROLS const control);

	////////////////////////////////////////////////////////////
    /// \brief Permet de choisir le mode de commande
    ///
    /// \param command Indique le mode de commande (NORMAL ou INVERTED)
    ///
    ////////////////////////////////////////////////////////////
	void setCommandType(JOYSTICKCOMMAND const command);

	////////////////////////////////////////////////////////////
    /// \brief Permet de contrôler les actions du robot en fonction
    /// des commandes envoyer par la manette.
    /// Attention !!! Il faut l'appeler après chaque read_gamepad()
    /// Exemple:
    /// \code
    /// PS2X ps2x; // Création d'un objet PS2X grâce à la bibliothèque PS2X
    /// Robot robot; // Création d'un objet Robot
    /// // Nous somme dans la boucle principal d'arduino
    /// void loop() {
	/// 	ps2x.read_gamepad(); // Lecture des données envoyé par la manette
	/// 	robot.processCommand(ps2x); // Le robot réagit en fonction des données lut
	/// }
	/// \endcode
    ///
    /// \param manette Représente la manette en question
    ///
    ////////////////////////////////////////////////////////////
	void processCommand(PS2X &manette);

private:
	////////////////////////////////////////////////////////////
    /// \brief Permet d'inverser les commandes du robot afin de
    /// simplifier le pilotage de ce dernier lorsqu'il nous fait
    /// face.
    ///
    ////////////////////////////////////////////////////////////
	void invertTankCommand();

	////////////////////////////////////////////////////////////
    /// \brief Appel les fonctions update() de tous les composants
    /// qu'il possède afin de les mettre à jour et d'effectuer les
    /// nouvelles actions.
    ///
    ////////////////////////////////////////////////////////////
	void updateComponent();

private:
	////////////////////////////////////////////////////////////
    /// Variables membre
    ////////////////////////////////////////////////////////////
	WHEELSCONTROLS m_controlType; ///<  Stocke le type de control
	JOYSTICKCOMMAND m_commandType; ///<  Stocke le type de command
	
	////////////////////////////////////////////////////////////
    /// \brief stock tous les composants faisant parti du robot.
    ////////////////////////////////////////////////////////////
	Motor m_moteurGauche;
	Motor m_moteurDroit;
	Servo m_servo1;
	Servo m_servo2;
};

#endif

////////////////////////////////////////////////////////////
/// \class Robot
/// 
/// La classe robot permet de sélectionner toutes les pins
/// nécessaire au bon fonctionnement du programme. Elle permet
/// aussi de sélectionner le mode de contrôle ainsi que le mode
/// de commande.
/// \see setControlType, setCommandType
///
////////////////////////////////////////////////////////////
