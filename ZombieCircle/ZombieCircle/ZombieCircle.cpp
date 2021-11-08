#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "windows.h"
#include <iostream>
#include "Header.h"
#include <vector>

using namespace sf;
using namespace std;

int main()
{
	cout << "Saloute";
	//// Fenêtre
	//RenderWindow window(VideoMode(1100, 1100), "Ma première fenêtre");
	//window.setVerticalSyncEnabled(true);

	//// Player

	//RectangleShape player(Vector2f(50, 50));
	//player.setPosition(Vector2f(400.f, 300.f));

	//// Munitions
	//CircleShape projectile;
	//projectile.setFillColor(Color::Red);
	//projectile.setRadius(5.f);

	//vector<CircleShape> projectiles;
	//projectiles.push_back(CircleShape(projectile));
	//// Enemies
	//RectangleShape enemy;
	//enemy.setFillColor(Color::Magenta);
	//enemy.setSize(Vector2f(50.f, 50.f));
	//enemy.setPosition(window.getView().getCenter());
	//
	//vector <RectangleShape> enemies;
	////enemies.push_back(RectangleShape(enemy));

	//// Background

	//RectangleShape bg(Vector2f(1100, 1100));
	//FloatRect bgRect = bg.getLocalBounds();
	//bg.setOrigin(bgRect.left + bgRect.width / 2.0f, bgRect.top + bgRect.height / 2.0f);
	//bg.setPosition(window.getView().getCenter());
	//bg.setFillColor(Color::Black);
	//

	//Clock clock;

	//while (window.isOpen())
	//{
	//	// Inputs
	//	Event event;
	//	while (window.pollEvent(event))
	//	{
	//		switch (event.type)
	//		{
	//			case Event::Closed:
	//				window.close();
	//				break;

	//			default:
	//				break;
	//		}
	//	}

	//	// Logique
	//	Time elapsedTime = clock.restart(); //< Calcul du temps écoulé depuis la dernière boucle

	//	RectMovement(player, elapsedTime.asSeconds());

	//	// Rendu
	//	window.clear();

	//	
	//	window.draw(bg);
	//	window.draw(player);
	//	window.draw(enemy);

	//	for (size_t i = 0; i = enemies.size(); i++) 
	//		window.draw(enemies[i]);
	//	

	//	for (size_t i = 0; i < projectiles.size(); i++)
	//		window.draw(projectiles[i]);

	//	window.display();
	//}
}

// Gestion des déplacement d'un rectangle
//void RectMovement(sf::RectangleShape& rect, float deltaTime)
//{
//	float speed = 300.f; // 300 pixels par seconde
//
//	if (Keyboard::isKeyPressed(Keyboard::Up))
//		rect.move(Vector2f(0.f, -speed * deltaTime));
//
//	if (Keyboard::isKeyPressed(Keyboard::Down))
//		rect.move(Vector2f(0.f, speed * deltaTime));
//
//	if (Keyboard::isKeyPressed(Keyboard::Left))
//		rect.move(Vector2f(-speed * deltaTime, 0.f));
//
//	if (Keyboard::isKeyPressed(Keyboard::Right))
//		rect.move(Vector2f(speed * deltaTime, 0.f));
//}



