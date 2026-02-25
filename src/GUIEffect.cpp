#include "GUI.h"
#include "GUIEffect.h"
#include <SFML/Graphics.hpp>
#include <iostream>

bool GUIEffect::shouldRemain(){
    return GUI::getTime() - startTime < duration;
}

LightEffect::LightEffect(float x, float y, float r){
    this->startTime = GUI::getTime();
    this->duration = 600;
    this->x = x;
    this->y = y;
    this->r = r;
    this->lineVertexNum = 4*8;
    this->triangleVertexNum = 4*6;
    this->onEnd = [](){return;};
}

void LightEffect::drawTo(sf::Vertex* &line, sf::Vertex* &triangle){
    constexpr float rec_w = 8;
    constexpr float rec_h = 20;
    constexpr float disFromBottom = 0;
    int elapsed = GUI::getTime() - startTime;
    float t = (float)elapsed / duration;
    float hdown0 = disFromBottom + std::max(t-0.5f, 0.0f) * 2 * rec_h;
    float hup0 = disFromBottom + std::min(t, 0.5f) * 2 * rec_h;
    sf::Color innerColor = sf::Color(255, 201, 14);
    sf::Color outerColor = sf::Color(0, 0, 0);
    GUI::drawRectangle(line, triangle, x-rec_w/2, x+rec_w/2, y-r-hup0, y-r-hdown0, outerColor, innerColor);    
    GUI::drawRectangle(line, triangle, x-rec_w/2, x+rec_w/2, y+r+hdown0, y+r+hup0, outerColor, innerColor);    
    GUI::drawRectangle(line, triangle, x-r-hup0, x-r-hdown0, y+rec_w/2, y-rec_w/2, outerColor, innerColor);
    GUI::drawRectangle(line, triangle, x+r+hdown0, x+r+hup0, y+rec_w/2, y-rec_w/2, outerColor, innerColor);
}

void LightEffect::drawToWindow(sf::RenderWindow &window){
    return;
}

bool LightEffect::endOnUpdate(){
    return false;
}

MoveEffect::MoveEffect(int r, int c, int r_to, int c_to, int number, int endNumber, std::function<void()> onEnd):
r(r), c(c), r_to(r_to), c_to(c_to), number(number), endNumber(endNumber), content(GUI::font){
    this->content.setFillColor(sf::Color::Black);
    this->content.setString(GUI::getBitString(number));
    this->content.setCharacterSize(GUI::getChatacterSize());
    GUI::fixTextOrigin(this->content);
    this->startTime = GUI::getTime();
    this->duration = 100;
    this->lineVertexNum = 8;
    this->triangleVertexNum = 6;
    this->onEnd = onEnd;
}

void MoveEffect::drawTo(sf::Vertex *&line, sf::Vertex *&triangle){
    sf::Color innerColor = GUI::getNumberColor(number);
    sf::Color edgeColor = sf::Color::Black;
    auto [startX, startY] = GUI::getGridTopLeft(r, c);
    auto [endX, endY] = GUI::getGridTopLeft(r_to, c_to);
    float x_dis = endX - startX;
    float y_dis = endY - startY;
    int t = GUI::getTime() - startTime;
    float k = (float)t / duration;
    float grid_w = GUI::getGridWidth();
    float edge_w = GUI::getGridEdgeWidth();
    float x = startX + edge_w + x_dis * k;
    float y = startY + edge_w + y_dis * k;
    float inner_w = grid_w - edge_w;
    GUI::drawRectangle(line, triangle, x, x+inner_w, y, y+inner_w, edgeColor, innerColor);
    this->content.setPosition({x + inner_w / 2, y + inner_w / 2});
}

void MoveEffect::drawToWindow(sf::RenderWindow &window){
    window.draw(this->content);
}

bool MoveEffect::endOnUpdate(){
    return true;
}

putNumberEffect::putNumberEffect(int r, int c, int number){
    this->r = r;
    this->c = c;
    this->number = number;
    this->startTime = GUI::getTime();
    this->duration = 100;
    this->lineVertexNum = 0;
    this->triangleVertexNum = 0;
    this->onEnd = [r, c, number](){
        GUI::putNumberWithEffect(r, c, number);
    };
}

void putNumberEffect::drawTo(sf::Vertex *&line, sf::Vertex *&triangle){
    return;
}

void putNumberEffect::drawToWindow(sf::RenderWindow &window){
    return;
}

bool putNumberEffect::endOnUpdate(){
    return true;
}
