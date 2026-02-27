#include <functional>
#include <SFML/Graphics.hpp>
#include <string>

#pragma once

class GUIEffect{
private:
    static std::vector<std::shared_ptr<GUIEffect>> effects;
public:
    int startTime, duration;
    int lineVertexNum, triangleVertexNum;
    std::function<void()> onEnd;
    GUIEffect(int duration, int lineVertexNum, int triangleVertexNum, std::function<void()> onEnd);
    virtual void drawTo(sf::Vertex* &line, sf::Vertex* &triangle) = 0;
    virtual void drawToWindow(sf::RenderWindow &window) = 0;
    virtual bool endOnUpdate() = 0;
    bool shouldRemain();
    static void draw(sf::RenderWindow &window);
    static void addEffect(std::shared_ptr<GUIEffect> effect);
    static void updateEffects();
};

class LightEffect : public GUIEffect{
private:
    float x, y, r;
public:
    LightEffect(float x, float y, float r);
    void drawTo(sf::Vertex* &line, sf::Vertex* &triangle) override;
    void drawToWindow(sf::RenderWindow &window) override;
    bool endOnUpdate() override;
};

class MoveEffect : public GUIEffect{
private:
    int r, c, r_to, c_to;
    int number, endNumber;
    sf::Text content;
public:
    MoveEffect(int r, int c, int r_to, int c_to, int number, int endNumber, std::function<void()> onEnd);
    void drawTo(sf::Vertex* &line, sf::Vertex* &triangle) override;
    void drawToWindow(sf::RenderWindow &window) override;
    bool endOnUpdate() override;
};

class putNumberEffect : public GUIEffect{
private:
    int r, c;
    int number;
public:
    putNumberEffect(int r, int c, int number);
    void drawTo(sf::Vertex* &line, sf::Vertex* &triangle) override;
    void drawToWindow(sf::RenderWindow &window) override;
    bool endOnUpdate() override;
};

class explosionEffect : public GUIEffect{
private:
    int r, c;
public:
    explosionEffect(int r, int c);
    void drawTo(sf::Vertex* &line, sf::Vertex* &triangle) override;
    void drawToWindow(sf::RenderWindow &window) override;
    bool endOnUpdate() override;
};
