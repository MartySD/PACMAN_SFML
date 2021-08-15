#pragma once
#include <SFML/Graphics.hpp>
#include "PacmanEngine.h"
#include "PacmanPlayer.h"
#include "Square.h"
#include "Ghost.h"
#include "Heap.h"
using namespace sf;

int const ROW = 31;
int const COL = 28;




class Engine 
{
private:
	bool m_solveGame = false;

	bool m_UP;
	bool m_RIGHT;
	bool m_DOWN;
	bool m_LEFT;

	Pacman m_pacman;

	bool m_reverseChase = false;
	float m_chaseTimeAsSeconds = 0;

	int spawnedGhosts = 0;
	Ghost* m_ghostsPointer = NULL;
	Vector2f ghostSpawnPosition;

	void spawnGhost(Vector2i spawnSquare, int speed, int ID);

	void updatePacMan(float dtAsSeconds);
	void updateGhosts(float dtAsSeconds);
	void updateMap();

	int m_foodCounter = 0;

public:
	Square map[ROW][COL];

	Engine(int irelevant);

	void update(float drAsSeconds);
	Vector2i mapPositionToSquare(Vector2f worldPosition);
	int getFoodCounter();

	void movePacmanUp();
	void movePacmanRight();
	void movePacmanDown();
	void movePacmanLeft();

	Direction findNewWay(Ghost* ghost);

	Vector2f getPacmanPosition();
	float getPacmanRotation();
	IntRect getPacmanSheetSelection();

	Ghost* getGhosts();

	Effect getEffect();
	void deleteEffect();

	void startSolveGame();
	void stopSolveGame();

	bool collisionDetection();

	float getChaseTime();

	void engineRestart();
};

