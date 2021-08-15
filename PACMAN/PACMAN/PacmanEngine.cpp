#include "stdafx.h"
#include "PacmanEngine.h"


/* --------------------------- ENGINE ------------------------ */
Vector2i Engine::mapPositionToSquare(Vector2f worldPosition) {
	int x = worldPosition.x;	// convert float to int
	int y = worldPosition.y;
	int pacmanRadius = m_pacman.getPacmanSize() / 2;


	return Vector2i((y + pacmanRadius) / 30, (x + pacmanRadius) / 30);		// returns exact middle of sprite
}

int Engine::getFoodCounter() {
	return m_foodCounter;
}


void Engine::update(float dtAsSeconds) {
	if (m_chaseTimeAsSeconds > 0) {
		m_chaseTimeAsSeconds -= dtAsSeconds;
	}
	else {
		m_reverseChase = false;
	}

	if (m_solveGame == false) {
		return;
	}


	updatePacMan(dtAsSeconds);

	if (mapPositionToSquare(m_pacman.getPrevPosition()) != mapPositionToSquare(m_pacman.getPosition())) {	// OPTIMALIZATION
		updateMap();
	}
	updateGhosts(dtAsSeconds);





	m_UP = false;
	m_RIGHT = false;
	m_DOWN = false;
	m_LEFT = false;

	// TODO REST

}


void Engine::movePacmanUp() {
	m_UP = true;
}

void Engine::movePacmanRight() {
	m_RIGHT = true;
}

void Engine::movePacmanDown() {
	m_DOWN = true;
}

void Engine::movePacmanLeft() {
	m_LEFT = true;
}



void Engine::updatePacMan(float dtAsSeconds) {
	// Position
	m_pacman.updateSheetSelection(dtAsSeconds);

	Vector2f oldPosition = m_pacman.getPosition();
	Vector2f newPosition = oldPosition;

	if (m_UP) {
		newPosition.y -= dtAsSeconds * m_pacman.getSpeed();
	}
	if (m_RIGHT) {
		newPosition.x += dtAsSeconds * m_pacman.getSpeed();
	}
	if (m_DOWN) {
		newPosition.y += dtAsSeconds * m_pacman.getSpeed();
	}
	if (m_LEFT) {
		newPosition.x -= dtAsSeconds * m_pacman.getSpeed();
	}

	Vector2i newSquarePosition = mapPositionToSquare(newPosition);
	if (!map[newSquarePosition.x][newSquarePosition.y].isWallHere()) {	// NO WALL
		m_pacman.setPosition(newPosition);
		if (map[newSquarePosition.x][newSquarePosition.y].isFoodHere()) {
			map[newSquarePosition.x][newSquarePosition.y].removeFood();
			m_foodCounter--;
		}

	}

	else {	// SHIFTING MOVEMENT (AGAINST WALL STUCK)
		Vector2f modifiedPositionX = oldPosition;
		Vector2f modifiedPositionY = oldPosition;
		Vector2i modifiedSquarePositionX;
		Vector2i modifiedSquarePositionY;

		modifiedPositionX.x = newPosition.x;
		modifiedPositionY.y = newPosition.y;
		modifiedSquarePositionX = mapPositionToSquare(modifiedPositionX);
		modifiedSquarePositionY = mapPositionToSquare(modifiedPositionY);


		if (!map[modifiedSquarePositionX.x][modifiedSquarePositionX.y].isWallHere()) {
			m_pacman.setPosition(modifiedPositionX);
			if (map[modifiedSquarePositionX.x][modifiedSquarePositionX.y].isFoodHere()) {
				map[modifiedSquarePositionX.x][modifiedSquarePositionX.y].removeFood();
				m_foodCounter--;
			}
		}

		if (!map[modifiedSquarePositionY.x][modifiedSquarePositionY.y].isWallHere()) {
			m_pacman.setPosition(modifiedPositionY);
			if (map[modifiedSquarePositionY.x][modifiedSquarePositionY.y].isFoodHere()) {
				map[modifiedSquarePositionY.x][modifiedSquarePositionY.y].removeFood();
				m_foodCounter--;
			}
		}
	}
	

	// ROTATION
	m_pacman.setRotation(666);		// reset before every frame

	if (m_RIGHT == true) {
		m_pacman.setRotation(90);
	}

	if (m_LEFT == true) {
		m_pacman.setRotation(270);
	}

	if (m_UP == true) {
		m_pacman.setRotation(0);
		if (m_RIGHT) {
			m_pacman.setRotation(45);
		}
		if (m_LEFT) {
			m_pacman.setRotation(315);
		}
	}

	if (m_DOWN == true) {
		m_pacman.setRotation(180);
		if (m_RIGHT) {
			m_pacman.setRotation(135);
		}
		if (m_LEFT) {
			m_pacman.setRotation(225);
		}
	}
}


IntRect Engine::getPacmanSheetSelection() {
	return m_pacman.getSheetSelection();
}


