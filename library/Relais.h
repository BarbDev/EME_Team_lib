#ifndef RELAIS_HPP
#define RELAIS_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#if ARDUINO > 22
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

class Relais {
public:
	////////////////////////////////////////////////////////////
    /// \brief Constructeur, initialise la pin en sortie
    ///
    /// \param pin Numéro de pin correspondant au relais
    ///
    ////////////////////////////////////////////////////////////
	Relais(byte const pin);

	////////////////////////////////////////////////////////////
    /// \brief Permet de changer l'état du relais (ouvert ou fermé)
    ///
    /// \param state Indique le nouvel état du relais
    ///
    ////////////////////////////////////////////////////////////
	void setState(byte const state);

    ////////////////////////////////////////////////////////////
    /// \brief Permet d'obtenir l'état du relais
    ///
    /// \return L'état actuel du relais
    ///
    ////////////////////////////////////////////////////////////
    byte getState() const;

	////////////////////////////////////////////////////////////
    /// \brief Change l'état du relais (passe d'ouvert à fermé ou de fermé à ouvert)
    ///
    ////////////////////////////////////////////////////////////
	void changeState();

	////////////////////////////////////////////////////////////
    /// \brief Met à jour tous les paramètre du moteur (PWM et direction)
    ///
    ////////////////////////////////////////////////////////////
	void update();

private:
	////////////////////////////////////////////////////////////
    /// Variables membre
    ////////////////////////////////////////////////////////////
	const byte m_pin; ///<  Stocke la pin correspondant au relais
	byte m_state; ///<  Stocke l'état actuel du relais
};

#endif

////////////////////////////////////////////////////////////
/// \class Relais
/// 
/// La classe relais permet de gérer plus facilement n'importe quel relais.
///
////////////////////////////////////////////////////////////