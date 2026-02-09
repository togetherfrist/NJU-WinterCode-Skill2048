#include <vector>
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>

class GUI{
private:
    enum GUIstate{menu, ingame, statesCount};
    static GUIstate state;
    static sf::Font font;
    struct grid;
    static std::vector<std::vector<grid>> board;
    static void fixTextOrigin(sf::Text &text);
    struct button;
    static std::vector<button> buttons[statesCount];
    static void addButton(float x, float y, float w, float h, GUIstate displayState, const wchar_t *content, std::function<void()> onclick);
    static sf::Vector2f getMousePosition(sf::RenderWindow &window);
    static void addRectangleEdges(sf::Vertex *line, float x_left, float x_right, float y_up, float y_down, sf::Color color);
    static void addRectangleInterior(sf::Vertex *triangle, float x_left, float x_right, float y_up, float y_down, sf::Color color);
    static void drawButtons(sf::RenderWindow &window);
    static void click(sf::RenderWindow &window);
    static void drawBoard(sf::RenderWindow &window);
    static std::string getBitString(int n);
public:
    static void openGUI();
    static void startGame(int basic_n);
    static void generateNumber(int row, int col, int number);
    static void move(int r, int c, int r_to, int c_to);
};
