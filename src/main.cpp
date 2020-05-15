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

    Gloomhaven game(characterFile, monsterFile, debugModel);

    return 0;
}
