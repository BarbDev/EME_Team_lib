/******************************************************************
*  Super amazing PS2 controller Arduino Library v1.9
*		details and example sketch: 
*			http://www.billporter.info/?p=240
*
*    Original code by Shutter on Arduino Forums
*
*    Revamped, made into lib by and supporting continued development:
*              Bill Porter
*              www.billporter.info
*
*	 Contributers:
*		Eric Wetzel (thewetzel@gmail.com)
*		Kurt Eckhardt
*
*  Lib version history
*    0.1 made into library, added analog stick support. 
*    0.2 fixed config_gamepad miss-spelling
*        added new functions:
*          NewButtonState();
*          NewButtonState(unsigned int);
*          ButtonPressed(unsigned int);
*          ButtonReleased(unsigned int);
*        removed 'PS' from begining of ever function
*    1.0 found and fixed bug that wasn't configuring controller
*        added ability to define pins
*        added time checking to reconfigure controller if not polled enough
*        Analog sticks and pressures all through 'ps2x.Analog()' function
*        added:
*          enableRumble();
*          enablePressures();
*    1.1  
*        added some debug stuff for end user. Reports if no controller found
*        added auto-increasing sentence delay to see if it helps compatibility.
*    1.2
*        found bad math by Shutter for original clock. Was running at 50kHz, not the required 500kHz. 
*        fixed some of the debug reporting. 
*	1.3 
*	    Changed clock back to 50kHz. CuriousInventor says it's suppose to be 500kHz, but doesn't seem to work for everybody. 
*	1.4
*		Removed redundant functions.
*		Fixed mode check to include two other possible modes the controller could be in.
*       Added debug code enabled by compiler directives. See below to enable debug mode.
*		Added button definitions for shapes as well as colors.
*	1.41
*		Some simple bug fixes
*		Added Keywords.txt file
*	1.5
*		Added proper Guitar Hero compatibility
*		Fixed issue with DEBUG mode, had to send serial at once instead of in bits
*	1.6
*		Changed config_gamepad() call to include rumble and pressures options
*			This was to fix controllers that will only go into config mode once
*			Old methods should still work for backwards compatibility 
*    1.7
*		Integrated Kurt's fixes for the interrupts messing with servo signals
*		Reorganized directory so examples show up in Arduino IDE menu
*    1.8
*		Added Arduino 1.0 compatibility. 
*    1.9
*       Kurt - Added detection and recovery from dropping from analog mode, plus
*       integreated Chipkit (pic32mx...) support
*
*
*
*This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
<http://www.gnu.org/licenses/>
*  
******************************************************************/

////////////////////////////////////////////////////////////
// SECTION ACTIVATION DU DEBOGAGE
////////////////////////////////////////////////////////////
//#define PS2X_DEBUG
//#define PS2X_COM_DEBUG

#ifndef PS2X_lib_h
    #define PS2X_lib_h

#if ARDUINO > 22
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#ifdef __AVR__
    // AVR
    #include <avr/io.h>
    #define CTRL_CLK        100 //4 Valeur modifié
    #define CTRL_BYTE_DELAY 100 //3 Valeur modifié
#else
    // Pic32...
    #include <pins_arduino.h>
    #define CTRL_CLK        5
    #define CTRL_CLK_HIGH   5
    #define CTRL_BYTE_DELAY 4
#endif 

/** @name Boutons manette
 *  Représente les constantes des boutons d'une manette
 */
///@{
#define PSB_SELECT      0x0001
#define PSB_L3          0x0002
#define PSB_R3          0x0004
#define PSB_START       0x0008
#define PSB_PAD_UP      0x0010
#define PSB_PAD_RIGHT   0x0020
#define PSB_PAD_DOWN    0x0040
#define PSB_PAD_LEFT    0x0080
#define PSB_L2          0x0100
#define PSB_R2          0x0200
#define PSB_L1          0x0400
#define PSB_R1          0x0800
#define PSB_GREEN       0x1000
#define PSB_RED         0x2000
#define PSB_BLUE        0x4000
#define PSB_PINK        0x8000
#define PSB_TRIANGLE    0x1000
#define PSB_CIRCLE      0x2000
#define PSB_CROSS       0x4000
#define PSB_SQUARE      0x8000
///@}

