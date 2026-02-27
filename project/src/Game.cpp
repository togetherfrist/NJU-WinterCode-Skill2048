#include "Game.h"
#include "GUI.h"
#include "Skill.h"
#include <vector>
#include <random>
#include <functional>
#include <iostream>

struct Game::grid{
    int number;
    bool hasNumber, used;
    grid(int number, bool hasNumber, bool used){
        this->number = number;
        this->hasNumber = hasNumber;
        this->used = used;
    }
};

std::vector<std::vector<Game::grid>> Game::board;
std::mt19937 Game::rd = std::mt19937(time(0));
int Game::score = 0;

void Game::generateNumber(){
    int countEmptyGrid = 0;
    for(auto lines: board){
        for(grid grd: lines){
            if(!grd.hasNumber && grd.used){
                ++countEmptyGrid;
            }
        }
    }
    std::uniform_int_distribution<> positionRange(1, countEmptyGrid);
    int generatePosition = positionRange(rd);
    std::uniform_int_distribution<> rng(1, 10);
    int generatedNumber = (rng(rd) == 1 ? 2 : 1);
    int emptyGridPos = 0;
    for(int r = 0; r < board.size(); r++){
        for(int c = 0; c < board[r].size(); c++){
            grid grd = board[r][c];
            if(!grd.hasNumber && grd.used){
                ++emptyGridPos;
                if(emptyGridPos == generatePosition){
                    board[r][c].number = generatedNumber;
                    board[r][c].hasNumber = true;
                    GUI::generateNumber(r, c, generatedNumber);
                }
            }
        }
    }
}

void Game::start(){
    score = 0;
    int basic_n = 4;
    board = std::vector<std::vector<grid>>(basic_n, std::vector<grid>(basic_n, grid(0, false, true)));
    GUI::startGame(basic_n);
    Skill::reset();
    generateNumber();
    generateNumber();
}

void Game::move(int dr, int dc){
    GUIEffect::updateEffects();
    int h = board.size();
    int w = board[0].size();
    int r0 = 0, c0 = 0;
    if(dr == 1) r0 = h-1;
    else if(dc == 1) c0 = w-1;
    int r_end = r0 == 0 ? h : -1;
    int c_end = c0 == 0 ? w : -1;
    int ndr = (dr == 0 ? 1 : 0);
    int ndc = 1 - ndr;
    bool isMoved = false;
    for(int r = r0, c = c0; r != r_end && c != c_end; r += ndr, c += ndc){
        bool hasLast = false;
        int last_r = r, last_c = c;
        int lastNum = 0;
        int r_to = r, c_to = c;
        auto moveRCRC = [&isMoved](int r, int c, int r_to, int c_to, int num, int endNum){
            GUI::move(r, c, r_to, c_to, num, endNum);
            board[r][c].hasNumber = false;
            board[r_to][c_to].hasNumber = true;
            board[r_to][c_to].number = endNum;
            isMoved = true;
        };
        auto moveLast = [&moveRCRC, &hasLast, &last_r, &last_c, &lastNum, &r_to, &c_to](){
            if(!hasLast) return;
            hasLast = false;
            if(last_r == r_to && last_c == c_to) return;
            moveRCRC(last_r, last_c, r_to, c_to, lastNum, lastNum);
        };
        auto moveRC = [&moveRCRC, &moveLast, &last_r, &last_c, &hasLast, &lastNum, dr, dc, &r_to, &c_to, &isMoved](int r, int c){
            grid &grd = board[r][c];
            if(!grd.used){
                moveLast();
                r_to = r - dr, c_to = c - dc;
            }else if(grd.hasNumber){
                if(hasLast){
                    if(grd.number == lastNum){
                        int new_num = lastNum << 1;
                        score += new_num;
                        if(last_r != r_to || last_c != c_to){
                            moveRCRC(last_r, last_c, r_to, c_to, lastNum, new_num);
                        }
                        moveRCRC(r, c, r_to, c_to, lastNum, new_num);
                        r_to -= dr, c_to -= dc;
                        hasLast = false;
                    }else{
                        moveLast();
                        r_to -= dr, c_to -= dc;
                        last_r = r, last_c = c, lastNum = grd.number;
                        hasLast = true;
                    }
                }else{
                    last_r = r, last_c = c, lastNum = grd.number;
                    hasLast = true;
                }
            }
        };
        for(int r1 = r, c1 = c; r1 != r_end && c1 != c_end; r1 -= dr, c1 -= dc){
            moveRC(r1, c1);
        }
        moveLast();
    }
    if(isMoved){
        generateNumber();
    }
    checkEnd();
}

int Game::getScore(){
    return score;
}

void Game::checkEnd(){
    constexpr int dr[4] = {1, 0, -1, 0};
    constexpr int dc[4] = {0, 1, 0, -1};
    int h = board.size(), w = board[0].size();
    for(int r = 0; r < h; r++){
        for(int c = 0; c < w; c++){
            grid grd = board[r][c];
            if(grd.used){
                if(!grd.hasNumber) return;
                for(int k = 0; k < 4; k++){
                    int r1 = r + dr[k];
                    int c1 = c + dc[k];
                    if(r1 < 0 || c1 < 0 || r1 >= h || c1 >= w) continue;
                    grid g1 = board[r1][c1];
                    if(g1.used && g1.hasNumber && g1.number == grd.number) return;
                }
            }
        }
    }
    if(!Skill::checkGameEnd()) return;
    GUI::endGame();
}

int Game::getMaxTile(){
    int maxTile = 0;
    for(auto line: board){
        for(grid grd: line){
            if(grd.hasNumber){
                maxTile = std::max(maxTile, grd.number);
            }
        }
    }
    return maxTile;
}

bool Game::isUsed(int r, int c){
    return r >= 0 && r < board.size() && c >= 0 && c < board[0].size() && board[r][c].used;
}

void Game::addGrid(int r, int c){
    int r0 = board.size(), c0 = board[0].size();
    if(r < 0){
        int d = -r;
        board.insert(board.begin(), d, std::vector<grid>(c0, grid(0, false, false)));
        r = 0;
    }else if(r >= r0){
        int d = r - r0 + 1;
        board.insert(board.end(), d, std::vector<grid>(c0, grid(0, false, false)));
    }
    if(c < 0){
        int d = -c;
        for(auto &line: board){
            line.insert(line.begin(), d, grid(0, false, false));
            c = 0;
        }
    }else if(c >= c0){
        int d = c - c0 + 1;
        for(auto &line: board){
            line.insert(line.end(), d, grid(0, false, false));
        }
    }
    board[r][c].used = true;
}

void Game::setGrid(int r, int c, int number, bool hasNumber, bool used){
    board[r][c] = grid(number, hasNumber, used);
}

void Game::updateGUIBoard(){
    GUIEffect::updateEffects();
    GUI::setBoard(board.size(), board[0].size());
    for(int r = 0; r < board.size(); r++){
        for(int c = 0; c < board[0].size(); c++){
            auto &grd = board[r][c];
            GUI::setGrid(r, c, grd.number, grd.hasNumber, grd.used);
        }
    }
}

bool Game::getHasNumber(int r, int c){
    return board[r][c].hasNumber;
}

int Game::getNumber(int r, int c){
    return board[r][c].number;
}




