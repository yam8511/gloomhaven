#include <string>
#include <vector>
#include <math.h>
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
    bool Equal(Point2d p) { return p.posX == this->posX && p.posY == this->posY; }
    int Distance(Point2d p)
    {
        int d = 0;
        int dx = p.X() - this->X();
        int dy = p.Y() - this->Y();
        d = sqrt(dx * dx + dy * dy);
        return d;
    }
    void ShowMe() { printf("座標: (%d, %d)\n", this->posX, this->posY); }
};