void Engine::spawnGhost(Vector2i spawnSquare, int speed, int ID) {
	if (m_ghostsPointer == NULL) {
		m_ghostsPointer = new Ghost(spawnSquare, speed, spawnedGhosts, ID);
	}
	else {
		Ghost* newGhostPointer = new Ghost(spawnSquare, speed, spawnedGhosts, ID);
		newGhostPointer[0].setNextGhost(m_ghostsPointer);
		m_ghostsPointer = newGhostPointer;
	}
	spawnedGhosts++;
}


void Engine::updateGhosts(float dtAsSeconds) {
	Ghost* ghostPointer = m_ghostsPointer;

	while (ghostPointer != NULL) {		// UPDATE EACH GHOST INDIVIDUALLY

		Vector2i squareCords = mapPositionToSquare(ghostPointer[0].getPosition());
		Direction squareDirection = map[squareCords.x][squareCords.y].getDirectionToPacman();
		Vector2f newPosition = ghostPointer[0].getPosition();
		int ghostSpeed = ghostPointer[0].getSpeed();

		// todo ADD RUNNIG AWAY IN SPECIAL CASES


		// CHASE (GHOST CLOSE TO PACMAN)	(ghost notice and follow)
		if (map[squareCords.x][squareCords.y].getDistanceFromPacman() < 8) {
			ghostPointer[0].setDirection(Direction::NONE);

			switch (squareDirection)
			{
			case RIGHT:
				newPosition.x += ghostSpeed * dtAsSeconds;
				break;

			case UP:
				newPosition.y -= ghostSpeed * dtAsSeconds;
				break;

			case LEFT:
				newPosition.x -= ghostSpeed * dtAsSeconds;
				break;

			case DOWN:
				newPosition.y += ghostSpeed * dtAsSeconds;
				break;

			default:
				break;
			}
		}


		// PATROL
		else {
			// always have some direstion, never stops (except for direction == NONE)
			if (ghostPointer[0].getDirection() == Direction::NONE) {
				ghostPointer[0].setDirection(map[squareCords.x][squareCords.y].getDirectionToPacman());
			}

			switch (ghostPointer[0].getDirection()) {
			case RIGHT:
				newPosition.x += ghostSpeed * dtAsSeconds;
				break;
			case UP:
				newPosition.y -= ghostSpeed * dtAsSeconds;
				break;
			case LEFT:
				newPosition.x -= ghostSpeed * dtAsSeconds;
				break;
			case DOWN:
				newPosition.y += ghostSpeed * dtAsSeconds;
				break;
			default:
				break;
			}

			Vector2i newSquareCords = mapPositionToSquare(newPosition);

			if (newSquareCords != squareCords && map[newSquareCords.x][newSquareCords.y].isDecitionPointHere()) {

				// optimalize here
				bool endRandomSeek = false;		// ENDS WHILE CYCLE
				while (endRandomSeek == false)		// remove counter
				{
					int random0_3 = rand() % 4;
					Direction whichWayToGo = UP;
					switch (random0_3) {
					case 0:
						whichWayToGo = DOWN;
						break;

					case 1:
						whichWayToGo = RIGHT;
						break;

					case 2:
						whichWayToGo = UP;
						break;

					case 3:
						whichWayToGo = LEFT;
						break;
					}
					
					
					int ghostID = ghostPointer[0].getID();
					
					bool goHome = rand() % 2;


					switch (ghostID)
					{		// 0 = DOWN, 1 = RIGHT, 2 = UP, 3 = LEFT
					case 1:	// LEFT UP CORNER
						if (newSquareCords.x > 15 && whichWayToGo == DOWN && goHome) {
							whichWayToGo = UP;
						}
						if (newSquareCords.y > 16 && whichWayToGo == RIGHT && goHome) {
							whichWayToGo = LEFT;
						}
						break;
					case 2:	// RIGHT UP CORNER
						if (newSquareCords.x > 15 && whichWayToGo == DOWN && goHome) {
							whichWayToGo = UP;
						}
						if (newSquareCords.y < 11 && whichWayToGo == LEFT && goHome) {
							whichWayToGo = RIGHT;
						}
						break;
					case 3: // LEFT DOWN CORNER
						if (newSquareCords.x < 12 && whichWayToGo == UP && goHome) {
							whichWayToGo = DOWN;
						}
						if (newSquareCords.y > 16 && whichWayToGo == RIGHT && goHome) {
							whichWayToGo = LEFT;
						}
						break;
					case 4:	// RIGHT DOWN CORNER
						if (newSquareCords.x < 12 && whichWayToGo == UP && goHome) {
							whichWayToGo = DOWN;
						}
						if (newSquareCords.y < 11 && whichWayToGo == LEFT && goHome) {
							whichWayToGo = RIGHT;
						}
						break;
					default:
						break;
					}
					

					switch (whichWayToGo)
					{
					case DOWN:
						if (map[newSquareCords.x + 1][newSquareCords.y].isWallHere() == false) {
							ghostPointer[0].setDirection(Direction::DOWN);
							endRandomSeek = true;
						}
						break;

					case RIGHT:
						if (map[newSquareCords.x][newSquareCords.y + 1].isWallHere() == false) {
							ghostPointer[0].setDirection(Direction::RIGHT);
							endRandomSeek = true;
						}
						break;

					case UP:
						if (map[newSquareCords.x - 1][newSquareCords.y].isWallHere() == false) {
							ghostPointer[0].setDirection(Direction::UP);
							endRandomSeek = true;
						}
						break;

					case LEFT:
						if (map[newSquareCords.x][newSquareCords.y - 1].isWallHere() == false) {
							ghostPointer[0].setDirection(Direction::LEFT);
							endRandomSeek = true;
						}
						break;
					}
				}
			}
		}

		if (!map[mapPositionToSquare(newPosition).x][mapPositionToSquare(newPosition).y].isWallHere()) {
			ghostPointer[0].setPosition(newPosition);
		}
		else {
			// TODO JUST FOR START BOX
			ghostPointer[0].setDirection(findNewWay(ghostPointer));
		}
		ghostPointer = ghostPointer[0].getNextGhost();
	}
}



