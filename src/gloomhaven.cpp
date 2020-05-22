#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "util.cpp"
#include "character.cpp"
#include "monster.cpp"
#include "map_data.cpp"
using namespace std;

// 遊戲
class Gloomhaven
{
private:
    map<string, Character> players; // 角色資料
    map<string, Monster> monsters;  // 怪物資料

    MapData mapData;                   // 地圖
    vector<Character> selectedPlayers; // 選中角色
    int selectedPlayerNum;             // 選擇角色數量
    vector<Monster> selectedMonsters;  // 選中怪物
    int selectedMonsterNum;            // 選擇怪物數量
    vector<int> eachMonsterLevel;      // 每一隻怪物的等級

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

            this->players[name] = player;
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
        vector<string> ss;
        char buffer[200];

        in.getline(buffer, sizeof(buffer)); // 讀取怪物數量
        int num = stoi(buffer);             // 怪物數量

        for (int i = 0; i < num; i++)
        {
            in.getline(buffer, sizeof(buffer)); // 讀取怪物基本資訊
            ss = split(buffer, " ");

            name = ss[0];                    // 怪物名字
            normalBlood = stoi(ss[1]);       // 普通等級之血量
            normalAttack = stoi(ss[2]);      // 普通等級之攻擊傷害
            normalAttackRange = stoi(ss[3]); // 普通等級之射程
            greatBlood = stoi(ss[4]);        // 菁英等級之血量
            greatAttack = stoi(ss[5]);       // 菁英等級之攻擊傷害
            greatAttackRange = stoi(ss[6]);  // 菁英等級之射程
            Monster monster = Monster(name, normalBlood, normalAttack, normalAttackRange,
                                      greatBlood, greatAttack, greatAttackRange);

            for (int j = 0; j < 6; j++)
            {
                int agile;  // 數值變數
                bool reset; // 重洗標誌
                vector<string> actions;

                in.getline(buffer, sizeof(buffer)); // 讀取怪物基本資訊
                ss = split(buffer, " ");
                int lastIndex = ss.size() - 1;
                agile = stoi(ss[2]);
                reset = ss[lastIndex] == "r";
                for (int k = 3; k < lastIndex; k++)
                    actions.push_back(ss[k]);

                monster.AddSkill(agile, actions, reset);
            }

            if (this->debugMode)
                monster.ShowMe();

            this->monsters.insert(pair<string, Monster>(name, monster));
        }
        in.close(); // 關閉檔案
    }

