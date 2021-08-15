#include "stdafx.h"
#include "Ghost.h"

Ghost::Ghost(Vector2i spawnSquare, int speed, int sheetOrder, int ID) {
	float x = spawnSquare.x * 30 + rand() % 28;
	float y = spawnSquare.y * 30 + rand() % 28;

	m_position = Vector2f(x, y);
	m_speed = speed + rand() % 10;
	m_nextGhost = NULL;
	m_sheetOrder = sheetOrder;
	m_ID = ID;
}

void Ghost::setPosition(Vector2f newPosition) {
	m_position = newPosition;
}

Vector2f Ghost::getPosition() {
	return m_position;
}

float Ghost::getSpeed() {
	return m_speed;
}

int Ghost::getSheetOrder() {
	return m_sheetOrder;
}

Direction Ghost::getDirection() {
	return m_direction;
}

void Ghost::setDirection(Direction newDirection) {
	m_direction = newDirection;
}

int Ghost::getGhostSize() {
	return m_ghostSize;
}

Ghost* Ghost::getNextGhost() {
	return m_nextGhost;
}

void Ghost::setNextGhost(Ghost* newGhost) {
	m_nextGhost = newGhost;
}

bool Ghost::isChasingPacman() {
	return m_chasingPacman;
}

void Ghost::pacmanChase(bool chase) {
	m_chasingPacman = chase;
}

int Ghost::getID() {
	return m_ID;
}