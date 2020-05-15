#include <string>
#include <vector> // std::vector, std::slice
#include "character_skill.cpp"
using namespace std;

// 角色
class Character
{
private:
    string name;                   // 名字
    int blood;                     // 血量
    int initCardNum;               // 起始手牌數量
    vector<CharacterSkill> skills; // 技能總表

public:
    Character(string name, int blood, int initCardNum)
    {
        this->name = name;
        this->blood = blood;
        this->initCardNum = initCardNum;
        this->skills = vector<CharacterSkill>();
    };

    void AddSkill(int agile, vector<string> upActions, vector<string> downActions)
    {
        this->skills.push_back(CharacterSkill(agile, upActions, downActions));
    };

    void ShowMe()
    {
        cout << "===========" << endl;
        printf("角色: 名字: %s, 血量: %d, 初始手牌: %d\n", this->name.c_str(), this->blood, this->initCardNum);
        for (int i = 0; i < this->skills.size(); i++)
        {
            this->skills[i].ShowMe();
        }
    }
};
