#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <math.h>
#include "windows.h"
#include "Header.h"
#include <vector>




int main()
{
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

	int playerLife = 3;
	cout << "PLayer Life : " << playerLife << endl;

	//Life Shape
	Vector2f newpos (100.f, 0.f);
	CircleShape heart;
	vector<CircleShape> heartv;
	heart.setRadius(50.F);
	heart.setPointCount(3);
	heart.setOrigin(heart.getRadius(), heart.getRadius());
	heart.rotate(180);
	heart.setFillColor(Color::Green);
	heart.setPosition(100, 100);

	// Projectiles
	Bullet b1;
	
	vector<Bullet> bullets;


	// Enemies


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
		mousePosWindow = Vector2f(Mouse::getPosition(window));
		aimDir = mousePosWindow - playerCenter;
		aimDirNorm = aimDir / static_cast<float>(sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)));

		/*cout << aimDirNorm.x << " " << aimDirNorm.y << "\n";*/

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
		if (Mouse::isButtonPressed(Mouse::Left) && playerLife > 0 ) {
			b1.shape.setPosition(playerCenter);
			b1.currVelocity = aimDirNorm * b1.maxSpeed;

			bullets.push_back(Bullet(b1));
		}

		// Rendu
		window.clear();


		window.draw(bg);
		window.draw(player);
		

		// Tir des projectiles 
		for (size_t i = 0; i < bullets.size(); i++) {
			window.draw(bullets[i].shape);
			bullets[i].shape.move(bullets[i].currVelocity);
			
			if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > window.getSize().x || bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > window.getSize().y) {
				bullets.erase(bullets.begin() + i);
			}
		}

		//Life Bar
		for (size_t i = 0; i < 3; i++)
		{
			heartv.push_back(heart);
			if (i > 0)
			{
				heart.setPosition(heart.getPosition() + newpos, heart.getPosition());
			}
			
			window.draw(heart);
		}

		//Player Dead
		if (Keyboard::isKeyPressed(Keyboard::Key::K))
		{
			playerLife -= 1;
			cout << "PLayer Life : " << playerLife << endl;
		}
		if (playerLife == 0)
		{
			player.setFillColor(Color::Transparent);
			
			
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
