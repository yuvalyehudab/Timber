// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main() {

	VideoMode vm(1920, 1080);

	// create and open a window for the game
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	// texture holds graphics on GPU
	Texture textureBackground;

	textureBackground.loadFromFile("graphics/background.png");

	Sprite spriteBackground;

	spriteBackground.setTexture(textureBackground);

	spriteBackground.setPosition(0, 0);

	//create a tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	const int NUM_TREES = 4;
	Sprite spriteBackTrees[NUM_TREES];

	for (int i = 0; i < NUM_TREES; i++) {
		spriteBackTrees[i].setTexture(textureTree);
		spriteBackTrees[i].setScale(0.5f, 1.1f);
	}
	spriteBackTrees[0].setPosition(90, -30);
	spriteBackTrees[1].setPosition(350, -300);
	spriteBackTrees[2].setPosition(1300, -140);
	spriteBackTrees[3].setPosition(1500, -220);

	//prepare the bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	// is bee moving?
	bool beeActive = false;
	
	// how fast bee flies
	float beeSpeed = 0.0f;

	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");

	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);

	// position clouds on the left of the screen
	// at different heights
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);

	// are cloud on screen?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	// clouds speed
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	//control time ha ha!
	Clock clock;

	// Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Track wether the game is running
	bool paused = true;

	// text section
	int score = 0;

	Text messageText;
	Text scoreText;

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	messageText.setFont(font);
	scoreText.setFont(font);

	messageText.setString("Press ENTER to start!");
	scoreText.setString("score = 0");

	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
	textRect.height / 2.0f);

	// Position the text!
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	// prepare branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	// player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	side playerSide = side::LEFT;

	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	// Axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false;

	// prepare sounds
	// chopping sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	// player ends under branch
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);

		// set sprties origin to dead center
		// we can then spin it round without changing its position
		branches[i].setOrigin(220, 20);
	}

	// keep the window open
	while (window.isOpen()) {
		/*
		****************************
		* handle the players input
		****************************
		*/

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;

				spriteAxe.setPosition(2000,
					spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// start the game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			score = 0;
			timeRemaining = 6;

			//make branches disappear
			// starting 2nd position

			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			// hide gravestone
			spriteRIP.setPosition(675, 2000);

			// move player into position
			spritePlayer.setPosition(580, 720);

			acceptInput = true;
		}

		if (acceptInput)
		{
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				playerSide = side::RIGHT;
				score++;

				// Update score text
				std::stringstream writer;
				writer << "score = " << score;
				scoreText.setString(writer.str());


				// add time to time bar
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT,
					spriteAxe.getPosition().y);
				
				spritePlayer.setPosition(1200, 720);

				updateBranches(score);

				// set the log flying left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				// play chop sound
				chop.play();
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				playerSide = side::LEFT;
				score++;
				// Update score text
				std::stringstream writer;
				writer << "score = " << score;
				scoreText.setString(writer.str());


				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT,
					spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);

				updateBranches(score);
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;

				// play chop sound
				chop.play();
			}
		}
		/*
		* update the scene
		*/

		if (!paused) {

			// measure time
			Time dt = clock.restart();

			timeRemaining -= dt.asSeconds();

			// size up time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				//pause the gaem
				paused = true;
				messageText.setString("Out of time!!");

				//reposition the text because of changed size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + 
					textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// play death sound
				outOfTime.play();
			}

			if (!beeActive)
			{
				// Random speed for the bee
				srand((int)time(0));
				beeSpeed = (rand() % 200) + 200;

				//bee height
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);

				beeActive = true;
			}
			else
			{
				//move the bee
				spriteBee.setPosition(
					spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);

				// the be has arrived to the end of the screen
				if (spriteBee.getPosition().x < -100)
				{
					//just return to base position!
					beeActive = false;
				}
			}

			// clouds
			if (!cloud1Active) {

				//cloud1 speed
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				// cloud height
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x +
					(dt.asSeconds() * cloud1Speed),
					spriteCloud1.getPosition().y);

				// cloud reached the end
				if (spriteCloud1.getPosition().x > 1920)
				{
					cloud1Active = false;
				}
			}
			if (!cloud2Active)
			{
				srand((int)time(0) * 20);
				cloud2Speed = rand() % 200;

				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x +
					(cloud2Speed * dt.asSeconds()),
					spriteCloud2.getPosition().y);

				// reached end?
				if (spriteCloud2.getPosition().x > 1920)
				{
					cloud2Active = false;
				}
			}

			if (!cloud3Active)
			{
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else
			{
				spriteCloud3.setPosition(
					spriteCloud3.getPosition().x +
					(cloud3Speed * dt.asSeconds()),
					spriteCloud3.getPosition().y);

				if (spriteCloud3.getPosition().x > 1920)
				{
					cloud3Active = false;
				}
			}

			// update branches
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;

				if (branchPositions[i] == side::LEFT)
				{
					// move the sprite to the left side
					branches[i].setPosition(610, height);

					//flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					branches[i].setPosition(1330, height);
					branches[i].setRotation(0);
				}
				else
				{
					branches[i].setPosition(3000, height); //invisible?
				}
			}

			if (logActive)
			{
				spriteLog.setPosition(
					spriteLog.getPosition().x +
					(logSpeedX * dt.asSeconds()),

					spriteLog.getPosition().y +
					(logSpeedY * dt.asSeconds())
				);

				//check that log is not on edge
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().y > 2000)
				{
					//set it up ready to be a whole new log next frame!
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			if (branchPositions[NUM_BRANCHES - 1] == playerSide)
			{
				// Game Over
				paused = true;
				acceptInput = false;

				spriteRIP.setPosition(525, 760);

				// hide player
				spritePlayer.setPosition(2000, 660);

				messageText.setString("Squished!!");

				// center message..
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f,
					1080 / 2.0f);

				// death sound 
				death.play();
			}
		} //end if (!paused)

		/*
		* ******************
		 Draw the scene
		* ******************
		 */

		// clear everything from the last frame
		window.clear();

		// draw game scene
		window.draw(spriteBackground);

		for (int i = 0; i < NUM_TREES; i++) {
			window.draw(spriteBackTrees[i]);
		}

		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}

		window.draw(spriteTree);

		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteLog);
		window.draw(spriteRIP);

		window.draw(spriteBee);

		// draw score
		window.draw(scoreText);

		window.draw(timeBar);

		if (paused)
		{
			window.draw(messageText);
		}

		// Show everything we just drew
		window.display();
	}

	return 0;
}

void updateBranches(int seed)
{
	for (int i = NUM_BRANCHES - 1; i > 0; i--)
	{
		// move branches one place down!
		branchPositions[i] = branchPositions[i - 1];
	}

	// spawn a new branch at position 0!
	srand((int)time(0) + seed);
	int r = (rand() % 5);

	switch (r) {
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}