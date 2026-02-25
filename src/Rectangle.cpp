#include "Rectangle.h"

void doNothing(){}

std::vector<Rectangle> Rectangle::rectangles[GUI::statesCount];

Rectangle::Rectangle(float x, float y, float w, float h, std::wstring content, sf::Color color)
: x(x), y(y), w(w), h(h), content(GUI::font, content), color(color), hoverColor(color), onclick(doNothing) {
    this->content.setCharacterSize(36.0f);
    this->content.setFillColor(sf::Color::Black);
    GUI::fixTextOrigin(this->content);
    this->content.setPosition({x + w/2.0f, y + h/2.0f});
}

bool Rectangle::contains(float posX, float posY){
    return x <= posX && posX <= x + w && y <= posY && posY <= y + h;
}

void Rectangle::draw(sf::RenderWindow &window){
    auto displayRectangles = &rectangles[GUI::getState()][0];
    int n = rectangles[GUI::getState()].size();
    sf::VertexArray triangle(sf::PrimitiveType::Triangles, n * 6);
    sf::VertexArray line(sf::PrimitiveType::Lines, n * 8);
    std::pair<float, float> mousePosition = GUI::getMousePosition(window);
    auto [mouseX, mouseY] = mousePosition;
    sf::Vertex *triangleDraw = &triangle[0];
    sf::Vertex *lineDraw = &line[0];
    for(int i = 0; i < n; i++){
        float x = displayRectangles[i].x;
        float y = displayRectangles[i].y;
        float w = displayRectangles[i].w;
        float h = displayRectangles[i].h;
        auto lineColor = sf::Color::Black;
        auto interiorColor = displayRectangles[i].contains(mouseX, mouseY) ? displayRectangles[i].hoverColor : displayRectangles[i].color;
        GUI::drawRectangle(lineDraw, triangleDraw, x, x+w, y, y+h, lineColor, interiorColor);
    }
    window.draw(triangle);
    window.draw(line);
    for(int i = 0; i < n; i++){
        if(displayRectangles[i].content.getString() == "") continue;
        window.draw(displayRectangles[i].content);
    }
}

void Rectangle::click(float x, float y){
    for(auto &rectangle: rectangles[GUI::getState()]){
        if(rectangle.contains(x, y)){
            rectangle.onclick();
        }
    }
}

void Rectangle::addRectangle(GUI::GUIState state, float x, float y, float w, float h, const wchar_t *content, sf::Color color){
    rectangles[state].push_back(Rectangle(x, y, w, h, content, color));
}

void Rectangle::addButton(GUI::GUIState state, float x, float y, float w, float h, const wchar_t *content, sf::Color color, sf::Color hoverColor, std::function<void()> onclick){
    rectangles[state].push_back(Button(x, y, w, h, content, color, hoverColor, onclick));
}

Button::Button(float x, float y, float w, float h, std::wstring content, sf::Color color, sf::Color hoverColor, std::function<void()> onclick)
: Rectangle(x, y, w, h, content, color){
    this->hoverColor = hoverColor;
    this->onclick = onclick;
}

