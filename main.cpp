#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include"Highscore.h"
#include"Garden.h"
class Menu;
int startgame();
class GameOverMenu {
private:
public:
	sf::RenderWindow& window;
	sf::Font font;
	sf::Text titleText;
	sf::Text menuTexts[2]; // Adjust as needed
	int selectedItemIndex;
	int menuCount;

	GameOverMenu(sf::RenderWindow& win) : window(win), selectedItemIndex(0), menuCount(2) {
		if (!font.loadFromFile("font2.ttf")) {
			std::cerr << "Failed to load font!" << std::endl;
			// Handle error
		}

		titleText.setFont(font);
		titleText.setString("Game Over");
		titleText.setCharacterSize(100);
		titleText.setFillColor(sf::Color::White);
		titleText.setPosition(window.getSize().x / 2 - titleText.getGlobalBounds().width / 2, 60);

		std::string menuStrings[2] = { "Restart", "Exit" }; // Adjust as needed

		for (int i = 0; i < menuCount; ++i) {
			menuTexts[i].setFont(font);
			menuTexts[i].setString(menuStrings[i]);
			menuTexts[i].setCharacterSize(45);
			menuTexts[i].setFillColor(sf::Color::White);
			menuTexts[i].setPosition(window.getSize().x / 2 - menuTexts[i].getGlobalBounds().width / 2,
				window.getSize().y / 2 + i * 50);
		}
		menuTexts[0].setFillColor(sf::Color::Green);
	}

	void draw() {
		window.draw(titleText);
		for (int i = 0; i < menuCount; ++i) {
			window.draw(menuTexts[i]);
		}
	}

	void MoveUp() {
		if (selectedItemIndex - 1 >= 0) {
			menuTexts[selectedItemIndex].setFillColor(sf::Color::White);
			selectedItemIndex--;
			menuTexts[selectedItemIndex].setFillColor(sf::Color::Green);
		}
	}

	void MoveDown() {
		if (selectedItemIndex + 1 < menuCount) {
			menuTexts[selectedItemIndex].setFillColor(sf::Color::White);
			selectedItemIndex++;
			menuTexts[selectedItemIndex].setFillColor(sf::Color::Green);
		}
	}

	int getSelectedItemIndex() { return selectedItemIndex; }
};

int gameOver(RenderWindow& org) {
	sf::RenderWindow window(sf::VideoMode(1800, 980), "Plants vs Zombies - Game Over");
	GameOverMenu gameOverMenu(window);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Up) {
					gameOverMenu.MoveUp();
				}
				else if (event.key.code == sf::Keyboard::Down) {
					gameOverMenu.MoveDown();
				}
				else if (event.key.code == sf::Keyboard::Return) {
					switch (gameOverMenu.getSelectedItemIndex()) {
					case 0:

						org.close();
						window.close();
						startgame();
						break;
					case 1:
						window.close();
						org.close();
						exit(0);
						break;
					}
				}
				break;
			}
		}

		window.clear();
		gameOverMenu.draw();
		window.display();
	}

	return 0;
}

class Game {
	RenderWindow window;
	Maze maze;
	Font font;
	ScoreList highscore;
	Power* star;
	bool inserted = false;
	int score = 0;
	int* activerewards;
	int numImages = 6;
	sf::Texture* textures = new sf::Texture[numImages];
	sf::Sprite* sprites = new sf::Sprite[numImages];
	int lives = 1;
	int val = 0;
	string name = "Hashir";
	Plantlist plants;
	Zombielist zombies;
	int wealth = 0;
	float wait = 14.0;
	int sunflowercontrol = 10;
	int level = 1;
public:
	Game() : window(VideoMode(1800, 980), "Plants vs Zombie") {
		font.loadFromFile("font2.ttf");
		highscore.readFromFile();
		activerewards = new int[numImages];
		for (int i = 0; i < numImages; i++)
		{
			activerewards[i] = 1;
		}
		name = "";
		getNameInput(name, window);
	}


