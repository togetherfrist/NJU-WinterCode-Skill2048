#include "Skill.h"
#include "GUI.h"
#include <vector>
#include <string>
#include <iostream>

Skill::Skill(float x, float y, std::string path):
x(x), y(y),path(path), texture(path), sprite(texture){
    sprite.setPosition({x, y});
}

std::list<Skill> Skill::skills({
    Skill(1300, 300, "./resources/images/addGrid.jpg")
});

void Skill::init(){
    for(auto& skill: skills){
        if(!skill.texture.loadFromFile(skill.path)){
            std::cout << "loading texture " << skill.path << " error\n";
        };
        skill.sprite.setTexture(skill.texture);
    }
}

void Skill::draw(sf::RenderWindow &window){
    if(GUI::getState() == GUI::GUIState::menu) return;
    for(auto skill: skills){
        window.draw(skill.sprite);
    }
}