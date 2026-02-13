#include "GUIDataDisplay.h"
#include "GUI.h"
#include <vector>
#include <iostream>

std::vector<GUIDataDisplay> GUIDataDisplay::datas;

GUIDataDisplay::GUIDataDisplay(float xLeft, float xRight, float yUp, float yDown, GUI::GUIState displayState, std::function<std::string()> getData):
xLeft(xLeft), xRight(xRight), yUp(yUp), yDown(yDown), displayState(displayState), getData(getData), content(GUI::font){
    this->content.setFillColor(sf::Color::Black);
    this->content.setCharacterSize(42);
}

void GUIDataDisplay::display(sf::RenderWindow &window){
    if(displayState != GUI::getState()) return;
    content.setString(getData());
    GUI::fixTextOrigin(content);
    content.setPosition({(xLeft + xRight) / 2, (yUp + yDown) / 2});
    window.draw(content);
}

void GUIDataDisplay::addDisplay(float xLeft, float xRight, float yUp, float yDown, GUI::GUIState displayState, std::function<std::string()> getData){
    datas.push_back(GUIDataDisplay(xLeft, xRight, yUp, yDown, displayState, getData));
}

void GUIDataDisplay::displayDatas(sf::RenderWindow &window){
    for(auto data: datas){
        data.display(window);
    }
}