/** @name Boutons Guitar Hero
 *  Représente les boutons d'un controller Guitar Hero
 */
///@{
#define UP_STRUM		0x0010
#define DOWN_STRUM		0x0040
#define STAR_POWER		0x0100
#define GREEN_FRET		0x0200
#define YELLOW_FRET		0x1000
#define RED_FRET		0x2000
#define BLUE_FRET		0x4000
#define ORANGE_FRET		0x8000
#define WHAMMY_BAR		8
///@}

/** @name Joystick manette
 *  Représente les joysticks de la manette
 */
///@{
#define PSS_RX 5
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8
///@}

/** @name Boutons analogiques manette
 *  Représente les versions analogique des boutons de la manette
 * \see pressures
 */
///@{
#define PSAB_PAD_RIGHT    9
#define PSAB_PAD_UP      11
#define PSAB_PAD_DOWN    12
#define PSAB_PAD_LEFT    10
#define PSAB_L2          19
#define PSAB_R2          20
#define PSAB_L1          17
#define PSAB_R1          18
#define PSAB_GREEN       13
#define PSAB_RED         14
#define PSAB_BLUE        15
#define PSAB_PINK        16
#define PSAB_TRIANGLE    13
#define PSAB_CIRCLE      14
#define PSAB_CROSS       15
#define PSAB_SQUARE      16
///@}

#define SET(x,y) (x|=(1<<y))
#define CLR(x,y) (x&=(~(1<<y)))
#define CHK(x,y) (x & (1<<y))
#define TOG(x,y) (x^=(1<<y))

class PS2X {
    // Méthodes (fonctions) accessibles depuis l'objet créée à partir de la classe
public:
    ////////////////////////////////////////////////////////////
    /// \brief Constructeur, initialise certaines variables
    ///
    ////////////////////////////////////////////////////////////
    PS2X();

    ////////////////////////////////////////////////////////////
    /// \brief Permet de savoir si le bouton en question est en train
    /// d'être appuyé.
    ///
    /// \param button Correspond à une constante représentant un bouton
    ///
    /// \return true si appuyé, sinon false
    ///
    /// \see Boutons manette
    ///
    ////////////////////////////////////////////////////////////
    boolean Button(uint16_t button);

    ////////////////////////////////////////////////////////////
    /// \brief Fonction inconnu et inutilisé
    ///
    ////////////////////////////////////////////////////////////
    unsigned int ButtonDataByte();

    ////////////////////////////////////////////////////////////
    /// \brief Permet de savoir un des boutons de la manette à
    /// changé d'état.
    ///
    /// \return true si un bouton à changé d'état, sinon false
    ///
    ////////////////////////////////////////////////////////////
    boolean NewButtonState();

    ////////////////////////////////////////////////////////////
    /// \brief Permet de savoir si le bouton en question à changé d'état.
    ///
    /// \param button Correspond à une constante représentant un bouton
    ///
    /// \return true si le bouton a changé d'état, sinon false
    ///
    /// \see Boutons manette
    ///
    ////////////////////////////////////////////////////////////
    boolean NewButtonState(unsigned int button);

    ////////////////////////////////////////////////////////////
    /// \brief Permet de savoir si le bouton en question viens juste
    /// d'être appuyé.
    ///
    /// \param button Correspond à une constante représentant un bouton
    ///
    /// \return true si vient juste d'être préssé, sinon false
    ///
    /// \see Boutons manette
    ///
    ////////////////////////////////////////////////////////////
    boolean ButtonPressed(unsigned int button);

