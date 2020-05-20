#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
using namespace std;

// 怪物出現類型
enum class MonsterAppear
{
    None = 0,   // 不出現
    Normal = 1, // 普通
    Great = 2,  // 菁英
};

// 地圖障礙類型
typedef char MapObject;
const MapObject None = '-';         // 無
const MapObject Wall = '0';         // 牆壁
const MapObject Floor = '1';        // 地板
const MapObject Stuck = '2';        // 障礙物
const MapObject Door = '3';         // 門
const MapObject AvailablePos = '_'; // 可選位子
const MapObject SelectedPos = '*';  // 已選位子

MapObject parseMapObject(char x)
{
    if (x == Wall ||
        x == Floor ||
        x == Stuck ||
        x == Door)
        return MapObject(x);

    return None;
}

char showMapObject(MapObject x)
{
    if (x == None) // 無
        return None;

    if (x == Wall) // 牆壁
        return ' ';

    if (x == Floor) // 地板
        return Floor;

    if (x == Stuck) // 障礙物
        return Stuck;

    if (x == Door) // 門
        return Door;

    if (x == AvailablePos) // 可選位子
        return AvailablePos;

    if (x == SelectedPos) // 已選位子
        return SelectedPos;

    return x;
}

// 移動類型
enum Move
{
    W = 1, // 上
    A = 2, // 左
    S = 3, // 下
    D = 4, // 右
    E = 0, // 不動
};

vector<string> split(const string &str, const string &delim)
{
    vector<string> res;
    if ("" == str)
        return res;

    char *strs = new char[str.length() + 1];
    strcpy(strs, str.c_str());

    char *d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while (p)
    {
        string s = p;
        res.push_back(s);
        p = strtok(NULL, d);
    }

    return res;
}

void PrintAndExit(string e)
{
    cout << e << endl;
    exit(1);
    // throw e;
}

string getInputLine()
{
    char buffer[512];
    cin.getline(buffer, 512);
    return string(buffer);
}

vector<string> getInputLineSplit()
{
    char buffer[512];
    cin.getline(buffer, 512);
    return split(buffer, " ");
}