Direction Engine::findNewWay(Ghost* ghostPointer) {
	Vector2i squareCords = mapPositionToSquare(ghostPointer[0].getPosition());
	map[squareCords.x][squareCords.y];


	while (true)
		{
			int AUX = ghostPointer[0].getPosition().x;
			int whichWayToGo = rand() % 4;
			switch (whichWayToGo)
			{
			case 0:
				if (map[squareCords.x + 1][squareCords.y].isWallHere() == false) {
					return (Direction::DOWN);
				}
				break;

			case 1:
				if (map[squareCords.x][squareCords.y + 1].isWallHere() == false) {
					return (Direction::RIGHT);
				}
				break;

			case 2:
				if (map[squareCords.x - 1][squareCords.y].isWallHere() == false) {
					return (Direction::UP);
				}
				break;

			case 3:
				if (map[squareCords.x][squareCords.y - 1].isWallHere() == false) {
					return (Direction::LEFT);
				}
				break;
			}
		}
}



void Engine::updateMap() {		// update path
	Vector2i targetSquare = mapPositionToSquare(m_pacman.getPosition());

	for (int row = 0; row < ROW; row++) {		// víc vlaken?
		for (int col = 0; col < COL; col++) {
			map[row][col].setDistanceFromPacman(255);
		}
	}

	map[targetSquare.x][targetSquare.y].setDistanceFromPacman(0);

	Heap heap;
	while (targetSquare != Vector2i(-1, -1)) {
		
		int row = targetSquare.x;
		int col = targetSquare.y;

		// map surrounding and adjust path value

		// UP
		if ((row > 0) && map[row - 1][col].isWallHere() == false && map[row - 1][col].getDistanceFromPacman() > (map[row][col].getDistanceFromPacman() + 1)) {
			map[row - 1][col].setDistanceFromPacman(map[row][col].getDistanceFromPacman() + 1);		// makes distance shorter
			if (m_reverseChase == false) {
				map[row - 1][col].setDirectionToPacman(Direction::DOWN);
			}
			else {
				map[row - 1][col].setDirectionToPacman(Direction::UP);
			}

			heap.add(Vector2i(row - 1, col));
		}

		// RIGHT
		if ((col < COL - 1) && map[row][col + 1].isWallHere() == false && map[row][col + 1].getDistanceFromPacman() > (map[row][col].getDistanceFromPacman() + 1)) {
			map[row][col + 1].setDistanceFromPacman(map[row][col].getDistanceFromPacman() + 1);		// makes distance shorter
			if (m_reverseChase == false) {
				map[row][col + 1].setDirectionToPacman(Direction::LEFT);
			}
			else {
				map[row][col + 1].setDirectionToPacman(Direction::RIGHT);
			}

			heap.add(Vector2i(row, col + 1));
		}

		// DOWN
		if ((row < ROW - 1) && map[row + 1][col].isWallHere() == false && map[row + 1][col].getDistanceFromPacman() > (map[row][col].getDistanceFromPacman() + 1)) {
			map[row + 1][col].setDistanceFromPacman(map[row][col].getDistanceFromPacman() + 1);		// makes distance shorter
			if (m_reverseChase == false) {
				map[row + 1][col].setDirectionToPacman(Direction::UP);
			}
			else {
				map[row + 1][col].setDirectionToPacman(Direction::DOWN);
			}

			heap.add(Vector2i(row + 1, col));
		}

		// LEFT
		if ((col > 0) && map[row][col - 1].isWallHere() == false && map[row][col - 1].getDistanceFromPacman() > (map[row][col].getDistanceFromPacman() + 1)) {
			map[row][col - 1].setDistanceFromPacman(map[row][col].getDistanceFromPacman() + 1);		// makes distance shorter
			if (m_reverseChase == false) {
				map[row][col - 1].setDirectionToPacman(Direction::RIGHT);
			}
			else {
				map[row][col - 1].setDirectionToPacman(Direction::LEFT);
			}

			heap.add(Vector2i(row, col - 1));
		}

		targetSquare = heap.get();
	}
}