    ////////////////////////////////////////////////////////////
    /// \brief Permet de savoir si le bouton en question viens juste
    /// d'être relâché.
    ///
    /// \param button Correspond à une constante représentant un bouton
    ///
    /// \return true si vient juste d'être relâché, sinon false
    ///
    /// \see Boutons manette
    ///
    ////////////////////////////////////////////////////////////
    boolean ButtonReleased(unsigned int button);

    ////////////////////////////////////////////////////////////
    /// \brief Permet de lire les données venant de la manette.
    /// A appeler au moins une fois par seconde.
    ///
    ////////////////////////////////////////////////////////////
    void read_gamepad();

    ////////////////////////////////////////////////////////////
    /// \brief Permet de lire les données venant de la manette.
    /// A appeler au moins une fois par seconde.
    ///
    /// \param motor1 true -> fait vibrer les moteurs de la manette, false -> les arrêtent
    /// \param motor2 Valeur de la PWM des moteurs (automatiquement calibrer à 40mini)
    ///
    /// \return true si la manette est OK (toujours en mode analogique), sinon false
    ///
    ////////////////////////////////////////////////////////////
    boolean  read_gamepad(boolean motor1, byte motor2);

    ////////////////////////////////////////////////////////////
    /// \brief Permet de lire le type de manette connecté.
    ///
    /// \return Le type de manette lu.
    ///
    ////////////////////////////////////////////////////////////
    byte readType();

    ////////////////////////////////////////////////////////////
    /// \brief Permet de configurer les pins sur lesquelles la
    /// manette est connecté.
    ///
    /// \param clk Pin correspondant à l'horloge
    /// \param cmd Pin correspondant à la commande
    /// \param att Pin correspondant à l'attention ou selection
    /// \param dat Pin correspondant à data
    ///
    /// \return Un code d'erreur
    ///
    ////////////////////////////////////////////////////////////
    byte config_gamepad(uint8_t clk, uint8_t cmd, uint8_t att, uint8_t dat);

    ////////////////////////////////////////////////////////////
    /// \brief Permet de configurer les pins sur lesquelles la
    /// manette est connecté.
    ///
    /// \param clk Pin correspondant à l'horloge
    /// \param cmd Pin correspondant à la commande
    /// \param att Pin correspondant à l'attention ou selection
    /// \param dat Pin correspondant à data
    /// \param pressures true -> active le retour de valeurs analogiques pour tous les bouton, false -> désactive le retour
    /// \param rumble true -> autorise la vibration de la manette, false -> interdit la vibration de la manette
    ///
    /// \return Un code d'erreur
    ///
    ////////////////////////////////////////////////////////////
    byte config_gamepad(uint8_t clk, uint8_t cmd, uint8_t att, uint8_t dat, bool pressures, bool rumble);

    void enableRumble();
    bool enablePressures();

    ////////////////////////////////////////////////////////////
    /// \brief Permet de lire une données de type analogique.
    ///
    /// \param button Correspond à une constante représentant un bouton ayant un retour analogique ou à un joystick
    ///
    /// \return Une comprise entre 0-255 (0 -> bouton non appuyé, <0 bouton appuyé)
    ///
    ////////////////////////////////////////////////////////////
    byte Analog(byte button);

    void reconfig_gamepad();

    ////////////////////////////////////////////////////////////
    /// \brief Permet d'avoir un retour via la console arduino
    /// des erreurs possible.
    ///
    /// \param error Le code d'erreur
    ///
    /// \see config_gamepad
    ///
    ////////////////////////////////////////////////////////////
    void printCheckError(byte error);

    ////////////////////////////////////////////////////////////
    /// \brief Permet d'avoir un retour via la console arduino
    /// du type de manette.
    ///
    /// \param type Le type de manette
    ///
    /// \see readType
    ///
    ////////////////////////////////////////////////////////////
    void printCheckControllerType(byte type);

