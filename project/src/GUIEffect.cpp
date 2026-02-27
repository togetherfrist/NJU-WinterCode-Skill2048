#include "GUI.h"
#include "GUIEffect.h"
#include <SFML/Graphics.hpp>
#include <iostream>

std::vector<std::shared_ptr<GUIEffect>> GUIEffect::effects;

GUIEffect::GUIEffect(int duration, int lineVertexNum, int triangleVertexNum, std::function<void()> onEnd):
startTime(GUI::getTime()), duration(duration), lineVertexNum(lineVertexNum), triangleVertexNum(triangleVertexNum), onEnd(onEnd) {
    return;
}

bool GUIEffect::shouldRemain(){
    return GUI::getTime() - startTime < duration;
}

void GUIEffect::draw(sf::RenderWindow &window){
    int lineVertexNum = 0;
    int triangleVertexNum = 0;
    for(auto &effect : effects){
        lineVertexNum += effect->lineVertexNum;
        triangleVertexNum += effect->triangleVertexNum;
    }
    sf::VertexArray line(sf::PrimitiveType::Lines, lineVertexNum);
    sf::Vertex* lineDraw = &line[0];
    sf::VertexArray triangle(sf::PrimitiveType::Triangles, triangleVertexNum);
    sf::Vertex* triangleDraw = &triangle[0];
    for(auto &effect : effects){
        effect->drawTo(lineDraw, triangleDraw);
    }
    window.draw(triangle);
    window.draw(line);
    for(auto &effect : effects){
        effect->drawToWindow(window);
    }
    std::vector<std::shared_ptr<GUIEffect>> remainingEffects;
    for(int i = 0; i < effects.size(); i++){
        auto &effect = effects[i];
        if(effect->shouldRemain()){
            remainingEffects.push_back(effect);
        }else{
            effect->onEnd();
        }
    }
    effects = remainingEffects;
}

void GUIEffect::addEffect(std::shared_ptr<GUIEffect> effect){
    effects.push_back(effect);
}

void GUIEffect::updateEffects(){
    std::vector<std::shared_ptr<GUIEffect>> remainingEffects;
    for(auto effect : effects){
        if(effect->endOnUpdate()){
            effect->onEnd();
        }else{
            remainingEffects.push_back(effect);
        }
    }
    effects = remainingEffects;
}

LightEffect::LightEffect(float x, float y, float r)
:GUIEffect(600, 4*8, 4*6, [](){}), x(x), y(y), r(r) {
    return;
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

MoveEffect::MoveEffect(int r, int c, int r_to, int c_to, int number, int endNumber, std::function<void()> onEnd)
:GUIEffect(100, 8, 6, onEnd), r(r), c(c), r_to(r_to), c_to(c_to), number(number), endNumber(endNumber), content(GUI::font){
    this->content.setFillColor(sf::Color::Black);
    this->content.setString(GUI::getBitString(number));
    this->content.setCharacterSize(GUI::getChatacterSize());
    GUI::fixTextOrigin(this->content);
}

void MoveEffect::drawTo(sf::Vertex *&line, sf::Vertex *&triangle){
    sf::Color innerColor = GUI::getGridColor(number);
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

putNumberEffect::putNumberEffect(int r, int c, int number)
:GUIEffect(100, 0, 0, [r, c, number](){GUI::putNumberWithEffect(r, c, number);}), r(r), c(c), number(number) {
    return;
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

explosionEffect::explosionEffect(int r, int c)
:GUIEffect(300, 0, 6, [](){}), r(r), c(c) {
    return;
}

void explosionEffect::drawTo(sf::Vertex *&line, sf::Vertex *&triangle){
    int elapsed = GUI::getTime() - startTime;
    float t = (float)elapsed / duration;
    sf::Color color = sf::Color(255, 0, 0, 100*(1-t));
    auto [x, y] = GUI::getGridTopLeft(r, c);
    float girdWidth = GUI::getGridWidth();
    float edgeWidth = GUI::getGridEdgeWidth();
    GUI::drawRectangleInterior(triangle, x+edgeWidth, x+girdWidth, y+edgeWidth, y+girdWidth, color);
}

void explosionEffect::drawToWindow(sf::RenderWindow &window){
    return;
}

bool explosionEffect::endOnUpdate(){
    return true;
}
