#include "Game.h"
#include "GUI.h"
#include <vector>
#include <random>
#include <functional>
#include <iostream>

struct Game::grid{
    int number;
    bool hasnumber, used;
    grid(int number, bool hasnumber, bool used){
        this->number = number;
        this->hasnumber = hasnumber;
        this->used = used;
    }
};

std::vector<std::vector<Game::grid>> Game::board;
std::mt19937 Game::rd = std::mt19937(time(0));

void Game::generateNumber(){
    int countEmptyGrid = 0;
    for(auto lines: board){
        for(grid grd: lines){
            if(!grd.hasnumber && grd.used){
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
            if(!grd.hasnumber && grd.used){
                ++emptyGridPos;
                if(emptyGridPos == generatePosition){
                    board[r][c].number = generatedNumber;
                    board[r][c].hasnumber = true;
                    GUI::generateNumber(r, c, generatedNumber);
                }
            }
        }
    }
}

void Game::start(){
    int basic_n = 4;
    board = std::vector<std::vector<grid>>(basic_n, std::vector<grid>(basic_n, grid(0, false, true)));
    GUI::startGame(basic_n);
    generateNumber();
    generateNumber();
}

void Game::move(int dr, int dc){
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
        int er = r + dr, ec = c + dc;
        auto moveRC = [dr, dc, &er, &ec, &isMoved](int r, int c){
            int num = board[r][c].number;
            int r_to = r, c_to = c;
            bool composed = false;
            for(int r1 = r + dr, c1 = c + dc; r1 != er || c1 != ec; r1 += dr, c1 += dc){
                if(!board[r1][c1].used){
                    break;
                }
                if(!board[r1][c1].hasnumber){
                    r_to = r1, c_to = c1;
                }else{
                    if(num == board[r1][c1].number){
                        r_to = r1, c_to = c1;
                        composed = true;
                    }
                    break;
                }
            }
            if(r_to != r || c_to != c){
                isMoved = true;
                GUI::move(r, c, r_to, c_to);
                if(composed){
                    num <<= 1;
                    er = r_to, ec = c_to;
                    GUI::generateNumber(r_to, c_to, num);
                }
                board[r][c].hasnumber = false;
                board[r_to][c_to].hasnumber = true;
                board[r_to][c_to].number = num;
            }
        };
        for(int r1 = r, c1 = c; r1 != r_end && c1 != c_end; r1 -= dr, c1 -= dc){
            if(board[r1][c1].used && board[r1][c1].hasnumber){
                moveRC(r1, c1);
            }
        }
    }
    if(isMoved){
        generateNumber();
    }
}
