#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <math.h>
#include "windows.h"
#include "Header.h"
#include <vector>
#include <time.h>




int main()
{
	srand(time(NULL));
	// Fen�tre
	RenderWindow window(VideoMode(1100, 1100), "ZombieCircle");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	float xwindow = window.getSize().x;
	float ywindow = window.getSize().y;

	

	// Player

	CircleShape player;
	player.setRadius(25.f);
	player.setFillColor(Color::Magenta);

	Vector2f playerCenter;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;

	// Projectiles
	Bullet b1;
	Bullet shooterB1;
	

	vector<Bullet> bullets;
	vector<Bullet> shooterBullets;

	//Munition
	int maxammo = 25;
	int currentammo = 10;
	float reloadtime = 1.f;
	cout << "Ammo : " << currentammo << endl;

	bool canR = true;
	float rCooldownMax = 150.f;
	float rCooldown = 0.f;

	//Affichage Munition
	Text currentammotext;
	string ammostri = to_string(currentammo);
	currentammotext.setString(ammostri);
	currentammotext.setFillColor(Color::White);
	currentammotext.setPosition(/*xwindow - 100*/player.getPosition().x, /*ywindow - 100*/ player.getPosition().y);

	Text maxammotext;
	string maxammostri = to_string(maxammo);
	maxammotext.setString(maxammostri);

	//Cooldown
	bool canAttack = true;
	float attackCooldownMax = 10.f;
	float attackCooldown = 10.f;
	
	// Player en vie 

	bool isAlive = true;

	// Enemies
	std::vector<Enemy> rusherEnemy = SpawnEnemyRusher(3);
	std::vector<Shooter> shooterEnemy = SpawnEnemyShooter(2);

	// Background

	RectangleShape bg(Vector2f(1100, 1100));
	FloatRect bgRect = bg.getLocalBounds();
	bg.setOrigin(bgRect.left + bgRect.width / 2.0f, bgRect.top + bgRect.height / 2.0f);
	bg.setPosition(window.getView().getCenter());
	bg.setFillColor(Color::Black);


	Clock clocked;

	

	while (window.isOpen())
	{
		// Inputs
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;

			default:
				break;

			}
		}


		// Logique

		Time elapsedTime = clocked.restart(); //< Calcul du temps ecoule depuis la derniere boucle

		// Mise � jour 
		// Vecteurs
		playerCenter = Vector2f(player.getPosition().x + player.getRadius(), player.getPosition().y + player.getRadius());
		mousePosWindow = Vector2f(Mouse::getPosition(window));
		aimDir = mousePosWindow - playerCenter;
		aimDirNorm = aimDir / static_cast<float>(sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)));
		
		

		//D�placement du joueur 
		if (Keyboard::isKeyPressed(Keyboard::Q) && isAlive == true)
			player.move(-5.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::D) && isAlive == true)
			player.move(5.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::Z) && isAlive == true)
			player.move(0.f, -5.f);
		if (Keyboard::isKeyPressed(Keyboard::S) && isAlive == true)
			player.move(0.f, 5.f);

		// Mort du joueur

		if (isAlive == false) {
			player.setFillColor(Color::Transparent);
		}

		//Tir du joueur 
		if (Mouse::isButtonPressed(Mouse::Left) && canAttack && currentammo > 0 && isAlive) {
			if (Mouse::isButtonPressed(Mouse::Left)) {
				b1.shape.setPosition(playerCenter);
				b1.currVelocity = aimDirNorm * b1.maxSpeed;

				bullets.push_back(Bullet(b1));
				currentammo--;
			
				canAttack = false;
				cout << "Ammo : " << currentammo << " / " << "MaxAmmo : " << maxammo << endl;

			}
			
		}

		// Cooldown d'attaque ( en n'attaquant pas pendant un moment , la prochaine attaque enverra 2 bullets d'affilé ) 
		if (attackCooldown >= attackCooldownMax) {
			attackCooldown = 0.f;
			canAttack = true;
			
		}
		else {
			attackCooldown += 0.5f;
		}

		// Cooldown reload
		
		if (rCooldown >= rCooldownMax) {
			rCooldown = 0.f;
			canR = true;
			
		}
		else {
			rCooldown += 0.5f;
		}

		//Munition
		if (Keyboard::isKeyPressed(Keyboard::Key::R) && maxammo > 0 && canR)
		{
			
			Reload(currentammo, maxammo);
			canR = false;
			
			
		}
			window.clear();

			window.draw(bg);
			
			window.draw(player);
			window.draw(currentammotext);
			
			//boucle pour chaque ennemis
				//RUSHER
			for (int i = 0; i < rusherEnemy.size(); i++)
			{
				//fonctions general du rusher
				RusherParameters(rusherEnemy[i], player);
				// Mort du joueur en contact d'un ennemi 
				if ((player.getPosition().x < rusherEnemy[i].enemyCircleShape.getPosition().x + rusherEnemy[i].enemyCircleShape.getRadius()) && (player.getPosition().y < rusherEnemy[i].enemyCircleShape.getPosition().y + rusherEnemy[i].enemyCircleShape.getRadius()) && (player.getPosition().x > rusherEnemy[i].enemyCircleShape.getPosition().x - rusherEnemy[i].enemyCircleShape.getRadius()) && (player.getPosition().y > rusherEnemy[i].enemyCircleShape.getPosition().y - rusherEnemy[i].enemyCircleShape.getRadius()))
				{
					if (!rusherEnemy[i].isDead)
					{
						isAlive = false;
					}
				}
				//Affiche les ennemis s'ils sont pas morts
				//pour les rusher
				if (!rusherEnemy[i].isDead || (rusherEnemy[i].isReviving < rusherEnemy[i].respawnPourcentage))
				{
					window.draw(rusherEnemy[i].enemyCircleShape);
				}
				else
				{
					rusherEnemy.erase(rusherEnemy.begin() + i);
				}
			}

				//SHOOTER
			for (int i = 0; i < shooterEnemy.size(); i++)
			{

				ShooterParameters(shooterEnemy[i], player, shooterBullets, shooterB1);
				if (!shooterEnemy[i].isDead || (shooterEnemy[i].isReviving < shooterEnemy[i].respawnPourcentage))
				{
					window.draw(shooterEnemy[i].shooterShape);
				}
				else
				{
					shooterEnemy.erase(shooterEnemy.begin() + i);
				}
			}
			
			// Tir des projectiles 
			for (size_t i = 0; i < bullets.size(); i++) {
				window.draw(bullets[i].shape);
				bullets[i].shape.move(bullets[i].currVelocity);

				// Mort des ennemis 
				//RUSHER
				for (int j = 0; j < rusherEnemy.size(); j++)
				{
					if ((bullets[i].shape.getPosition().x < rusherEnemy[j].enemyCircleShape.getPosition().x + rusherEnemy[j].enemyCircleShape.getRadius()) && (bullets[i].shape.getPosition().y < rusherEnemy[j].enemyCircleShape.getPosition().y + rusherEnemy[j].enemyCircleShape.getRadius()) && (bullets[i].shape.getPosition().x > rusherEnemy[j].enemyCircleShape.getPosition().x - rusherEnemy[j].enemyCircleShape.getRadius()) && (bullets[i].shape.getPosition().y > rusherEnemy[j].enemyCircleShape.getPosition().y - rusherEnemy[j].enemyCircleShape.getRadius()))
					{
						rusherEnemy[j].enemyCircleShape.setFillColor(Color::White);
						rusherEnemy[j].isDead = true;
						
					}
				}
				//SHOOTER
				for (int j = 0; j < shooterEnemy.size(); j++)
				{
					if ((bullets[i].shape.getPosition().x < shooterEnemy[j].shooterShape.getPosition().x + shooterEnemy[j].shooterShape.getRadius()) && (bullets[i].shape.getPosition().y < shooterEnemy[j].shooterShape.getPosition().y + shooterEnemy[j].shooterShape.getRadius()) && (bullets[i].shape.getPosition().x > shooterEnemy[j].shooterShape.getPosition().x - shooterEnemy[j].shooterShape.getRadius()) && (bullets[i].shape.getPosition().y > shooterEnemy[j].shooterShape.getPosition().y - shooterEnemy[j].shooterShape.getRadius()))
					{
						shooterEnemy[j].shooterShape.setFillColor(Color::White);
						shooterEnemy[j].isDead = true;

					}
				}
				//Suppression des projectiles en dehors de l'écran 
				if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > window.getSize().x || bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > window.getSize().y) {
					bullets.erase(bullets.begin() + i);
					
				}
			}

			//tir des shooter
			/*for (int j = 0; j < shooterEnemy.size(); j++)
			{
				if (shooterEnemy[j].canShoot)
				{
					shooterEnemy[j].shooted = clock() / (float)CLOCKS_PER_SEC;
					shooterB1.shape.setPosition(shooterEnemy[j].shooterShape.getPosition());
					shooterB1.currVelocity.x = player.getPosition().x + player.getRadius();
					shooterB1.currVelocity.y = player.getPosition().y + player.getRadius();
					shooterBullets.push_back(Bullet(shooterB1));
					shooterEnemy[j].canShoot = false;
				}
				else if (!shooterEnemy[j].canShoot)
				{
					if (shooterEnemy[j].coolDown < shooterEnemy[j].shootMaxCoolDown)
					{
						shooterEnemy[j].coolDown = clock() / (float)CLOCKS_PER_SEC;
					}
					else
					{
						shooterEnemy[j].canShoot = true;
					}

				}

			}
			for (size_t i = 0; i < shooterBullets.size(); i++) 
			{
				window.draw(shooterBullets[i].shape);
				shooterBullets[i].shape.move(shooterBullets[i].currVelocity);
				if ((shooterBullets[i].shape.getPosition().x < player.getPosition().x + player.getRadius()) && (shooterBullets[i].shape.getPosition().y < player.getPosition().y + player.getRadius()) && (player.getPosition().x > player.getPosition().x - player.getRadius()) && (shooterBullets[i].shape.getPosition().y > player.getPosition().y - player.getRadius()))
				{
					isAlive = false;
				}
			}*/

			//Limitations de la bordure d'écran
			if (player.getPosition().x < 0.f)
				player.setPosition(0.f, player.getPosition().y);
			
			if (player.getPosition().y < 0.f)
				player.setPosition(player.getPosition().x, 0.f);
			
			if (player.getPosition().x + player.getGlobalBounds().width > xwindow)
				player.setPosition(xwindow - player.getGlobalBounds().width, player.getPosition().y);
			
			if (player.getPosition().y + player.getGlobalBounds().height > ywindow)
				player.setPosition(player.getPosition().x, ywindow - player.getGlobalBounds().height);
		window.display();

		// Esc pour quitter le jeu 
		if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
			window.close();
	}

	
	
}

