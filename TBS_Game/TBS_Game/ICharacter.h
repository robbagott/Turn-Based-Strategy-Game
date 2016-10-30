#ifndef CHARACTER_H
#define CHARACTER_H

#include "IGameActor.h"

class ICharacter : public IGameActor
{
public:
	virtual ~ICharacter() {}

	virtual sf::Vector2f position() const = 0;
	virtual void setPosition(const int& x, const int& y) = 0;
	virtual sf::Vector2i gridPos() const;
	virtual void setGridPos(const int& x, const int& y);

	int health() const;
	virtual void setHealth(const int& val) = 0;
	bool isAlive();
	bool isMoved();
	void setMoved(bool isMoved);
	int strength() const;
	int armor() const;
	int intelligence() const;
	int evasion() const;
	int energy() const;
	int movePoints() const;
	virtual bool friendly() const = 0;

	virtual void attack(ICharacter* attackedCharacter) = 0;
	
	void showOverlay(bool show);

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
	//If character has been moved
	bool m_isMoved = false;

	//MOST LIKELY TEMPORARY**********************
	bool m_friendly;

	bool m_showOverlay = false;
	//Player controlled or enemy controlled
	sf::Vector2i m_gridPos;
};

#endif