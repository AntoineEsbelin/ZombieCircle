#pragma once

using namespace sf;
using namespace std;



// Classe pour les munitions 

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

// Struct enemi zombie de base ( cours et pourcentae de chance de revivre )

struct Enemy
{
	bool isDead = false;
	CircleShape enemyCircleShape;
	float rusherSpeed = 2; // rand() % 4 + 1;
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



struct Shooter
{
	CircleShape shooterShape;
	bool isDead = false;
	float shooterSpeed = rand() % 4 + 1;
	float timeOfDeath = 0.f;
	float coolDown = timeOfDeath + timeBeforeRevive;
	float timeBeforeRevive = 3.f;
	float reviveTime = 0.f;
	int isReviving = rand() % 100;
	int respawnPourcentage = 40;

	float shootMaxCoolDown = shooted + 2.f;
	float shooted = 0.f;
	bool canShoot = false;
	float shootCoolDown = 0.f;
};



struct shooterBullet
{
	CircleShape bulletShape;
	float bulletSpeed = .05f;
	Vector2f playerPosition;
};

void Reload(int& currentammo, int& maxammo);
std::vector<Enemy> SpawnEnemyRusher(int number);
std::vector<Shooter> SpawnEnemyShooter(int number);
void RusherParameters(Enemy& rusher, CircleShape& player);
void ShooterParameters(Shooter& shooter, CircleShape& player, vector<shooterBullet>& shooterBullets);
void Reload(int& currentammo, int& maxammo);
void Ammo(RenderWindow& window, int& currentAmmo, CircleShape& ammoBox, vector<Enemy> ennemies);


