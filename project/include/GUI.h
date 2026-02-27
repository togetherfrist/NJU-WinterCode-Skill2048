#include "GUIEffect.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <functional>

#pragma once

class GUI{
public:
    enum GUIState{menu, ingame, end, statesCount};
    static const std::string projectPath;
private:
    static constexpr int window_w = 1920, window_h = 1080;
    static GUIState state;
    static sf::Clock clock;
    struct grid;
    static std::vector<std::vector<grid>> board;
    static constexpr float GridsToLeft = 600;
    static constexpr float GridsToTop = 300;
    static constexpr float GridsToBottom = 200;
    static sf::Music bgm;
    static void drawBoard(sf::RenderWindow &window);
    static std::pair<int,int> getSelectedGrid(sf::RenderWindow &window);
    static void drawSelecting(sf::RenderWindow &window);
    static float getGridWidthHW(int h, int w);
public:
    static long long gameStartTime;
    static long long gameDuration;
    static int gameMaxTile;
    static bool selectingGrid;
    static sf::Font font;
    static void fixTextOrigin(sf::Text &text);
    static GUI::GUIState getState();
    static std::pair<float, float> getMousePosition(sf::RenderWindow &window);
    static void openGUI();
    static void drawRectangleEdges(sf::Vertex* &line, float x_left, float x_right, float y_up, float y_down, sf::Color color);
    static void drawRectangleInterior(sf::Vertex* &triangle, float x_left, float x_right, float y_up, float y_down, sf::Color color);
    static void drawRectangle(sf::Vertex* &line, sf::Vertex* &triangle, float x_left, float x_right, float y_up, float y_down, sf::Color edgeColor, sf::Color interiorColor);
    static long long getTime();
    static std::string getBitString(int n);
    static float getGridWidth();
    static float getGridEdgeWidth();
    static std::pair<float, float> getGridTopLeft(int r, int c);
    static sf::Color getNumberColor(int number);
    static int getChatacterSize();
    static void setBoard(int h, int w);
    static void setGrid(int r, int c, int number, bool hasNumber, bool used);
    static void startGame(int basic_n);
    static void generateNumber(int row, int col, int number);
    static void putNumberWithEffect(int row, int col, int number);
    static void putNumber(int row, int col, int number);
    static void move(int r, int c, int r_to, int c_to, int number, int endNumber);
    static void endGame();
    static void setVolume(float volume);
};

class VolumeBar{   
private: 
    static constexpr float MaxBGMVolume = 30.0f;
    static constexpr float VolumeBarX = 1700, VolumeBarY = 960, VolumeBarW = 150, VolumeBarH = 28;
    static bool controllingVolume;
    static bool contains(float x, float y);
    static sf::Text text;
public:
    static float bgmVolume;
    static void init();
    static void draw(sf::RenderWindow &window);
    static void mousePressed(float x, float y);
    static void mouseMove(float x, float y);
    static void mouseReleased();

};
