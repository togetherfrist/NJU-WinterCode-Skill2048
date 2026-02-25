#include "GUI.h"
#include "Rectangle.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <string>
#include <functional>

#pragma once

class Skill{
private:
    constexpr static float w = 100;
    float x, y;
    int availableTimes;
    int remainingTimes; 
    std::string path;
    sf::Texture texture;
    sf::Sprite sprite;
    std::wstring description;
    sf::Text text;
    std::function<void()> onclick;
    std::function<bool(int r, int c)> canSelect;
    std::function<void(int r, int c)> onSelectGrid;
    static std::vector<Skill> skills;
    static int usedSkillIndex;
    static void updateSkillText(Skill &skill);
    static void reduceSkillTimes();
public:
    Skill(float x, float y, int availableTimes, std::string path, std::wstring description, std::function<void()> onclick, std::function<bool(int r, int c)> canSelect, std::function<void(int, int)> onSelectGrid);
    static void init();
    static void reset();
    static void draw(sf::RenderWindow &window);
    bool contains(float posX, float posY);
    static void click(float x, float y);
    static sf::Color getSelectingColor(int r, int c);
    static void selectGrid(int r, int c);
};