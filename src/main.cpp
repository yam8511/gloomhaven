#include <iostream>
#include <string>
#include "gloomhaven.cpp"
using namespace std;

int main(int argc, char const *argv[])
{
    /**
     * 顯示輸入參數
     */
    cout << "===================" << endl;
    for (int i = 0; i < argc; i++)
        cout << argv[i] << endl;
    cout << "===================" << endl;

    string characterFile = string(argv[1]);
    string monsterFile = string(argv[2]);
    bool debugModel = string(argv[3]) == "1";
    string tmp;

    while (true)
    {
        cout << "輸入play來開始遊戲，或輸入exit來結束遊戲" << endl;
        tmp = getInputLine();

        if (tmp == "play")
        {
            Gloomhaven game(characterFile, monsterFile, debugModel);
            game.Run();
        }
        else if (tmp == "exit")
            exit(0);

        if (debugModel)
            exit(0);
    }

    return 0;
}
