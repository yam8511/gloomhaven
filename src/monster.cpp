#include <string>
#include <vector>
#include "monster_skill.cpp"
using namespace std;

// 怪物
class Monster
{
private:
    int no;                                           // 號碼
    MonsterAppear level;                              // 等級
    string name;                                      // 名字
    int normalBlood, normalAttack, normalAttackRange; // 普通等級之血量、普通等級之攻擊傷害、普通等級之射程
    int greatBlood, greatAttack, greatAttackRange;    // 菁英等級之血量、菁英等級之攻擊傷害、菁英等級之射程
    vector<MonsterSkill *> skills;                    // 技能總表
    vector<CharacterSkill *> trash;                   // 丟棄手牌
public:
    Monster(string name, int normalBlood = 0, int normalAttack = 0, int normalAttackRange = 0,
            int greatBlood = 0, int greatAttack = 0, int greatAttackRange = 0)
    {
        this->name = name;
        this->normalBlood = normalBlood;
        this->normalAttack = normalAttack;
        this->normalAttackRange = normalAttackRange;
        this->greatBlood = greatBlood;
        this->greatAttack = greatAttack;
        this->greatAttackRange = greatAttackRange;
        this->skills = vector<MonsterSkill *>();
    }

    Monster *Clone()
    {
        Monster *m = new Monster(
            this->name,
            this->normalBlood, this->normalAttack, this->normalAttackRange,
            this->greatBlood, this->greatAttack, this->greatAttackRange);
        for (int i = 0; i < this->skills.size(); i++)
        {
            MonsterSkill *sk1 = this->skills[i];
            vector<string> act1 = sk1->Actions();
            vector<string> act2;
            for (int j = 0; j < act1.size(); j++)
                act2.push_back(act1[j]);

            MonsterSkill *sk2 = new MonsterSkill(m, i, sk1->Agile(), act2, sk1->Reset());
            m->AddSkill(sk2);
        }

        return m;
    }

    int No() { return this->no; }
    string Name() { return this->name; }

    Monster *SetNo(int no)
    {
        this->no = no;
        return this;
    }

    Monster *SetLevel(MonsterAppear level)
    {
        this->level = level;
        return this;
    }

    Monster *AddSkill(MonsterSkill *sk)
    {
        this->skills.push_back(sk);
        return this;
    }
    MonsterSkill *RandSkill()
    {
        return this->skills[0];
        // return this->skills[randInt(this->skills.size())];
    }

    void ShowMe()
    {
        cout << "===========" << endl;
        cout << "怪物: 名字: " << this->name << ends;
        printf(", 普通血量: %d, 普通傷害%d, 普通射程: %d, 菁英血量: %d, 菁英傷害%d, 菁英射程: %d\n",
               this->normalBlood, this->normalAttack, this->normalAttackRange,
               this->greatBlood, this->greatAttack, this->greatAttackRange);
        for (int i = 0; i < this->skills.size(); i++)
        {
            this->skills[i]->ShowMe();
        }
    }
};
