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
	int currentammo = maxammo;
	float reloadtime = 1.f;
	cout << "Ammo : " << currentammo << endl;
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
	
	// Player en vie 

	bool isAlive = true;

	// Enemies
	std::vector<Enemy> enemy = SpawnEnemyRusher(5);

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
		
		//boucle pour chaque ennemis
		for (int i = 0; i < enemy.size(); i++)
		{
			if (!enemy[i].isDead)
			{
				//mouvements des ennemis
				if ((enemy[i].enemyCircleShape.getPosition().x > player.getPosition().x))
				{
					enemy[i].enemyCircleShape.move(-enemy[i].rusherSpeed, 0.f);
				}
				if ((enemy[i].enemyCircleShape.getPosition().y > player.getPosition().y))
				{
					enemy[i].enemyCircleShape.move(0.f, -enemy[i].rusherSpeed);
				}
				if ((enemy[i].enemyCircleShape.getPosition().x < player.getPosition().x))
				{
					enemy[i].enemyCircleShape.move(enemy[i].rusherSpeed, 0.f);
				}
				if ((enemy[i].enemyCircleShape.getPosition().y < player.getPosition().y))
				{
					enemy[i].enemyCircleShape.move(0.f, enemy[i].rusherSpeed);
				}

				// Mort du joueur en contact d'un ennemi 
				if ((player.getPosition().x < enemy[i].enemyCircleShape.getPosition().x + enemy[i].enemyCircleShape.getRadius()) && (player.getPosition().y < enemy[i].enemyCircleShape.getPosition().y + enemy[i].enemyCircleShape.getRadius()) && (player.getPosition().x > enemy[i].enemyCircleShape.getPosition().x - enemy[i].enemyCircleShape.getRadius()) && (player.getPosition().y > enemy[i].enemyCircleShape.getPosition().y - enemy[i].enemyCircleShape.getRadius()))
					isAlive = false;
				

				enemy[i].timeOfDeath = clock() / CLOCKS_PER_SEC;
			}
			else
			{
				//quand mort s'il peut revivre, attend son cooldown avant de revivre
				if (enemy[i].isReviving < enemy[i].respawnPourcentage)
				{
					if (enemy[i].reviveTime < enemy[i].timeOfDeath + enemy[i].timeBeforeRevive)
					{
						enemy[i].reviveTime = clock() / CLOCKS_PER_SEC;						
					}
					else
					{
						enemy[i].isDead = false;
						enemy[i].isReviving = rand() % 100;
						enemy[i].enemyCircleShape.setFillColor(Color::Green);
					}
					
				}
			}
		}

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


		//Munition
		if (Keyboard::isKeyPressed(Keyboard::Key::R) && maxammo > 0)
		{
			
			Reload(currentammo, maxammo);
			
			
		}
			window.clear();

			window.draw(bg);
			
			window.draw(player);
			window.draw(currentammotext);
			
			
			//Affiche les ennemis s'ils sont pas morts
			for (int i = 0; i < enemy.size(); i++)
			{
				if (!enemy[i].isDead || (enemy[i].isReviving < enemy[i].respawnPourcentage))
				{
					window.draw(enemy[i].enemyCircleShape);
				}
			}
			// Tir des projectiles 
			for (size_t i = 0; i < bullets.size(); i++) {
				window.draw(bullets[i].shape);
				bullets[i].shape.move(bullets[i].currVelocity);

				// Mort des ennemis 
				for (int j = 0; j < enemy.size(); j++)
				{
					if ((bullets[i].shape.getPosition().x < enemy[j].enemyCircleShape.getPosition().x + enemy[j].enemyCircleShape.getRadius()) && (bullets[i].shape.getPosition().y < enemy[j].enemyCircleShape.getPosition().y + enemy[j].enemyCircleShape.getRadius()) && (bullets[i].shape.getPosition().x > enemy[j].enemyCircleShape.getPosition().x - enemy[j].enemyCircleShape.getRadius()) && (bullets[i].shape.getPosition().y > enemy[j].enemyCircleShape.getPosition().y - enemy[j].enemyCircleShape.getRadius()))
					{
						enemy[j].enemyCircleShape.setFillColor(Color::White);
						enemy[j].isDead = true;
						
					}
				}
				//Suppression des projectiles en dehors de l'écran 
				if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > window.getSize().x || bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > window.getSize().y) {
					bullets.erase(bullets.begin() + i);
					canAttack = true;
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



