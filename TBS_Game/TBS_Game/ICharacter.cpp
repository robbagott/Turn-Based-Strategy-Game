#include "ICharacter.h"

int ICharacter::health() const{
	return m_health;
}
void ICharacter::setHealth(const int& val) {
	m_health = val;
}
int ICharacter::strength() const{
	return m_strength;
}
int ICharacter::armor() const{
	return m_armor;
}
int ICharacter::intelligence() const{
	return m_intelligence;
}
int ICharacter::evasion() const{
	return m_evasion;
}
int ICharacter::energy() const{
	return m_energy;
}
int ICharacter::movePoints() const{
	return m_movePoints;
}

sf::Vector2i ICharacter::gridPos() const {
	return sf::Vector2i(m_gridx, m_gridy);
}
void ICharacter::setGridPos(const int& x, const int& y) {
	m_gridx = x;
	m_gridy = y;
}