    ////////////////////////////////////////////////////////////
    /// \brief Permet d'avoir la valeur de l'axe d'un joystick
    /// corrigé (le milieu représente désormais 0 au lieu de 127 et les bords 255).
    /// Cette fonction prend en compte les possibles tolérance mise
    ///
    /// \param joystickAxis Le type de manette
    ///
    /// \return La valeur correctement mappé
    ///
    /// \see setAllAxisTolerance
    /// \see setAxisTolerance
    ///
    ////////////////////////////////////////////////////////////
    inline byte getCorrectedAxis(byte const joystickAxis);

    ////////////////////////////////////////////////////////////
    /// \brief Permet de préciser une tolérance pour un axe d'un
    /// joystick précis. Attention, si cette fonction est appelé
    /// pour un axe d'un joystick, elle doit être aussi pour tous les
    /// autre (donc cette fonction doit être toujours appelé 4fois au total).
    ///
    /// ATTENTION: une seul fonction précisant la tolérance doit être appelé:
    /// 4x setAxisTolerance(byte axis, byte lowerBound, byte upperBound); ou
    /// setAllAxisTolerance(byte lowerBound, byte upperBound); ou
    /// setAllAxisTolerance(byte tolerance);
    ///
    /// \param axis Représente l'axe en question
    /// \param lowerBound Représente la borne minimal avant de ne plus être centré
    /// \param upperBound Représente la borne maximal avant de ne plus être centré
    ///
    /// \see setAllAxisTolerance
    ///
    ////////////////////////////////////////////////////////////
    void setAxisTolerance(byte axis, byte lowerBound, byte upperBound);

    ////////////////////////////////////////////////////////////
    /// \brief Permet de préciser une tolérance pour tous les axes en
    /// précisant la borne minimal et maximal
    ///
    /// ATTENTION: une seul fonction précisant la tolérance doit être appelé:
    /// 4x setAxisTolerance(byte axis, byte lowerBound, byte upperBound); ou
    /// setAllAxisTolerance(byte lowerBound, byte upperBound); ou
    /// setAllAxisTolerance(byte tolerance);
    ///
    /// \param lowerBound Représente la borne minimal avant de ne plus être centré
    /// \param upperBound Représente la borne maximal avant de ne plus être centré
    ///
    /// \see setAxisTolerance
    ///
    ////////////////////////////////////////////////////////////
    void setAllAxisTolerance(byte lowerBound, byte upperBound);

    ////////////////////////////////////////////////////////////
    /// \brief Permet de préciser une tolérance pour tous les axes.
    ///
    /// ATTENTION: une seul fonction précisant la tolérance doit être appelé:
    /// 4x setAxisTolerance(byte axis, byte lowerBound, byte upperBound); ou
    /// setAllAxisTolerance(byte lowerBound, byte upperBound); ou
    /// setAllAxisTolerance(byte tolerance);
    ///
    /// \param tolerance Tolerance à appliquer
    ///
    /// \see setAxisTolerance
    ///
    ////////////////////////////////////////////////////////////
    void setAllAxisTolerance(byte tolerance);

    ////////////////////////////////////////////////////////////
    /// \brief Permet de savoir si le joystick est centré, prend en compte
    /// la tolérance si cette dernière a été défini.
    ///
    /// \param axis Représente le joystick et l'axe en question
    ///
    /// \see setAllAxisTolerance
    /// \see setAxisTolerance
    ///
    ////////////////////////////////////////////////////////////
    inline boolean axisCentered(byte axis);

    ////////////////////////////////////////////////////////////
    /// \brief Permet d'avoir un retour des coordonnées de tous les
    /// axis de la manette via la console d'arduino.
    ///
    ////////////////////////////////////////////////////////////
    inline void printAllAxis();

