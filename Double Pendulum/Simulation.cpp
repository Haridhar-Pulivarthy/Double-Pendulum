#include "Simulation.hpp"


Simulation::Simulation() : window(sf::VideoMode(1000, 1000), "Double Pendulum", sf::Style::Fullscreen, settings) {}

void Simulation::initialize()
{
    settings.antialiasingLevel = 8; // Ensures smooth rotation and less pixelization

    pendulumHolder.setRadius(10);
    pendulumHolder.setOrigin(pendulumHolder.getRadius(), pendulumHolder.getRadius());
    pendulumHolder.setFillColor(sf::Color::Black);
    pendulumHolder.setOutlineColor(sf::Color::White);
    pendulumHolder.setOutlineThickness(2);
    pendulumHolder.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));

    bgColor = sf::Color::Black;
}

void Simulation::runSimulation()
{
    initialize();
    while (window.isOpen())
    {
        checkForInput();
        if (!isPaused) updatePendulums();
        updateText();
        window.clear(bgColor);
        drawObjects();
        window.display();
    }
}

void Simulation::checkForInput()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)) window.close();
        else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::A)
        {
            if (currStyle == "3") addMultiplePendulums(0.000001, 1000);
            else addSinglePendulum(-1, -1);
        }
        else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::S)
        {
            if (currStyle == "3") addMultipleSections(0.000001);
            else addSingleSection(-1);
        }
        else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Up)
            if (currSimulationSpeed <= 3.9)
                changeSimulationSpeed(1);
        else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Down)
            if (currSimulationSpeed > .1)
                changeSimulationSpeed(-1);
        else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
            togglePause();
        else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Num1)
            styleOne();
        else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Num2)
            styleTwo();
        else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Num3)
            styleThree();
        else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::T)
            changeTrailType();
        else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::C)
            clear();
        else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::LShift)
            toggleControls();
    }
}

void Simulation::addSinglePendulum(float angle1, float angle2)
{
    //if one of the passed arguments is -1, give first section either angle of 270 or 90
    if (angle1 == -1)
    {
        int randPick = rand() % 2 + 1;
        if (randPick == 1) angle1 = 90;
        else angle1 = 270;
    }
    if (angle2 == -1) angle2 = rand() % 360 + 0;

    Pendulum firstSection(133.33, angle1, 400, window.getSize().x / 2, window.getSize().y / 2);
    pendulums.push_back(firstSection);

    if (currStyle == "2") pendulums[pendulums.size() - 1].styleTwo();
    else if (currStyle == "3")
    {
        if (pendulums.size() > 1) pendulums[pendulums.size() - 1].styleThree(pendulums[pendulums.size() - 2].getArmColor());
        else pendulums[pendulums.size() - 1].styleThree(getRandomColor());
    }

    if (angle1 == 90) addSingleSection(angle1 + angle2);
    else addSingleSection(angle1 - angle2);
}

void Simulation::addSingleSection(float angle)
{
    if (pendulums.size() > 0)
    {
        if (angle == -1) angle = rand() % 360 + 0;
        pendulums[pendulums.size() - 1].add(133.33, angle, 400);
        if (currStyle == "2") pendulums[pendulums.size() - 1].styleTwo();
        else if (currStyle == "3") pendulums[pendulums.size() - 1].styleThree(pendulums[pendulums.size() - 1].getArmColor());
    }
}

void Simulation::addMultiplePendulums(float angleDiff, float numPendulums)
{
    if (currStyle == "3")
    {
        float angleIncRad = angleDiff / numPendulums;
        float angleIncDeg = angleIncRad * (180 / PI);

        float startingAngle = rand() % 170 + 100;
        float secondAngleIncrement = rand() % 45 + 0;

        for (int i = 0; i < numPendulums; i++)
        {
            addSinglePendulum(startingAngle, 0);
            startingAngle += angleIncDeg;
        }
        styleThree();
    }
}

void Simulation::addMultipleSections(float angleDiff)
{
    float numPendulums = pendulums.size();
    float angleIncRad = angleDiff / numPendulums;
    float angleIncDeg = angleIncRad * (180 / PI);

    if (numPendulums > 0 && currStyle == "3")
    {
        float startingAngle = rand() % 45 + 0;
        for (int i = 0; i < numPendulums; i++)
        {
            pendulums[i].add(100, startingAngle, 300);
            startingAngle += angleIncDeg;
        }
        styleThree();
    }
}

void Simulation::styleOne()
{
    currStyle = "1";
    for (int i = 0; i < pendulums.size(); i++) pendulums[i].styleOne();
}

void Simulation::styleTwo()
{
    currStyle = "2";
    for (int i = 0; i < pendulums.size(); i++) pendulums[i].styleTwo();
}

void Simulation::styleThree()
{
    currStyle = "3";
    int colorPicker = rand() % 2 + 1;
    if (colorPicker == 1) styleThreeColorHelpOne();
    else styleThreeColorHelpTwo();
}

int Simulation::styleThreeTransparencyHelp()
{
    int transparency;
    if (style3TransparencyHelp == 1)
    {
        transparency = rand() % 10 + 1;
        style3TransparencyHelp += 1;
    }
    else if (style3TransparencyHelp == 2)
    {
        transparency = rand() % 20 + 60;
        style3TransparencyHelp += 1;
    }
    else
    {
        transparency = 255;
        style3TransparencyHelp = 1;
    }
    return transparency;
}

