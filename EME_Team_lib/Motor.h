#ifndef MOTOR_H
#define MOTOR_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#if ARDUINO > 22
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "types.h"

class Motor {
public:
	////////////////////////////////////////////////////////////
    /// \brief Initialise les pin en sorti, et les membres de la classe.
    ///
    /// \param pwmPin Numéro de pin correspondant à la PWM du moteur
    /// \param dirPin Numéro de pin correspondant à la DIRECTION du moteur
    /// \param autoUpdate Indique si le relais se met à jour dès son changement d'état
    ///
    ////////////////////////////////////////////////////////////
	Motor(byte const pwmPin, byte const dirPin, boolean const autoUpdate = false);

	////////////////////////////////////////////////////////////
    /// \brief Permet de changer le sens de rotation du moteur
    ///
    /// \param direction Indique le nouveau sens de rotation à prendre
    ///
    ////////////////////////////////////////////////////////////
	void setDir(MOTORDIR const direction);

	////////////////////////////////////////////////////////////
    /// \brief Permet de changer la PWM du moteur
    ///
    /// \param speed Régle la vitesse de rotation (de 0 à 255)
    ///
    ////////////////////////////////////////////////////////////
	void setSpeed(byte const speed);

    ////////////////////////////////////////////////////////////
    /// \brief Permet d'augmenter la PWM du moteur de 1
    ///
    /// \param increment Paramètre non obligatoire, permet de préciser l'incrémentation, par défaut à 1
    ///
    ////////////////////////////////////////////////////////////
    void incrementSpeed(byte const increment = 1);

	////////////////////////////////////////////////////////////
    /// \brief Permet d'obtenir le sens de rotation actuel du moteur
    ///
    /// \return Le sens de rotation
    ///
    ////////////////////////////////////////////////////////////
	MOTORDIR getDir() const;

	////////////////////////////////////////////////////////////
    /// \brief Permet d'obtenir la PWM actuel du moteur
    ///
    /// \return La PWM (de 0 à 255)
    ///
    ////////////////////////////////////////////////////////////
	byte getSpeed() const;

	////////////////////////////////////////////////////////////
    /// \brief Stop la rotation du moteur (PWM = 0)
    ///
    ////////////////////////////////////////////////////////////
	void stop();

	////////////////////////////////////////////////////////////
    /// \brief Inverse le sens de rotation actuel du moteur
    ///
    ////////////////////////////////////////////////////////////
	void invertDir();

	////////////////////////////////////////////////////////////
    /// \brief Met à jour tous les paramètre du moteur (PWM et direction)
    /// doit être appelé après chaque fonction changeant le comportement
    /// du moteur pour que ces dernières fassent effet
    ///
    ////////////////////////////////////////////////////////////
	void update();

private:
    ////////////////////////////////////////////////////////////
    /// Variables membre
    ////////////////////////////////////////////////////////////
	const byte m_motorPin; ///<  Stocke le n° de pin sur laquelle envoyer la PWM au moteur
	const byte m_dirPin; ///<  Stocke le n° de pin sur laquelle envoyer la direction au moteur
	MOTORDIR m_direction; ///<  Stocke le sens de rotation actuel du moteur
	byte m_speed; ///<  Stocke la PWM actuel du moteur
    boolean m_autoUpdate; ///<  Stocke si l'auto-update est actif ou non
};

#endif // !MOTOR_H

////////////////////////////////////////////////////////////
/// \class Motor
/// 
/// La classe moteur permet de gérer plus facilement n'importe quel
/// moteur à courant continu.
///
////////////////////////////////////////////////////////////