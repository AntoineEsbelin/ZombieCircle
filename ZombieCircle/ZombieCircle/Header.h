#pragma once
//void RectMovement(sf::RectangleShape& rect, float deltaTime);

//void RectMovement(sf::RectangleShape& rect, float deltaTime);



using namespace sf;
using namespace std;

void Reload(int& currentammo, int& maxammo);

class Bullet {
public:
	CircleShape shape;
	Vector2f currVelocity;
	float maxSpeed;

	Bullet(float radius = 5.f) : currVelocity(0.f, 0.f), maxSpeed(15.f)
	{
		this->shape.setRadius(radius);
		this->shape.setFillColor(Color::Red);
	}
};

struct Enemy
{
	bool isDead = false;
	CircleShape enemyCircleShape;
	float rusherSpeed = rand() % 4 + 1;
	float timeOfDeath = 0.f;
	float coolDown = timeOfDeath + timeBeforeRevive;
	float timeBeforeRevive = 2.f;
	float reviveTime = 0.f;
	int isReviving = rand() % 100;
	int respawnPourcentage = 0;
};

struct Ammo
{
	bool isPicked = false;
	RectangleShape ammoShape;
	bool giveAmmo;
	int ammoMount = 5;
	int ammoisSpawning = rand() % 100;
	int spawnPourcent = 100;
};

std::vector<Enemy> SpawnEnemyRusher(int number)
{
	std::vector<Enemy> rusher(number);
	for(int i = 0; i < rusher.size(); i++)
	{
		int enemyStartPosX = rand() % 1100;
		int enemyStartPosY = rand() % 1100;
		rusher[i].enemyCircleShape.setRadius(15.f);
		rusher[i].enemyCircleShape.setFillColor(Color::Green);
		rusher[i].enemyCircleShape.setPosition(enemyStartPosX, enemyStartPosY);
	}
	return rusher;
}


