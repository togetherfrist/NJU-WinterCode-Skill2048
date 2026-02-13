#include "GUIEffect.h"
#include <vector>
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>

#pragma once

class GUI{
public:
    enum GUIState{menu, ingame, statesCount};
private:
    static constexpr int window_w = 1920, window_h = 1080;
    static int gameStartTime;
    static GUIState state;
    static sf::Clock clock;
    struct grid;
    static std::vector<std::vector<grid>> board;
    struct button;
    static std::vector<button> buttons[statesCount];
    static void addButton(float x, float y, float w, float h, GUIState displayState, const wchar_t *content, std::function<void()> onclick);
    static sf::Vector2f getMousePosition(sf::RenderWindow &window);
    static void drawButtons(sf::RenderWindow &window);
    static void click(sf::RenderWindow &window);
    static constexpr float GridsToLeft = 700;
    static constexpr float GridsToTop = 300;
    static void drawBoard(sf::RenderWindow &window);
    static std::vector<std::shared_ptr<GUIEffect>> effects;
    static void drawEffects(sf::RenderWindow &window);
public:
    static sf::Font font;
    static void fixTextOrigin(sf::Text &text);
    static GUI::GUIState getState();
    static void openGUI();
    static void addRectangleEdges(sf::Vertex* &line, float x_left, float x_right, float y_up, float y_down, sf::Color color);
    static void addRectangleInterior(sf::Vertex* &triangle, float x_left, float x_right, float y_up, float y_down, sf::Color color);
    static void addRectangle(sf::Vertex* &line, sf::Vertex* &triangle, float x_left, float x_right, float y_up, float y_down, sf::Color edgeColor, sf::Color interiorColor);
    static int getTime();
    static std::string getBitString(int n);
    static float getGridWidth();
    static float getGridEdgeWidth();
    static sf::Vector2f getGridTopLeft(int r, int c);
    static sf::Color getNumberColor(int number);
    static void startGame(int basic_n);
    static void generateNumber(int row, int col, int number);
    static void putNumberWithEffect(int row, int col, int number);
    static void putNumber(int row, int col, int number);
    static void move(int r, int c, int r_to, int c_to, int number, int endNumber);
    static void updateEffects();
    static void addMoveEffect(int r, int c, int r_to, int c_to, int number, int endNumber, std::function<void()> onEnd);
};
