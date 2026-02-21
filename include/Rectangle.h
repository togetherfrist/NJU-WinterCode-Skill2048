#include "GUI.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

class Rectangle{
private:
    static std::vector<Rectangle> rectangles[GUI::statesCount];
protected:
    float x, y, w, h;
    sf::Text content;
    sf::Color color;
    sf::Color hoverColor;
    std::function<void()> onclick;
public:
    Rectangle(float x, float y, float w, float h, const wchar_t *content, sf::Color color);
    bool contains(float posX, float posY);
    static void drawAll(sf::RenderWindow &window);
    static void click(sf::RenderWindow &window);
    static void addRectangle(GUI::GUIState state, float x, float y, float w, float h, const wchar_t *content, sf::Color color);
    static void addButton(GUI::GUIState state, float x, float y, float w, float h, const wchar_t *content, sf::Color color, sf::Color hoverColor, std::function<void()> onclick);
};

class Button : public Rectangle{
public:
    Button(float x, float y, float w, float h, const wchar_t *content, sf::Color color, sf::Color hoverColor, std::function<void()> onclick);
};