#include "Leaderboard.h"
#include "GUI.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

struct Leaderboard::record{
    int score;
    long long time;
    record(int score, long long time){
        this->score = score;
        this->time = time;
    }
    bool friend operator <(const record a, const record b){
        if(a.score != b.score){
            return a.score > b.score;
        }
        return a.time < b.time;
    }
};
std::multiset<Leaderboard::record> Leaderboard::leaderboard;
sf::Text Leaderboard::text(GUI::font);

std::wstring Leaderboard::getString(){
    std::wstringstream stream;
    stream << std::setw(3) << L"排名" << std::setw(7) << L"得分" << std::setw(9) << L"用时" << L'\n';
    int id = 0;
    for(record rec: leaderboard){
        ++id;
        stream << std::setw(4) << id << std::setw(10) << rec.score << std::setw(14) << getTimeString(rec.time) << '\n';
    }
    return stream.str();
}

std::wstring Leaderboard::getTimeString(long long time){
    std::wstringstream stream;
    int millisecond = time % 1000;
    int second = time / 1000;
    int minute = second / 60;
    second %= 60;
    stream << std::setw(2) << std::setfill(L'0') << minute << L":";
    stream << std::setw(2) << std::setfill(L'0') << second << L".";
    stream << std::setw(3) << std::setfill(L'0') << millisecond;
    return stream.str();
}

void Leaderboard::updateText(){
    text.setString(getString());
}

void Leaderboard::update(int score, long long time){
    leaderboard.insert(record(score, time));
    constexpr int N = 10;
    if(leaderboard.size() > N){
        leaderboard.erase(--leaderboard.end());
    }
    updateText();
    save();
}

void Leaderboard::draw(sf::RenderWindow &window){
    if(GUI::getState() != GUI::GUIState::menu) return;
    constexpr float x = 50, y = 300;
    static bool init = false;
    if(!init){
        init = true;
        text.setPosition({x, y});
        text.setFillColor(sf::Color::Black);
        text.setCharacterSize(36);
    }
    window.draw(text);
}

void Leaderboard::save(){
    const std::string path = GUI::projectPath + "/data/leaderboard.txt";
    std::ofstream out(path);
    for(record rec: leaderboard){
        out << rec.score << ' ' << rec.time << '\n';
    }
}

void Leaderboard::load(){
    const std::string path = GUI::projectPath + "/data/leaderboard.txt";
    std::ifstream in(path);
    record rec(0, 0);
    while(in >> rec.score >> rec.time){
        leaderboard.insert(rec);
    }
    updateText();
}
