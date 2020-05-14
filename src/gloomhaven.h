#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <vector>
#include "character.h"
using namespace std;

// 角色
class Gloomhaven
{
private:
    vector<Character> players; // 角色資料
    bool debugMode;

    void setDebugMode(bool mode)
    {
        this->debugMode = mode;
    }

    void loadCharacter(string characterFile)
    {
        /**
         * 讀取角色設定檔
         */
        fstream in(characterFile);
        if (!in)
        {
            throw "讀取角色設定檔失敗";
            return;
        }

        string tmp, name, lastTmp;        // 字串變數
        int blood, initCardNum, cardNum;  // 數值變數
        int lastIndex = -1, index, agile; // 數值變數
        in >> tmp;                        // 讀取角色數量
        int num = stoi(tmp);              // 角色數量
        for (int i = 0; i < num; i++)
        {
            if (lastTmp != "")
            {
                name = lastTmp;
                lastTmp = "";
            }
            else
                in >> name; // 讀取名字

            in >> tmp; // 讀取血量設定
            blood = stoi(tmp);
            in >> tmp; // 讀取初始牌數
            initCardNum = stoi(tmp);
            Character player = Character(name, blood, initCardNum);

            in >> tmp; // 讀取牌數
            cardNum = stoi(tmp);

            in >> tmp; // 讀取index
            index = stoi(tmp);

            for (int j = 0; j < cardNum; j++)
            {
                in >> tmp; // 讀取敏捷資訊
                agile = stoi(tmp);
                vector<string> upAction = vector<string>();
                vector<string> downAction = vector<string>();

                in >> tmp;
                while (tmp != "-") // 上半部動作
                {
                    upAction.push_back(tmp);
                    in >> tmp;
                }

                while (true)
                {
                    try
                    {
                        in >> tmp;
                        lastTmp = tmp;
                        index = stoi(tmp); // 檢查是不是數字
                        if (lastIndex != -1 && index > -1)
                            break;

                        downAction.push_back(tmp);
                        lastIndex = index;
                        index = -1;
                    }
                    catch (invalid_argument &e) // 如果不是數字
                    {
                        if (!((tmp == "attack" || tmp == "move" || tmp == "range" || tmp == "shield" || tmp == "heal")))
                        {
                            break;
                        }

                        lastIndex = -1;
                        downAction.push_back(tmp);
                    }
                }

                player.AddSkill(agile, upAction, downAction);
            }

            if (this->debugMode)
                player.ShowMe();

            this->players.push_back(player);
        }
        in.close(); // 關閉檔案
    }

    void loadMonster(string monsterFile)
    {
        /**
         * 讀取怪物設定檔
         */
        fstream in(monsterFile);
        if (!in)
        {
            throw "讀取怪物設定檔失敗";
        }

        string tmp, name, lastTmp;        // 字串變數
        int blood, initCardNum, cardNum;  // 數值變數
        int lastIndex = -1, index, agile; // 數值變數
        in >> tmp;                        // 讀取角色數量
        int num = stoi(tmp);              // 角色數量
        for (int i = 0; i < num; i++)
        {
            if (lastTmp != "")
            {
                name = lastTmp;
                lastTmp = "";
            }
            else
                in >> name; // 讀取名字

            in >> tmp; // 讀取血量設定
            blood = stoi(tmp);
            in >> tmp; // 讀取初始牌數
            initCardNum = stoi(tmp);
            Character player = Character(name, blood, initCardNum);

            in >> tmp; // 讀取牌數
            cardNum = stoi(tmp);

            in >> tmp; // 讀取index
            index = stoi(tmp);

            for (int j = 0; j < cardNum; j++)
            {
                in >> tmp; // 讀取敏捷資訊
                agile = stoi(tmp);
                vector<string> upAction = vector<string>();
                vector<string> downAction = vector<string>();

                in >> tmp;
                while (tmp != "-") // 上半部動作
                {
                    upAction.push_back(tmp);
                    in >> tmp;
                }

                while (true)
                {
                    try
                    {
                        in >> tmp;
                        lastTmp = tmp;
                        index = stoi(tmp); // 檢查是不是數字
                        if (lastIndex != -1 && index > -1)
                            break;

                        downAction.push_back(tmp);
                        lastIndex = index;
                        index = -1;
                    }
                    catch (invalid_argument &e) // 如果不是數字
                    {
                        if (!((tmp == "attack" || tmp == "move" || tmp == "range" || tmp == "shield" || tmp == "heal")))
                        {
                            break;
                        }

                        lastIndex = -1;
                        downAction.push_back(tmp);
                    }
                }

                player.AddSkill(agile, upAction, downAction);
            }

            if (this->debugMode)
                player.ShowMe();

            this->players.push_back(player);
        }
        in.close(); // 關閉檔案
    }

public:
    Gloomhaven(string characterFile, string monsterFile, bool debug = true)
    {
        // Debug模式
        this->setDebugMode(debug);
        this->players = vector<Character>();

        /**
         * 讀取角色設定檔
         */
        this->loadCharacter(characterFile);

        /**
         * 讀取怪物設定檔
         */
        // this->loadMonster(monsterFile);
    };
};