	void drawPauseMenu(RenderWindow& org) {
		sf::RenderWindow window(sf::VideoMode(800, 680), "Plants vs Zombies");
		bool returnPressed = false;
		bool restartPressed = false;
		bool resumePressed = false;
		ScoreList highscore;
		highscore.readFromFile();

		while (!returnPressed && window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape) {
						returnPressed = true;
					}
					else if (event.key.code == sf::Keyboard::R) {
						restartPressed = true;
						returnPressed = true; 
					}
					else if (event.key.code == sf::Keyboard::Space) { 
						resumePressed = true;
						returnPressed = true;
					}
					break;
				}
			}

			window.clear();

			sf::Font font;
			font.loadFromFile("font2.ttf");
			sf::Text text;
			text.setFont(font);
			text.setCharacterSize(30);
			text.setFillColor(sf::Color::White);

			float x = 1450.f;
			int y_pos = 300;
			int count = 0;

			sf::Text returnText("Press Escape to Return ", font, 34);
			returnText.setFillColor(sf::Color::White);
			returnText.setPosition(1050.f, 800.f);
			window.draw(returnText);

			sf::Text restartText("Press R to Restart Game", font, 34);
			restartText.setFillColor(sf::Color::White);
			restartText.setPosition(100.f, 400.f);
			window.draw(restartText);

			sf::Text resumeText("Press Space to Resume Game", font, 34);
			resumeText.setFillColor(sf::Color::White);
			resumeText.setPosition(100.f, 500.f);
			window.draw(resumeText);

			window.display();


			if (restartPressed) {
				startgame();
				org.close();
				return;
			}


			if (resumePressed) {
				return;
			}
		}
	}



	void run() {
		Clock clock1, clock2,clock3,clock4,clock5,clock6;
		bool display_star = false;
		int waittime = 1;
		int starting = rand() % 5;
			zombies.insert(level, 0, maze.coords[12][starting][0], maze.coords[12][starting][1]);

		
		while (window.isOpen()) {
			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed)
					window.close();
				else if (event.key.code == sf::Keyboard::P) {
					//drawPauseMenu(window);
					gameOver(window);
				}
				else if (event.type == sf::Event::MouseButtonPressed) {

					for (int i = 0; i < numImages; ++i) {
						if (sprites[i].getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {

							val = i + 1;
							sprites[i].setScale(1.0f, 1.15f);
						}
						else {
							sprites[i].setScale(1.0f, 1.0f);
						}
					}
					if (display_star) {
						if (star->sprite.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
							cout << "Gained Power" << endl;
							star = nullptr;
							display_star = false;
							clock1.restart();
							waittime = rand() % sunflowercontrol + 1;
							wealth += 80;
							cout << wealth << endl;
							val = 0;
						}
					}

					if (val != 0) {
						Vector2f clickPosition(event.mouseButton.x, event.mouseButton.y);
						Vector2f closestCoordinate = maze.getClosestCoordinate(clickPosition);
						cout << "Closest Coordinate: (" << closestCoordinate.x << ", " << closestCoordinate.y << ")" << endl;
						int blockX = static_cast<int>((closestCoordinate.x - 211) / maze.verticalLineGap);
						int blockY = static_cast<int>((closestCoordinate.y - 201) / maze.horizontalLineGap);
						cout << blockX << "  " << blockY << endl;
						if (event.mouseButton.x > 270) {

							if (closestCoordinate.x != -1 || closestCoordinate.y != -1) {
								int adjustedPosX = static_cast<int>((closestCoordinate.x - 211) / maze.verticalLineGap) * maze.verticalLineGap + 211 + maze.verticalLineGap / 2;
								int adjustedPosY = static_cast<int>((closestCoordinate.y - 201) / maze.horizontalLineGap) * maze.horizontalLineGap + 201 + maze.horizontalLineGap / 2;
								bool adequate = false;
								if (val == 1 && wealth >= 100) {
									adequate = true;

								}
								else if (val == 2 && wealth >= 100) {
									adequate = true;

								}
								else if (val == 3 && wealth >= 50) {
									adequate = true;

								}
								else if (val == 4 && wealth >= 150) {
									adequate = true;

								}
								else if (val == 5 && wealth >= 150) {
									adequate = true;

								}
								else if (val == 6 && wealth >= 150) {
									adequate = true;

								}

								if (adequate) {

									if (plants.insert(val, 0, adjustedPosX, adjustedPosY - 40)) {
										if (val == 1) {
											wealth -= 100;
										}
										else if (val == 2) {
											wealth -= 100;
											sunflowercontrol--;
											if (sunflowercontrol < 2) {
												sunflowercontrol = 2;
											}
										}
										else if (val == 3) {
											wealth -= 50;
										}
										else if (val == 4) {
											wealth -= 150;
										}
										else if (val == 5) {
											wealth -= 150;
										}
										maze.valid[blockX][blockY] = 0;
										cout << "After placing" << val << endl;
									}
								}
								val = 0;

							}
						}
					}


					cout << val << endl;
				}

			}
			if (clock1.getElapsedTime().asSeconds() >= waittime && !display_star) {
				display_star = true;
				int xpos = rand() % 1200;
				xpos += 200;
				int ypos = rand() % 500;
				ypos += 300;
				star = new Power(xpos, ypos);
				clock2.restart();
			}
			if (clock3.getElapsedTime().asSeconds() >= wait) {
				for (int i = 0; i < level; i++)
				{
				int val = rand() % 5;
				zombies.insert(i, level, maze.coords[12][val][0], maze.coords[12][val][1]);
				clock3.restart();

				}
			}
			if (clock6.getElapsedTime().asSeconds() >= 0.15f) {
				zombies.movement(window);
				clock6.restart();
			}
			window.clear();

			maze.drawmaze(window);
			displayTopThreeScores();
			displayrewards();
			plants.display(window);
			if (display_star && clock2.getElapsedTime().asSeconds() >= 0.008f) {

				star->move();
				clock2.restart();
			}
			if (display_star) {
				star->display(window);
				if (star->posy <= 50) {
					star = nullptr;
					display_star = false;
					clock1.restart();
					waittime = rand() % 4 + 1;
				}
			}
			if (clock4.getElapsedTime().asSeconds() >= 1.0f) {
			plants.initiatefire(zombies,window);
			clock4.restart();
			}
			if (clock5.getElapsedTime().asSeconds() >= 5.0f) {
				zombies.decrementlife(lives);
				clock5.restart();
			}
			if (lives < 0) {
				highscore.insert(score, name);
				gameOver(window);
			}
			plants.detectcollision(zombies, window,score);
			zombies.display(window);
			printHealth();
			window.display();
			levelManager();
		}
	}
	void displayrewards() {
		std::string imagePaths[] = {
	"peashooter.png",
	"sunflower.png",
	"repeater.png",
	"walknut.png",
	"snowpea.png",
	"cherrybomb.png"
		};
		const float spacing = 200.0f;
		float startX = 450.0f;
		float startY = 10.0f;

		for (int i = 0; i < numImages; ++i) {
			textures[i].loadFromFile(imagePaths[i]);
		}

		for (int i = 0; i < numImages; ++i) {
			sprites[i].setTexture(textures[i]);
			sprites[i].setPosition(startX + i * 140, startY);
		}


		for (int i = 0; i < level; i++)
		{
			if (activerewards[i] != 0) {
				window.draw(sprites[i]);
			}
		}

	}

	void displayTopThreeScores() {

		Text heading;
		heading.setFont(font);
		heading.setCharacterSize(40);
		heading.setFillColor(Color::Red);
		heading.setPosition(1450, 20);
		string scoreText = "Scoreboard";
		heading.setString(scoreText);
		window.draw(heading);
		Text text;
		text.setFont(font);
		text.setCharacterSize(24);
		text.setFillColor(Color::White);


		float x = 1450.f;
		float y = 80.0f;
		int prev_score = score;
		score_body* temp = highscore.head;
		int count = 0;
		bool done_display = false;
		while (temp != nullptr && count < 3) {
			if (temp->score < score && !done_display) {
				done_display = true;
				scoreText = to_string(count + 1) + ".  " + "You" + "(" + to_string(score) + ")";
				text.setFillColor(Color::Blue);
			}
			else {
				scoreText = to_string(count + 1) + ".  " + temp->name + "(" + to_string(temp->score) + ")";
				text.setFillColor(Color::White);
				temp = temp->next;
				prev_score = temp->score;
			}
			text.setString(scoreText);

			text.setPosition(x, y);

			window.draw(text);

			y += 30.f;

			count++;
		}
	}


	void printHealth() {

		int closestEnemyHealth = INT_MAX;
		float minDistance = FLT_MAX;
		Text playerHealthText, enemyHealthText, Myscore, mylives, myname,levels;
		playerHealthText.setFont(font);
		enemyHealthText.setFont(font);
		levels.setFont(font);
		Myscore.setFont(font);
		mylives.setFont(font);
		myname.setFont(font);
		playerHealthText.setCharacterSize(30);
		enemyHealthText.setCharacterSize(30);
		Myscore.setCharacterSize(30);
		mylives.setCharacterSize(30);
		myname.setCharacterSize(30);
		levels.setCharacterSize(60);
		playerHealthText.setFillColor(Color::Yellow);
		playerHealthText.setFillColor(Color::Yellow);
		enemyHealthText.setFillColor(Color::Yellow);
		Myscore.setFillColor(Color::Yellow);
		mylives.setFillColor(Color::Yellow);
		myname.setFillColor(Color::Yellow);
		playerHealthText.setPosition(650, 150);
		Myscore.setPosition(900, 150);
		mylives.setPosition(1040, 150);
		myname.setPosition(450, 150);
		levels.setPosition(50, 50);
		Texture lifeT;
		lifeT.loadFromFile("lives.png");
		Sprite life(lifeT);



		//for (Zombie* zombie = zombies.start; zombie != nullptr; zombie = zombie->next) {
		//	if (enemy->visible) {
		//		float distance = sqrt(pow(player->posX - enemy->posX, 2) + pow(player->posY - enemy->posY, 2));
		//		if (distance < minDistance) {
		//			minDistance = distance;
		//			closestEnemyHealth = enemy->health;
		//		}
		//	}
		//}

		levels.setString("Level: " + to_string(level));
		playerHealthText.setString("Sun Wealth: " + to_string(wealth));

		Myscore.setString("Score: " + to_string(score));
		mylives.setString("Lives: ");
		myname.setString("Name: " + name);

		for (int i = 0; i < lives; i++)
		{
			life.setPosition(1110 + i * 35, 155);
			window.draw(life);
		}
		window.draw(myname);
		window.draw(Myscore);
		window.draw(levels);
		window.draw(mylives);
		window.draw(playerHealthText);



	}

	void levelManager() {

		if (score >= level * 600) {
			level++;
			wait -= 0.5;
		}
		if (level > 6) {
			level = 6;

			gameOver(window);
			return;
		}
	}
	void getNameInput(std::string& playerName, sf::RenderWindow& window) {
		sf::Font font;
		if (!font.loadFromFile("font2.ttf")) {
			std::cerr << "Failed to load font!" << std::endl;
		}

		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(30);
		text.setFillColor(sf::Color::White);
		text.setString("Enter your name: ");
		text.setPosition(100.f, 100.f);

		sf::Text inputText;
		inputText.setFont(font);
		inputText.setCharacterSize(30);
		inputText.setFillColor(sf::Color::White);
		inputText.setPosition(text.getPosition().x + text.getGlobalBounds().width, 100.f);

		std::string inputString;

		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
				}
				else if (event.type == sf::Event::TextEntered) {
					if (event.text.unicode == 8) {
						if (!inputString.empty()) {
							inputString.pop_back();
						}
					}
					else if (event.text.unicode < 128) {
						inputString += static_cast<char>(event.text.unicode);
					}
				}
				else if (event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::Enter) {
						playerName = inputString;
						return;
					}
				}
			}

			inputText.setString(inputString);

			window.clear();
			window.draw(text);
			window.draw(inputText);
			window.display();
		}
	}

};





