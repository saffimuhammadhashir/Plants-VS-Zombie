#pragma once
#include <SFML/Graphics.hpp>
#include<iostream>
#include<time.h>

using namespace sf;
using namespace std;


class Maze {
	Texture walls_texture_horizontal;
	Texture house, grassleft, path, fence;
	Sprite home,lawngrassleft,pathway1,pathway2, fences1, fences2, fences3;
	Texture walls_texture_vertical;
	RectangleShape** blocks;
	RectangleShape** foods;
	Color* playerColors;

public:
	int verticalLineGap;
	int horizontalLineGap;
	int n;
	int m;
	Sprite** walls_sprite;
	int** valid;
	int*** coords;

	Maze() {
		srand(time(0));
		n = 6;
		m = 13;
		blocks = new RectangleShape * [m];
		foods = new RectangleShape * [n];
		playerColors = new Color[4];
		playerColors[0] = Color::Red;
		playerColors[1] = Color::Blue;
		playerColors[2] = Color::Green;
		playerColors[3] = Color::Yellow;
		verticalLineGap = 1600 /m;
		horizontalLineGap = 800 / n;
		house.loadFromFile("house3.png");
		grassleft.loadFromFile("grassleft.jpg");
		path.loadFromFile("path.png");
		
		fence.loadFromFile("fence.png");


		fences1.setTexture(fence);
		fences2.setTexture(fence);
		fences3.setTexture(fence);
		
		pathway1.setTexture(path);
		pathway2.setTexture(path);
		home.setTexture(house);
		home.setPosition(-160, 100);
		lawngrassleft.setTexture(grassleft);
		lawngrassleft.setPosition(1630, 880);
		pathway1.setPosition(200, 440);
		pathway2.setPosition(870, 440);

		lawngrassleft.setRotation(180);
		lawngrassleft.setScale(2.25f, 0.92f);
		fences1.setPosition(0, 700);
		fences2.setPosition(550, 700);
		fences3.setPosition(1100, 700);

		for (int i = 0; i < m; ++i) {
			blocks[i] = new RectangleShape[n];
			for (int j = 0; j < n; ++j) {
				RectangleShape block(Vector2f(verticalLineGap - 4, horizontalLineGap - 4));
				block.setPosition(Vector2f(i * verticalLineGap + 211, j * horizontalLineGap + 201));
				block.setFillColor(Color::Transparent);
				block.setOutlineColor(Color::Black);
				block.setOutlineThickness(1);
				blocks[i][j] = block;
			}
		}

		walls_sprite = new Sprite * [n + m];
		for (int i = 0; i < n; ++i) {
			walls_sprite[i] = new Sprite[n];
		}

		valid = new int* [m];
		for (int i = 0; i < m; ++i) {
			valid[i] = new int[n];
			for (int j = 0; j < n; ++j) {
				valid[i][j] = 1;
			}

		}

		coords = new int** [m];
		for (int i = 0; i < m; ++i) {
			coords[i] = new int* [n];
			for (int j = 0; j < n; ++j) {
				coords[i][j] = new int[2];
				coords[i][j][0] = i * verticalLineGap + 160;
				coords[i][j][1] = j * horizontalLineGap + 201;
				cout << coords[i][j][0] << "|" << coords[i][j][1] << " ";
			}
			cout << endl;
		}


	}

	void drawmaze(RenderWindow& window) {
		Texture bg_texture;
		bg_texture.loadFromFile("background.png");
		Sprite bg_sprite(bg_texture);
		bg_sprite.scale(2, 2);
		bg_sprite.setPosition(0, 200);

		Texture inventory_texture;
		inventory_texture.loadFromFile("inventory.png");
		Sprite inventory_sprite(inventory_texture);
		inventory_sprite.scale(5, 0.6);
		inventory_sprite.setPosition(0, 0);

		Texture highscore_texture;
		highscore_texture.loadFromFile("highscore.png");
		Sprite highscore_sprite(highscore_texture);
		highscore_sprite.scale(5, 0.64);
		highscore_sprite.setPosition(1300, 0);

		window.draw(bg_sprite);
		window.draw(inventory_sprite);
		window.draw(highscore_sprite);

		for (int i = 0; i < m; ++i) {
			Vertex line[] = {
				Vertex(Vector2f(i * verticalLineGap + 150, 211)),
				Vertex(Vector2f(i * verticalLineGap + 150, 875)) };
			window.draw(line, 2, Lines);
		}

		for (int i = 0; i <= n; ++i) {
			Vertex line[] = {
				Vertex(Vector2f(150, i * horizontalLineGap + 211)),
				Vertex(Vector2f(1625, i * horizontalLineGap + 211)) };
			window.draw(line, 2, Lines);
		}

		window.draw(lawngrassleft);
		window.draw(pathway1);
		window.draw(pathway2);
		window.draw(fences1); 
		window.draw(fences2);
		window.draw(fences3);
		window.draw(home);

	}

