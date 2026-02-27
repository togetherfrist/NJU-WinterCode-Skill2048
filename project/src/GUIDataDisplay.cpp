#include "GUIDataDisplay.h"
#include "GUI.h"
#include "Game.h"
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

void GUIDataDisplay::init(){
    addDisplay(750, 200, GUI::GUIState::ingame, [](){
        std::wstringstream wss;
        wss << L"分数：" << Game::getScore();
        return wss.str();
    });
    addDisplay(1100, 200, GUI::GUIState::ingame, [](){
        long long seconds = (GUI::getTime() - GUI::gameStartTime) / 1000;
        std::wstringstream str;
        str << L"时长: " << std::setw(2) << std::setfill(L'0') << (seconds / 60);
        str << L":" << std::setw(2) << std::setfill(L'0') << (seconds % 60);
        return str.str();
    });
    GUIDataDisplay::addDisplay(960, 200, GUI::GUIState::end, [](){
        return L"游戏结束";
    });
    GUIDataDisplay::addDisplay(400, 300, GUI::GUIState::end, [](){
        int seconds = GUI::gameDuration / 1000;
        std::wstringstream str;
        str << L"本局用时: " << std::setw(2) << std::setfill(L'0') << (seconds / 60);
        str << L":" << std::setw(2) << std::setfill(L'0') << (seconds % 60);
        return str.str();
    });
    GUIDataDisplay::addDisplay(400, 500, GUI::GUIState::end, [](){
        std::wstringstream wss;
        wss << L"本局得分：" << Game::getScore();
        return wss.str();
    });
    GUIDataDisplay::addDisplay(400, 700, GUI::GUIState::end, [](){
        std::wstringstream wss;
        wss << L"最大方块：" << GUI::gameMaxTile;
        return wss.str();
    });
}

void GUIDataDisplay::addDisplay(float x, float y, GUI::GUIState displayState, std::function<std::wstring()> getData){
    datas.push_back(GUIDataDisplay(x, y, displayState, getData));
}

void GUIDataDisplay::displayDatas(sf::RenderWindow &window){
    for(auto data: datas){
        data.display(window);
    }
}
