#include <string>
#include <vector>
using namespace std;

// 座標
class Point2d
{
private:
    int posX, posY; // X,Y座標
public:
    Point2d(int x = 0, int y = 0)
    {
        this->posX = x;
        this->posY = y;
    };

    int X() { return this->posX; }
    int Y() { return this->posY; }
    void ShowMe() { printf("座標: (%d, %d)\n", this->posX, this->posY); }
};
