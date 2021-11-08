#include <SFML/Graphics.hpp>
#include "windows.h"
#include <iostream>




int main()
{
   
    sf::RenderWindow window(sf::VideoMode(800, 600), "ZombieCircle");

    //Cercle
    sf::CircleShape circle;
    circle.setRadius(100.0f);
    circle.setPosition(300.0f, 150.0f);
    circle.setFillColor(sf::Color::Red);
  
    while (window.isOpen()) {
        
        sf::Event event;
        event.type = sf::Event::KeyPressed;
        event.key.code = sf::Keyboard::Space;
        while (window.pollEvent(event)) {
            // Process any input event here
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        // Whatever I want to draw goes here
        window.draw(circle);
        window.display();

        //std::cout << "nombre d'espace pressé est : " << event.key.code << "\n";
    }
}


