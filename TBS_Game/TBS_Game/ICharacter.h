#ifndef CHARACTER_H
#define CHARACTER_H

#include "IGameActor.h"

class ICharacter : public IGameActor
{
public:
	virtual ~ICharacter() {}

	virtual sf::Vector2f getPosition() const = 0;
	virtual void setPosition(const sf::Vector2f&) = 0;

protected:
	//How much damage can be taken
	int m_health = 0;
	//How much resistance to physical attack
	//How much damage dealt by physical attacks
	int m_strength = 0;
	//armor, like strength, increases resistance to physical attack
	int m_armor = 0;
	//How much resistance to mental attacks
	//How much damage mental attacks do
	int m_intelligence = 0;
	//How much chance to evade
	int m_evasion = 0;
	//How much of any mental attacks a character has
	int m_energy = 0;
	//How many tiles unit can cover per turn
	int m_movePoints = 0;
};

#endif