// PACMAN
Vector2f Engine::getPacmanPosition() {
	return m_pacman.getPosition();
}

float Engine::getPacmanRotation() {

	return m_pacman.getRotation();
}


Ghost* Engine::getGhosts() {
	return m_ghostsPointer;
}

Effect Engine::getEffect() {
	Vector2i pacmanCords = mapPositionToSquare(m_pacman.getPosition());
	return map[pacmanCords.x][pacmanCords.y].getEffect();
}

void Engine::deleteEffect() {
	Vector2i pacmanCords = mapPositionToSquare(m_pacman.getPosition());
	map[pacmanCords.x][pacmanCords.y].setSpecialEffectHere(Effect::SOBER);

	m_reverseChase = true;
	m_chaseTimeAsSeconds = 8;
}

void Engine::startSolveGame() {
	m_solveGame = true;
}

void Engine::stopSolveGame() {
	m_solveGame = false;
}


bool Engine::collisionDetection() {
	Ghost* ghost = m_ghostsPointer;

	while (ghost != NULL) {
		Vector2f pacmanPosition = m_pacman.getPosition();
		Vector2f ghostPosition = ghost[0].getPosition();
		float x = pacmanPosition.x - ghostPosition.x;
		float y = pacmanPosition.y - ghostPosition.y;
		float distancePacmanGhost = sqrt(x*x + y*y);

		if (distancePacmanGhost < 30 && m_reverseChase == false) {
			return true;
		}
		ghost = ghost[0].getNextGhost();
	}

	return false;
}

float Engine::getChaseTime() {
	return m_chaseTimeAsSeconds;
}














/* --------------------------------------------------- **MAP**   ---------------------------------------------------------------- */


Engine::Engine(int irrelevant) {
	// SPAWN GHOSTS
	spawnGhost(Vector2i(12, 13), 120, 1);
	spawnGhost(Vector2i(13, 13), 120, 2);
	spawnGhost(Vector2i(12, 12), 130, 3);
	spawnGhost(Vector2i(12, 12), 130, 4);

	ghostSpawnPosition = m_ghostsPointer[0].getPosition();


	engineRestart();
}