	Sprite& getWallSprite(int i, int j) {
		return walls_sprite[i][j];
	}

	bool isValid(int i, int j) const {
		if (i >= m || j >= n) {
			return false;
		}
		return valid[i][j] != 0;
	}
	Vector2f getClosestCoordinate(Vector2f clickPosition) {
		int closestX = -1;
		int closestY = -1;
		float minDistance = std::numeric_limits<float>::max();

		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j) {
				int blockX = coords[i][j][0];
				int blockY = coords[i][j][1];
				if (clickPosition.x > blockX-10 && clickPosition.y > blockY+5 ) {

				float distance = sqrt(pow(blockX - clickPosition.x, 2) + pow(blockY - clickPosition.y, 2));
				if (distance < minDistance) {
					minDistance = distance;
					closestX = i;
					closestY = j;
				}
				}
				else if(clickPosition.x> 1625 || clickPosition.y> 875)
					return Vector2f(-1, -1);
			}
		}

		if (closestX != -1 && closestY != -1 && valid[closestX][closestY]) {
			cout << "inner: " << closestX << " " << closestY << endl;
			return Vector2f(coords[closestX][closestY][0], coords[closestX][closestY][1]);
		}
		else {

			return Vector2f(-1, -1);
		}
	}

	void changeBlockColor(int i, int j, const Color& color) {
		if (i >= 0 && i < m && j >= 0 && j < n) {
			blocks[i][j].setFillColor(color);
		}
	}

};


class Fire {
public:
	Sprite sprite;
	Texture fire;

	int posX = 0;
	int posY = 0;
	bool visible = true;
	Fire* next = nullptr;
	Fire* prev = nullptr;

	Fire() {}

	Fire(int posx, int posy, int type) {
		posX = posx;
		posY = posy;
		if (type == 1) {
		fire.loadFromFile("peafire1.png");
		sprite.scale(0.15, 0.15);
		}
		else if (type == 3) {
			fire.loadFromFile("peafire3.png");
			sprite.scale(0.1, 0.1);
		}
		else if (type == 5) {
			fire.loadFromFile("peafire2.png");
			sprite.scale(0.10, 0.10);
		}
		else {
			fire.loadFromFile("fire2.png");
			sprite.scale(0.25, 0.25);
		}
		sprite.setTexture(fire);
		sprite.setPosition(posX, posY);

	}

	void set_position() {
		if (visible) {
			sprite.setPosition(posX, posY);
		}
	}

	void moveRight() {
		posX+=10;
		if (posX >= 2000) {
			visible = false;
		}

		set_position();
	}

	void draw(RenderWindow& window) {
		if (visible == true)
			window.draw(sprite);
	}
};

class Firelist {
public:
	Fire* start = nullptr;
	Firelist() {}

	void insert(int posx, int posy, int val) {
		Fire* create = new Fire(posx, posy, val);
		if (start == nullptr) {
			start = create;
		}
		else {
			Fire* curr = start;
			while (curr->next) {
				curr = curr->next;
			}
			curr->next = create;
			create->prev = curr;
		}
	}

	void draw(RenderWindow& window) {
		if (start) {
			Fire* curr = start;
			while (curr) {
				curr->draw(window);
				curr = curr->next;
			}
		}
	}

