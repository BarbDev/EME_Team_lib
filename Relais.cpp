#include "Relais.h"

void Relais::setup(byte const pin) {
	pinMode(pin, OUTPUT);
	m_pin = pin;
}

void Relais::setState(byte const state) {
	m_state = state;
}

void Relais::changeState() {
	if (m_state == HIGH)
		m_state = LOW;
	else
		m_state = HIGH;
}

void Relais::update() {
	digitalWrite(m_pin, m_state);
}