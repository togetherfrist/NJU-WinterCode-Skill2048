#include "GUI.h"
#include "Game.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <functional>

GUI::GUIstate GUI::state = menu;
sf::Font GUI::font;
sf::Clock GUI::clock;
struct GUI::grid{
    int number;
    bool hasnumber;
    std::string content;
    bool used;
    grid(int number = 0, bool hasnumber = false, std::string content = "", bool used = true){
        this->number = number;
        this->hasnumber = hasnumber;
        this->content = content;
        this->used = used;
    }
};
std::vector<std::vector<GUI::grid>> GUI::board;

void GUI::fixTextOrigin(sf::Text &text){
    sf::FloatRect textbounds = text.getLocalBounds();
    float text_x = textbounds.position.x;
    float text_y = textbounds.position.y;
    float text_w = textbounds.size.x;
    float text_h = textbounds.size.y;
    text.setOrigin({text_x + text_w/2.0f, text_y + text_h/2.0f});
}

struct GUI::button{
    float x, y, w, h;
    sf::Text content;
    std::function<void()> onclick;
    button(float x, float y, float w, float h, const wchar_t *content, std::function<void()> onclick)
    : x(x), y(y), w(w), h(h), content(GUI::font, content), onclick(onclick) {
        this->content.setCharacterSize(36.0f);
        this->content.setFillColor(sf::Color::Black);
        fixTextOrigin(this->content);
        this->content.setPosition({x + w/2.0f, y + h/2.0f});
    }
    bool contains(float posX, float posY){
        return x <= posX && posX <= x + w && y <= posY && posY <= y + h;
    }
};

std::vector<GUI::button> GUI::buttons[statesCount];

void GUI::addButton(float x, float y, float w, float h, GUIstate displayState, const wchar_t *content, std::function<void()> onclick){
    GUI::buttons[state].push_back(button(x, y, w, h, content, onclick));
}

sf::Vector2f GUI::getMousePosition(sf::RenderWindow &window){
    sf::Vector2u realWindowSize = window.getSize();
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    float originalX = float(mousePosition.x) / realWindowSize.x * window_w;
    float originalY = float(mousePosition.y) / realWindowSize.y * window_h;
    return sf::Vector2f({originalX, originalY});
}

void GUI::addRectangleEdges(sf::Vertex* &line, float x_left, float x_right, float y_up, float y_down, sf::Color color){
    line[0].position = sf::Vector2f(x_left, y_up);
    line[1].position = sf::Vector2f(x_right, y_up);
    line[2].position = sf::Vector2f(x_left, y_down);
    line[3].position = sf::Vector2f(x_right, y_down);
    line[4].position = sf::Vector2f(x_left, y_up);
    line[5].position = sf::Vector2f(x_left, y_down);
    line[6].position = sf::Vector2f(x_right, y_up);
    line[7].position = sf::Vector2f(x_right, y_down);
    for(int i = 0; i < 8; i++){
        line[i].color = color;
    }
    line += 8;
}

void GUI::addRectangleInterior(sf::Vertex* &triangle, float x_left, float x_right, float y_up, float y_down, sf::Color color){
    triangle[0].position = sf::Vector2f(x_left, y_up);
    triangle[1].position = sf::Vector2f(x_right, y_up);
    triangle[2].position = sf::Vector2f(x_right, y_down);
    triangle[3].position = sf::Vector2f(x_left, y_up);
    triangle[4].position = sf::Vector2f(x_left, y_down);
    triangle[5].position = sf::Vector2f(x_right, y_down);
    for(int i = 0; i < 6; i++){
        triangle[i].color = color;
    }
    triangle += 6;
}

void GUI::addRectangle(sf::Vertex* &line, sf::Vertex* &triangle, float x_left, float x_right, float y_up, float y_down, sf::Color edgeColor, sf::Color interiorColor){
    GUI::addRectangleEdges(line, x_left, x_right, y_up, y_down, edgeColor);
    GUI::addRectangleInterior(triangle, x_left, x_right, y_up, y_down, interiorColor);
}

int GUI::getTime(){
    return GUI::clock.getElapsedTime().asMilliseconds();
}

