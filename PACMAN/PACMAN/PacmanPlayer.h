#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;


class Pacman {
private:
	Vector2f m_position;
	Vector2f m_prevPosition;
	float m_speed = 150;		// pixels per second 
	int m_pacmanSize = 30;
	float m_rotation;
	int m_sheetSelection;
	float m_sheetSwapTime = 0.05; // as seconds
	float m_lastSwap = 0; // as seconds

public:
	Vector2f getPosition();
	Vector2f getPrevPosition();
	void setPosition(Vector2f newPosition);
	int getPacmanSize();
	float getRotation();
	void setRotation(float newRotation);

	float getSpeed();

	void updateSheetSelection(float dtAsSeconds);
	IntRect getSheetSelection();

};


