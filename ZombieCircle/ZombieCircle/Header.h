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
	int respawnPourcentage = 50;
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

void RusherParameters(Enemy& rusher, CircleShape &player, RenderWindow &window)
{
	if (!rusher.isDead)
	{
		//mouvements des ennemis
		if ((rusher.enemyCircleShape.getPosition().x > player.getPosition().x))
		{
			rusher.enemyCircleShape.move(-rusher.rusherSpeed, 0.f);
		}
		if ((rusher.enemyCircleShape.getPosition().y > player.getPosition().y))
		{
			rusher.enemyCircleShape.move(0.f, -rusher.rusherSpeed);
		}
		if ((rusher.enemyCircleShape.getPosition().x < player.getPosition().x))
		{
			rusher.enemyCircleShape.move(rusher.rusherSpeed, 0.f);
		}
		if ((rusher.enemyCircleShape.getPosition().y < player.getPosition().y))
		{
			rusher.enemyCircleShape.move(0.f, rusher.rusherSpeed);
		}

		rusher.timeOfDeath = clock() / CLOCKS_PER_SEC;
	}
	else
	{
		//quand mort s'il peut revivre, attend son cooldown avant de revivre
		if (rusher.isReviving < rusher.respawnPourcentage)
		{
			if (rusher.reviveTime < rusher.timeOfDeath + rusher.timeBeforeRevive)
			{
				rusher.reviveTime = clock() / CLOCKS_PER_SEC;
			}
			else
			{
				rusher.isDead = false;
				rusher.isReviving = rand() % 100;
				rusher.enemyCircleShape.setFillColor(Color::Green);
			}

		}
	}

	
}

void Reload(int& currentammo, int& maxammo)
{
	cout << "Reloading.." << endl;
	if (maxammo < 0)
	{
		maxammo = 0;
	}
	if (currentammo < 0)
	{
		currentammo = 0;
	}
	currentammo += 5;
	maxammo -= 5;

	cout << "Ammo : " << currentammo << " / " << "MaxAmmo : " << maxammo << endl;


}
