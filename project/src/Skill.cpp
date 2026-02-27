#include "Skill.h"
#include "GUI.h"
#include "Game.h"
#include "GUIEffect.h"
#include <vector>
#include <string>
#include <iostream>
#include <functional>

Skill::Skill(float x, float y, int availableTimes, std::string path, std::wstring description, std::function<void()> onclick, std::function<bool(int r, int c)> canSelect, std::function<void(int r, int c)> onSelectGrid)
    : x(x), y(y), availableTimes(availableTimes), remainingTimes(availableTimes), path(path), texture(path), sprite(texture), description(description), text(GUI::font, "", 25), onclick(onclick), canSelect(canSelect), onSelectGrid(onSelectGrid){
    sprite.setPosition({x, y});
    this->text.setFillColor(sf::Color::Black);
    this->text.setPosition({x+w+5, y-5});
}

std::vector<std::pair<int, int>> Skill::selectedGrids;
std::vector<Skill> Skill::skills;
int Skill::usedSkillIndex = -1;

void Skill::init(){
    skills = std::vector<Skill>({        
        Skill(1400, 300, 3, GUI::projectPath + "/resources/images/addGrid.jpg", L"添砖加瓦\n选择一个格子的相邻位置，\n添加一个格子", [](){
            GUI::selectingGrid = true;
        },
        [](int r, int c){
            if(Game::isUsed(r, c)) return false;
            constexpr int dr[4] = {1, 0, -1, 0};
            constexpr int dc[4] = {0, 1, 0, -1};
            for(int k = 0; k < 4; k++){
                int r1 = r + dr[k];
                int c1 = c + dc[k];
                if(Game::isUsed(r1, c1)){
                    return true;
                }
            }
            return false;
        },
        [](int r, int c){
            Game::addGrid(r, c);
            Game::updateGUIBoard();
            GUI::selectingGrid = false;
            reduceSkillTimes();
        }),
        Skill(1400, 500, 1, GUI::projectPath + "/resources/images/crossExplosion.jpg", L"十字爆炸\n选择一个格子或与格子相邻的位置，\n清除该位置和上下左右位置的数字", [](){
            GUI::selectingGrid = true;
        }, [](int r, int c){
            constexpr int dr[5] = {0, 1, 0, -1, 0};
            constexpr int dc[5] = {0, 0, 1, 0, -1};
            for(int k = 0; k < 5; k++){
                int r1 = r + dr[k], c1 = c + dc[k];
                if(Game::isUsed(r1, c1)){
                    return true;
                }
            }
            return false;
        }, [](int r, int c){
            GUI::selectingGrid = false;
            constexpr int dr[5] = {0, 1, 0, -1, 0};
            constexpr int dc[5] = {0, 0, 1, 0, -1};
            for(int k = 0; k < 5; k++){
                int r1 = r + dr[k], c1 = c + dc[k];
                if(Game::isUsed(r1, c1)){
                    Game::setGrid(r1, c1, 0, false, true);
                }
            }
            Game::updateGUIBoard();
            for(int k = 0; k < 5; k++){
                int r1 = r + dr[k], c1 = c + dc[k];
                if(Game::isUsed(r1, c1)){
                    GUIEffect::addEffect(std::make_shared<explosionEffect>(r1, c1));
                }
            }
            reduceSkillTimes();
        }),
        Skill(1400, 700, 2, GUI::projectPath + "/resources/images/swap.jpg", L"换位\n选择两个格子，将其交换位置", [](){
            GUI::selectingGrid = true;
        }, [](int r, int c){
            return Game::isUsed(r, c);
        }, [](int r, int c){
            if(selectedGrids.empty()){
                selectedGrids.push_back({r, c});
            }else{
                auto [r1, c1] = selectedGrids[0];
                bool hasNumber = Game::getHasNumber(r, c);
                int number = hasNumber ? Game::getNumber(r, c) : 0;
                bool hasNumber1 = Game::getHasNumber(r1, c1);
                int number1 = hasNumber1 ? Game::getNumber(r1, c1) : 0;
                if(hasNumber){
                    GUI::move(r, c, r1, c1, number, number);
                }
                if(hasNumber1){
                    GUI::move(r1, c1, r, c, number1, number1);
                }
                Game::setGrid(r, c, number1, hasNumber1, true);
                Game::setGrid(r1, c1, number, hasNumber, true);
                GUI::selectingGrid = false;
                selectedGrids.clear();
                reduceSkillTimes();
            }
        })
    });
    for(auto &skill: skills){
        skill.sprite.setTexture(skill.texture);
        updateSkillText(skill);
    }
}

void Skill::updateSkillText(Skill &skill){
    std::wstringstream stream;
    stream << skill.description << L"\n剩余可使用次数: " << skill.remainingTimes;
    skill.text.setString(stream.str());
}

void Skill::reduceSkillTimes(){
    auto &skill = skills[usedSkillIndex];
    skill.remainingTimes -= 1;
    updateSkillText(skill);
}

void Skill::reset(){
    GUI::selectingGrid = false;
    selectedGrids.clear();
    for(auto &skill: skills){
        skill.remainingTimes = skill.availableTimes;
        updateSkillText(skill);
    }
}

void Skill::draw(sf::RenderWindow &window){
    if(GUI::getState() == GUI::GUIState::menu) return;
    for(auto &skill: skills){
        window.draw(skill.sprite);
        window.draw(skill.text);
    }
}

bool Skill::contains(float posX, float posY){
    return x <= posX && posX <= x + w && y <= posY && posY <= y + w;
}

void Skill::click(float x, float y){
    if(GUI::getState() != GUI::ingame) return;
    for(int i = 0; i < skills.size(); i++){
        if(skills[i].contains(x, y) && skills[i].remainingTimes > 0){
            usedSkillIndex = i;
            skills[i].onclick();
        }
    }
}

sf::Color Skill::getSelectingColor(int r, int c){
    return skills[usedSkillIndex].canSelect(r, c) ? sf::Color(255, 255, 0, 120) : sf::Color(255, 0, 0, 120);
}

void Skill::selectGrid(int r, int c){
    auto &skill = skills[usedSkillIndex];
    if(skill.canSelect(r, c)){
        skill.onSelectGrid(r, c);
    }
}

bool Skill::checkGameEnd(){
    for(auto &skill: skills){
        if(skill.remainingTimes > 0) return false;
    }
    return true;
}
