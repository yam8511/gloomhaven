#include <string>
#include <vector> // std::vector, std::slice
#include "character_skill.cpp"
using namespace std;

// 角色
class Character
{
private:
    string name;                     // 名字
    int no;                          // 號碼
    int blood, shield;               // 血量
    int initCardNum;                 // 起始手牌數量
    vector<CharacterSkill *> skills; // 技能總表
    vector<CharacterSkill *> hand;   // 手牌
    vector<CharacterSkill *> trash;  // 丟棄手牌

    string discardStr()
    {
        string s = "";
        if (this->trash.size() > 0)
        {
            s += to_string(this->trash[0]->No());
            for (int i = 1; i < this->trash.size(); i++)
            {
                s += " ," + to_string(this->trash[i]->No());
            }
        }
        return s;
    }

public:
    Character(string name, int blood, int initCardNum)
    {
        this->name = name;
        this->blood = blood;
        this->shield = 0;
        this->initCardNum = initCardNum;
        this->skills = vector<CharacterSkill *>();
        this->hand = vector<CharacterSkill *>();
        this->trash = vector<CharacterSkill *>();
    };

    Character *Clone()
    {
        Character *newCharacter = new Character(
            this->name,
            this->blood,
            this->initCardNum);
        for (int i = 0; i < this->skills.size(); i++)
        {
            newCharacter->skills.push_back(this->skills[i]->Clone());
        }

        return newCharacter;
    }
    void SetNo(int i) { this->no = i; }
    void AddShield(int n)
    {
        this->shield += n;
    }
    int No() { return this->no; }
    bool Hurt(int hp)
    {
        this->blood = this->blood - hp;
        if (this->blood < 0)
            this->blood = 0;

        return this->blood == 0; // 回傳有沒有陣亡
    }
    int HP() { return this->blood; }
    int Shield() { return this->shield; }

    string Alias()
    {
        return getPlayerCode(this->no);
    }

    void AddSkill(CharacterSkill *skill)
    {
        this->skills.push_back(skill);
    };

    int InitCardNum() { return this->initCardNum; }

    CharacterSkill *GetCard(int no)
    {
        for (int i = 0; i < this->skills.size(); i++)
        {
            if (this->skills[i]->No() == no)
                return this->skills[i];
        }

        return NULL;
    }

    CharacterSkill *GetCardFromStr(string no)
    {
        for (int i = 0; i < this->skills.size(); i++)
        {
            if (to_string(this->skills[i]->No()) == no)
                return this->skills[i];
        }

        return NULL;
    }

    int TrashCardSize() { return this->trash.size(); }
    int HandCardSize() { return this->hand.size(); }
    bool Dead()
    {
        return this->blood == 0 ||
               (this->TrashCardSize() < 2 &&
                this->HandCardSize() < 2);
    }

    // 選卡
    void SelecCard(CharacterSkill *selectedCard)
    {
        selectedCard->ChangePlayer(this);
        this->hand.push_back(selectedCard);
    }

    void CheckHand()
    {
        string s = "hand: ";
        if (this->hand.size() > 0)
        {
            s += to_string(this->hand[0]->No());
            for (int i = 1; i < this->hand.size(); i++)
            {
                s += " ," + to_string(this->hand[i]->No());
            }
        }

        s += ";discard: " + this->discardStr();
        printf("%s\n", s.c_str());
    }

    int TakeLongRest() // 長休
    {
        this->blood += 2;

        // 從棄牌堆中刪除一張，其他加回手牌
        do
        {
            printf("長休: 請從棄牌堆[%s]捨棄一張牌，其餘加入手牌:", this->discardStr().c_str());
            string s = getInputLine();
            for (int i = 0; i < this->trash.size(); i++)
            {
                if (to_string(this->trash[i]->No()) == s)
                {
                    this->hand.push_back(this->trash[i]);
                    this->trash.erase(this->trash.begin() + i);
                    return this->trash[i]->No();
                }
            }
        } while (true);
    }

    void ShowMe()
    {
        cout << "===========" << endl;
        printf("角色: 名字: %s, 血量: %d, 初始手牌: %d\n", this->name.c_str(), this->blood, this->initCardNum);
        for (int i = 0; i < this->skills.size(); i++)
        {
            this->skills[i]->ShowMe();
        }
    }
};
