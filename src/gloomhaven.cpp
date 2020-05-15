#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <vector>
#include "character.cpp"
#include "monster.cpp"
#include "util.cpp"
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
            PrintAndExit("讀取角色設定檔失敗");

        string name;                      // 字串變數
        int blood, initCardNum, cardNum;  // 數值變數
        int lastIndex = -1, index, agile; // 數值變數
        char buffer[200];
        in.getline(buffer, sizeof(buffer)); // 讀取角色數量
        int num = stoi(buffer);             // 角色數量

        for (int i = 0; i < num; i++)
        {
            in.getline(buffer, sizeof(buffer));
            vector<string> ss = split(buffer, " ");
            if (ss.size() != 3)
                PrintAndExit("解析角色基本資訊失敗: " + string(buffer));

            name = ss[0];              // 讀取名字
            blood = stoi(ss[1]);       // 讀取血量設定
            initCardNum = stoi(ss[2]); // 讀取初始牌數

            Character player = Character(name, blood, initCardNum);

            in.getline(buffer, sizeof(buffer)); // 讀取牌數
            cardNum = stoi(buffer);

            for (int j = 0; j < cardNum; j++)
            {
                in.getline(buffer, sizeof(buffer)); // 讀取牌資料
                ss = split(buffer, " ");

                bool pickUpAction = true;
                agile = stoi(ss[1]); // 讀取敏捷資訊
                vector<string> upAction = vector<string>();
                vector<string> downAction = vector<string>();

                for (int k = 2; k < ss.size(); k++)
                {
                    if (ss[k] == "-")
                        pickUpAction = false;
                    else if (pickUpAction)
                        upAction.push_back(ss[k]);
                    else
                        downAction.push_back(ss[k]);
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
            PrintAndExit("讀取怪物設定檔失敗");

        string name;                                      // 字串變數
        int normalBlood, normalAttack, normalAttackRange; // 普通等級之血量、攻擊傷害、射程
        int greatBlood, greatAttack, greatAttackRange;    // 菁英等級之血量、攻擊傷害、射程
        int agile;                                        // 數值變數
        char buffer[200];

        in.getline(buffer, sizeof(buffer)); // 讀取怪物數量
        int num = stoi(buffer);             // 怪物數量

        for (int i = 0; i < num; i++)
        {
            in.getline(buffer, sizeof(buffer)); // 讀取怪物基本資訊
            vector<string> ss = split(buffer, " ");

            name = ss[0];                    // 怪物名字
            normalBlood = stoi(ss[1]);       // 普通等級之血量
            normalAttack = stoi(ss[2]);      // 普通等級之攻擊傷害
            normalAttackRange = stoi(ss[3]); // 普通等級之射程
            greatBlood = stoi(ss[4]);        // 菁英等級之血量
            greatAttack = stoi(ss[5]);       // 菁英等級之攻擊傷害
            greatAttackRange = stoi(ss[6]);  // 菁英等級之射程
            Monster monster(name, normalBlood, normalAttack, normalAttackRange,
                            greatBlood, greatAttack, greatAttackRange);

            // Character player = Character(name, blood, initCardNum);
            // this->players.push_back(player);
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
