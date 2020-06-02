#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <random>
#include "game.h"
using namespace std;

// 動作類型
typedef int Action;
const Action ActionRest = 1;    // 長休
const Action ActionCard = 2;    // 出牌
const Action ActionMonster = 3; // 怪物

typedef struct
{
    int index;
    int agile;
    Action action;
    CharacterSkill *card1, *card2;
    MonsterSkill *sk;
} readyAction;

bool compareReadyAction(const readyAction &x, const readyAction &y)
{
    if (x.agile == y.agile) // 1. 排序敏捷
    {
        int cX = 0;
        int cY = 0;
        if (x.action == ActionMonster)
            cX = 1;
        if (y.action == ActionMonster)
            cY = 1;

        if (cX == cY)                 // 2. 排序角色或怪物
            return x.index < y.index; // 3. 排序名字

        return cX < cY;
    }

    return x.agile < y.agile;
}

int randInt(int max) // 亂數產生數字
{
    std::random_device rd;
    std::default_random_engine gen = std::default_random_engine(rd());
    std::uniform_int_distribution<int> dis(0, max);

    return dis(gen);
}

// 怪物出現類型
typedef int MonsterAppear;
const MonsterAppear MonsterNone = 0;   // 不出現
const MonsterAppear MonsterNormal = 1; // 普通
const MonsterAppear MonsterGreat = 2;  // 菁英

// 地圖障礙類型
typedef char MapObject;
const MapObject None = '-';            // 無
const MapObject Wall = '0';            // 牆壁
const MapObject Floor = '1';           // 地板
const MapObject Stuck = '2';           // 障礙物
const MapObject Door = '3';            // 門
const MapObject PlayerObject = '!';    // 玩家的位子
const MapObject MonsterObject = '@';   // 怪物的位子
const MapObject DisableObject = '#';   // 關閉的位子
const MapObject AvailableObject = '_'; // 可選位子
const MapObject SelectedObject = '*';  // 已選位子

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

    if (x == DisableObject) // 關閉的位子
        return ' ';

    if (x == AvailableObject) // 可選位子
        return AvailableObject;

    if (x == SelectedObject) // 已選位子
        return SelectedObject;

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