    ////////////////////////////////////////////////////////////
    /// \brief Permet d'avoir la borne minimal de l'axe. Prend en compte
    /// la tolérance si cette dernière a été précisé
    ///
    /// \param axis Représente le joystick et l'axe en question
    ///
    /// \see setAllAxisTolerance
    /// \see setAxisTolerance
    ///
    ////////////////////////////////////////////////////////////
    byte getMinBound(byte const axis);

    ////////////////////////////////////////////////////////////
    /// \brief Permet d'avoir la borne maximal de l'axe. Prend en compte
    /// la tolérance si cette dernière a été précisé
    ///
    /// \param axis Représente le joystick et l'axe en question
    ///
    /// \see setAllAxisTolerance
    /// \see setAxisTolerance
    ///
    ////////////////////////////////////////////////////////////
    byte getMaxBound(byte const axis);

    // Membres (variables) et méthodes (fonctions) accessibles uniquement à l'intérieur de la classe
private:
    struct BoundsTolerance
    {
        byte lower;
        byte upper;
    };
    void initTolerance(BoundsTolerance &tolerance);

    inline void CLK_SET(void);
    inline void CLK_CLR(void);
    inline void CMD_SET(void);
    inline void CMD_CLR(void);
    inline void ATT_SET(void);
    inline void ATT_CLR(void);
    inline bool DAT_CHK(void);

    unsigned char _gamepad_shiftinout(char); // envoi un char à la manette et en renvoi un char récupéré de la manette
    unsigned char PS2data[21];
    void sendCommandString(byte*, byte);
    unsigned char i;
    unsigned int last_buttons;
    unsigned int buttons;
    struct BoundsTolerance m_LXtolerance;
    struct BoundsTolerance m_LYtolerance;
    struct BoundsTolerance m_RXtolerance;
    struct BoundsTolerance m_RYtolerance;
    byte m_globalTolerance;

    #ifdef __AVR__
        uint8_t maskToBitNum(uint8_t);
        uint8_t _clk_mask; 
        volatile uint8_t *_clk_oreg;
        uint8_t _cmd_mask; 
        volatile uint8_t *_cmd_oreg;
        uint8_t _att_mask; 
        volatile uint8_t *_att_oreg;
        uint8_t _dat_mask; 
        volatile uint8_t *_dat_ireg;
    #else
        uint8_t maskToBitNum(uint8_t);
        uint16_t _clk_mask; 
        volatile uint32_t *_clk_lport_set;
        volatile uint32_t *_clk_lport_clr;
        uint16_t _cmd_mask; 
        volatile uint32_t *_cmd_lport_set;
        volatile uint32_t *_cmd_lport_clr;
        uint16_t _att_mask; 
        volatile uint32_t *_att_lport_set;
        volatile uint32_t *_att_lport_clr;
        uint16_t _dat_mask; 
        volatile uint32_t *_dat_lport;
    #endif

    unsigned long last_read;
    byte read_delay;
    byte controller_type;
    boolean en_Rumble;
    boolean en_Pressures;
};

////////////////////////////////////////////////////////////
// Définition des fonctions inline appelé hors de la classe ici
// sinon lors de la résolution des liens: 'erreur fonction non défini'
// Le mot clé inline est l'équivalent d'une macro fait avec un define.
// Cela permet d'optimiser le programme mais il faut l'utilisé avec
// parcimonie car cela augmente la taille du programme et peut avoir
// l'effet inverse d'une optimisation et donc ralentir le programme.
//
////////////////////////////////////////////////////////////

