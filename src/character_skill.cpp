#include <iostream>
#include <string>
#include <vector>
#include "game.h"
using namespace std;

// 角色技能卡資訊
class CharacterSkill
{
private:
    int no;                                // 編號
    int agile;                             // 敏捷
    vector<string> upActions, downActions; // 動作
    Character *owner;                      // 擁有者

public:
    CharacterSkill(int no, int agile, vector<string> upActions, vector<string> downActions)
    {
        this->no = no;
        this->agile = agile;
        this->upActions = upActions;
        this->downActions = downActions;
        this->owner = NULL;
    };

    int No() { return this->no; }
    int Agile() { return this->agile; }
    void ChangePlayer(Character *player) { this->owner = player; }

    void ShowMe()
    {
        cout << "-------------------" << endl;
        printf("敏捷: %d, ", this->agile);
        cout << "上半部動作: " << endl;
        for (int i = 0; i < this->upActions.size(); i++)
        {
            string action = this->upActions[i];
            cout << action << ", " << ends;
        }
        cout << endl;
        cout << "下半部動作: " << endl;
        for (int i = 0; i < this->downActions.size(); i++)
        {
            string action = this->downActions[i];
            cout << action << ", " << ends;
        }
        cout << endl;
    }
};
