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

/** @name Relais
 *  Sélection des pins des relais du robot
 */
///@{
#define PIN_RELAIS_1 4
#define PIN_RELAIS_2 7
///@}

/** @name Moteur
 *  Sélection des pins de PWM des moteurs du robot
 */
///@{
#define PWM_PIN_MOTOR_1 3
#define PWM_PIN_MOTOR_2 6
#define PWM_PIN_MOTOR_3 5
#define PWM_PIN_MOTOR_4 9
///@}

/** @name Direction moteur
 *  Sélection des pins de direction des moteurs du robot
 */
///@{
#define DIR_PIN_MOTOR_1 2
#define DIR_PIN_MOTOR_2 12
#define DIR_PIN_MOTOR_3 8
#define DIR_PIN_MOTOR_4 13
///@}

/** @name Servomoteur
 *  Sélection des pins des servomoteurs du robot
 */
///@{
#define PIN_SERVO_1 1
#define PIN_SERVO_2 10
#define PIN_SERVO_3 11
///@}

/** @name Vitesses des moteurs
 */
///@{
#define VITESSE_MAX 255
#define VITESSE_MIN 0
///@}

#endif // !CONSTANTES_H
