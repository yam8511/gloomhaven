#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
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

    MapData mapData;                        // 地圖
    vector<Character> selectedPlayers;      // 選中角色
    int selectedPlayerNum;                  // 選擇角色數量
    vector<Monster> selectedMonsters;       // 選中怪物
    int selectedMonsterNum;                 // 選擇怪物數量
    vector<MonsterAppear> eachMonsterLevel; // 每一隻怪物的等級

    bool debugMode;

    void setDebugMode(bool mode)
    {
        this->debugMode = mode;
    }

    void loadCharacter(string characterFile) // 載入角色資料
    {
        /**
         * 讀取角色設定檔
         */
        fstream in(characterFile);
        if (!in)
            PrintAndExit("讀取角色設定檔失敗");

        string name;                          // 字串變數
        int blood, initCardNum, cardNum;      // 數值變數
        int lastIndex = -1, index, agile, no; // 數值變數
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
                no = stoi(ss[0]);    // 讀取敏捷資訊
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

                player.AddSkill(CharacterSkill(no, agile, upAction, downAction));
            }

            if (this->debugMode)
                player.ShowMe();

            this->players[name] = player;
        }
        in.close(); // 關閉檔案
    }

    void loadMonster(string monsterFile) // 載入怪物資料
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

                monster.AddSkill(MonsterSkill(agile, actions, reset));
            }

            if (this->debugMode)
                monster.ShowMe();

            this->monsters.insert(pair<string, Monster>(name, monster));
        }
        in.close(); // 關閉檔案
    }

    void inputPlayers() // 玩家輸入角色數量
    {
        char buffer[200];
        string tmp;
        int num;

    SELECT_PLAY_NUM:
        cout << "玩家輸入角色數量 (2~4)" << endl;
        if (this->debugMode)
        {
            num = 2;
        }
        else
        {
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
        }

        this->selectedPlayerNum = num;
        this->selectedPlayers = vector<Character>();
        for (int i = 0; i < num; i++)
        {
        RESET:
            printf("玩家#%d : 請決定出場角色與要攜帶的技能卡\n", i + 1);

            vector<string> ss;
            if (this->debugMode)
            {
                ss.push_back("brute");
                ss.push_back("1");
                ss.push_back("2");
                ss.push_back("3");
                ss.push_back("4");
                ss.push_back("5");
                ss.push_back("6");
            }
            else
            {
                ss = getInputLineSplit();
            }

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
                    player.SelecCard(*card);
                }
                catch (const std::exception &e)
                {
                    goto RESET;
                }
            }
            this->selectedPlayers.push_back(player);
        }
    }

    void loadMap() // 玩家輸入地圖
    {
        char buffer[200];
        string tmp;
        int num = 0;

    SELECT_MAP:
        cout << "玩家輸入地圖名稱" << endl;
        if (this->debugMode)
            tmp = "map1.txt";
        else
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
        int level = 0;
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

            try
            {
                level = stoi(ss[this->selectedPlayerNum + 1]);
            }
            catch (const std::exception &e)
            {
                cout << "讀取地圖配置錯誤(怪物等級非數字): " << ss[this->selectedPlayerNum + 1] << endl;
                in.close();
                goto SELECT_MAP;
            }

            if (level == MonsterNone)
                continue;

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

            Monster monster = this->monsters[name];
            this->selectedMonsters.push_back(monster);
            mops.push_back(Point2d(x, y));
            this->eachMonsterLevel.push_back(level);
        }
        this->mapData.SetMonstersPos(mops);

        if (this->debugMode)
            this->mapData.ShowMe();

        in.close();

        // 開始選擇開場位子
        for (int i = 0; i < this->selectedPlayerNum; i++)
        {
            bool interrupt = false;
            while (!interrupt)
            {
                this->mapData.ShowMap();
                cout << "玩家" << i + 1 << ": 選擇開場位子" << endl;
                string line = getInputLine();
                for (int j = 0; j < line.size(); j++)
                {
                    interrupt = this->mapData.MovePlayerInitPos(line[j]);
                    if (interrupt)
                        break;
                }
            }
        }
        this->mapData.FinishInitState();
        if (this->debugMode)
            this->mapData.ShowMe();

        cout << "------------------" << endl;

        this->mapData.ShowMap();
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
        this->inputPlayers();
        this->loadMap();

        bool gameOver = false;
        int round = 0;
        while (!gameOver)
        {
            this->mapData.ShowMap();
            printf("-------------\nround %d:\n", ++round);

            /** 準備階段-角色 **/
            map<int, Character> liveTeam;
            map<int, Character> deadTeam;
            vector<readyAction> allAction;
            for (int i = 0; i < this->selectedPlayers.size(); i++)
            {
                // 如果手牌跟棄牌堆小於2，表示陣亡
                if (this->selectedPlayers[i].Dead())
                    deadTeam[i] = this->selectedPlayers[i];
                else
                    liveTeam[i] = this->selectedPlayers[i];
            }

            while (liveTeam.size() > 0)
            {
                printf("請決定角色與動作: ");
                vector<string> ss = getInputLineSplit();
                if (ss.size() < 2) // 如果輸入小於2，重新輸入
                    continue;

                if (ss[1] != "-1" && ss[1] != "check" && ss.size() < 3)
                    continue;

                char name = *ss[0].c_str();
                if (int(name) > 90 || int(name) < 65)
                    continue;

                int index = int(name) - int('A');
                if (this->debugMode)
                    printf("---> 選擇名稱: %c\n", name);

                if (deadTeam.find(index) != deadTeam.end()) // 如果玩家已經陣亡，則顯示
                {
                    printf("%c is killed!\n", name);
                    continue;
                }

                if (liveTeam.find(index) == liveTeam.end()) // 如果玩家不存在，也重新輸入
                {
                    bool hasExists = false;
                    for (int i = 0; i < allAction.size(); i++)
                    {
                        if (allAction[i].index == i)
                        {
                            hasExists = true;
                            break;
                        }
                    }

                    if (hasExists)
                        printf("%c 已經決定過!\n", name);
                    else
                        printf("%c 不存在!\n", name);

                    continue;
                }

                Character p = liveTeam[index];

                string action = ss[1];
                if (this->debugMode)
                    printf("---> 選擇動作: %s\n", action.c_str());
                if (action == "-1") // 長休
                {
                    if (p.TrashCardSize() < 2)
                    {
                        printf("棄牌堆數量(%d)需要兩張以上，無法長休\n", p.TrashCardSize());
                        continue;
                    }

                    allAction.push_back({
                        .index = index,
                        .agile = 99,
                        .action = ActionRest,
                    });
                }
                else if (action == "check") // 確認手牌
                {
                    p.CheckHand();
                    continue;
                }
                else // 出牌
                {
                    CharacterSkill *card1 = NULL, *card2 = NULL;

                    card1 = p.GetCardFromStr(action);
                    if (card1 == NULL)
                    {
                        printf("手牌中沒有%s\n", action.c_str());
                        continue;
                    }

                    card2 = p.GetCardFromStr(ss[2]);
                    if (card2 == NULL)
                    {
                        printf("手牌中沒有%s\n", ss[2].c_str());
                        continue;
                    }

                    if (this->debugMode)
                        printf("---> 選擇卡牌: %d %d\n", card1->No(), card2->No());

                    allAction.push_back({
                        .index = index,
                        .agile = p.Agile() + card1->Agile(),
                        .action = ActionCard,
                        .card1 = card1,
                        .card2 = card2,
                    });
                }
                liveTeam.erase(index);
            }

            /** 準備階段-怪物 **/
            map<int, Point2d> mons = this->mapData.GetShowMonster(NULL);
            map<int, Point2d>::iterator it;

            for (it = mons.begin(); it != mons.end(); it++)
            {
                printf("<-- 怪物 i: %d\n", it->first);
                int i = it->first;
                if (i >= this->selectedMonsters.size())
                    throw "怪物資訊有問題！";

                Monster mon = this->selectedMonsters[i];
                printf("<-- 怪物: %s", mon.Name().c_str());
                MonsterSkill sk = mon.RandSkill();
                printf("<-- 怪物技能: %s\n", sk.Text().c_str());
                allAction.push_back({
                    .index = i,
                    .agile = sk.Agile(),
                    .action = ActionMonster,
                    .card1 = NULL,
                    .card2 = NULL,
                    .sk = &sk,
                });
            }

            /** 準備階段-輸出 **/
            sort(allAction.begin(), allAction.end(), compareReadyAction);
            for (int i = 0; i < allAction.size(); i++)
            {
                readyAction cmd = allAction[i];
                if (cmd.action == ActionMonster)
                {
                    printf(
                        "%s %d %s\n",
                        this->selectedMonsters[cmd.index].Name().c_str(),
                        cmd.agile,
                        cmd.sk->Text().c_str());
                }
                else if (cmd.action == ActionCard)
                {
                    printf(
                        "%c %d %d %d\n",
                        char(cmd.index + int('A')),
                        cmd.agile,
                        cmd.card1->No(),
                        cmd.card2->No());
                }
                else
                {
                    printf("%c %d\n",
                           char(cmd.index + int('A')),
                           cmd.agile);
                }
            }

            printf("＝＝＝＝＝＝ 動作階段 ＝＝＝＝＝＝\n");
            /** 動作階段 **/
            for (int i = 0; i < allAction.size(); i++)
            {
                readyAction cmd = allAction[i];
                if (cmd.action == ActionMonster) // 怪物動作
                {
                }
                else if (cmd.action == ActionRest) // 長休
                {
                }
                else if (cmd.action == ActionCard) // 出牌
                {
                }
            }

            /** 結算階段 **/
        }
    }
};
