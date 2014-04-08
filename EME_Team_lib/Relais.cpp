#include "Relais.h"

Relais::Relais(byte const pin, boolean const autoUpdate) : m_pin(pin), m_state(LOW), m_autoUpdate(autoUpdate) {
	pinMode(pin, OUTPUT);
}

void Relais::setState(byte const state) {
	m_state = state;
	if (m_autoUpdate)
		update();
}

byte Relais::getState() const {
	return m_state;
}

void Relais::changeState() {
	if (m_state == HIGH)
		m_state = LOW;
	else
		m_state = HIGH;
	if (m_autoUpdate)
		update();
}

void Relais::update() {
	digitalWrite(m_pin, m_state);
}