#pragma once

#include "Pendulum.hpp"
#include "PendulumSegment.hpp"

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;

#define PI 3.14159265
#define LOG(x) cout << x << endl

class Simulation
{
public:
    Simulation();
    void initialize();
    void runSimulation();
    void checkForInput();

    void addSinglePendulum(float angle1, float angle2);
    void addSingleSection(float angle);
    void addMultiplePendulums(float angleDiff, float numPendulums);
    void addMultipleSections(float angleDiff);

    void styleOne();
    void styleTwo();
    void styleThree();
    int styleThreeTransparencyHelp();
    void styleThreeColorHelpOne();
    void styleThreeColorHelpTwo();

    void clear();
    void changeSimulationSpeed(float upOrDown);
    void changeTrailType();
    void togglePause();

    void updatePendulums();
    void updateText();
    void drawObjects();

    sf::Color getRandomColor();
    sf::Color HSVtoRGB(float H, float S, float V);


private:
    sf::ContextSettings settings;
    sf::RenderWindow window;
    sf::Clock clock;
    sf::Clock trailClock;

    sf::CircleShape pendulumHolder;
    sf::Font font;
    sf::Text simSpeedText;
    sf::Text currStyleText;
    sf::Text controlsText;
    sf::Text trailTypeText;
    sf::Color bgColor;

    std::vector<Pendulum> pendulums;

    float currSimulationSpeed = 1;
    bool isPaused = false;
    bool isControlsShowing = false;
    int style3TransparencyHelp = 1;

    std::string trailType = "Fade"; 
    std::string currStyle = "1";
};