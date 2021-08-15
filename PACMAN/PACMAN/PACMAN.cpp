#include "stdafx.h"
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// OWN LIBRARIES
#include "PACMAN.h"
#include "TextureHolder.h"
#include "PacmanEngine.h"


using namespace sf;

int main()
{
	/* ------------------------------------   DEKLARACE   -------------------------------------*/

	// WINDOW
	Vector2f monitorResolution;
	monitorResolution.x = VideoMode::getDesktopMode().width;
	monitorResolution.y = VideoMode::getDesktopMode().height;
	Vector2f viewVector;
	viewVector.x = 31 * 30 / monitorResolution.y * monitorResolution.x;
	viewVector.y = 31 * 30;


	VideoMode vm(monitorResolution.x, monitorResolution.y);									// Create a video mode object
	RenderWindow window(vm, "PAC_MAN", Style::Fullscreen);		// Create and open a window for the game
	window.setFramerateLimit(120);
	

	View baseView;
	baseView.setSize(viewVector);
	baseView.setCenter(viewVector.x / 2, viewVector.y / 2);
	window.setView(baseView);

	// SOUNDS

	SoundBuffer pacmanSoundBuffer;
	pacmanSoundBuffer.loadFromFile("sound/pacmanSound.wav");

	SoundBuffer pacmanSoundBufferAlcohol;
	pacmanSoundBufferAlcohol.loadFromFile("sound/pacmanSoundAlcohol.wav");

	SoundBuffer pacmanSoundBufferCocain;
	pacmanSoundBufferCocain.loadFromFile("sound/pacmanSoundCocain.wav");

	SoundBuffer pacmanSoundBufferWeed;
	pacmanSoundBufferWeed.loadFromFile("sound/pacmanSoundWeed.wav");

	SoundBuffer pacmanSoundBufferLSD;
	pacmanSoundBufferLSD.loadFromFile("sound/pacmanSoundLSD.wav");


	Sound pacmanSound;
	pacmanSound.setBuffer(pacmanSoundBuffer);
	float soundTimer = 0;



	// MIXTURE OF MINOR DECLARATION
	TextureHolder holder;
	Clock clock;

	// DRUG EFFECT SWAP
	Effect currentEffect = SOBER;
	Effect prevEffect = SOBER;
	bool changeEffect = false;
	bool swapEffect = false;
	int swapCounter = 0;
	const int MAX_SWAPS = 20;
	float swapTimes[2*MAX_SWAPS];
	swapTimes[0] = 0.2;
	swapTimes[1] = 0.2;
	float swapRoundTimeAsSeconds = 0;
	float devSwapSpeed = 40;
	for (int a = MAX_SWAPS / 2; a > 2; a--) {
		float floatA = a;
		float even = floatA / devSwapSpeed;
		float odd = (MAX_SWAPS / 2 - floatA) / devSwapSpeed;
		
		swapTimes[2 * a] = even;
		swapTimes[2 * a + 1] = odd;
	}

	float effectTimeAsSeconds = 0;
	float totalGameTimeAsSeconds = 0;
	bool startSnowWallRise = false;
	float snowWallRiseTime = 0;

	bool pause = true;
	bool showLoadingScreen = true;
	bool showLostScreen = false;
	bool showWinScreen = false;
	float pauseTimer = 2.0;

	// CHASE METER
	RectangleShape timeBar;
	float timeBarWidth = 80;
	float timeBarHeight = 100;
	timeBar.setSize(Vector2f(timeBarWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition(330, 385);

	// ENGINE
	Engine engine(5);

	// TEXT
	Font gameFont;
	gameFont.loadFromFile("fonts/KOMIKAP_.ttf");

	Text textFoodCounter;
	textFoodCounter.setFont(gameFont);
	textFoodCounter.setColor(Color(255, 255, 255));
	textFoodCounter.setCharacterSize(90);
	textFoodCounter.setPosition(1050, 180);

	Text textDistance;
	textDistance.setFont(gameFont);
	textDistance.setColor(Color(255, 255, 255));
	textDistance.setCharacterSize(10);

	Text textLoadingScreen;
	textLoadingScreen.setString("Press SPACE \n    to START");
	textLoadingScreen.setPosition(1000, 100);
	textLoadingScreen.setFont(gameFont);
	textLoadingScreen.setColor(Color(255, 211, 38));
	textLoadingScreen.setCharacterSize(60);


	// EFFECT TEXTURES
	// BACKGROUND
	Texture textureBackgroundWeed = TextureHolder::GetTexture("graphics/backgroundWeed.png");
	Texture textureBackgroundCocain = TextureHolder::GetTexture("graphics/backgroundCocain.png");
	Texture textureBackgroundLSD = TextureHolder::GetTexture("graphics/backgroundLSD.png");
	Texture textureBackgroundAlcohol = TextureHolder::GetTexture("graphics/backgroundAlcohol.png");

	// wall sheet
	
	Texture textureWallSheetLSD = TextureHolder::GetTexture("graphics/wallSheetLSD.png");
	Texture textureWallSheetWeed = TextureHolder::GetTexture("graphics/wallSheetWeed.png");
	Texture textureWallSheetCocain = TextureHolder::GetTexture("graphics/wallSheetCocain.png");
	Texture textureWallSheetAlcohol = TextureHolder::GetTexture("graphics/wallSheetAlcohol.png");


	// food
	
	Texture textureFoodLDS = TextureHolder::GetTexture("graphics/foodSheetLSD.png");
	Texture textureFoodWeed = TextureHolder::GetTexture("graphics/foodSheetWeed.png");
	Texture textureFoodCocain = TextureHolder::GetTexture("graphics/foodSheetCocain.png");
	Texture textureFoodAlcohol = TextureHolder::GetTexture("graphics/foodSheetAlcohol.png");

	// pacman

	Texture texturePacmanSheetLSD = TextureHolder::GetTexture("graphics/pacmanSheetLSD.png");
	Texture texturePacmanSheetWeed = TextureHolder::GetTexture("graphics/pacmanSheetWeed.png");
	Texture texturePacmanSheetCocain = TextureHolder::GetTexture("graphics/pacmanSheetCocain.png");
	Texture texturePacmanSheetAlcohol = TextureHolder::GetTexture("graphics/pacmanSheetAlcohol.png");

	// ghost

	Texture textureGhostLSD = TextureHolder::GetTexture("graphics/ghostSheetLSD.png");
	Texture textureGhostWeed = TextureHolder::GetTexture("graphics/ghostSheetWeed.png");
	Texture textureGhostCocain = TextureHolder::GetTexture("graphics/ghostSheetCocain.png");
	Texture textureGhostAlcohol = TextureHolder::GetTexture("graphics/ghostSheetAlcohol.png");


	// SPRITES WITH BASE TEXTURES
		// TMP
		Sprite spriteTMP;
		Texture textureTMP = TextureHolder::GetTexture("graphics/TMP.png");
		spriteTMP.setTexture(textureTMP);
		spriteTMP.setPosition(Vector2f(700, 700));

		// HUD
		Sprite spriteHUD;
		Texture textureHUD = TextureHolder::GetTexture("graphics/HUD.png");
		spriteHUD.setTexture(textureHUD);
		spriteHUD.setPosition(840, 0);

		// LOADING SCREEN
		Sprite spriteLoadingScreen;
		Texture textureLoadingScreen = TextureHolder::GetTexture("graphics/loadingScreen.png");
		spriteLoadingScreen.setTexture(textureLoadingScreen);

		// LOST SCREEN
		Sprite spriteLost;
		Texture textureLost = TextureHolder::GetTexture("graphics/lostScreen.png");
		spriteLost.setTexture(textureLost);

		// WIN SCREEN
		Sprite spriteWin;
		Texture textureWin = TextureHolder::GetTexture("graphics/winScreen.png");
		spriteWin.setTexture(textureWin);


		// BACKGROUND
		Sprite spriteBackground;
		Texture textureBackground = TextureHolder::GetTexture("graphics/background.png");
		spriteBackground.setTexture(textureBackgroundCocain);
		spriteBackground.setTexture(textureBackground);
		spriteBackground.setOrigin(1920 / 2, 1080 / 2);
		spriteBackground.setPosition(1920 / 2, 1080 / 2);

		// WALL
		Sprite spriteWall;
		Texture textureWallSheet = TextureHolder::GetTexture("graphics/wallSheet.png");
		spriteWall.setTexture(textureWallSheet);

		IntRect intRectWall;
		intRectWall.height = 30;
		intRectWall.width = 30;
		intRectWall.top = 0;

		// FOOD
		Sprite spriteFood;
		Texture textureFood = TextureHolder::GetTexture("graphics/foodSheet.png");
		spriteFood.setTexture(textureFood);

		IntRect intRectFoodSheet;
		intRectFoodSheet.width = 30;
		intRectFoodSheet.height = 30;
		intRectFoodSheet.top = 0;

		// PACMAN
		Sprite spritePacman;
		Texture texturePacmanSheet = TextureHolder::GetTexture("graphics/pacmanSheet.png");
		spritePacman.setTexture(texturePacmanSheet);
		spritePacman.setPosition(engine.getPacmanPosition());
		spritePacman.setOrigin(Vector2f(15, 15));
		float pacmanRotation = 180;

		// GHOSTS
		Sprite spriteGhost;
		Texture textureGhost = TextureHolder::GetTexture("graphics/ghostSheet.png");
		spriteGhost.setTexture(textureGhost);
		spriteGhost.setOrigin(15, 15);

		IntRect intRectGhostSheet;
		intRectGhostSheet.height = 30;
		intRectGhostSheet.width = 30;
		intRectGhostSheet.top = 0;

		// EFFECTS
		Sprite spriteEffect;
		Texture textureEffect = TextureHolder::GetTexture("graphics/effectSheet.png");
		spriteEffect.setTexture(textureEffect);

		IntRect intRectEffectSheet;
		intRectEffectSheet.width = 30;
		intRectEffectSheet.height = 30;
		intRectEffectSheet.top = 0;

		Sprite spriteSnowWall;
		Texture textureBackgroundCocainSwonWall = TextureHolder::GetTexture("graphics/backgroundCocainSwonWall.png");
		spriteSnowWall.setTexture(textureBackgroundCocainSwonWall);
		

		// DEV SPRITES
		Sprite spritePathOrientation;
		Texture texturePathOrientation = TextureHolder::GetTexture("graphics/pathSheet.png");
		spritePathOrientation.setTexture(texturePathOrientation);

		IntRect intRectPathOrientation;
		intRectPathOrientation.height = 30;
		intRectPathOrientation.width = 30;
		intRectPathOrientation.top = 0;
		spritePathOrientation.setTextureRect(intRectPathOrientation);

		Sprite spriteGhostDirection;
		spriteGhostDirection.setTexture(texturePathOrientation);

		IntRect intRectGhostDirection;
		intRectGhostDirection.height = 30;
		intRectGhostDirection.width = 30;
		intRectGhostDirection.top = 0;
		spriteGhostDirection.setTextureRect(intRectGhostDirection);






		
	/* ---------------------------------	END OF DECLARATION	-----------------------------*/


	/* ---------------------------------	GAME LOOP	------- -------------------------------*/
	while (window.isOpen()) {
		
		/* ------ TEXTURE + SOUND CHANGE / DRUG EFFECT SWAP ------ */
		if (swapEffect && swapRoundTimeAsSeconds > swapTimes[swapCounter]) {
			swapRoundTimeAsSeconds = 0;

			Effect switchEffect;
			if (swapCounter % 2 == 1) {		// new
				switchEffect = currentEffect;
			}
			else {							// old
				switchEffect = prevEffect;
			}

			switch (switchEffect)
			{
			case WEED:
				spriteBackground.setTexture(textureBackgroundWeed);
				spriteWall.setTexture(textureWallSheetWeed);
				spriteFood.setTexture(textureFoodWeed);
				spriteGhost.setTexture(textureGhostWeed);
				spritePacman.setTexture(texturePacmanSheetWeed);

				pacmanSound.setBuffer(pacmanSoundBufferWeed);
				break;

			case COCAIN:
				startSnowWallRise = true;

				spriteBackground.setTexture(textureBackgroundCocain);
				spriteFood.setTexture(textureFoodCocain);
				spriteWall.setTexture(textureWallSheetCocain);
				spritePacman.setTexture(texturePacmanSheetCocain);
				spriteGhost.setTexture(textureGhostCocain);

				pacmanSound.setBuffer(pacmanSoundBufferCocain);
				break;

			case LSD:
				spriteBackground.setTexture(textureBackgroundLSD);
				spriteWall.setTexture(textureWallSheetLSD);
				spriteFood.setTexture(textureFoodLDS);
				spritePacman.setTexture(texturePacmanSheetLSD);
				spriteGhost.setTexture(textureGhostLSD);

				pacmanSound.setBuffer(pacmanSoundBufferLSD);
				break;

			case ALCOHOL:
				spriteBackground.setTexture(textureBackgroundAlcohol);
				spriteWall.setTexture(textureWallSheetAlcohol);
				spriteFood.setTexture(textureFoodAlcohol);
				spritePacman.setTexture(texturePacmanSheetAlcohol);
				spriteGhost.setTexture(textureGhostAlcohol);

				pacmanSound.setBuffer(pacmanSoundBufferAlcohol);
				break;


			case SOBER:
				spriteBackground.setTexture(textureBackground);
				spriteWall.setTexture(textureWallSheet);
				spriteFood.setTexture(textureFood);
				spritePacman.setTexture(texturePacmanSheet);
				spriteGhost.setTexture(textureGhost);

				pacmanSound.setBuffer(pacmanSoundBuffer);
				break;

			default:
				break;
			}

			swapCounter++;
			if (swapCounter == MAX_SWAPS) {
				swapCounter = 0;
				swapEffect = false;
			}
		}



		if (changeEffect) {
			changeEffect = false;
			swapEffect = true;
			effectTimeAsSeconds = 30;
		}


		/* ------ INPUT -------*/
		if (!pause) {


			if (Keyboard::isKeyPressed(Keyboard::W))	// UP
			{
				engine.movePacmanUp();
			}
			if (Keyboard::isKeyPressed(Keyboard::D))	// RIGHT
			{
				engine.movePacmanRight();
			}
			if (Keyboard::isKeyPressed(Keyboard::S))	// DOWN
			{
				engine.movePacmanDown();
			}
			if (Keyboard::isKeyPressed(Keyboard::A))	// LEFT
			{
				engine.movePacmanLeft();
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))	// ESC
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::R))	// RESET
		{
			engine.engineRestart();
			showLostScreen = false;
			showWinScreen = false;
		}

		if (Keyboard::isKeyPressed(Keyboard::Space))	// SPACE / PAUSE
		{
			if (pause && pauseTimer < 0) {
				pause = false;
				showLoadingScreen = false;
				engine.startSolveGame();
				pauseTimer = 2;
			}
			else if (pauseTimer < 0) {
				pause = true;
				engine.stopSolveGame();
				pauseTimer = 2;
			}
		}



		/* -------- FRAME SOLVING --------- */


		Time dt = clock.restart();										// Update the delta time
		float dtAsSeconds = dt.asSeconds();								// Make a decimal fraction of 1 from the delta time
		pauseTimer -= dtAsSeconds;
		totalGameTimeAsSeconds += dtAsSeconds;
		swapRoundTimeAsSeconds += dtAsSeconds;
		if (startSnowWallRise) {
			snowWallRiseTime += dtAsSeconds;
		}
		soundTimer -= dtAsSeconds;


		if (soundTimer < 0 && pause == false) {
			soundTimer = 1;

			pacmanSound.play();
		}


		engine.update(dtAsSeconds);

		Vector2f pacmanPosition = engine.getPacmanPosition();
		pacmanPosition.x += 15;
		pacmanPosition.y += 15;
		spritePacman.setPosition(pacmanPosition);


		// EFFECTS
		Effect newEffect = currentEffect;
		newEffect = engine.getEffect();

		if (effectTimeAsSeconds == 0 && currentEffect != Effect::SOBER) {
			changeEffect = true;
			currentEffect = Effect::SOBER;

		}

		if (currentEffect != newEffect) {
			if (newEffect != SOBER) {
				changeEffect = true;
				prevEffect = currentEffect;
				currentEffect = newEffect;
				engine.deleteEffect();
			}
		}

		effectTimeAsSeconds -= dtAsSeconds;
		if (effectTimeAsSeconds < 0) {
			effectTimeAsSeconds = 0;
		}

		// COLISION DETECTION
		if (engine.collisionDetection()) {
			pause = true;
			showLostScreen = true;
			engine.stopSolveGame();
		}
		
		// FOOD DETECTION
		if (engine.getFoodCounter() == 0) {
			pause = true;
			showWinScreen = true;
			engine.stopSolveGame();
		}
		

		/* ------------ WINDOW DRAWNING ------------- */
		window.clear();

		spriteBackground.setRotation(0);
		spriteBackground.setPosition(1920 / 2, 1080 / 2);

		if (currentEffect == WEED) {
			window.draw(spriteBackground);
			spriteBackground.setRotation(20 * totalGameTimeAsSeconds);
			spriteBackground.setPosition(15 * 28, 15 * 30);
		}

		if (currentEffect == LSD) {
			//int n = 5;
			int timeMultipleOfN = totalGameTimeAsSeconds / 5;
			float n_0nTime = totalGameTimeAsSeconds - timeMultipleOfN * 5;

			// UP AND DOWN ANIMATION
			Vector2f newPosition(1920 / 2, 1080 / 2);
			if (n_0nTime < 2.5) {
				newPosition.y = 1080 / 2 - n_0nTime * 10;
			}

			if (n_0nTime > 2.5) {
				newPosition.y = 1080 / 2 - 2.5 * 10 + (n_0nTime - 2.5) * 10;
			}
			spriteBackground.setPosition(newPosition);
		}


		if (currentEffect == COCAIN) {
			int n = 5;
			int timeMultipleOfN = totalGameTimeAsSeconds / n;
			float n_0nTime = totalGameTimeAsSeconds - timeMultipleOfN * n;

			spriteBackground.setPosition(1920 / 2, -1080 / 2 + n_0nTime * 1080 / 5);

			spriteSnowWall.setPosition(0, 1080 - 1080 / 30 * snowWallRiseTime);


		}

		window.draw(spriteBackground);


		if (currentEffect == COCAIN) {	// must be displayed after background is displayed
			window.draw(spriteSnowWall);
		}


		// WALLS + FOOD
		for (int row = 0; row < ROW; row++) {
			for (int col = 0; col < COL; col++) {
				if (engine.map[row][col].isWallHere()) {	// WALL
					Vector2f wallPosition;
					wallPosition.y = row * 30;
					wallPosition.x = col * 30;
					spriteWall.setPosition(wallPosition);
					intRectWall.left = engine.map[row][col].getSheetSelection();
					spriteWall.setTextureRect(intRectWall);
					window.draw(spriteWall);
				}
				if (engine.map[row][col].isFoodHere()) {	// FOOD
					Vector2f foodPosition;
					foodPosition.y = row * 30;
					foodPosition.x = col * 30;
					spriteFood.setPosition(foodPosition);
					intRectFoodSheet.left = engine.map[row][col].getFoodSheetLeft();
					spriteFood.setTextureRect(intRectFoodSheet);

					spriteFood.setRotation(0);
					spriteFood.setOrigin(0, 0);
					if (currentEffect == ALCOHOL) {
						spriteFood.setOrigin(15, 15);
						spriteFood.setPosition(spriteFood.getPosition().x + 15, spriteFood.getPosition().y + 15);
						spriteFood.setRotation(rand() % 360);
						
					}

					window.draw(spriteFood);
				}
				if (engine.map[row][col].getEffect() != SOBER) {
					Vector2f effectPosiion;
					effectPosiion.y = row * 30;
					effectPosiion.x = col * 30;
					switch (engine.map[row][col].getEffect())
					{
					case WEED:
						intRectEffectSheet.left = 0;
						break;
					case ALCOHOL:
						intRectEffectSheet.left = 30;
						break;
					case LSD:
						intRectEffectSheet.left = 60;
						break;
					case COCAIN:
						intRectEffectSheet.left = 90;
					default:
						break;
					}
					spriteEffect.setTextureRect(intRectEffectSheet);
					spriteEffect.setPosition(effectPosiion);
					window.draw(spriteEffect);

				}

			}
		}



		bool devTool_ShowPathValue = 0;
		bool devTool_ShowPathOrientation = 0;
		bool devTool_ShowGhostDirection = 0;


		if (devTool_ShowPathValue) {
			for (int row = 0; row < ROW; row++) {
				for (int col = 0; col < COL; col++) {
					if (engine.map[row][col].isWallHere() == false) {
						std::stringstream aa;
						aa << "" << engine.map[row][col].getDistanceFromPacman();

						textDistance.setString(aa.str());

						textDistance.setPosition(Vector2f(col * 30, row * 30));
						window.draw(textDistance);
					}
				}
			}
		}

		if (devTool_ShowPathOrientation) {
			for (int row = 0; row < ROW; row++) {
				for (int col = 0; col < COL; col++) {
					if (engine.map[row][col].isWallHere() == false) {
						switch (engine.map[row][col].getDirectionToPacman())
						{
						case DOWN:
							intRectPathOrientation.left = 0;
							break;
						case LEFT:
							intRectPathOrientation.left = 30;
							break;
						case UP:
							intRectPathOrientation.left = 60;
							break;
						case RIGHT:
							intRectPathOrientation.left = 90;
							break;

						default:
							break;
						}
						spritePathOrientation.setTextureRect(intRectPathOrientation);
						spritePathOrientation.setPosition(Vector2f(col * 30, row * 30));
						window.draw(spritePathOrientation);
					}
				}
			}
		}


		// GHOSTS
		Ghost* ghost = engine.getGhosts();
		while (ghost != NULL)
		{

			Vector2f ghostPosition = ghost[0].getPosition();
			ghostPosition.x += 15;
			ghostPosition.y += 15;

			spriteGhost.setPosition(ghostPosition);
			intRectGhostSheet.left = ghost[0].getSheetOrder() * 30;
			spriteGhost.setTextureRect(intRectGhostSheet);

			spriteGhost.setRotation(0);
			if (currentEffect == LSD) {
				spriteGhost.setRotation(-360 * totalGameTimeAsSeconds);
			}

			window.draw(spriteGhost);

			ghost = ghost[0].getNextGhost();
		}

		if (devTool_ShowGhostDirection) {
			Ghost* ghost = engine.getGhosts();
			while (ghost != NULL)
			{
				spriteGhostDirection.setPosition(ghost[0].getPosition());
				switch (ghost[0].getDirection())
				{
				case UP:
					intRectGhostDirection.left = 60;
					break;

				case RIGHT:
					intRectGhostDirection.left = 90;
					break;

				case DOWN:
					intRectGhostDirection.left = 0;
					break;

				case LEFT:
					intRectGhostDirection.left = 30;
					break;
				case NONE:
					intRectGhostDirection.left = 120;
					break;
				}
				spriteGhostDirection.setTextureRect(intRectGhostDirection);
				window.draw(spriteGhostDirection);

				ghost = ghost[0].getNextGhost();
			}
		}


		// PACMAN
		float newPacmanRotation = engine.getPacmanRotation();

		if (newPacmanRotation != 666) {
			pacmanRotation = newPacmanRotation;
		}
		spritePacman.setTextureRect(engine.getPacmanSheetSelection());
		spritePacman.setRotation(pacmanRotation);
		window.draw(spritePacman);


		// HUD + SCORE
		window.draw(spriteHUD);

		std::stringstream ss;
		ss << "" << 303 - engine.getFoodCounter();

		textFoodCounter.setString(ss.str());
		window.draw(textFoodCounter);

		timeBarWidth = engine.getChaseTime() * 22;
		if (timeBarWidth < 0) {
			timeBarWidth = 0;
		}
		timeBar.setSize(Vector2f(timeBarWidth, timeBarHeight));
		window.draw(timeBar);

		
		// LOADING SCREEN
		if (showLoadingScreen) {
			window.draw(spriteLoadingScreen);
			
			int n = 1;
			int timeMultipleOfN = totalGameTimeAsSeconds / n;
			float n_0nTime = totalGameTimeAsSeconds - timeMultipleOfN * n;
			if (n_0nTime < 0.5) {
				textLoadingScreen.setCharacterSize(60 + 30 * n_0nTime);
			}
			else {
				textLoadingScreen.setCharacterSize(60 + 15 - 30 * (n_0nTime - 0.5));
			}
			

			Vector2f middleOfText;
			middleOfText.x = textLoadingScreen.getGlobalBounds().width / 2;
			middleOfText.y = textLoadingScreen.getGlobalBounds().height / 2;
			textLoadingScreen.setOrigin(middleOfText);
			textLoadingScreen.setPosition(1150, 200);

			window.draw(textLoadingScreen);
		}

		// LOST SCREEN
		if (showLostScreen) {
			window.draw(spriteLost);
		}

		if (showWinScreen) {
			window.draw(spriteWin);
		}


		window.display();
	}
}

// TO DO LIST:

/*

	- UTIKANI GHOSTU

	- EFEKTY NA PACMANA (rychlost pohybu, nahodne odchylky)

	- LEVELY



- VYLEPSIT AI ghostu (chase jenom nejakou dobu)




*/

