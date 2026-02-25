#include <vector>
#include <random>

#pragma once

class Game{
private:
    struct grid;
    static std::vector<std::vector<grid>> board;
    static std::mt19937 rd;
    static int score;
    static void generateNumber();
public:
    static void start();
    static void move(int dr, int dc);
    static int getScore();
    static void checkEnd();
    static int getMaxTile();
    static bool isUsed(int r, int c);
    static void addGrid(int r, int c);
    static void setGrid(int r, int c, int number, bool hasNumber, bool used);
    static void updateGUIBoard();
};