	void move() {
		if (start) {

			Fire* curr = start;
			while (curr) {
				curr->moveRight();
				curr = curr->next;
			}
			 deleteInvisibleFires();
		}
	}
	void deleteInvisibleFires() {
		Fire* curr = start;
		while (curr) {
			if (!curr->visible) {
				Fire* temp = curr;
				if (curr == start) {
					start = curr->next;
					if (start)
						start->prev = nullptr;
					delete temp;
					curr = start;
				}
				else {
					curr->prev->next = curr->next;
					if (curr->next)
						curr->next->prev = curr->prev;
					Fire* nextNode = curr->next;
					delete curr;
					curr = nextNode;
				}
			}
			else {
				curr = curr->next;
			}
		}
	}

};

///Plants
class Power {
	Texture power;
public:
	Sprite sprite;
	int posx,posy;
	Power() {

	}
	Power(int x,int y) {
		power.loadFromFile("newfire3.png");
		sprite.setTexture(power);
		posx = x;
		posy = y;
		sprite.setPosition(posx, posy);

	}
	void move() {
		posy-=1;
		sprite.setPosition(posx, posy);
	}
	void display(RenderWindow& window) {
		window.draw(sprite);
	}


};

//zombie
class Zombie {
public:
	bool visible = true;
	int posX = 0;
	int posY = 0;
	int health = 0;
	Zombie* next = nullptr;
	Zombie* prev = nullptr;
	int type;
	Sprite sprite;
	int value;
	Zombie() {}

	Zombie(int type) : type(type) {}

	virtual void draw(RenderWindow& window) = 0;
	void move() {
		posX -= 10;
		sprite.setPosition(posX, posY);
	}

};

class simple : public Zombie {
public:

	Texture plant;

	simple(int type, int posx, int posy) : Zombie(type) {
		posX = posx;
		posY = posy;
		health = 6;
		value = 50;
		plant.loadFromFile("simple.png");
		sprite.setTexture(plant);
		sprite.setPosition(posx, posy);
		sprite.setScale(0.20f, 0.20f);

	}

	void draw(RenderWindow& window) override {
		if (visible)
			window.draw(sprite);
	}
};

class Football : public Zombie {
public:

	Texture plant;

	Football(int type, int posx, int posy) : Zombie(type) {
		posX = posx;
		posY = posy;
		health = 7;
		value = 75;
		plant.loadFromFile("footbalzombie.png");
		sprite.setTexture(plant);
		sprite.setPosition(posx, posy);
		sprite.setScale(0.20f, 0.20f);
	}

	void draw(RenderWindow& window) override {
		if (visible)
			window.draw(sprite);
	}
};

class Flying : public Zombie {
public:

	Texture plant;

	Flying(int type, int posx, int posy) : Zombie(type) {
		posX = posx;
		posY = posy;
		health = 8;
		value = 100;
		plant.loadFromFile("flying.png");
		sprite.setTexture(plant);
		sprite.setPosition(posx, posy);
		sprite.setScale(0.20f, 0.20f);
	}

	void draw(RenderWindow& window) override {
		if (visible)
			window.draw(sprite);
	}
};

class Dancing : public Zombie {
public:

	Texture plant;
	Dancing(int type, int posx, int posy) : Zombie(type) {
		posX = posx;
		posY = posy;
		health = 6;
		value = 125;
		plant.loadFromFile("dancing.png");
		sprite.setTexture(plant);
		sprite.setPosition(posx, posy);
		sprite.setScale(0.20f, 0.20f);
	}

	void draw(RenderWindow& window) override {
		if (visible)
			window.draw(sprite);
	}
};

class Dolphin : public Zombie {
public:

	Texture plant;
	Dolphin(int type, int posx, int posy) : Zombie(type) {
		posX = posx;
		posY = posy;
		health = 10;
		value = 150;
		plant.loadFromFile("dolphinrider.png");
		sprite.setTexture(plant);
		sprite.setPosition(posx, posy);
		sprite.setScale(0.20f, 0.20f);
	}

	void draw(RenderWindow& window) override {
		if (visible)
			window.draw(sprite);
	}
};

class Zombielist {
public:
	Zombie* start = nullptr;

	Zombielist() {}

