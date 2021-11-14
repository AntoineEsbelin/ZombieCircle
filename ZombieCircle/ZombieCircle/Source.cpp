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
	
	

	vector<Bullet> bullets;

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
	std::vector<Enemy> rusherEnemy = SpawnEnemyRusher(5);

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
			for (int i = 0; i < rusherEnemy.size(); i++)
			{
				//fonctions general du rusher
				RusherParameters(rusherEnemy[i], player, window);
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
			
			// Tir des projectiles 
			for (size_t i = 0; i < bullets.size(); i++) {
				window.draw(bullets[i].shape);
				bullets[i].shape.move(bullets[i].currVelocity);

				// Mort des ennemis 
				for (int j = 0; j < rusherEnemy.size(); j++)
				{
					if ((bullets[i].shape.getPosition().x < rusherEnemy[j].enemyCircleShape.getPosition().x + rusherEnemy[j].enemyCircleShape.getRadius()) && (bullets[i].shape.getPosition().y < rusherEnemy[j].enemyCircleShape.getPosition().y + rusherEnemy[j].enemyCircleShape.getRadius()) && (bullets[i].shape.getPosition().x > rusherEnemy[j].enemyCircleShape.getPosition().x - rusherEnemy[j].enemyCircleShape.getRadius()) && (bullets[i].shape.getPosition().y > rusherEnemy[j].enemyCircleShape.getPosition().y - rusherEnemy[j].enemyCircleShape.getRadius()))
					{
						rusherEnemy[j].enemyCircleShape.setFillColor(Color::White);
						rusherEnemy[j].isDead = true;
						
					}
				}
				//Suppression des projectiles en dehors de l'écran 
				if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > window.getSize().x || bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > window.getSize().y) {
					bullets.erase(bullets.begin() + i);
					
				}
			}

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