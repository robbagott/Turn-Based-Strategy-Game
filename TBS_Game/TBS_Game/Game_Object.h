#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class GameObject
{
public:
	virtual void update() = 0;
	virtual void draw() = 0;
};

#endif