	void insert(int type, int reward, int posx, int posy) {
		Zombie* create = nullptr;
	//%= val;
	//	cout << "Zombie : " << type<<"   " << val << endl;	srand(time(0));
	//	int type = rand() + rand();
		//type 
		cout << "Type : " << type << endl;
		if (type == 1 ||  type==0) {
			create = new simple(reward, posx, posy);
		}
		else if (type == 2) {
			create = new Football(reward, posx, posy);
		}
		else if (type == 3) {
			create = new Flying(reward, posx, posy);
		}
		else if (type == 4) {
			create = new Dancing(reward, posx, posy);
		}
		else if (type == 5) {
			create = new Dolphin(reward, posx, posy);
		}

		if (create) {
			cout << "Placed" << endl;
			if (start == nullptr) {
				start = create;
			}
			else {
				Zombie* curr = start;
				while (curr->next) {
					curr = curr->next;
				}
				create->prev = curr;
				curr->next = create;
			}
		}
	}

	void move() {
		if (start != nullptr) {

			Zombie* curr = start;
			while (curr) {
				curr->move();
				curr = curr->next;
			}
		}
	}

	void display(RenderWindow& window) {
		srand(time(0));
		if (start != nullptr) {

			Zombie* curr = start;
			while (curr) {
				curr->draw(window);
				curr = curr->next;
			}
		}
	}

	void movement(RenderWindow& window) {
		srand(time(0));
		if (start != nullptr) {

			Zombie* curr = start;
			while (curr) {
				curr->draw(window);
				int chk = rand() % 2;
				if (curr->posX > 200 && chk)
					curr->move();
				else if (curr->posY > 500 && chk) {
					curr->posY = curr->posY - 1;
					curr->sprite.setPosition(curr->posX, curr->posY);
				}
				else if (curr->posY < 350 && chk) {
					curr->posY = curr->posY + 1;
					curr->sprite.setPosition(curr->posX, curr->posY);
				}
				curr = curr->next;
			}
		}
	}

	void decrementlife(int& life) {
		Zombie* curr = start;
		while (curr) {
			if (curr->posX <= 200 ) {
				curr->visible = false;
				life--;
			}
			curr = curr->next;
		}
		deleteInvisibleFires();
	}
	void deleteInvisibleFires() {
		Zombie* curr = start;
		while (curr) {
			if (!curr->visible) {
				Zombie* temp = curr;
				if (curr == start) {
					start = curr->next;
					if (start)
						start->prev = nullptr;
					delete temp;
					curr = start;
				}
				else {
					curr->prev->next = curr->next;
					if (curr->next)
						curr->next->prev = curr->prev;
					Zombie* nextNode = curr->next;
					delete curr;
					curr = nextNode;
				}
			}
			else {
				curr = curr->next;
			}
		}
	}
};


///Plants
class Plant {
public:
	bool visible = true;
	int posX = 0;
	int posY = 0;
	int health = 1000;
	Plant* next = nullptr;
	int type;
	Firelist fires;

	Plant() {}
	Plant(int type) : type(type) {}

	virtual void draw(RenderWindow& window) = 0;
	void fire(RenderWindow& window) {
		fires.draw(window);
	}
};
class Peashooter : public Plant {
public:
	Sprite sprite;
	Texture plant;

	Peashooter(int type, int posx, int posy) : Plant(type) {
		posX = posx;
		posY = posy;
		plant.loadFromFile("peashooterplant.png");
		sprite.setTexture(plant);
		sprite.setPosition(posx, posy);
		sprite.setScale(0.75f, 0.75f);
	
	}

	void draw(RenderWindow& window) override {
		if (visible)
			window.draw(sprite);
	}

};

class Sunflower : public Plant {
public:
	Sprite sprite;
	Texture plant;

	Sunflower(int type, int posx, int posy) : Plant(type) {
		posX = posx;
		posY = posy;
		plant.loadFromFile("sunflowerplant.png");
		sprite.setTexture(plant);
		sprite.setPosition(posx, posy);
		sprite.setScale(0.75f, 0.75f);
	}

	void draw(RenderWindow& window) override {
		if (visible)
			window.draw(sprite);
	}
};

class Repeater : public Plant {
public:
	Sprite sprite;
	Texture plant;

	Repeater(int type, int posx, int posy) : Plant(type) {
		posX = posx;
		posY = posy;
		plant.loadFromFile("repeaterplant.png");
		sprite.setTexture(plant);
		sprite.setPosition(posx, posy);
		sprite.setScale(0.75f, 0.75f);
	}

	void draw(RenderWindow& window) override {
		if (visible)
			window.draw(sprite);
	}
};

