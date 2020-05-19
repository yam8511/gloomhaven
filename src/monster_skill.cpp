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