std::vector<Enemy> SpawnEnemyRusher(int number)
{
	std::vector<Enemy> rusher(number);
	for (int i = 0; i < rusher.size(); i++)
	{
		int enemyStartPosX = rand() % 1100;
		int enemyStartPosY = rand() % 1100;
		rusher[i].enemyCircleShape.setRadius(15.f);
		rusher[i].enemyCircleShape.setFillColor(Color::Green);
		rusher[i].enemyCircleShape.setPosition(enemyStartPosX, enemyStartPosY);
	}
	return rusher;
}

std::vector<Shooter> SpawnEnemyShooter(int number)
{
	std::vector<Shooter> shooter(number);
	for (int i = 0; i < shooter.size(); i++)
	{
		int enemyStartPosX = rand() % 1100;
		int enemyStartPosY = rand() % 1100;
		shooter[i].shooterShape.setRadius(20.f);
		shooter[i].shooterShape.setPointCount(3);
		shooter[i].shooterShape.setFillColor(Color::Red);
		shooter[i].shooterShape.setPosition(enemyStartPosX, enemyStartPosY);
	}
	return shooter;
}

void RusherParameters(Enemy& rusher, CircleShape& player)
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

void ShooterParameters(Shooter& shooter, CircleShape& player, vector<Bullet>& shooterBullets, Bullet &shooterB1)
{
	if (!shooter.isDead)
	{
		//mouvements des ennemis
		if (!((shooter.shooterShape.getPosition().x < player.getPosition().x + (player.getRadius() * 25)) && (shooter.shooterShape.getPosition().y < player.getPosition().y + (player.getRadius() * 25)) && (shooter.shooterShape.getPosition().x > player.getPosition().x - (player.getRadius() * 25)) && (shooter.shooterShape.getPosition().y > player.getPosition().y - (player.getRadius() * 25))))
		{
			if ((shooter.shooterShape.getPosition().x > player.getPosition().x))
			{
				shooter.shooterShape.move(-shooter.shooterSpeed, 0.f);
			}
			if ((shooter.shooterShape.getPosition().y > player.getPosition().y))
			{
				shooter.shooterShape.move(0.f, -shooter.shooterSpeed);
			}
			if ((shooter.shooterShape.getPosition().x < player.getPosition().x))
			{
				shooter.shooterShape.move(shooter.shooterSpeed, 0.f);
			}
			if ((shooter.shooterShape.getPosition().y < player.getPosition().y))
			{
				shooter.shooterShape.move(0.f, shooter.shooterSpeed);
			}
		}
		shooter.timeOfDeath = clock() / CLOCKS_PER_SEC;
	}
	else
	{
		//quand mort s'il peut revivre, attend son cooldown avant de revivre
		if (shooter.isReviving < shooter.respawnPourcentage)
		{
			if (shooter.reviveTime < shooter.timeOfDeath + shooter.timeBeforeRevive)
			{
				shooter.reviveTime = clock() / CLOCKS_PER_SEC;
			}
			else
			{
				shooter.isDead = false;
				shooter.isReviving = rand() % 100;
				shooter.shooterShape.setFillColor(Color::Red);
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
//condition pour aimanter un element
// variable etant pour le shooter
//if(shooter.shooterShape.getPosition().x < player.getPosition().x + (player.getRadius() * 2)) && (shooter.shooterShape.getPosition().y < player.getPosition().y + (player.getRadius() * 2)) && (shooter.shooterShape.getPosition().x > player.getPosition().x - (player.getRadius() * 2)) && (shooter.shooterShape.getPosition().y > player.getPosition().y - (player.getRadius() * 2))