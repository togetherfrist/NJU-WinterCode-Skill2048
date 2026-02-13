#include <functional>
#include <SFML/Graphics.hpp>
#include <string>
#include <GUI.h>
#include <vector>

#pragma once

class GUIDataDisplay{
private:
    float xLeft, xRight, yUp, yDown;
    std::function<std::string()> getData;
    sf::Text content;
    GUI::GUIState displayState;
    static std::vector<GUIDataDisplay> datas;
public:
    GUIDataDisplay(float xLeft, float xRight, float yUp, float yDown, GUI::GUIState displayState, std::function<std::string()> getData);
    void display(sf::RenderWindow &window);
    static void addDisplay(float xLeft, float xRight, float yUp, float yDown, GUI::GUIState displayState, std::function<std::string()> getData);
    static void displayDatas(sf::RenderWindow &window);
};