class Menu {
private:
public:
	sf::RenderWindow& window;
	sf::Font font;
	sf::Text titleText;
	sf::Text menuTexts[4];
	int selectedItemIndex;
	int menuCount;
	sf::Text instructionsText;
	sf::Text returnText;
	bool showInstructions;

	Menu(sf::RenderWindow& win) : window(win), selectedItemIndex(0), menuCount(4), showInstructions(false) {
		if (!font.loadFromFile("font2.ttf")) {
			std::cerr << "Failed to load font!" << std::endl;
			// Handle error
		}

		titleText.setFont(font);
		titleText.setString("Plants vs Zombies");
		titleText.setCharacterSize(100);
		titleText.setFillColor(sf::Color::Black);
		titleText.setPosition(window.getSize().x / 2 - titleText.getGlobalBounds().width / 2, 60);

		std::string menuStrings[4] = { "Play", "Instructions", "Highscore", "Exit" };

		for (int i = 0; i < menuCount; ++i) {
			menuTexts[i].setFont(font);
			menuTexts[i].setString(menuStrings[i]);
			menuTexts[i].setCharacterSize(85);
			menuTexts[i].setFillColor(sf::Color::Black);
			menuTexts[i].setPosition(window.getSize().x / 2 - menuTexts[i].getGlobalBounds().width / 2,
				window.getSize().y / 2 + i * 50);
		}
		menuTexts[0].setFillColor(sf::Color::Blue);

		instructionsText.setFont(font);
		instructionsText.setString("Instructions:\n\nWelcome to Plants vs Zombies! Your goal is to defend your home from the incoming waves of zombies using an arsenal of plants with unique abilities.\n\nGameplay:\n- You start each level with a limited number of sun points, which you can use to plant various types of plants.\n- Sun points are generated over time by sun-producing plants or collected from falling suns during gameplay.\n- Zombies will approach from the right side of the screen in multiple lanes. Your plants will need to attack them to prevent them from reaching your house.\n- Plant your defensive plants strategically to counter different types of zombies.\n- As you progress, you'll encounter tougher zombies and more challenging levels.\n- Use your resources wisely, as some plants require more sun points than others.\n\nPlant Types:\n1. Peashooter: Basic attacking plant that shoots peas at zombies in its lane.\n2. Sunflower: Generates sun points over time, essential for planting other plants.\n3. Wall-nut: Provides a barrier to block zombies and protect your other plants.\n4. Cherry Bomb: Explodes in a large area, dealing massive damage to nearby zombies.\n5. Snow Pea: Slows down zombies with frozen peas, making them easier to defeat.\n6. Potato Mine: Burrows underground and explodes when a zombie steps on it.\n\nControls:\n- Use the mouse to select and place plants on the lawn.\n- Click on suns that fall from the sky to collect additional sun points.\n- During gameplay, click on plants to upgrade or remove them from the lawn.\n\nWinning:\n- Successfully defend your house from all waves of zombies to win the level.\n- Earn stars and other rewards based on your performance.\n\nGood luck, and have fun defending your home from the zombie invasion in Plants vs Zombies!");
		instructionsText.setCharacterSize(24);
		instructionsText.setFillColor(sf::Color::White);
		instructionsText.setPosition(50, 100); // Adjust position as needed

		// Initialize return text
		returnText.setFont(font);
		returnText.setString("Back");
		returnText.setCharacterSize(30);
		returnText.setFillColor(sf::Color::White);
		returnText.setPosition(window.getSize().x / 2 - returnText.getGlobalBounds().width / 2,
			window.getSize().y - 100); // Adjust position as needed
	}