public:
    Gloomhaven(string characterFile, string monsterFile, bool debug = true)
    {
        // Debug模式
        this->setDebugMode(debug);
        this->players = map<string, Character>();
        this->monsters = map<string, Monster>();

        /*
         * 讀取角色設定檔
         */
        this->loadCharacter(characterFile);

        /**
         * 讀取怪物設定檔
         */
        this->loadMonster(monsterFile);
    };

    void Run()
    {
        char buffer[200];
        string tmp;
        int num;

    SELECT_PLAY_NUM:
        cout << "玩家輸入角色數量 (2~4)" << endl;
        tmp = getInputLine();
        try
        {
            num = stoi(tmp);
        }
        catch (const std::exception &e)
        {
            num = 0;
        }
        if (num < 2 || num > 4)
            goto SELECT_PLAY_NUM;

        this->selectedPlayerNum = num;
        this->selectedPlayers = vector<Character>();
        for (int i = 0; i < num; i++)
        {
        RESET:
            printf("玩家#%d : 請決定出場角色與要攜帶的技能卡\n", i + 1);

            vector<string> ss = getInputLineSplit();
            if (ss.size() == 0)
                goto RESET;

            string name = ss[0];
            if (this->players.find(name) == this->players.end())
            {
                cout << "角色找不到: " << name << endl;
                goto RESET; // 表示找不到
            }

            Character player = this->players[name];
            if (player.InitCardNum() != ss.size() - 1)
            {
                cout << "該角色的初始牌數為: " << player.InitCardNum() << endl;
                goto RESET;
            }

            for (int j = 1; j < ss.size(); j++)
            {
                try
                {
                    int index = stoi(ss[j]);
                    CharacterSkill *card = player.GetCard(index);
                    if (card == NULL)
                    {
                        cout << "牌組技能找不到: " << index << endl;
                        goto RESET;
                    }
                }
                catch (const std::exception &e)
                {
                    goto RESET;
                }
            }
            this->selectedPlayers.push_back(player);
        }

    SELECT_MAP:
        cout << "玩家輸入地圖名稱" << endl;
        tmp = getInputLine();
        fstream in(tmp);
        if (!in)
        {
            cout << "讀取地圖設定檔失敗" << endl;
            goto SELECT_MAP;
        }

        in.getline(buffer, sizeof(buffer)); // 讀取地圖大小
        vector<string> ss = split(buffer, " ");
        if (ss.size() != 2)
        {
            cout << "讀取地圖大小格式錯誤(請輸入 m n): " << string(buffer) << endl;
            in.close();
            goto SELECT_MAP;
        }

        int m, n;
        try
        {
            m = stoi(ss[0]);
            n = stoi(ss[1]);
        }
        catch (const std::exception &e)
        {
            cout << "讀取地圖大小格式錯誤(請輸入數字): " << string(buffer) << endl;
            in.close();
            goto SELECT_MAP;
        }

        this->mapData = MapData(m, n);

        vector<vector<MapObject>> layout;
        for (int i = 0; i < m; i++) // 讀取地圖配置
        {
            in.getline(buffer, sizeof(buffer));
            string ss = string(buffer);
            if (ss.size() != n)
            {
                cout << "讀取地圖配置錯誤(不符合地圖大小): " << string(buffer) << endl;
                in.close();
                goto SELECT_MAP;
            }

            vector<MapObject> objs;
            for (int j = 0; j < n; j++)
            {
                MapObject x = parseMapObject(ss[j]);
                if (x == None)
                {
                    cout << "讀取地圖配置錯誤(地圖編號有錯): " << ss[j] << endl;
                    in.close();
                    goto SELECT_MAP;
                }
                objs.push_back(x);
            }

            layout.push_back(objs);
        }

        this->mapData.SetLayout(layout);

        in.getline(buffer, sizeof(buffer)); // 讀取初始選擇位子
        ss = split(buffer, " ");
        if (ss.size() % 2 != 0)
        {
            cout << "讀取地圖配置錯誤(初始位置格式錯誤): " << string(buffer) << endl;
            in.close();
            goto SELECT_MAP;
        }

        vector<Point2d> poss;
        for (int i = 0; i < ss.size(); i += 2)
        {
            try
            {
                int x = stoi(ss[i]);
                int y = stoi(ss[i + 1]);
                Point2d p = Point2d(x, y);
                if (this->debugMode)
                    p.ShowMe();
                poss.push_back(p);
            }
            catch (const std::exception &e)
            {
                cout << "讀取地圖配置錯誤(初始位置不是數字): " << string(buffer) << endl;
                in.close();
                goto SELECT_MAP;
            }
        }
        this->mapData.SetPlayerInitPos(poss);

        in.getline(buffer, sizeof(buffer)); // 讀取怪物數量
        try
        {
            num = stoi(buffer);
            this->selectedMonsterNum = num;
            this->selectedMonsters = vector<Monster>();
            this->eachMonsterLevel = vector<int>();
        }
        catch (const std::exception &e)
        {
            cout << "讀取地圖配置錯誤(怪物數量不是數字): " << string(buffer) << endl;
            in.close();
            goto SELECT_MAP;
        }

        vector<Point2d> mops;
        for (int i = 0; i < this->selectedMonsterNum; i++)
        {
            in.getline(buffer, sizeof(buffer)); // 讀取怪物數量配置
            ss = split(buffer, " ");
            if (ss.size() != 6)
            {
                cout << "讀取地圖配置錯誤(怪物資訊格式錯誤): " << string(buffer) << endl;
                in.close();
                goto SELECT_MAP;
            }

            string name = ss[0];
            int x, y;

            if (this->monsters.find(name) == this->monsters.end())
            {
                cout << "讀取地圖配置錯誤(怪物找不到): " << name << endl;
                in.close();
                goto SELECT_MAP;
            }

            try
            {
                x = stoi(ss[1]);
                y = stoi(ss[2]);
            }
            catch (const std::exception &e)
            {
                cout << "讀取地圖配置錯誤(怪物座標錯誤): (" << ss[1] << "," << ss[2] << ")" << endl;
                in.close();
                goto SELECT_MAP;
            }

            try
            {
                num = stoi(ss[this->selectedPlayerNum + 1]);
            }
            catch (const std::exception &e)
            {
                cout << "讀取地圖配置錯誤(怪物數量非數字): " << ss[this->selectedPlayerNum + 1] << endl;
                in.close();
                goto SELECT_MAP;
            }

            Monster monster = this->monsters[name];
            this->selectedMonsters.push_back(monster);
            mops.push_back(Point2d(x, y));
            this->eachMonsterLevel.push_back(num);
        }
        this->mapData.SetMonstersPos(mops);

        if (this->debugMode)
            this->mapData.ShowMe();

        in.close();

        // 開始選擇開場位子
        for (int i = 0; i < this->selectedPlayerNum; i++)
        {
            bool interrupt = false;
            cout << "玩家" << i + 1 << ": 選擇開場位子" << endl;
            while (!interrupt)
            {
                string line = getInputLine();
                for (int j = 0; j < line.size(); j++)
                {
                    interrupt = this->mapData.MovePlayerInitPos(line[j]);
                    this->mapData.ShowMe();
                    if (interrupt)
                        break;
                }
            }
        }
        this->mapData.FinishInitState();
        this->mapData.ShowMe();
    }
};
