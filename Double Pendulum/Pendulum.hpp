#pragma once

#include "PendulumSegment.hpp"
#include <vector>

class Pendulum
{
private:
	std::vector<PendulumSection> pendulum;
	std::vector<std::vector<sf::Vertex>> trails;
	sf::Color armColor;

	bool isTrails;

	std::string currStyle = "1";
	std::string trailType = "Fade";

public:

	Pendulum(int mass, float angle, int length, int x, int y);

	int getSize();
	sf::Color getArmColor();

	void start(int mass, float angle, int length, int x, int y);
	void add(int mass, float angle, int length);

	void calculatePendulum();
	void updatePos(float simulationSpeed);

	void newTrail();
	void updateTrails(std::string trailType);
	void updateTrailColors();

	void styleOne();
	void styleTwo();
	void styleThree(sf::Color newArmColor);

	void drawArm(sf::RenderWindow& window);
	void drawBody(sf::RenderWindow& window);
	void drawTrail(sf::RenderWindow& window);

	sf::Color getRandomColor();
	sf::Color HSVtoRGB(float H, float S, float V);
};