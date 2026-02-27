#include <functional>
#include <SFML/Graphics.hpp>
#include <string>
#include <GUI.h>
#include <vector>

#pragma once

class GUIDataDisplay{
private:
    float x, y;
    std::function<std::wstring()> getData;
    sf::Text content;
    GUI::GUIState displayState;
    static std::vector<GUIDataDisplay> datas;
public:
    GUIDataDisplay(float x, float y, GUI::GUIState displayState, std::function<std::wstring()> getData);
    void display(sf::RenderWindow &window);
    static void init();
    static void addDisplay(float x, float y, GUI::GUIState displayState, std::function<std::wstring()> getData);
    static void displayDatas(sf::RenderWindow &window);
};