#ifndef CONSTANTES_H
#define CONSTANTES_H

/** @name Manette
 *  Sélection des pins connecté au contrôleur
 */
///@{
#define PS2_DAT        A0  ///<  Représente la ligne DATA
#define PS2_CMD        A1  ///<  Représente la ligne COMMAND
#define PS2_SEL        A2  ///<  Représente la ligne SELECTION ou ATTENTION
#define PS2_CLK        A3  ///<  Représente la ligne CLOCK
///@}

/** @name Configuration manette
 *  Sélection des modes du contrôleur:
 * - pressures = lecture analogique de la pression des boutons
 * - rumble    = vibration moteur
 */
///@{
#define pressures   true ///<  Prend la valeur true ou false @hideinitializer
#define rumble      false ///<  Prend la valeur true ou false @hideinitializer
///@}

/** @name Moteur
 *  Sélection des pins de PWM des moteurs du robot
 */
///@{
#define PWM_PIN_MOTOR_1 3
#define PWM_PIN_MOTOR_2 5
///@}

/** @name Direction moteur
 *  Sélection des pins de direction des moteurs du robot
 */
///@{
#define DIR_PIN_MOTOR_1 8
#define DIR_PIN_MOTOR_2 7
///@}

/** @name Servomoteur
 *  Sélection des pins des servomoteurs du robot
 */
///@{
#define PIN_SERVO_1 9
#define PIN_SERVO_2 10
///@}

/** @name Vitesses des moteurs
 */
///@{
#define VITESSE_MAX 255
#define VITESSE_MIN 0
///@}

#endif // !CONSTANTES_H
