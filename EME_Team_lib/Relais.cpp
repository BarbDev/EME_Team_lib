#include "Relais.h"

Relais::Relais(byte const pin) : m_pin(pin) {
	pinMode(pin, OUTPUT);
}

void Relais::setState(byte const state) {
	m_state = state;
}

byte Relais::getState() const {
	return m_state;
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