#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 怪物技能卡資訊
class MonsterSkill
{
private:
    int agile;              // 敏捷
    vector<string> actions; // 動作
    bool reset;             // 重洗標誌

public:
    MonsterSkill(int agile, vector<string> actions, bool reset)
    {
        this->agile = agile;
        this->actions = actions;
        this->reset = reset;
    };

    int Agile() { return this->agile; }
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
