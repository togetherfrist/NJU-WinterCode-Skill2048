#include "GUI.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <string>

class Skill{
private:
    constexpr static float w = 100;
    float x, y;
    std::string path;
    sf::Texture texture;
    sf::Sprite sprite;
    static std::list<Skill> skills;
public:
    Skill(float x, float y, std::string path);
    static void init();
    static void draw(sf::RenderWindow &window);
};