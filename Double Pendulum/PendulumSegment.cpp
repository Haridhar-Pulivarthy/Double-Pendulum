#include "PendulumSegment.hpp"

#define LOG(x) std::cout << x << std::endl

PendulumSection::PendulumSection(int mass, float angle, int armLength, int x, int y)
{
	startPosition.x = x;
	startPosition.y = y;
	initialize(mass, angle, armLength);
}

PendulumSection::PendulumSection(int mass, float angle, int armLength, PendulumSection& upperPendulum)
{
	startPosition = upperPendulum.getEndingPosition();
	initialize(mass, angle, armLength);
}

void PendulumSection::initialize(int mass, float angle, int armLength)
{
	this->mass = mass;
	this->armLength = armLength;
	this->angle = angle * PI / 180;

	angularAcceleration = 0;
	angularVelocity = 0;

	arm.setSize(sf::Vector2f(10, armLength));
	arm.setOrigin(arm.getSize().x / 2, 0);
	arm.setPosition(sf::Vector2f(startPosition.x, startPosition.y));
	arm.setFillColor(sf::Color(156, 156, 156));
	arm.setRotation(angle);
	arm.setOutlineColor(sf::Color::Black);
	arm.setOutlineThickness(2);

	body.setRadius((mass / 5.0f));
	body.setOrigin(sf::Vector2f(body.getRadius(), body.getRadius()));
	body.setPosition(getEndingPosition());
	body.setOutlineColor(sf::Color::Black);
	body.setOutlineThickness(3);
	body.setFillColor(sf::Color(219, 219, 219));
}

sf::Vector2f PendulumSection::getEndingPosition()
{
	sf::Vector2f endingPosition;
	endingPosition.x = startPosition.x - armLength * sin(angle);
	endingPosition.y = startPosition.y + armLength * cos(angle);
	return(endingPosition);
}

int PendulumSection::getArmLength() { return armLength; }
void PendulumSection::setArmLength(int newLength) { armLength = newLength; }
void PendulumSection::setPosition(sf::Vector2f position) { startPosition = position; }
void PendulumSection::setMass(int mass) { this->mass = mass; }

void PendulumSection::updatePos(float dt, PendulumSection& upperPendulum, bool isTopSection)
{
	if (!isTopSection) startPosition = upperPendulum.getEndingPosition();
	angularVelocity += angularAcceleration * dt;
	angle += angularVelocity * dt;

	arm.setPosition(startPosition.x, startPosition.y);
	arm.setRotation(angle * (180 / PI));
	body.setPosition(getEndingPosition());
}

void PendulumSection::calculatePendulum(PendulumSection& upperPendulum)
{
	float m1 = upperPendulum.mass;
	float m2 = mass;
	float a1 = upperPendulum.angle;
	float a2 = angle;
	float L1 = upperPendulum.armLength;
	float L2 = armLength;
	float w1 = upperPendulum.angularVelocity;
	float w2 = angularVelocity;

	float num1 = -G * (2 * m1 + m2) * sin(a1);
	float num2 = -m2 * G * sin(a1 - 2 * a2);
	float num3 = -2 * sin(a1 - a2) * m2 * (w2 * w2 * L2 + w1 * w1 * L1 * cos(a1 - a2));
	float denominator = L1 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));
	upperPendulum.angularAcceleration = (num1 + num2 + num3) / denominator;

	num1 = 2 * sin(a1 - a2);
	num2 = w1 * w1 * L1 * (m1 + m2);
	num3 = G * (m1 + m2) * cos(a1) + w2 * w2 * L2 * m2 * cos(a1 - a2);
	denominator = L2 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));
	angularAcceleration = (num1 * (num2 + num3)) / denominator;
}

void PendulumSection::drawArm(sf::RenderWindow& window) { window.draw(arm); }
void PendulumSection::drawBody(sf::RenderWindow& window) { window.draw(body); }