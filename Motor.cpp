#include "constantes.h"
#include "Motor.h"


Motor::Motor(byte const pwmPin, byte const dirPin) : m_motorPin(pwmPin), m_dirPin(dirPin) {
	pinMode(pwmPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	m_direction = CLOCKWISE;
	m_speed = 0;
}

void Motor::setDir(MOTORDIR const direction) {
	m_direction = direction;
}

void Motor::setSpeed(byte const speed) {
	m_speed = speed;
}

MOTORDIR Motor::getDir() const {
	return m_direction;
}

byte Motor::getSpeed() const {
	return m_speed;
}

void Motor::stop() {
	setSpeed(VITESSE_MIN);
}

void Motor::invertDir() {
	if (m_direction == CLOCKWISE)
		m_direction = ANTI_CLOCKWISE;
	else
		m_direction = CLOCKWISE;
}

void Motor::update() {
	digitalWrite(m_dirPin, m_direction);
	analogWrite(m_motorPin, m_speed);
}
