#include <SFML/Graphics.hpp>
#include <set>
#include <string>

#pragma once

class Leaderboard{
private:
    struct record;
    static std::multiset<record> leaderboard;
    static sf::Text text;
    static std::string getString();
    static std::string getTimeString(long long time);
    static void updateText();
public:
    static void update(int score, long long time);
    static void draw(sf::RenderWindow &window);
    static void save();
    static void load();
};

