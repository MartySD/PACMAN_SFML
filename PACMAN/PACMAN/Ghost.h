#pragma once
#include <SFML/Graphics.hpp>
#include "MyEnums.h"
using namespace sf;

//enum GhostDirection { right, up, left, down, NONE };

class Ghost {
private:
	Vector2f m_position;
	float m_speed = 120;		/* pixels per second */
	int m_ghostSize = 30 + rand() % 20;
	int m_sheetOrder;
	Direction m_direction = Direction::UP;
	bool m_chasingPacman = false;
	int m_ID;

	Ghost* m_nextGhost;


public:
	Ghost(Vector2i spawnSquare, int speed, int sheetOrder, int ID);

	Vector2f getPosition();
	void setPosition(Vector2f newPosition);
	int getGhostSize();

	float getSpeed();
	int getSheetOrder();
	Direction getDirection();
	void setDirection (Direction newDirection);

	Ghost* getNextGhost();
	void setNextGhost(Ghost* newGhost);

	bool isChasingPacman();
	void pacmanChase(bool chase);

	int getID();

};