#include "stdafx.h"
#include "Square.h"


bool Square::isWallHere() {
	return m_wall;
}

void Square::placeWall() {
	m_wall = true;
}

void Square::removeWall() {
	m_wall = false;
}

bool Square::isFoodHere() {
	return m_food;
}

void Square::placeFood() {
	m_food = true;
}

void Square::removeFood() {
	m_food = false;
}

int Square::getDistanceFromPacman() {
	return m_distanceFromPacman;
}

void Square::setDistanceFromPacman(int newDistanceFromPacman) {
	m_distanceFromPacman = newDistanceFromPacman;
}

Direction Square::getDirectionToPacman() {
	return m_directionToPacman;
}
void Square::setDirectionToPacman(Direction direction) {
	m_directionToPacman = direction;
}

void Square::setDecitionPointHere() {		// GHOST IN THESE CORDS DECIDE ABOUT CHANGING DIRECTION
	m_decitionPoint = true;
}

bool Square::isDecitionPointHere() {
	return m_decitionPoint;
}

void Square::setSheetSelection(int newSheetSelection) {
	m_sheetSelection = newSheetSelection;
}

int Square::getSheetSelection() {
	return m_sheetSelection;
}

void Square::setSpecialEffectHere(Effect newEffect) {
	m_effect = newEffect;
}

Effect Square::getEffect() {
	return m_effect;
}

int Square::getFoodSheetLeft() {
	return m_foodSheetLeft;
}

void Square::setFoodSheetLeft(int newSheetLeft) {
	m_foodSheetLeft = newSheetLeft;
}