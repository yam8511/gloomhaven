#include <iostream>
#include <fstream>
#include <vector>
#include "./src/util.cpp"
using namespace std;

int main(int argc, char const *argv[])
{
    string s = getInputLine();
    vector<char> moves;
    for (int i = 0; i < s.size(); i++)
    {
        cout << s[i] << ends;
        moves.push_back(s[i]);
    }
    cout << endl;
    cout << "==========" << endl;

    moves.erase(moves.begin() + 1);
    for (size_t i = 0; i < moves.size(); i++)
    {
        cout << moves[i] << endl;
    }

    return 0;
}
