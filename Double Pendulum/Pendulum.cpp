#include "Pendulum.hpp"

Pendulum::Pendulum(int mass, float angle, int length, int x, int y) { start(mass, angle, length, x, y); }

int Pendulum::getSize() { return pendulum.size(); }

sf::Color Pendulum::getArmColor() { return armColor; }

void Pendulum::start(int mass, float angle, int length, int x, int y)
{
    PendulumSection currPendulum(mass, angle, length, x, y);
    pendulum.push_back(currPendulum);
    newTrail();
}

void Pendulum::add(int mass, float angle, int length)
{
    PendulumSection currPendulum(mass, angle, length, pendulum[pendulum.size() - 1]);
    pendulum.push_back(currPendulum);
    newTrail();
}

void Pendulum::calculatePendulum()
{
    for (int i = pendulum.size(); i > 1; i--) pendulum[i - 1].calculatePendulum(pendulum[i - 2]);
}

void Pendulum::updatePos(float simulationSpeed)
{

    pendulum[0].updatePos(simulationSpeed, pendulum[0], true);
    for (int i = 1; i < pendulum.size(); i++) pendulum[i].updatePos(simulationSpeed, pendulum[i - 1], false);
}

void Pendulum::newTrail()
{
    std::vector<sf::Vertex> singleTrail;
    trails.push_back(singleTrail);
}

void Pendulum::updateTrails(std::string trailType)
{
    this->trailType = trailType;

    if (trailType == "Fade" || trailType == "Always")
    {
        while (trails.size() < pendulum.size()) newTrail();

        float maxTrailSize = 350;
        sf::Vertex tempVertexHolder;

        for (int i = 0; i < pendulum.size(); i++)
        {
            tempVertexHolder.position = pendulum[i].getEndingPosition();
            if (currStyle == "1")
            {
                if (i == 0) tempVertexHolder.color = sf::Color(255, 194, 253);
                else if (i == 1) tempVertexHolder.color = sf::Color(194, 252, 255);
                else tempVertexHolder.color = sf::Color(203, 245, 184);
            }
            else if (currStyle == "2") tempVertexHolder.color = pendulum[i].body.getFillColor();
            else tempVertexHolder.color = pendulum[i].arm.getFillColor();

            trails[i].push_back(tempVertexHolder);

            if (trailType == "Fade")
            {
                if (trails[i].size() > maxTrailSize) trails[i].erase(trails[i].begin(), trails[i].begin() + (trails[i].size() - (maxTrailSize - 25)));
            }

            float opacityCoefficient = 255.0f / trails[i].size();
            for (int j = 0; j < trails[i].size(); j++) trails[i][j].color.a = j * opacityCoefficient;
        }
    }
    else trails.clear();
}

void Pendulum::updateTrailColors()
{
    if (trailType == "Always")
    {
        for (int i = 0; i < trails.size(); i++)
            for (int j = 0; j < trails[i].size(); j++)
            {
                if (currStyle == "1")
                {
                    if (i == 0) trails[i][j].color = sf::Color(255, 194, 253);
                    else if (i == 1) trails[i][j].color = sf::Color(194, 252, 255);
                    else trails[i][j].color = sf::Color(203, 245, 184);
                }
                else if (currStyle == "2") trails[i][j].color = pendulum[i].body.getFillColor();
                else trails[i][j].color = pendulum[i].arm.getFillColor();

                float opacityCoefficient = 255.0f / trails[i].size();
                trails[i][j].color.a = j * opacityCoefficient;
            }
    }
}

void Pendulum::styleOne()
{
    currStyle = "1";
    for (int i = 0; i < pendulum.size(); i++)
    {

        pendulum[i].arm.setFillColor(sf::Color(156, 156, 156));
        pendulum[i].arm.setOutlineColor(sf::Color::Black);

        pendulum[i].setArmLength(400);
        pendulum[i].setMass(133.33);

        pendulum[i].arm.setSize(sf::Vector2f(10, pendulum[i].getArmLength()));

        pendulum[i].body.setFillColor(sf::Color(219, 219, 219));
        pendulum[i].body.setOutlineColor(sf::Color::Black);
    }
    updateTrailColors();
}

void Pendulum::styleTwo()
{
    currStyle = "2";
    for (int i = 0; i < pendulum.size(); i++)
    {
        pendulum[i].arm.setOutlineColor(sf::Color::Transparent);
        pendulum[i].arm.setFillColor(sf::Color::Transparent);

        pendulum[i].setArmLength(400);
        pendulum[i].setMass(133.33);

        pendulum[i].arm.setSize(sf::Vector2f(10, pendulum[i].getArmLength()));
    }

    pendulum[0].body.setFillColor(sf::Color::Transparent);
    pendulum[0].body.setOutlineColor(sf::Color::Transparent);
    for (int i = 1; i < pendulum.size(); i++)
    {
        pendulum[i].body.setFillColor(getRandomColor());
        pendulum[i].body.setOutlineColor(sf::Color::Black);
    }
    updateTrailColors();
}

void Pendulum::styleThree(sf::Color newArmColor)
{
    currStyle = "3";
    armColor = newArmColor;
    for (int i = 0; i < pendulum.size(); i++)
    {
        pendulum[i].body.setFillColor(sf::Color::Transparent);
        pendulum[i].body.setOutlineColor(sf::Color::Transparent);
        pendulum[i].arm.setFillColor(armColor);
        pendulum[i].arm.setOutlineColor(armColor);

        pendulum[i].setArmLength(441);
        pendulum[i].setMass(147);

        pendulum[i].arm.setSize(sf::Vector2f(1, pendulum[i].getArmLength()));
    }
    updateTrailColors();
}

void Pendulum::drawArm(sf::RenderWindow& window)
{
    for (int i = 0; i < pendulum.size(); i++) pendulum[i].drawArm(window);
}

void Pendulum::drawBody(sf::RenderWindow& window)
{
    for (int i = 0; i < pendulum.size(); i++) pendulum[i].drawBody(window);
}

void Pendulum::drawTrail(sf::RenderWindow& window)
{
    if (trailType == "Always" || trailType == "Fade")
        for (int i = 0; i < trails.size(); i++)
            if (trails[i].size() > 0)
                window.draw(&trails[i][0], trails[i].size(), sf::LineStrip);

}

sf::Color Pendulum::getRandomColor()
{
    float H = rand() % 335 + 1;
    return HSVtoRGB(H, 50, 100);
}

sf::Color Pendulum::HSVtoRGB(float H, float S, float V)
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