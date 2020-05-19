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
    map<Point2d, Monster> monsterPos; // 怪物位子

public:
    MapData(int m = 0, int n = 0)
    {
        this->height = m;
        this->width = n;
    };

    void SetLayout(vector<vector<MapObject>> layout) { this->layout = layout; }
    void SetPlayerInitPos(vector<Point2d> poss) { this->initPos = poss; }

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
            showData[p.Y()][p.X()] = showMapObject(AvailablePos);
        }

        for (int y = 0; y < this->height; y++)
        {
            for (int x = 0; x < this->width; x++)
                cout << showMapObject(showData[y][x]) << ends;
            cout << endl;
        }
    }
};
