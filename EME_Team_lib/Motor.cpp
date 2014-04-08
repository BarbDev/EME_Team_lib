#include "Motor.h"


Motor::Motor(byte const pwmPin, byte const dirPin, boolean const autoUpdate) : m_motorPin(pwmPin), m_dirPin(dirPin), m_autoUpdate(autoUpdate) {
	pinMode(pwmPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	m_direction = CLOCKWISE;
	m_speed = 0;
}

void Motor::setDir(MOTORDIR const direction) {
	m_direction = direction;
	if (m_autoUpdate)
		update();
}

void Motor::setSpeed(byte const speed) {
	m_speed = speed;
	if (m_autoUpdate)
		update();
}

void Motor::incrementSpeed(byte const increment) {
	if (m_speed + increment > 255)
		m_speed = 255;
	else
		m_speed = m_speed + increment;
	if (m_autoUpdate)
		update();
}

MOTORDIR Motor::getDir() const {
	return m_direction;
}

byte Motor::getSpeed() const {
	return m_speed;
}

void Motor::stop() {
	setSpeed(0);
}

void Motor::invertDir() {
	if (m_direction == CLOCKWISE)
		m_direction = ANTI_CLOCKWISE;
	else
		m_direction = CLOCKWISE;
	if (m_autoUpdate)
		update();
}

void Motor::update() {
	digitalWrite(m_dirPin, m_direction);
	analogWrite(m_motorPin, m_speed);
}