void GUI::drawButtons(sf::RenderWindow &window){
    auto displayButtons = &buttons[GUI::state][0];
    int n = buttons[GUI::state].size();
    sf::VertexArray triangle(sf::PrimitiveType::Triangles, n * 6);
    sf::VertexArray line(sf::PrimitiveType::Lines, n * 8);
    sf::Vector2f mousePosition = getMousePosition(window);
    float mouseX = mousePosition.x;
    float mouseY = mousePosition.y;
    sf::Vertex *triangleDraw = &triangle[0];
    sf::Vertex *lineDraw = &line[0];
    for(int i = 0; i < n; i++){
        float x = displayButtons[i].x;
        float y = displayButtons[i].y;
        float w = displayButtons[i].w;
        float h = displayButtons[i].h;
        auto lineColor = sf::Color::Black;
        auto interiorColor = displayButtons[i].contains(mouseX, mouseY) ? sf::Color(200, 255, 200) : sf::Color::White;
        addRectangle(lineDraw, triangleDraw, x, x+w, y, y+h, lineColor, interiorColor);
    }
    window.draw(triangle);
    window.draw(line);
    for(int i = 0; i < n; i++){
        window.draw(displayButtons[i].content);
    }
}

void GUI::click(sf::RenderWindow &window){
    sf::Vector2f mousePosition = getMousePosition(window);
    float x = mousePosition.x;
    float y = mousePosition.y;
    for(auto button: GUI::buttons[GUI::state]){
        if(button.contains(x, y)){
            button.onclick();
        }
    }
}

float GUI::getGridWidth(){
    return (window_w - 2*GridsToLeft) / board[0].size();
}

float GUI::getGridEdgeWidth(){
    return getGridWidth() / 20;
}

sf::Vector2f GUI::getGridTopLeft(int r, int c){
    float grid_w = getGridWidth();
    float outerXLeft = GridsToLeft + c * grid_w;
    float outerYUp = GridsToTop + r * grid_w;
    return sf::Vector2f(outerXLeft, outerYUp);
}

sf::Color GUI::getNumberColor(int number){
    constexpr int L = 5, N = 5;
    int l = std::log2(number);
    int baseR[N] = {230, 255, 255, 255, 50};
    int baseG[N] = {230, 255, 50, 50, 50};
    int baseB[N] = {230, 50, 50, 255, 255};
    int n = l / L;
    int r = l % L;
    float k = float(r) / L;
    if(n >= N-1) return sf::Color(baseR[N-1], baseG[N-1], baseB[N-1]);
    return sf::Color(
        baseR[n] * (1-k) + baseR[n+1] * k,
        baseG[n] * (1-k) + baseG[n+1] * k,
        baseB[n] * (1-k) + baseB[n+1] * k
    );
}

void GUI::drawBoard(sf::RenderWindow &window){
    if(GUI::state == menu) return;
    int countGrids = 0;
    for(auto lines: board){
        for(grid grd: lines){
            if(grd.used){
                ++countGrids;
            }
        }
    }
    sf::VertexArray outer(sf::PrimitiveType::Triangles, countGrids * 6);
    sf::Vertex* outerDraw = &outer[0];
    sf::VertexArray inner(sf::PrimitiveType::Triangles, countGrids * 6);
    sf::Vertex* innerDraw = &inner[0];
    int gridindex = 0;
    float grid_w = getGridWidth();
    float line_w = getGridEdgeWidth();
    float inner_w = grid_w - line_w;
    float inner_h = inner_w;
    float outer_w = grid_w + line_w;
    float outer_h = outer_w;
    sf::Color outerColor = sf::Color(130, 180, 255);
    sf::Color emptyColor = sf::Color::White;
    std::vector<sf::Text> numberTexts(countGrids, sf::Text(font));
    for(int r = 0; r < board.size(); r++){
        for(int c = 0; c < board[r].size(); c++){
            grid grd = board[r][c];
            if(grd.used){
                sf::Vector2f topLeft = getGridTopLeft(r, c);
                float outerXLeft = topLeft.x;
                float outerYUp = topLeft.y;
                addRectangleInterior(outerDraw, outerXLeft, outerXLeft + outer_w, outerYUp, outerYUp + outer_h, outerColor);
                float innerXLeft = outerXLeft + line_w;
                float innerYUp = outerYUp + line_w;
                sf::Color innerColor = grd.hasnumber ? getNumberColor(grd.number) : emptyColor;
                addRectangleInterior(innerDraw, innerXLeft, innerXLeft + inner_w, innerYUp, innerYUp + inner_h, innerColor);
                numberTexts[gridindex].setString(grd.content);
                numberTexts[gridindex].setFillColor(sf::Color::Black);
                fixTextOrigin(numberTexts[gridindex]);
                numberTexts[gridindex].setPosition({innerXLeft + inner_w/2.0f, innerYUp + inner_h/2.0f});
                ++gridindex;
            }
        }
    }
    window.draw(outer);
    window.draw(inner);
    for(auto text: numberTexts){
        window.draw(text);
    }
}

