#include <iostream>
#include <string>
using namespace std;

string getInputLine()
{
    char buffer[512];
    cin.getline(buffer, 512);
    string tmp = string(buffer);
    tmp.erase(std::remove(tmp.begin(), tmp.end(), '\r'), tmp.end());
    tmp.erase(std::remove(tmp.begin(), tmp.end(), '\n'), tmp.end());
    return tmp;
}

int main()
{
    string a = getInputLine();
    cout << a << "what?" << endl;

    if (a == "play")
    {
        cout << "OK" << endl;
    }
    else
    {
        cout << "No" << endl;
    }

    return 0;
}
