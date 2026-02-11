#include <functional>
#include <SFML/Graphics.hpp>
#include <string>

#pragma once

class GUIEffect{
public:
    int startTime, duration;
    int lineVertexNum, triangleVertexNum;
    std::function<void()> onEnd;
    virtual void drawTo(sf::Vertex* &line, sf::Vertex* &triangle) = 0;
    virtual void drawToWindow(sf::RenderWindow &window) = 0;
    virtual bool endOnUpdate() = 0;
    virtual bool shouldRemain() = 0;
    virtual ~GUIEffect() = default;
};

class LightEffect : public GUIEffect{
private:
    float x, y, r;
public:
    LightEffect(float x, float y, float r);
    void drawTo(sf::Vertex* &line, sf::Vertex* &triangle) override;
    void drawToWindow(sf::RenderWindow &window) override;
    bool endOnUpdate() override;
    bool shouldRemain() override;
};

class MoveEffect : public GUIEffect{
private:
    int startTime;
    int duration;
    int r, c, r_to, c_to;
    int number, endNumber;
    sf::Text content;
public:
    MoveEffect(int r, int c, int r_to, int c_to, int number, int endNumber, std::function<void()> onEnd);
    void drawTo(sf::Vertex* &line, sf::Vertex* &triangle) override;
    void drawToWindow(sf::RenderWindow &window) override;
    bool endOnUpdate() override;
    bool shouldRemain() override;
};