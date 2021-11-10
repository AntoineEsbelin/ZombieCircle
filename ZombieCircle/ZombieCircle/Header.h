#pragma once
//void RectMovement(sf::RectangleShape& rect, float deltaTime);

using namespace sf;
using namespace std;

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
};

Enemy SpawnEnemyRusher()
{
	Enemy enemy;
	int enemyStartPosX = rand() % 1100;
	int enemyStartPosY = rand() % 1100;
	enemy.enemyCircleShape.setRadius(15.f);
	enemy.enemyCircleShape.setFillColor(Color::Green);
	enemy.enemyCircleShape.setPosition(enemyStartPosX, enemyStartPosY);
	std::cout << enemyStartPosX << " " << enemyStartPosY << std::endl;
	return enemy;
}

//void enemyMove(CircleShape enemy, CircleShape player)
//{
//	
//}