std::vector<std::shared_ptr<GUIEffect>> GUI::effects;
void GUI::drawEffects(sf::RenderWindow &window){
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
    for(auto &effect : effects){
        if(effect->shouldRemain()){
            remainingEffects.push_back(effect);
        }else{
            effect->onEnd();
        }
    }
    effects = remainingEffects;
}

void GUI::openGUI(){
    GUI::state = menu;

    sf::RenderWindow window(sf::VideoMode({window_w, window_h}), "Super 2048");
    window.setKeyRepeatEnabled(false);

    const sf::Texture bg_texture("./Resources/Images/bg_1920x1080.png");
    sf::Sprite bg_sprite(bg_texture);

    if(!font.openFromFile("Resources/Fonts/SourceHanSerifSC-Regular.otf")){
        std::cout << "Error while loading font\n";
    }

    sf::Music music("./Resources/Music/04_Nobiri.mp3");
    music.setVolume(10.0f);
    music.play();

    constexpr float button_w = window_w / 5;
    constexpr float button_h = window_h / 10;
    addButton(window_w/2-button_w/2, window_h/2, button_w, button_h, menu, L"开始游戏", [](){
        Game::start();
    });

    const auto onClose = [&window](const sf::Event::Closed&){
        window.close();
    };

    const auto onKeyPressed = [&window](const sf::Event::KeyPressed &keyPressed){
        if(state == ingame){
            switch(keyPressed.scancode){
                case sf::Keyboard::Scancode::W :
                case sf::Keyboard::Scancode::Up :
                    Game::move(-1, 0);
                    break;
                case sf::Keyboard::Scancode::S :
                case sf::Keyboard::Scancode::Down :
                    Game::move(1, 0);
                    break;
                case sf::Keyboard::Scancode::A :
                case sf::Keyboard::Scancode::Left :
                    Game::move(0, -1);
                    break;
                case sf::Keyboard::Scancode::D :
                case sf::Keyboard::Scancode::Right :
                    Game::move(0, 1);
                    break;
                case sf::Keyboard::Scancode::R :
                    Game::start();
                    break;
            }
        }
    };

    const auto onMouseButtonPressed = [&window](const sf::Event::MouseButtonPressed mousePressed){
        if(mousePressed.button == sf::Mouse::Button::Left){
            click(window);
        }
    };

    while(window.isOpen()){
        window.handleEvents(onClose, onKeyPressed, onMouseButtonPressed);
        window.clear();
        window.draw(bg_sprite);
        drawButtons(window);
        drawBoard(window);
        drawEffects(window);
        window.display();
    }
}

void GUI::startGame(int basic_n){
    board = std::vector<std::vector<grid>>(basic_n, std::vector<grid>(basic_n, grid()));
    state = ingame;
}

std::string GUI::getBitString(int n){
    const int lenPerLine = 7;
    std::string s(lenPerLine, '0');
    int p = 0;
    while(1){
        if(p == s.size()){
            s = std::string(lenPerLine, '0') + "\n" + s;
            ++p;
        }
        s[s.size()-1-p] = (n&1) + '0';
        n >>= 1;
        if(n == 0) break;
        ++p;
    }
    return s;
}

void GUI::putNumberWithEffect(int row, int col, int number){
    putNumber(row, col, number);
    float grid_w = getGridWidth();
    float line_w = getGridEdgeWidth();
    sf::Vector2f topLeft = getGridTopLeft(row, col);
    float outer_w = grid_w + line_w;
    float outer_h = outer_w;
    float outerXLeft = topLeft.x;
    float outerYUp = topLeft.y;
    effects.push_back(std::make_shared<LightEffect>(outerXLeft + outer_w / 2, outerYUp + outer_h / 2, outer_w / 2));
}

void GUI::putNumber(int row, int col, int number){
    board[row][col].hasnumber = true;
    board[row][col].number = number;
    board[row][col].content = getBitString(number);
}

void GUI::addMoveEffect(int r, int c, int r_to, int c_to, int number, int endNumber, std::function<void()> onEnd){
    board[r][c] = grid();
    effects.push_back(std::make_shared<MoveEffect>(r, c, r_to, c_to, number, endNumber, onEnd));
}

void GUI::move(int r, int c, int r_to, int c_to, int number, int endNumber){
    addMoveEffect(r, c, r_to, c_to, number, endNumber, [r_to, c_to, endNumber](){
        putNumber(r_to, c_to, endNumber);
    });
}

void GUI::updateEffects(){
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
