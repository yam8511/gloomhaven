#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <random>
#include "game.h"
#include <algorithm>
using namespace std;

string getPlayerCode(int i)
{
    vector<string> Acode;
    Acode.push_back("A");
    Acode.push_back("B");
    Acode.push_back("C");
    Acode.push_back("D");
    Acode.push_back("E");
    Acode.push_back("F");
    Acode.push_back("G");
    Acode.push_back("H");
    Acode.push_back("I");
    Acode.push_back("J");
    Acode.push_back("K");
    Acode.push_back("L");
    Acode.push_back("M");
    Acode.push_back("N");
    Acode.push_back("O");
    Acode.push_back("P");
    Acode.push_back("Q");
    Acode.push_back("R");
    Acode.push_back("S");
    Acode.push_back("T");
    Acode.push_back("U");
    Acode.push_back("V");
    Acode.push_back("W");
    Acode.push_back("X");
    Acode.push_back("Y");
    Acode.push_back("Z");

    string a = "";
    if (i < Acode.size())
        a = Acode[i];

    Acode.clear();
    return a;
}

string getMonsterCode(int i)
{
    vector<string> acode;
    acode.push_back("a");
    acode.push_back("b");
    acode.push_back("c");
    acode.push_back("d");
    acode.push_back("e");
    acode.push_back("f");
    acode.push_back("g");
    acode.push_back("h");
    acode.push_back("i");
    acode.push_back("j");
    acode.push_back("k");
    acode.push_back("l");
    acode.push_back("m");
    acode.push_back("n");
    acode.push_back("o");
    acode.push_back("p");
    acode.push_back("q");
    acode.push_back("r");
    acode.push_back("s");
    acode.push_back("t");
    acode.push_back("u");
    acode.push_back("v");
    acode.push_back("w");
    acode.push_back("x");
    acode.push_back("y");
    acode.push_back("z");

    string a = "";
    if (i < acode.size())
        a = acode[i];

    acode.clear();
    return a;
}
// 動作類型
typedef int Action;
const Action ActionRest = 1;    // 長休
const Action ActionCard = 2;    // 出牌
const Action ActionMonster = 3; // 怪物

typedef struct
{
    string name;
    int agile;
    Action action;
    Character *p;
    CharacterSkill *s1, *s2;
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

        if (cX == cY)               // 2. 排序角色或怪物
            return x.name < y.name; // 3. 排序名字

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
    W = 'w', // 上
    A = 'a', // 左
    S = 's', // 下
    D = 'd', // 右
    E = 'e', // 不動
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

string trimNewline(string tmp)
{
    tmp.erase(std::remove(tmp.begin(), tmp.end(), '\r'), tmp.end());
    tmp.erase(std::remove(tmp.begin(), tmp.end(), '\n'), tmp.end());
    return tmp;
}

string getInputLine()
{
    char buffer[512];
    cin.getline(buffer, 512);
    return trimNewline(string(buffer));
}

vector<string> getInputLineSplit()
{
    char buffer[512];
    cin.getline(buffer, 512);

    vector<string> tmps = split(buffer, " ");
    if (tmps.size() > 0)
    {
        tmps[tmps.size() - 1] = trimNewline(tmps[tmps.size() - 1]);
    }

    return tmps;
}
