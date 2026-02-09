#include <vector>
#include <random>

class Game{
private:
    struct grid;
    static std::vector<std::vector<grid>> board;
    static std::mt19937 rd;
    static void generateNumber();
public:
    static void start();
    static void move(int dr, int dc);
};