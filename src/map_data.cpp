#include <string>
#include <vector>
#include <map>
#include "point2d.cpp"
using namespace std;

// 地圖資訊
class MapData
{
private:
    int width, height;
    vector<vector<MapObject>> layout; // 地圖配置 (障礙物)
    vector<Point2d> initPos;          // 初始可選位子
    Point2d cursorPos;                // 指標位子
    vector<Point2d> playerPos;        // 角色位子
    vector<Point2d> monsterPos;       // 怪物位子

public:
    MapData(int m = 0, int n = 0)
    {
        this->height = m;
        this->width = n;
    };

    void SetLayout(vector<vector<MapObject>> layout) { this->layout = layout; }
    void SetPlayerInitPos(vector<Point2d> poss)
    {
        this->initPos = poss;
        Point2d cursorPos(this->width - 1, this->height - 1);
        for (int i = 0; i < poss.size(); i++)
        {
            Point2d p = poss[i];
            if (p.Y() < cursorPos.Y() ||
                (p.Y() == cursorPos.Y() && p.X() < cursorPos.X()))
                cursorPos = p;
        }
        this->cursorPos = cursorPos;
    }
    void SetCursorPos(Point2d pos) { this->cursorPos = pos; }
    void SetMonstersPos(vector<Point2d> mops) { this->monsterPos = mops; }
    void SetPlayersPos(vector<Point2d> poss) { this->playerPos = poss; }

    // 移動玩家初始位子
    bool MovePlayerInitPos(char action)
    {
        if (action == 'e') // 確定位子
        {
            // TODO: 更新地圖
            // 加入角色位子
            // 調整指標位子
            return true;
        }

        Point2d nextPos;
        if (action == 'w') // 上
        {
            nextPos = Point2d(this->cursorPos.X(), this->cursorPos.Y() - 1);
        }
        else if (action = 's') // 下
        {
            nextPos = Point2d(this->cursorPos.X(), this->cursorPos.Y() + 1);
        }
        else if (action == 'a') // 左
        {
            nextPos = Point2d(this->cursorPos.X() - 1, this->cursorPos.Y());
        }
        else if (action == 'd') // 右
        {
            nextPos = Point2d(this->cursorPos.X() + 1, this->cursorPos.Y());
        }

        // 超過邊界則不移動
        if (nextPos.X() < 0 || nextPos.Y() < 0 || nextPos.X() >= this->width || nextPos.Y() >= this->height)
            return;

        for (int i = 0; i < this->initPos.size(); i++)
        {
            if (this->initPos[i].X() == nextPos.X() && this->initPos[i].Y() == nextPos.Y())
            {
                this->initPos.erase(this->initPos.begin() + i);
                this->cursorPos = nextPos;
                break;
            }
        }

        return false;
    }

    Point2d GetCursorPos() { return this->cursorPos; }

    void ShowMe()
    {
        vector<vector<MapObject>> showData;
        for (int i = 0; i < this->layout.size(); i++)
        {
            vector<MapObject> tmp;
            for (int j = 0; j < this->layout[i].size(); j++)
            {
                tmp.push_back(this->layout[i][j]);
            }
            showData.push_back(tmp);
        }

        for (int i = 0; i < this->initPos.size(); i++)
        {
            Point2d p = this->initPos[i];
            showData[p.Y()][p.X()] = AvailablePos;
        }

        showData[this->cursorPos.Y()][this->cursorPos.X()] = SelectedPos;

        char a = 'a';
        for (int i = 0; i < this->monsterPos.size(); i++)
        {
            Point2d p = this->monsterPos[i];
            showData[p.Y()][p.X()] = a;
            a++;
        }

        a = 'A';
        for (int i = 0; i < this->playerPos.size(); i++)
        {
            Point2d p = this->playerPos[i];
            showData[p.Y()][p.X()] = a;
            a++;
        }

        for (int y = 0; y < this->height; y++)
        {
            for (int x = 0; x < this->width; x++)
                cout << showMapObject(showData[y][x]) << ends;
            cout << endl;
        }
    }
};