/****************************************************************************************/
inline byte PS2X::getCorrectedAxis(byte const joystickAxis) {
    if (!axisCentered(joystickAxis)) {
        byte coord = Analog(joystickAxis);
        if (m_LXtolerance.lower != 0 || m_LYtolerance.lower != 0 || m_RXtolerance.lower != 0 || m_RYtolerance.lower != 0) {
            switch (joystickAxis) {
                case PSS_LX:
                    if (coord < m_LXtolerance.lower)
                        return map(coord, m_LXtolerance.lower, 0, 0, 255);
                    else
                        return map(coord, m_LXtolerance.upper, 255, 0, 255);
                    break;
                case PSS_LY:
                    if (coord < m_LYtolerance.lower)
                        return map(coord, m_LYtolerance.lower, 0, 0, 255);
                    else
                        return map(coord, m_LYtolerance.upper, 255, 0, 255);
                    break;
                case PSS_RX:
                    if (coord < m_RXtolerance.lower)
                        return map(coord, m_RXtolerance.lower, 0, 0, 255);
                    else
                        return map(coord, m_RXtolerance.upper, 255, 0, 255);
                    break;
                case PSS_RY:
                    if (coord < m_RYtolerance.lower)
                        return map(coord, m_RYtolerance.lower, 0, 0, 255);
                    else
                        return map(coord, m_RYtolerance.upper, 255, 0, 255);
                    break;
            }
        }
        else if (m_globalTolerance != 0) {
            if (coord < 127)
                return map(coord, 127 - m_globalTolerance, 0, 0, 255);
            else
                return map(coord, 127 + m_globalTolerance, 255, 0, 255);
        }
        else
            return coord;
    }

  return 0;
}

/****************************************************************************************/
inline boolean PS2X::axisCentered(byte axis) {
    if (m_LXtolerance.lower != 0 || m_LYtolerance.lower != 0 || m_RXtolerance.lower != 0 || m_RYtolerance.lower != 0) {
        switch (axis) {
            case PSS_LX:
                if (Analog(PSS_LX) > m_LXtolerance.lower && Analog(PSS_LX) < m_LXtolerance.upper)
                return true;
                break;
            case PSS_LY:
                if (Analog(PSS_LY) > m_LYtolerance.lower && Analog(PSS_LY) < m_LYtolerance.upper)
                return true;
                break;
            case PSS_RX:
                if (Analog(PSS_RX) > m_RXtolerance.lower && Analog(PSS_RX) < m_RXtolerance.upper)
                return true;
                break;
            case PSS_RY:
                if (Analog(PSS_RY) > m_RYtolerance.lower && Analog(PSS_RY) < m_RYtolerance.upper)
                return true;
                break;
        }
    }
    else if (m_globalTolerance != 0) {
        return (Analog(axis) > 127 - m_globalTolerance && Analog(axis) < 127 + m_globalTolerance);
    }
    else
        return (Analog(axis) == 127);
    return false;
}

/****************************************************************************************/
inline void PS2X::printAllAxis() {
    byte axisLX = Analog(PSS_LX);
    byte axisLY = Analog(PSS_LY);
    byte axisRX = Analog(PSS_RX);
    byte axisRY = Analog(PSS_RY);

    Serial.print("Coordonnées: ");
    Serial.print("axisLX: ");
    Serial.print(axisLX, DEC);
    Serial.print(" axisLY: ");
    Serial.print(axisLY, DEC);
    Serial.print(" axisRX: ");
    Serial.print(axisRX, DEC);
    Serial.print(" axisRY: ");
    Serial.println(axisRY, DEC);
}

#endif

////////////////////////////////////////////////////////////
/// \class PS2X
/// 
/// La classe PS2X permet de contrôler des manettes PS2 comme
/// un controller pour Guitar Hero.
/// Elle a été trouvé sur le site www.billporter.info, et
/// modifié pour correspondre à nos besoins.
///
/// Cette bibliothéque a un mode debug qui permet d'écrire
/// tous les signaux transmit et reçu entre l'arduino et la
/// manette. Pour l'activer il faut aller modifié le .h même
/// de la bibliothéque et décommenter les lignes:
/// \code
/// //#define PS2X_DEBUG
/// //#define PS2X_COM_DEBUG
/// \endcode
///
////////////////////////////////////////////////////////////