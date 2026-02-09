#include <functional>

class LightEffect{
private:
    int count;
    int countEnd;
    float x, y;
    std::function<void()> onEnd;
    void next();
    
};