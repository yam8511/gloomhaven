#include <string>
#include <vector> // std::vector, std::slice
#include "monster_skill.cpp"
using namespace std;

// 角色
class Monster
{
private:
    // 包含怪物名稱、、
    string name;                                      // 名字
    int normalBlood, normalAttack, normalAttackRange; // 普通等級之血量、普通等級之攻擊傷害、普通等級之射程
    int greatBlood, greatAttack, greatAttackRange;    // 菁英等級之血量、菁英等級之攻擊傷害、菁英等級之射程
    vector<MonsterSkill> skills;                      // 技能總表

public:
    Monster(string name, int normalBlood, int normalAttack, int normalAttackRange,
            int greatBlood, int greatAttack, int greatAttackRange)
    {
        this->name = name;
        this->normalBlood = normalBlood;
        this->normalAttack = normalAttack;
        this->normalAttackRange = normalAttackRange;
        this->greatBlood = greatBlood;
        this->greatAttack = greatAttack;
        this->greatAttackRange = greatAttackRange;
        this->skills = vector<MonsterSkill>();
    };

    void AddSkill(int agile, vector<string> actions, bool reset)
    {
        this->skills.push_back(MonsterSkill(agile, actions, reset));
    };

    void ShowMe()
    {
        cout << "===========" << endl;
        printf("怪物: 名字: %s, 普通血量: %d, 普通傷害%d, 普通射程: %d, 菁英血量: %d, 菁英傷害%d, 菁英射程: %d\n",
               this->name.c_str(),
               this->normalBlood, this->normalAttack, this->normalAttackRange,
               this->greatBlood, this->greatAttack, this->greatAttackRange);
        for (int i = 0; i < this->skills.size(); i++)
        {
            this->skills[i].ShowMe();
        }
    }
};
