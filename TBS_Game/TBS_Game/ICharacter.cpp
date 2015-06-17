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
	return m_gridPos;
}
void ICharacter::setGridPos(const int& x, const int& y) {
	m_gridPos.x = x;
	m_gridPos.y = y;
}

void ICharacter::showOverlay(bool show) {
	m_showOverlay = show;
}