void Simulation::styleThreeColorHelpOne()
{
    sf::Color newColor;
    int transparency = styleThreeTransparencyHelp();

    int colorPicker = rand() % 360 + 0;
    float H = colorPicker;
    float S = 0;
    float V = 100;
    float amtOfIncS = (100.0f / pendulums.size()) * 2.0f;
    float amtOfIncV = (100.0f / pendulums.size()) * 2.0f;
    for (int i = pendulums.size(); i > 0; i--)
    {
        if (S < 100)
        {
            S += amtOfIncS;
            if (S > 100) S = 100;
        }
        else
        {
            V -= amtOfIncV;
            if (V < 15) V = 15;
        }
        newColor = HSVtoRGB(H, S, V);
        newColor.a = transparency;
        pendulums[i - 1].styleThree(newColor);
    }
}

void Simulation::styleThreeColorHelpTwo()
{
    sf::Color newColor;
    int transparency = styleThreeTransparencyHelp();

    int colorPicker = rand() % 360 + 0;
    float desiredColor = colorPicker;
    float H = desiredColor;
    float S = 0;
    float V = 100;
    bool check1 = false, check2 = false, check3 = false, check4 = false;

    float check1Inc = (25.0f / pendulums.size()) * 15.0f;
    float check2Inc = (100.0f / pendulums.size()) * 4.0f;
    float check3Inc = (35.0f / pendulums.size()) * 3.0f;
    float check4Inc = (75.0f / pendulums.size()) * 1.538f;

    for (int i = pendulums.size(); i > 0; i--)
    {
        if (!check1)
        {
            V -= check1Inc;
            if (V <= 75) check1 = true;
        }
        else if (!check2)
        {
            S += check2Inc;
            if (S >= 100) check2 = true;
        }
        else if (!check3)
        {
            H += check3Inc;
            if (H >= (desiredColor + 55)) check3 = true;
        }
        else V -= check4Inc;

        newColor = HSVtoRGB(H, S, V);
        newColor.a = transparency;
        pendulums[i - 1].styleThree(newColor);
    }
}

void Simulation::clear() { pendulums.clear(); }
void Simulation::changeSimulationSpeed(float upOrDown) { currSimulationSpeed += upOrDown * .1f; }
void Simulation::changeTrailType()
{
    if (trailType == "Fade") trailType = "Always";
    else if (trailType == "Always") trailType = "None";
    else trailType = "Fade";
    trailTypeText.setString("Trail Type: " + trailType);
}
void Simulation::togglePause()
{
    if (isPaused)
    {
        isPaused = false;
        clock.restart();
    }
    else isPaused = true;
}

void Simulation::updatePendulums()
{
    for (int i = 0; i < pendulums.size(); i++) pendulums[i].calculatePendulum();
    for (int i = 0; i < pendulums.size(); i++) pendulums[i].updatePos(clock.getElapsedTime().asSeconds() * currSimulationSpeed * 13);
    clock.restart();

    if (trailClock.getElapsedTime().asMilliseconds() > 0.1f)
    {
        for (int i = 0; i < pendulums.size(); i++) pendulums[i].updateTrails(trailType);
        trailClock.restart();
    }

}

void Simulation::updateText()
{
    if (isPaused) simSpeedText.setString("Simulation Speed: 0.0x");
    else
    {
        std::string simSpeedString = std::to_string(currSimulationSpeed);
        std::string rounded = simSpeedString.substr(0, simSpeedString.find(".") + 3);
        simSpeedText.setString("Simulation Speed: " + rounded + "x");
    }
    currStyleText.setString("Current Style: " + currStyle);
}

void Simulation::drawObjects()
{
    for (int i = 0; i < pendulums.size(); i++) pendulums[i].drawTrail(window);
    for (int i = 0; i < pendulums.size(); i++) pendulums[i].drawArm(window);
    window.draw(pendulumHolder);
    for (int i = 0; i < pendulums.size(); i++) pendulums[i].drawBody(window);

    window.draw(currStyleText);
    window.draw(simSpeedText);
    window.draw(trailTypeText);
    window.draw(controlsText);
}

sf::Color Simulation::getRandomColor()
{
    int picker = rand() % 3 + 1;
    int secondPicker = rand() % 2 + 1;
    float r, g, b;

    if (picker == 1)
    {
        if (secondPicker == 1)
        {
            r = 52;
            g = 235;
            b = rand() % 234 + 51;
        }
        else
        {
            r = 52;
            g = rand() % 234 + 51;
            b = 235;
        }
    }
    else if (picker == 2)
    {
        if (secondPicker == 1)
        {
            r = 235;
            g = 52;
            b = rand() % 234 + 51;
        }
        else
        {
            r = rand() % 234 + 51;
            g = 52;
            b = 235;
        }
    }
    else
    {
        if (secondPicker == 1)
        {
            r = rand() % 234 + 51;
            g = 235;
            b = 52;
        }
        else
        {
            r = 235;
            g = rand() % 234 + 51;
            b = 52;
        }
    }

    return sf::Color(r, g, b);
}

sf::Color Simulation::HSVtoRGB(float H, float S, float V)
{
    float s = S / 100;
    float v = V / 100;
    float C = s * v;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float m = v - C;
    float r, g, b;
    if (H >= 0 && H < 60) r = C, g = X, b = 0;
    else if (H >= 60 && H < 120) r = X, g = C, b = 0;
    else if (H >= 120 && H < 180) r = 0, g = C, b = X;
    else if (H >= 180 && H < 240) r = 0, g = X, b = C;
    else if (H >= 240 && H < 300) r = X, g = 0, b = C;
    else r = C, g = 0, b = X;

    float R = (r + m) * 255;
    float G = (g + m) * 255;
    float B = (b + m) * 255;

    return sf::Color(R, G, B);
}