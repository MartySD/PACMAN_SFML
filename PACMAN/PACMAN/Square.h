#pragma once
#include <SFML/Graphics.hpp>
#include "MyEnums.h"
using namespace sf;

//enum SquareDirection {RIGHT, UP, LEFT, DOWN, none };

class Square {
private:
	bool m_wall = false;
	bool m_food = false;
	bool m_decitionPoint = false;
	int m_distanceFromPacman;
	Direction m_directionToPacman = NONE;
	int m_sheetSelection = 210;
	Effect m_effect = SOBER;
	int m_foodSheetLeft = 0;

public:
	bool isWallHere();
	void placeWall();
	void removeWall();

	bool isFoodHere();
	void placeFood();
	void removeFood();

	void setDecitionPointHere();
	bool isDecitionPointHere();

	void setSpecialEffectHere(Effect newEffect);
	Effect getEffect();

	int getDistanceFromPacman();
	void setDistanceFromPacman(int newDistanceFromPacman);

	Direction getDirectionToPacman();
	void setDirectionToPacman(Direction direction);

	void setSheetSelection(int newSheetSelection);
	int getSheetSelection();

	int getFoodSheetLeft();
	void setFoodSheetLeft(int newSheetLeft);

};