class Walknut : public Plant {
public:
	Sprite sprite;
	Texture plant;

	Walknut(int type, int posx, int posy) : Plant(type) {
		posX = posx;
		posY = posy;
		plant.loadFromFile("walknutplant.png");
		sprite.setTexture(plant);
		sprite.setPosition(posx, posy);
		sprite.setScale(0.75f, 0.75f);
	}

	void draw(RenderWindow& window) override {
		if (visible)
			window.draw(sprite);
	}
};

class Snowpee : public Plant {
public:
	Sprite sprite;
	Texture plant;

	Snowpee(int type,int posx,int posy) : Plant(type) {
		posX = posx;
		posY = posy;
		plant.loadFromFile("snowpeaplant.png");
		sprite.setTexture(plant);
		sprite.setPosition(posx, posy);
		sprite.setScale(0.75f, 0.75f);
	}

	void draw(RenderWindow& window) override {
		if (visible)
			window.draw(sprite);
	}
};

class Cherrybomb : public Plant {
public:
	Sprite sprite;
	Texture plant;

	Cherrybomb(int type, int posx, int posy) : Plant(type) {
		posX = posx;
		posY = posy;
		plant.loadFromFile("cherrybombplant.png");
		sprite.setTexture(plant);
		sprite.setPosition(posx, posy);
		sprite.setScale(0.75f, 0.75f);
	}

	void draw(RenderWindow& window) override {
		if (visible)
			window.draw(sprite);
	}
};

class Plantlist {
public:
	Plant* start = nullptr;

	Plantlist() {}

	bool insert(int type, int reward, int posx, int posy) {
		Plant* create = nullptr;
		if (type == 0) {
			cout << "Not placed" << endl;
			return false;
		}
		else if (type == 1) {
			create = new Peashooter(type, posx, posy);
		}
		else if (type == 2) {
			create = new Sunflower(type, posx, posy);
		}
		else if (type == 3) {
			create = new Repeater(type, posx, posy);
		}
		else if (type == 4) {
			create = new Walknut(type, posx, posy);
		}
		else if (type == 5) {
			create = new Snowpee(type, posx, posy);
		}
		else if (type == 6) {
			create = new Cherrybomb(type, posx, posy);

		}
		if (create) {
			cout << "Placed" << endl;
			if (start == nullptr) {
				start = create;
			}
			else {
				Plant* curr = start;
				while (curr->next) {
					curr = curr->next;
				}
				curr->next = create;
			}
			return true;
		}
	}

	void initiatefire(Zombielist& list,RenderWindow& window) {
		Zombie* curr = list.start;
		
		if (curr != nullptr && start!=nullptr ) {
			while (curr!=nullptr)
			{
				Plant* chk = start;
				while (chk)
				{
					if (chk->posY-26 == curr->posY && chk->type!=2 && curr->posX>chk->posX && curr->visible) {
						//fire 

						chk->fires.insert(chk->posX,chk->posY,chk->type);
						//cin.ignore();
					}

					chk = chk->next;
				}

				curr = curr->next;
			}


		}

	}

	void detectcollision(Zombielist& list, RenderWindow& window,int& score) {
		Zombie* curr = list.start;

		if (curr != nullptr && start != nullptr) {
			while (curr != nullptr)
			{
				Plant* chk = start;
				while (chk)
				{

					Fire* chkfire = chk->fires.start;
					while (chkfire)
					{
						if (chkfire->posY == curr->posY + 26 && chkfire->posX >= curr->posX && curr->posX > chk->posX) {
							chkfire->visible = false;
							//cout << "Plant: " << chkfire->posX << ", Zombie: " << curr->posX << "===" << "Plant: " << chkfire->posY << ", Zombie: " << curr->posY << endl;
							curr->health--;
							if (!curr->health) {
								curr->visible = false;
								score += curr->value;
							}

						}
						chkfire = chkfire->next;
					}
					chk = chk->next;
				}

				curr = curr->next;
			}


		}
		list.deleteInvisibleFires();
	}

	void display(RenderWindow& window) {
		if (start != nullptr) {
			Plant* curr = start;
			while (curr) {
				curr->fire(window);
				curr->fires.move();
				curr->draw(window);
				curr = curr->next;
			}
		}
	}
};


