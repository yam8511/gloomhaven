#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include "game.h"
#include "util.cpp"
#include "map_data.cpp"
#include "character.cpp"
#include "monster.cpp"
using namespace std;

// 遊戲
class Gloomhaven
{
private:
    map<string, Character *> players; // 角色資料
    map<string, Monster *> monsters;  // 怪物資料

    MapData *mapData;                    // 地圖
    vector<Character *> selectedPlayers; // 選中角色
    int selectedPlayerNum;               // 選擇角色數量
    vector<Monster *> selectedMonsters;  // 選中怪物
    int selectedMonsterNum;              // 選擇怪物數量

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

            Character *player = new Character(name, blood, initCardNum);

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

                player->AddSkill(new CharacterSkill(no, agile, upAction, downAction));
            }

            if (this->debugMode)
                player->ShowMe();

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
            Monster *monster = new Monster(
                name,
                normalBlood, normalAttack, normalAttackRange,
                greatBlood, greatAttack, greatAttackRange);

            for (int j = 0; j < 6; j++)
            {
                int no;     // 號碼
                int agile;  // 數值變數
                bool reset; // 重洗標誌
                vector<string> actions;

                in.getline(buffer, sizeof(buffer)); // 讀取怪物基本資訊
                ss = split(buffer, " ");
                int lastIndex = ss.size() - 1;

                no = stoi(ss[1]);
                agile = stoi(ss[2]);
                reset = ss[lastIndex] == "r";
                for (int k = 3; k < lastIndex; k++)
                    actions.push_back(ss[k]);

                monster->AddSkill(new MonsterSkill(monster, no, agile, actions, reset));
            }

            if (this->debugMode)
                monster->ShowMe();

            this->monsters.insert(pair<string, Monster *>(name, monster));
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
        this->selectedPlayers = vector<Character *>();
        for (int i = 0; i < num; i++)
        {
        RESET:
            printf("玩家#%d : 請決定出場角色與要攜帶的技能卡\n", i + 1);

            vector<string> ss;
            ss = getInputLineSplit();

            if (ss.size() == 0)
                goto RESET;

            string name(ss[0]);
            if (this->players.find(name) == this->players.end())
            {
                cout << "角色找不到: " << name << endl;
                goto RESET; // 表示找不到
            }

            Character *player = this->players[name]->Clone();
            if (player->InitCardNum() != ss.size() - 1)
            {
                cout << "該角色的初始牌數為: " << player->InitCardNum() << endl;
                goto RESET;
            }

            player->SetNo(i);

            for (int j = 1; j < ss.size(); j++)
            {
                try
                {
                    int index = stoi(ss[j]);
                    CharacterSkill *card = player->GetCard(index);
                    if (card == NULL)
                    {
                        cout << "牌組技能找不到: " << index << endl;
                        goto RESET;
                    }
                    player->SelecCard(card);
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

        this->mapData = new MapData(m, n);

        vector<vector<MapObject>> layout;
        for (int i = 0; i < m; i++) // 讀取地圖配置
        {
            in.getline(buffer, sizeof(buffer));
            string ss = trimNewline(string(buffer));

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

        this->mapData->SetLayout(layout);

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
        this->mapData->SetPlayerInitPos(poss);

        in.getline(buffer, sizeof(buffer)); // 讀取怪物數量
        try
        {
            num = stoi(buffer);
            this->selectedMonsterNum = num;
            this->selectedMonsters = vector<Monster *>();
        }
        catch (const std::exception &e)
        {
            cout << "讀取地圖配置錯誤(怪物數量不是數字): " << string(buffer) << endl;
            in.close();
            goto SELECT_MAP;
        }

        vector<Point2d> mops;
        MonsterAppear level = MonsterNone;
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
                level = MonsterAppear(stoi(ss[this->selectedPlayerNum + 1]));
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

            Monster *monster = this->monsters[name]
                                   ->Clone()
                                   ->SetNo(this->selectedMonsters.size())
                                   ->SetLevel(level);
            this->selectedMonsters.push_back(monster);
            mops.push_back(Point2d(x, y));
        }
        this->mapData->SetMonstersPos(mops);

        if (this->debugMode)
            this->mapData->ShowMe();

        in.close();

        // 開始選擇開場位子
        for (int i = 0; i < this->selectedPlayerNum; i++)
        {
            bool interrupt = false;
            while (!interrupt)
            {
                this->mapData->ShowMap();
                cout << "玩家" << i + 1 << ": 選擇開場位子" << endl;
                string line = getInputLine();
                for (int j = 0; j < line.size(); j++)
                {
                    interrupt = this->mapData->MovePlayerInitPos(line[j]);
                    if (interrupt)
                        break;
                }
            }
        }
        this->mapData->FinishInitState();
        if (this->debugMode)
            this->mapData->ShowMe();

        cout << "------------------" << endl;

        this->mapData->ShowMap();
    }

public:
    Gloomhaven(string characterFile, string monsterFile, bool debug = true)
    {
        // Debug模式
        this->setDebugMode(debug);
        this->players = map<string, Character *>();
        this->monsters = map<string, Monster *>();

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
            this->mapData->ShowMap();
            printf("-------------\nround %d:\n", ++round);

            /** 準備階段-角色 **/
            map<string, Character *> liveTeam;
            map<string, Character *> deadTeam;
            vector<readyAction> allAction;
            for (int i = 0; i < this->selectedPlayers.size(); i++)
            {
                // cout << "i -> " << i << ", Alias -> " << this->selectedPlayers[i]->Alias() << " | " << endl;
                // 如果手牌跟棄牌堆小於2，表示陣亡
                if (this->selectedPlayers[i]->Dead())
                    deadTeam[this->selectedPlayers[i]->Alias()] = this->selectedPlayers[i];
                else
                    liveTeam[this->selectedPlayers[i]->Alias()] = this->selectedPlayers[i];
            }

            printf("====== 開始決定角色 ====\n");

            while (liveTeam.size() > 0)
            {
                printf("請決定角色與動作: ");
                vector<string> ss = getInputLineSplit();
                if (ss.size() < 2) // 如果輸入小於2，重新輸入
                    continue;

                if (ss[1] != "-1" && ss[1] != "check" && ss.size() < 3)
                    continue;

                ss[ss.size() - 1] = trimNewline(ss[ss.size() - 1]);

                string name = ss[0];

                if (deadTeam.find(name) != deadTeam.end()) // 如果玩家已經陣亡，則顯示
                {
                    printf("%s is killed!\n", name.c_str());
                    continue;
                }

                if (liveTeam.find(name) == liveTeam.end()) // 如果玩家不存在，也重新輸入
                {
                    bool hasExists = false;
                    for (int i = 0; i < allAction.size(); i++)
                    {
                        if (allAction[i].name == name)
                        {
                            hasExists = true;
                            break;
                        }
                    }

                    if (hasExists)
                        printf("%s 已經決定過!\n", name.c_str());
                    else
                        printf("%s 不存在!\n", name.c_str());

                    continue;
                }

                Character *p = liveTeam[name];

                if (ss[1] == "-1") // 長休
                {
                    if (p->TrashCardSize() < 2)
                    {
                        printf("棄牌堆數量(%d)需要兩張以上，無法長休\n", p->TrashCardSize());
                        continue;
                    }

                    allAction.push_back({
                        .name = p->Alias(),
                        .agile = 99,
                        .action = ActionRest,
                        .p = p,
                    });
                }
                else if (ss[1] == "check") // 確認手牌
                {
                    p->CheckHand();
                    continue;
                }
                else // 出牌
                {
                    CharacterSkill *card1, *card2;

                    card1 = p->GetCardFromStr(ss[1]);
                    if (card1 == NULL)
                    {
                        printf("手牌中沒有%s\n", ss[1].c_str());
                        continue;
                    }

                    card2 = p->GetCardFromStr(ss[2]);
                    if (card2 == NULL)
                    {
                        printf("手牌中沒有%s\n", ss[2].c_str());
                        continue;
                    }

                    allAction.push_back({
                        .name = p->Alias(),
                        .agile = card1->Agile(),
                        .action = ActionCard,
                        .p = p,
                        .s1 = card1,
                        .s2 = card2,
                    });
                }
                liveTeam.erase(name);
            }

            /** 準備階段-怪物 **/
            map<int, Point2d> mons = this->mapData->GetShowMonster(vector<vector<MapObject>>());
            map<int, Point2d>::iterator it;

            for (it = mons.begin(); it != mons.end(); it++)
            {
                int i = it->first;
                if (i >= this->selectedMonsters.size())
                    throw "怪物資訊有問題！";

                Monster *mon = this->selectedMonsters[i];
                MonsterSkill *sk = mon->RandSkill();
                char s[100] = {};
                mon->Name().copy(s, mon->Name().size());
                allAction.push_back({
                    .name = s,
                    .agile = sk->Agile(),
                    .action = ActionMonster,
                    .p = NULL,
                    .s1 = NULL,
                    .s2 = NULL,
                    .sk = sk,
                });
            }

            /** 準備階段-輸出 **/
            map<int, int> playerAgile;
            sort(allAction.begin(), allAction.end(), compareReadyAction);
            cout << endl;
            for (int i = 0; i < allAction.size(); i++)
            {
                readyAction cmd = allAction[i];
                if (cmd.action == ActionMonster)
                {
                    cout
                        << cmd.name << " "
                        << cmd.agile << " "
                        << cmd.sk->Text() << endl;
                }
                else if (cmd.action == ActionCard)
                {
                    playerAgile[cmd.s1->Owner()->No()] = cmd.agile;
                    cout
                        << cmd.name << " "
                        << cmd.agile << " "
                        << cmd.s1->No() << " "
                        << cmd.s2->No() << endl;
                }
                else
                {
                    playerAgile[cmd.s1->Owner()->No()] = cmd.agile;
                    cout
                        << cmd.name << " "
                        << cmd.agile << " -1"
                        << endl;
                }
            }

            /** 動作階段 **/
            cout << endl;
            cout << "======== 動作階段 =========" << endl;
            for (int i = 0; i < allAction.size(); i++)
            {
                readyAction cmd = allAction[i];

                if (cmd.action == ActionMonster) // 怪物動作
                {
                    if (this->debugMode)
                        cout
                            << cmd.name << " "
                            << cmd.agile << " "
                            << cmd.sk->Text() << endl;

                    vector<string> actions = cmd.sk->Actions();
                    for (int i = 0; i < actions.size(); i++)
                    {
                        // cout << "act " << i << " : " << actions[i] << endl;
                        string act = actions[i];
                        if (act == "move") // 移動
                        {
                            i++;
                            // cout << "arrow " << i << " : " << actions[i] << endl;
                            string arrow = actions[i];
                            this->mapData->Move(true, cmd.sk->Owner()->No(), actions[i]);
                            this->mapData->ShowMap();
                        }
                        else if (act == "shield") // 防護
                        {
                            i++;
                            int sh = stoi(actions[i]);
                            cmd.sk->Owner()->AddShield(sh);
                            cout
                                << cmd.sk->Owner()->Alias()
                                << " shield "
                                << sh
                                << " this turn" << endl;
                        }
                        else if (act == "heal") // 治療
                        {
                            i++;
                            int heal = stoi(actions[i]);
                            cmd.sk->Owner()->Heal(heal);
                            cout
                                << cmd.sk->Owner()->Alias()
                                << " heal " << heal
                                << ", now hp is "
                                << cmd.sk->Owner()->HP() << endl;
                        }
                        else if (act == "attack") // 攻擊
                        {
                            i++;
                            int hurt = stoi(actions[i]);
                            int range = 0;
                            if (i + 1 < actions.size() && actions[i + 1] == "range")
                            {
                                i += 2;
                                range = stoi(actions[i]);
                            }

                            vector<int> playersNo = this->mapData->CheckPlayersInMonsterAttackRange(
                                cmd.sk->Owner()->No(), range);

                            if (playersNo.size() > 0)
                            {
                                int targetNo = -1;
                                if (playersNo.size() == 1)
                                {
                                    // 範圍內的角色
                                    targetNo = playersNo[0];
                                }
                                else
                                {
                                    int minAgile = 99;
                                    for (int j = 0; j < playersNo.size(); j++)
                                    {
                                        int no = playersNo[j];
                                        if (playerAgile[no] < minAgile)
                                        {
                                            minAgile = playerAgile[no];
                                            targetNo = no;
                                        }
                                    }
                                }

                                if (targetNo > -1)
                                {
                                    Character *targetPlayer = this->selectedPlayers[targetNo];
                                    if (targetPlayer->Hurt(hurt))
                                        cout << targetPlayer->Alias() << " is killed!!" << endl;
                                }
                            }
                        }
                        else
                        {
                            continue;
                        }

                        cmd.sk->Owner()->Trash(cmd.sk->No());
                    }
                }
                else
                {
                    if (cmd.action == ActionCard) // 出牌
                    {
                        if (this->debugMode)
                            cout
                                << cmd.name << " "
                                << cmd.agile << " "
                                << cmd.s1->No() << " "
                                << cmd.s2->No() << endl;

                        if (cmd.p->Dead())
                            continue;

                        cout << cmd.p->Alias()
                             << "'s turn: card "
                             << cmd.s1->No() << " "
                             << cmd.s2->No() << endl;

                        bool ok = false;
                        char first = '\0', second = '\0';
                        while (!ok)
                        {
                            string input = getInputLine();
                            input = trimNewline(input);
                            cout << "動作 -> " << input << endl;
                            if (input == "check")
                            {
                                for (int j = 0; j < this->selectedPlayers.size(); j++)
                                {
                                    if (this->selectedPlayers[j]->Dead())
                                    {
                                        continue;
                                    }
                                    cout << this->selectedPlayers[j]->Alias()
                                         << "-hp: "
                                         << this->selectedPlayers[j]->HP()
                                         << " , shield: "
                                         << this->selectedPlayers[j]->Shield()
                                         << endl;
                                }

                                continue;
                            }
                            else if (*to_string(cmd.s1->No()).c_str() == input[0]) // 使用第一張
                            {
                                first = input[1];
                                if (first == 'u')
                                    second = 'd';
                                else
                                    second = 'u';
                            }
                            else if (*to_string(cmd.s2->No()).c_str() == input[0]) // 使用第一張
                            {
                                first = input[1];
                            }
                            else
                            {
                                continue;
                            }
                            ok = true;
                            if (first == 'u')
                                second = 'd';
                            else
                                second = 'u';

                            // TODO: 選擇手牌與執行動作
                        }
                    }
                    else // 長休
                    {
                        if (this->debugMode)
                            cout
                                << cmd.name << " "
                                << cmd.agile << " -1"
                                << endl;

                        if (cmd.p->Dead())
                            continue;

                        int cardNo = cmd.p->TakeLongRest();

                        cout << cmd.p->Alias()
                             << " heal 2, now hp is "
                             << cmd.p->HP() << endl
                             << "remove card: "
                             << cardNo << endl;
                    }
                }
            }

            /** 結算階段 **/
        }
    }
};