	void draw() {

		if (!showInstructions) {
			for (int i = 0; i < menuCount; ++i) {
				window.draw(menuTexts[i]);
			}
		}
		else {
			window.draw(instructionsText);
			window.draw(returnText);
		}
	}

	void MoveUp() {
		if (selectedItemIndex - 1 >= 0) {
			menuTexts[selectedItemIndex].setFillColor(sf::Color::Black);
			selectedItemIndex--;
			menuTexts[selectedItemIndex].setFillColor(sf::Color::Blue);
		}
	}

	void MoveDown() {
		if (selectedItemIndex + 1 < menuCount) {
			menuTexts[selectedItemIndex].setFillColor(sf::Color::Black);
			selectedItemIndex++;
			menuTexts[selectedItemIndex].setFillColor(sf::Color::Blue);
		}
	}

	int getSelectedItemIndex() { return selectedItemIndex; }

	void ShowInstructions(bool show) {
		showInstructions = show;
	}

	bool IsInstructionsShown() {
		return showInstructions;
	}

	bool HandleReturnButton(sf::Vector2f mousePosition) {
		if (returnText.getGlobalBounds().contains(mousePosition)) {
			showInstructions = false;
			return true;
		}
		return false;
	}
};



void displayTopTenScores() {
	sf::RenderWindow window(sf::VideoMode(1800, 980), "Plants vs Zombies");
	bool returnPressed = false; 
	ScoreList highscore;
	highscore.readFromFile();

	while (!returnPressed && window.isOpen()) { 
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape) { 
					returnPressed = true;
				}
				break;
			}
		}

		window.clear();

		sf::Font font;
		font.loadFromFile("font2.ttf");
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(30);
		text.setFillColor(sf::Color::White);

		float x = 1450.f;
		int y_pos = 300;
		int count = 0;
		score_body* temp = highscore.head;
		while (temp != nullptr && count < 10) {
			std::string scoreText = "";
			scoreText += std::to_string(count + 1) + ".  " + temp->name + " (" + std::to_string(temp->score) + ")\n";
			text.setString(scoreText);
			text.setPosition(x, y_pos);
			window.draw(text);
			y_pos += 40;
			temp = temp->next;
			count++;
		}

		sf::Text returnText("Press Escape to Return ", font, 34);
		returnText.setFillColor(sf::Color::White);
		returnText.setPosition(1050.f, 800.f);
		window.draw(returnText);

		window.display();
	}
}