void Engine::engineRestart() {

	// SPAWN PACMAN
	m_pacman.setPosition(Vector2f(13 * 30, 23 * 30));

	Ghost* ghostPointer = m_ghostsPointer;
	while (ghostPointer != NULL)
	{
		ghostPointer[0].setPosition(ghostSpawnPosition);
		ghostPointer = ghostPointer[0].getNextGhost();
	}

	m_foodCounter = 0;



	/* -------- MAP -------- */

	// BORDERS
	for (int row = 0; row < ROW; row++) {
		map[row][0].placeWall();
		map[row][COL - 1].placeWall();
	}

	for (int col = 0; col < COL; col++) {
		map[0][col].placeWall();
		map[ROW - 1][col].placeWall();
	}

	// INNER WALLS
	// 1
	map[1][13].placeWall();	map[1][14].placeWall();
	map[1][14].placeWall();

	// 2
	map[2][2].placeWall();
	map[2][3].placeWall();
	map[2][4].placeWall();
	map[2][5].placeWall();

	map[2][7].placeWall();
	map[2][8].placeWall();
	map[2][9].placeWall();
	map[2][10].placeWall();
	map[2][11].placeWall();

	map[2][13].placeWall();
	map[2][14].placeWall();

	map[2][16].placeWall();
	map[2][17].placeWall();
	map[2][18].placeWall();
	map[2][19].placeWall();
	map[2][20].placeWall();

	map[2][22].placeWall();
	map[2][23].placeWall();
	map[2][24].placeWall();
	map[2][25].placeWall();

	// 3
	map[3][2].placeWall();
	map[3][3].placeWall();
	map[3][4].placeWall();
	map[3][5].placeWall();

	map[3][7].placeWall();
	map[3][8].placeWall();
	map[3][9].placeWall();
	map[3][10].placeWall();
	map[3][11].placeWall();

	map[3][13].placeWall();
	map[3][14].placeWall();

	map[3][16].placeWall();
	map[3][17].placeWall();
	map[3][18].placeWall();
	map[3][19].placeWall();
	map[3][20].placeWall();

	map[3][22].placeWall();
	map[3][23].placeWall();
	map[3][24].placeWall();
	map[3][25].placeWall();

	// 4
	map[4][2].placeWall();
	map[4][3].placeWall();
	map[4][4].placeWall();
	map[4][5].placeWall();

	map[4][7].placeWall();
	map[4][8].placeWall();
	map[4][9].placeWall();
	map[4][10].placeWall();
	map[4][11].placeWall();

	map[4][13].placeWall();
	map[4][14].placeWall();

	map[4][16].placeWall();
	map[4][17].placeWall();
	map[4][18].placeWall();
	map[4][19].placeWall();
	map[4][20].placeWall();

	map[4][22].placeWall();
	map[4][23].placeWall();
	map[4][24].placeWall();
	map[4][25].placeWall();

	// 5

	// 6
	map[6][2].placeWall();
	map[6][3].placeWall();
	map[6][4].placeWall();
	map[6][5].placeWall();

	map[6][7].placeWall();
	map[6][8].placeWall();

	map[6][10].placeWall();
	map[6][11].placeWall();
	map[6][12].placeWall();
	map[6][13].placeWall();
	map[6][14].placeWall();
	map[6][15].placeWall();
	map[6][16].placeWall();
	map[6][17].placeWall();

	map[6][19].placeWall();
	map[6][20].placeWall();

	map[6][22].placeWall();
	map[6][23].placeWall();
	map[6][24].placeWall();
	map[6][25].placeWall();

	// 7
	map[7][2].placeWall();
	map[7][3].placeWall();
	map[7][4].placeWall();
	map[7][5].placeWall();

	map[7][7].placeWall();
	map[7][8].placeWall();

	map[7][10].placeWall();
	map[7][11].placeWall();
	map[7][12].placeWall();
	map[7][13].placeWall();
	map[7][14].placeWall();
	map[7][15].placeWall();
	map[7][16].placeWall();
	map[7][17].placeWall();

	map[7][19].placeWall();
	map[7][20].placeWall();

	map[7][22].placeWall();
	map[7][23].placeWall();
	map[7][24].placeWall();
	map[7][25].placeWall();

	// 8
	map[8][7].placeWall();
	map[8][8].placeWall();

	map[8][13].placeWall();
	map[8][14].placeWall();

	map[8][19].placeWall();
	map[8][20].placeWall();


	// 9
	map[9][1].placeWall();
	map[9][2].placeWall();
	map[9][3].placeWall();
	map[9][4].placeWall();
	map[9][5].placeWall();

	map[9][7].placeWall();
	map[9][8].placeWall();
	map[9][9].placeWall();
	map[9][10].placeWall();
	map[9][11].placeWall();

	map[9][13].placeWall();
	map[9][14].placeWall();

	map[9][16].placeWall();
	map[9][17].placeWall();
	map[9][18].placeWall();
	map[9][19].placeWall();
	map[9][20].placeWall();

	map[9][22].placeWall();
	map[9][23].placeWall();
	map[9][24].placeWall();
	map[9][25].placeWall();
	map[9][26].placeWall();

	//10
	map[10][1].placeWall();
	map[10][2].placeWall();
	map[10][3].placeWall();
	map[10][4].placeWall();
	map[10][5].placeWall();

	map[10][7].placeWall();
	map[10][8].placeWall();
	map[10][9].placeWall();
	map[10][10].placeWall();
	map[10][11].placeWall();

	map[10][13].placeWall();
	map[10][14].placeWall();

	map[10][16].placeWall();
	map[10][17].placeWall();
	map[10][18].placeWall();
	map[10][19].placeWall();
	map[10][20].placeWall();

	map[10][22].placeWall();
	map[10][23].placeWall();
	map[10][24].placeWall();
	map[10][25].placeWall();
	map[10][26].placeWall();

	// 11
	map[11][1].placeWall();
	map[11][2].placeWall();
	map[11][3].placeWall();
	map[11][4].placeWall();
	map[11][5].placeWall();

	map[11][7].placeWall();
	map[11][8].placeWall();

	map[11][19].placeWall();
	map[11][20].placeWall();

	map[11][22].placeWall();
	map[11][23].placeWall();
	map[11][24].placeWall();
	map[11][25].placeWall();
	map[11][26].placeWall();

	// 12
	map[12][1].placeWall();
	map[12][2].placeWall();
	map[12][3].placeWall();
	map[12][4].placeWall();
	map[12][5].placeWall();

	map[12][7].placeWall();
	map[12][8].placeWall();

	map[12][10].placeWall();
	map[12][11].placeWall();
	map[12][12].placeWall();
	map[12][13].placeWall();
	map[12][14].placeWall();
	map[12][15].placeWall();
	map[12][16].placeWall();
	map[12][17].placeWall();

	map[12][19].placeWall();
	map[12][20].placeWall();

	map[12][22].placeWall();
	map[12][23].placeWall();
	map[12][24].placeWall();
	map[12][25].placeWall();
	map[12][26].placeWall();

	// 13
	map[13][1].placeWall();
	map[13][2].placeWall();
	map[13][3].placeWall();
	map[13][4].placeWall();
	map[13][5].placeWall();

	map[13][7].placeWall();
	map[13][8].placeWall();

	map[13][10].placeWall();
	map[13][11].placeWall();
	map[13][12].placeWall();
	map[13][13].placeWall();
	map[13][14].placeWall();
	map[13][15].placeWall();
	map[13][16].placeWall();
	map[13][17].placeWall();

	map[13][19].placeWall();
	map[13][20].placeWall();

	map[13][22].placeWall();
	map[13][23].placeWall();
	map[13][24].placeWall();
	map[13][25].placeWall();
	map[13][26].placeWall();

	// 14
	map[14][10].placeWall();
	map[14][11].placeWall();
	map[14][12].placeWall();
	map[14][13].placeWall();
	map[14][14].placeWall();
	map[14][15].placeWall();
	map[14][16].placeWall();
	map[14][17].placeWall();

	// 15
	map[15][1].placeWall();
	map[15][2].placeWall();
	map[15][3].placeWall();
	map[15][4].placeWall();
	map[15][5].placeWall();

	map[15][7].placeWall();
	map[15][8].placeWall();

	map[15][10].placeWall();
	map[15][11].placeWall();
	map[15][12].placeWall();
	map[15][13].placeWall();
	map[15][14].placeWall();
	map[15][15].placeWall();
	map[15][16].placeWall();
	map[15][17].placeWall();

	map[15][19].placeWall();
	map[15][20].placeWall();

	map[15][22].placeWall();
	map[15][23].placeWall();
	map[15][24].placeWall();
	map[15][25].placeWall();
	map[15][26].placeWall();

	// 16
	map[16][1].placeWall();
	map[16][2].placeWall();
	map[16][3].placeWall();
	map[16][4].placeWall();
	map[16][5].placeWall();

	map[16][7].placeWall();
	map[16][8].placeWall();

	map[16][10].placeWall();
	map[16][11].placeWall();
	map[16][12].placeWall();
	map[16][13].placeWall();
	map[16][14].placeWall();
	map[16][15].placeWall();
	map[16][16].placeWall();
	map[16][17].placeWall();

	map[16][19].placeWall();
	map[16][20].placeWall();

	map[16][22].placeWall();
	map[16][23].placeWall();
	map[16][24].placeWall();
	map[16][25].placeWall();
	map[16][26].placeWall();


	// 17
	map[17][1].placeWall();
	map[17][2].placeWall();
	map[17][3].placeWall();
	map[17][4].placeWall();
	map[17][5].placeWall();

	map[17][7].placeWall();
	map[17][8].placeWall();

	map[17][19].placeWall();
	map[17][20].placeWall();

	map[17][22].placeWall();
	map[17][23].placeWall();
	map[17][24].placeWall();
	map[17][25].placeWall();
	map[17][26].placeWall();

	// 18
	map[18][1].placeWall();
	map[18][2].placeWall();
	map[18][3].placeWall();
	map[18][4].placeWall();
	map[18][5].placeWall();

	map[18][7].placeWall();
	map[18][8].placeWall();

	map[18][10].placeWall();
	map[18][11].placeWall();
	map[18][12].placeWall();
	map[18][13].placeWall();
	map[18][14].placeWall();
	map[18][15].placeWall();
	map[18][16].placeWall();
	map[18][17].placeWall();

	map[18][19].placeWall();
	map[18][20].placeWall();

	map[18][22].placeWall();
	map[18][23].placeWall();
	map[18][24].placeWall();
	map[18][25].placeWall();
	map[18][26].placeWall();

	// 19
	map[19][1].placeWall();
	map[19][2].placeWall();
	map[19][3].placeWall();
	map[19][4].placeWall();
	map[19][5].placeWall();

	map[19][7].placeWall();
	map[19][8].placeWall();

	map[19][10].placeWall();
	map[19][11].placeWall();
	map[19][12].placeWall();
	map[19][13].placeWall();
	map[19][14].placeWall();
	map[19][15].placeWall();
	map[19][16].placeWall();
	map[19][17].placeWall();

	map[19][19].placeWall();
	map[19][20].placeWall();

	map[19][22].placeWall();
	map[19][23].placeWall();
	map[19][24].placeWall();
	map[19][25].placeWall();
	map[19][26].placeWall();

	// 20
	map[20][13].placeWall();
	map[20][14].placeWall();

	// 21
	map[21][2].placeWall();
	map[21][3].placeWall();
	map[21][4].placeWall();
	map[21][5].placeWall();

	map[21][7].placeWall();
	map[21][8].placeWall();
	map[21][9].placeWall();
	map[21][10].placeWall();
	map[21][11].placeWall();

	map[21][13].placeWall();
	map[21][14].placeWall();

	map[21][16].placeWall();
	map[21][17].placeWall();
	map[21][18].placeWall();
	map[21][19].placeWall();
	map[21][20].placeWall();

	map[21][22].placeWall();
	map[21][23].placeWall();
	map[21][24].placeWall();
	map[21][25].placeWall();

	// 22
	map[22][2].placeWall();
	map[22][3].placeWall();
	map[22][4].placeWall();
	map[22][5].placeWall();

	map[22][7].placeWall();
	map[22][8].placeWall();
	map[22][9].placeWall();
	map[22][10].placeWall();
	map[22][11].placeWall();

	map[22][13].placeWall();
	map[22][14].placeWall();

	map[22][16].placeWall();
	map[22][17].placeWall();
	map[22][18].placeWall();
	map[22][19].placeWall();
	map[22][20].placeWall();

	map[22][22].placeWall();
	map[22][23].placeWall();
	map[22][24].placeWall();
	map[22][25].placeWall();

	// 23
	map[23][4].placeWall();
	map[23][5].placeWall();

	map[23][22].placeWall();
	map[23][23].placeWall();

	//24
	map[24][1].placeWall();
	map[24][2].placeWall();

	map[24][4].placeWall();
	map[24][5].placeWall();

	map[24][7].placeWall();
	map[24][8].placeWall();

	map[24][10].placeWall();
	map[24][11].placeWall();
	map[24][12].placeWall();
	map[24][13].placeWall();
	map[24][14].placeWall();
	map[24][15].placeWall();
	map[24][16].placeWall();
	map[24][17].placeWall();

	map[24][19].placeWall();
	map[24][20].placeWall();

	map[24][22].placeWall();
	map[24][23].placeWall();

	map[24][25].placeWall();
	map[24][26].placeWall();

	// 25
	map[25][1].placeWall();
	map[25][2].placeWall();

	map[25][4].placeWall();
	map[25][5].placeWall();

	map[25][7].placeWall();
	map[25][8].placeWall();

	map[25][10].placeWall();
	map[25][11].placeWall();
	map[25][12].placeWall();
	map[25][13].placeWall();
	map[25][14].placeWall();
	map[25][15].placeWall();
	map[25][16].placeWall();
	map[25][17].placeWall();

	map[25][19].placeWall();
	map[25][20].placeWall();

	map[25][22].placeWall();
	map[25][23].placeWall();

	map[25][25].placeWall();
	map[25][26].placeWall();

	// 26
	map[25][7].placeWall();
	map[25][8].placeWall();

	map[25][13].placeWall();
	map[25][14].placeWall();


	map[25][19].placeWall();
	map[25][20].placeWall();

	// 27
	map[27][2].placeWall();
	map[27][3].placeWall();
	map[27][4].placeWall();
	map[27][5].placeWall();
	map[27][6].placeWall();
	map[27][7].placeWall();
	map[27][8].placeWall();
	map[27][9].placeWall();
	map[27][10].placeWall();
	map[27][11].placeWall();

	map[27][13].placeWall();
	map[27][14].placeWall();

	map[27][16].placeWall();
	map[27][17].placeWall();
	map[27][18].placeWall();
	map[27][19].placeWall();
	map[27][20].placeWall();
	map[27][21].placeWall();
	map[27][22].placeWall();
	map[27][23].placeWall();
	map[27][24].placeWall();
	map[27][25].placeWall();

	// 28
	map[28][2].placeWall();
	map[28][3].placeWall();
	map[28][4].placeWall();
	map[28][5].placeWall();
	map[28][6].placeWall();
	map[28][7].placeWall();
	map[28][8].placeWall();
	map[28][9].placeWall();
	map[28][10].placeWall();
	map[28][11].placeWall();

	map[28][13].placeWall();
	map[28][14].placeWall();

	map[28][16].placeWall();
	map[28][17].placeWall();
	map[28][18].placeWall();
	map[28][19].placeWall();
	map[28][20].placeWall();
	map[28][21].placeWall();
	map[28][22].placeWall();
	map[28][23].placeWall();
	map[28][24].placeWall();
	map[28][25].placeWall();


	// WALL SHEET SELECTIONS
	for (int row = 0; row < ROW; row++) {
		for (int col = 0; col < COL; col++) {
			if (map[row][col].isWallHere()) {

				if (col > 0 && map[row][col - 1].isWallHere() && col < COL - 1 && map[row][col + 1].isWallHere()) {
					map[row][col].setSheetSelection(0);
				}

				if (row > 0 && map[row - 1][col].isWallHere() && row < ROW - 1 && map[row + 1][col].isWallHere()) {
					map[row][col].setSheetSelection(30);
				}

				if (row < ROW - 1 && row > 0 && col < COL - 1 && col > 0) {

					if (map[row + 1][col].isWallHere() && map[row][col - 1].isWallHere()
						&& map[row - 1][col].isWallHere() == false && map[row][col + 1].isWallHere() == false) {
						map[row][col].setSheetSelection(60);
					}

					if (map[row + 1][col].isWallHere() && map[row][col + 1].isWallHere()
						&& map[row - 1][col].isWallHere() == false && map[row][col - 1].isWallHere() == false) {
						map[row][col].setSheetSelection(90);
					}

					if (map[row - 1][col].isWallHere() && map[row][col + 1].isWallHere()
						&& map[row + 1][col].isWallHere() == false && map[row][col - 1].isWallHere() == false) {
						map[row][col].setSheetSelection(120);
					}

					if (map[row - 1][col].isWallHere() && map[row][col - 1].isWallHere()
						&& map[row + 1][col].isWallHere() == false && map[row][col + 1].isWallHere() == false) {
						map[row][col].setSheetSelection(150);
					}

					if (map[row - 1][col - 1].isWallHere() && map[row + 1][col - 1].isWallHere()
						&& map[row - 1][col + 1].isWallHere() && map[row + 1][col + 1].isWallHere()) {
						map[row][col].setSheetSelection(180);
					}
				}
			}
		}
	}


	// FOOD
	for (int row = 0; row < ROW; row++) {
		for (int col = 0; col < COL; col++) {
			if (map[row][col].isWallHere() == false) {
				map[row][col].placeFood();
				m_foodCounter++;
				map[row][col].setFoodSheetLeft((rand() % 3) * 30);
			}
		}
	}

	// EFFECTS
	map[3][1].setSpecialEffectHere(Effect::WEED);
	map[22][1].setSpecialEffectHere(Effect::COCAIN);
	map[3][26].setSpecialEffectHere(Effect::LSD);
	map[22][26].setSpecialEffectHere(Effect::ALCOHOL);


	// SPAWN PLACE FOR GHOSTS
	map[12][13].removeWall();
	map[12][14].removeWall();
	map[13][13].removeWall();
	map[13][14].removeWall();
	map[14][13].removeWall();
	map[14][14].removeWall();

	map[12][13].setDirectionToPacman(Direction::LEFT);
	map[12][14].setDirectionToPacman(Direction::RIGHT);
	map[13][13].setDirectionToPacman(Direction::DOWN);
	map[13][14].setDirectionToPacman(Direction::LEFT);
	map[14][13].setDirectionToPacman(Direction::RIGHT);
	map[14][14].setDirectionToPacman(Direction::DOWN);

	// DECITION POINTS OF CHANGING DIRECTION FOR GHOSTS (køižovatky)
	map[1][1].setDecitionPointHere();
	map[1][6].setDecitionPointHere();
	map[1][12].setDecitionPointHere();
	map[1][15].setDecitionPointHere();
	map[1][21].setDecitionPointHere();
	map[1][26].setDecitionPointHere();

	map[5][1].setDecitionPointHere();
	map[5][6].setDecitionPointHere();
	map[5][9].setDecitionPointHere();
	map[5][12].setDecitionPointHere();
	map[5][15].setDecitionPointHere();
	map[5][18].setDecitionPointHere();
	map[5][21].setDecitionPointHere();
	map[5][26].setDecitionPointHere();

	map[8][1].setDecitionPointHere();
	map[8][6].setDecitionPointHere();
	map[8][9].setDecitionPointHere();
	map[8][12].setDecitionPointHere();
	map[8][15].setDecitionPointHere();
	map[8][18].setDecitionPointHere();
	map[8][21].setDecitionPointHere();
	map[8][26].setDecitionPointHere();

	map[11][9].setDecitionPointHere();
	map[11][12].setDecitionPointHere();
	map[11][15].setDecitionPointHere();
	map[11][18].setDecitionPointHere();

	map[14][3].setDecitionPointHere();
	map[14][6].setDecitionPointHere();
	map[14][9].setDecitionPointHere();
	map[14][18].setDecitionPointHere();
	map[14][21].setDecitionPointHere();
	map[14][25].setDecitionPointHere();

	map[17][9].setDecitionPointHere();
	map[17][18].setDecitionPointHere();

	map[20][1].setDecitionPointHere();
	map[20][6].setDecitionPointHere();
	map[20][9].setDecitionPointHere();
	map[20][12].setDecitionPointHere();
	map[20][15].setDecitionPointHere();
	map[20][18].setDecitionPointHere();
	map[20][21].setDecitionPointHere();
	map[20][26].setDecitionPointHere();

	map[23][1].setDecitionPointHere();
	map[23][3].setDecitionPointHere();
	map[23][6].setDecitionPointHere();
	map[23][9].setDecitionPointHere();
	map[23][12].setDecitionPointHere();
	map[23][15].setDecitionPointHere();
	map[23][18].setDecitionPointHere();
	map[23][21].setDecitionPointHere();
	map[23][24].setDecitionPointHere();
	map[23][26].setDecitionPointHere();

	map[26][1].setDecitionPointHere();
	map[26][3].setDecitionPointHere();
	map[26][6].setDecitionPointHere();
	map[26][9].setDecitionPointHere();
	map[26][12].setDecitionPointHere();
	map[26][15].setDecitionPointHere();
	map[26][18].setDecitionPointHere();
	map[26][21].setDecitionPointHere();
	map[26][24].setDecitionPointHere();
	map[26][26].setDecitionPointHere();

	map[29][1].setDecitionPointHere();
	map[29][12].setDecitionPointHere();
	map[29][15].setDecitionPointHere();
	map[29][26].setDecitionPointHere();


}