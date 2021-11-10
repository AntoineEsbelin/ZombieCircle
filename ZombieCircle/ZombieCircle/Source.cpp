#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <math.h>
#include "windows.h"
#include "Header.h"
#include <vector>


float PI = 3.14159265359f;

int main()
{
	srand(time(NULL));
	// Fenêtre
	RenderWindow window(VideoMode(1100, 1100), "Ma première fenêtre");
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


	// Enemies
	Enemy enemy[3] = { SpawnEnemyRusher() , SpawnEnemyRusher() , SpawnEnemyRusher() };


	// Background

	RectangleShape bg(Vector2f(1100, 1100));
	FloatRect bgRect = bg.getLocalBounds();
	bg.setOrigin(bgRect.left + bgRect.width / 2.0f, bgRect.top + bgRect.height / 2.0f);
	bg.setPosition(window.getView().getCenter());
	bg.setFillColor(Color::Black);


	Clock clock;

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
		Time elapsedTime = clock.restart(); //< Calcul du temps écoulé depuis la dernière boucle

		// Mise à jour 
		// Vecteurs
		playerCenter = Vector2f(player.getPosition().x + player.getRadius(), player.getPosition().y + player.getRadius());
		//enemyMove(enemy, player);
		for (int i = 0; i < sizeof(enemy) / sizeof(*enemy); i++)
		{
			if (!enemy[i].isDead)
			{
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
			}
		}
		

		mousePosWindow = Vector2f(Mouse::getPosition(window));
		aimDir = mousePosWindow - playerCenter;
		aimDirNorm = aimDir / static_cast<float>(sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)));

		//cout << aimDirNorm.x << " " << aimDirNorm.y << "\n";

		//Déplacement du joueur 
		if (Keyboard::isKeyPressed(Keyboard::Q))
			player.move(-5.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::D))
			player.move(5.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::Z))
			player.move(0.f, -5.f);
		if (Keyboard::isKeyPressed(Keyboard::S))
			player.move(0.f, 5.f);

		//Tir du joueur 
		if (Mouse::isButtonPressed(Mouse::Left) ) {
			b1.shape.setPosition(playerCenter);
			b1.currVelocity = aimDirNorm * b1.maxSpeed;

			bullets.push_back(Bullet(b1));
		}

		// Rendu
		window.clear();


		window.draw(bg);
		window.draw(player);
		for (int i = 0; i < sizeof(enemy) / sizeof(*enemy); i++)
		{
			window.draw(enemy[i].enemyCircleShape);
		}

		// Tir des projectiles 

		
			for (size_t i = 0; i < bullets.size(); i++) {
				window.draw(bullets[i].shape);
				bullets[i].shape.move(bullets[i].currVelocity);
			
				if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > window.getSize().x || bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > window.getSize().y) {
					bullets.erase(bullets.begin() + i);
				}
				for (int j = 0; j < sizeof(enemy) / sizeof(*enemy); j++)
				{
					if ((bullets[i].shape.getPosition().x < enemy[j].enemyCircleShape.getPosition().x + enemy[j].enemyCircleShape.getRadius()) && (bullets[i].shape.getPosition().y < enemy[j].enemyCircleShape.getPosition().y + enemy[j].enemyCircleShape.getRadius()) && (bullets[i].shape.getPosition().x > enemy[j].enemyCircleShape.getPosition().x - enemy[j].enemyCircleShape.getRadius()) && (bullets[i].shape.getPosition().y > enemy[j].enemyCircleShape.getPosition().y - enemy[j].enemyCircleShape.getRadius()))
					{
						enemy[j].enemyCircleShape.setFillColor(Color::White);
						enemy[j].isDead = true;
					}
				}
			}
		//col screen
		// Left col
		if (player.getPosition().x < 0.f)
			player.setPosition(0.f, player.getPosition().y);
		//Top col
		if (player.getPosition().y < 0.f)
			player.setPosition(player.getPosition().x, 0.f);
		//Right col
		if (player.getPosition().x + player.getGlobalBounds().width > xwindow)
			player.setPosition(xwindow - player.getGlobalBounds().width, player.getPosition().y);
		//Bottom col
		if (player.getPosition().y + player.getGlobalBounds().height > ywindow)
			player.setPosition(player.getPosition().x, ywindow - player.getGlobalBounds().height);

		window.display();
	}
}
