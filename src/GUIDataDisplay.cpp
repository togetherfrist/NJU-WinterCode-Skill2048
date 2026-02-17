#include "GUIDataDisplay.h"
#include "GUI.h"
#include <vector>
#include <iostream>

std::vector<GUIDataDisplay> GUIDataDisplay::datas;

GUIDataDisplay::GUIDataDisplay(float x, float y, GUI::GUIState displayState, std::function<std::wstring()> getData):
x(x), y(y), displayState(displayState), getData(getData), content(GUI::font){
    this->content.setFillColor(sf::Color::Black);
    this->content.setCharacterSize(42);
}

void GUIDataDisplay::display(sf::RenderWindow &window){
    if(displayState != GUI::getState()) return;
    content.setString(getData());
    GUI::fixTextOrigin(content);
    content.setPosition({x, y});
    window.draw(content);
}

void GUIDataDisplay::addDisplay(float x, float y, GUI::GUIState displayState, std::function<std::wstring()> getData){
    datas.push_back(GUIDataDisplay(x, y, displayState, getData));
}

void GUIDataDisplay::displayDatas(sf::RenderWindow &window){
    for(auto data: datas){
        data.display(window);
    }
}
