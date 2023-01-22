#pragma once

#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>

#define PI 3.14159265

class PendulumSection
{
public:
	sf::CircleShape body;
	sf::RectangleShape arm;

	PendulumSection(int mass, float angle, int armLength, int x, int y);
	PendulumSection(int mass, float angle, int armLength, PendulumSection& upperPendulum);
	void initialize(int mass, float angle, int armLength);

	sf::Vector2f getEndingPosition();
	int getArmLength();
	void setArmLength(int newLength);
	void setPosition(sf::Vector2f position);
	void setMass(int mass);

	void calculatePendulum(PendulumSection& upperPendulum);
	void updatePos(float dt, PendulumSection& upperPendulum, bool isTopSection);

	void drawArm(sf::RenderWindow& window);
	void drawBody(sf::RenderWindow& window);

private:
	sf::Vector2f startPosition;
	int armLength;
	int mass;

	float angle;
	float angularAcceleration;
	float angularVelocity;

	const float G = 9.81;
};