#include <SFML/Graphics.hpp>
#include "windows.h"
#include <iostream>
#include "Header.h"




int main()
{
	sf::RenderWindow window(sf::VideoMode(1600, 1200), "Ma première fenêtre");
	window.setVerticalSyncEnabled(true);

	sf::RectangleShape rect(sf::Vector2f(100, 100));
	rect.setPosition(sf::Vector2f(400.f, 300.f));

	sf::RectangleShape bg(sf::Vector2f(1200, 1200));
	sf::FloatRect bgRect = bg.getLocalBounds();
	bg.setOrigin(bgRect.left + bgRect.width / 2.0f, bgRect.top + bgRect.height / 2.0f);
	bg.setPosition(window.getView().getCenter());
	bg.setFillColor(sf::Color::Green);
	

	sf::Clock clock;

	while (window.isOpen())
	{
		// Inputs
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;

				default:
					break;
			}
		}

		// Logique
		sf::Time elapsedTime = clock.restart(); //< Calcul du temps écoulé depuis la dernière boucle

		RectMovement(rect, elapsedTime.asSeconds());

		// Rendu
		window.clear();

		
		window.draw(bg);
		window.draw(rect);

		window.display();
	}
}

// Gestion des déplacement d'un rectangle
void RectMovement(sf::RectangleShape& rect, float deltaTime)
{
	float speed = 300.f; // 300 pixels par seconde

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		rect.move(sf::Vector2f(0.f, -speed * deltaTime));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		rect.move(sf::Vector2f(0.f, speed * deltaTime));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		rect.move(sf::Vector2f(-speed * deltaTime, 0.f));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		rect.move(sf::Vector2f(speed * deltaTime, 0.f));
}



