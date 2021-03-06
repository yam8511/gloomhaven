#include <iostream>
#include <string>
#include <vector>
#include "game.h"
using namespace std;

// 怪物技能卡資訊
class MonsterSkill
{
private:
    Monster *ownner;        // 怪物
    int no, agile;          // 號碼, 敏捷
    vector<string> actions; // 動作
    bool reset;             // 重洗標誌

public:
    MonsterSkill(Monster *m, int no, int agile, vector<string> actions, bool reset)
    {
        this->ownner = m;
        this->no = no;
        this->agile = agile;
        this->actions = actions;
        this->reset = reset;
    };

    int Agile() { return this->agile; }
    int No() { return this->no; }
    bool Reset() { return this->reset; }
    vector<string> Actions()
    {
        vector<string> acts;
        for (int i = 0; i < this->actions.size(); i++)
            acts.push_back(this->actions[i]);

        return acts;
    }

    Monster *Owner() { return this->ownner; }

    string Text()
    {
        string s = "";
        for (int i = 0; i < this->actions.size(); i++)
        {
            if (this->actions[i] == "move")
            {
                s += this->actions[i] + " ";
                i++;
                s += to_string(this->actions[i].length()) + " ";
            }
            else
            {
                s += this->actions[i] + " ";
            }
        }

        return s;
    }

    void ShowMe()
    {
        cout << "-------------------" << endl;
        printf("敏捷: %d, ", this->agile);
        if (this->reset)
            cout << "有重洗標誌, " << endl;
        else
            cout << "沒重洗標誌, " << endl;
        cout << "動作: " << ends;
        for (int i = 0; i < this->actions.size(); i++)
        {
            string action = this->actions[i];
            cout << action << ", " << ends;
        }
        cout << endl;
    }
};
