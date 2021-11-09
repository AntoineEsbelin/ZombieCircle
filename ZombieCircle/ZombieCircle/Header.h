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