int startgame() {
	sf::RenderWindow window(sf::VideoMode(1800, 980), "Plants vs Zombies");
	Menu menu(window);
	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("background.jpg");
	sf::Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.scale(1.4f, 1.0f);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (!menu.IsInstructionsShown()) {
					if (event.key.code == sf::Keyboard::Up) {
						menu.MoveUp();
					}
					else if (event.key.code == sf::Keyboard::Down) {
						menu.MoveDown();
					}
					else if (event.key.code == sf::Keyboard::Return) {
						Game g;
						switch (menu.getSelectedItemIndex()) {
						case 0:
							window.close();
							g.run();
							break;
						case 1:
							menu.titleText.setPosition(window.getSize().x / 2 - menu.titleText.getGlobalBounds().width / 2 + window.getSize().x / 5, 660);
							menu.ShowInstructions(true);
							menu.titleText.setPosition(window.getSize().x / 2 - menu.titleText.getGlobalBounds().width / 2, 60);
							break;
						case 2:

							displayTopTenScores(); 
							
							break;
						case 3:
							window.close();
							break;
						}
					}
				}
				else {
					if (event.key.code == sf::Keyboard::Return) {
						menu.ShowInstructions(false);
					}
				}
				break;
			case sf::Event::MouseButtonPressed:
				if (menu.IsInstructionsShown()) {

					if (menu.HandleReturnButton(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
						menu.ShowInstructions(false);
					}
				}
				break;
			}
		}

		window.clear();
		window.draw(backgroundSprite);
		menu.draw();
		window.display();
	}

	return 0;
}
int main() {
	startgame();
}
