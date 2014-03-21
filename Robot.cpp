#include "Robot.h"
#include "constantes.h"

void Robot::setup(byte const pinMotor1, byte const pinDir1, byte const pinMotor2, byte const pinDir2, byte const pinServo1, byte const pinServo2) {
    m_moteurGauche.setup(pinMotor1, pinDir1);
    m_moteurDroit.setup(pinMotor2, pinDir2);
    m_servo1.attach(pinServo1);
    m_servo2.attach(pinServo2);
    m_controlType = TANK_TYPE;
    m_commandType = NORMAL;
}

void Robot::setControlType(WHEELSCONTROLS const control) {
    m_controlType = control;
}

void Robot::setCommandType(JOYSTICKCOMMAND const command) {
    m_commandType = command;
}

void Robot::processCommand(PS2X &manette) {
    /* Dans ce switch, les joystick sont gérés */
	switch (m_controlType) {
        case TANK_TYPE:
            {
                switch (m_commandType) {
                    case NORMAL:
                    {
                        /* Joystick gauche */
                        if (manette.Analog(PSS_LY) == 255) {
                            m_moteurGauche.setDir(CLOCKWISE);
                            byte speed = m_moteurGauche.getSpeed();
                            if (speed < VITESSE_MAX)
                                m_moteurGauche.setSpeed(++speed);
                        }
                        else if (manette.Analog(PSS_LY) == 0) {
                            m_moteurGauche.setDir(ANTI_CLOCKWISE);
                            byte speed = m_moteurGauche.getSpeed();
                            if (speed < VITESSE_MAX)
                                m_moteurGauche.setSpeed(++speed);
                        }
                        else
                            m_moteurGauche.stop();

                        /* Joystick droit */
                        if (manette.Analog(PSS_RY) == 255) {
                            m_moteurDroit.setDir(CLOCKWISE);
                            byte speed = m_moteurGauche.getSpeed();
                            if (speed < VITESSE_MAX)
                                m_moteurDroit.setSpeed(++speed);
                        }
                        else if (manette.Analog(PSS_RY) == 0) {
                            m_moteurDroit.setDir(ANTI_CLOCKWISE);
                            byte speed = m_moteurGauche.getSpeed();
                            if (speed < VITESSE_MAX)
                                m_moteurDroit.setSpeed(++speed);
                        }
                        else
                            m_moteurDroit.stop();
                    }
                        break;
                    case INVERTED:
                        invertTankCommand();
                        break;
                }
            }
            break;
        case REGULAR:
            {
                /* Augmentation ou diminution de la vitesse */
                static byte speed = VITESSE_MIN;
                if (manette.Analog(PSS_RY) == 255) {
                    m_moteurDroit.setDir(CLOCKWISE);
                    m_moteurGauche.setDir(CLOCKWISE);
                    if (speed < VITESSE_MAX)
                        speed++;
                }
                else if (manette.Analog(PSS_RY) == 0) {
                    m_moteurGauche.setDir(ANTI_CLOCKWISE);
                    m_moteurDroit.setDir(ANTI_CLOCKWISE);
                    if (speed < VITESSE_MAX)
                        speed++;
                }
                else
                    if (speed > VITESSE_MIN)
                        speed--;

                /* Prise en compte de la direction */
                if (manette.Analog(PSS_LX) == 0) { // On veut tourner à gauche
                    m_moteurGauche.stop();
                }
                else if (manette.Analog(PSS_LX) == 255) { // On veut tourner à droite
                    m_moteurDroit.stop();
                }
            }
            break;
    }
    
    /* Servomoteur 1 */
    if (manette.Button(PSB_R1)) {
        m_servo1.write(0);
    }
    else if (manette.Button(PSB_R2)) {
        m_servo1.write(179);
    }

    /* Servomoteur 2 */
    if (manette.Button(PSB_L1)) {
        m_servo2.write(0);
    }
    else if (manette.Button(PSB_L2)) {
        m_servo2.write(179);
    }
    
    updateComponent();
}

void Robot::invertTankCommand() {
    /* Inversion des directions des 2 moteurs */
    m_moteurGauche.invertDir();
    m_moteurDroit.invertDir();

    /* Inversion des directions entre les 2 moteurs */
    enum MOTORDIR dirMotor1 = m_moteurGauche.getDir();

    m_moteurGauche.setDir(m_moteurDroit.getDir());
    m_moteurDroit.setDir(dirMotor1);

    /* Inversion des vitesses entre les 2 moteurs */
    byte motorSpeed1 = m_moteurGauche.getSpeed();

    m_moteurGauche.setSpeed(m_moteurDroit.getSpeed());
    m_moteurDroit.setSpeed(motorSpeed1);
}

void Robot::updateComponent() {
    m_moteurGauche.update();
    m_moteurDroit.update();
}
