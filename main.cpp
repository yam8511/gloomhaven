#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char const *argv[])
{
    fstream in("character1.txt");
    if (!in)
    {
        throw "讀取角色設定檔失敗";
    }

    string tmp;
    char buffer[200];

    in.getline(buffer, sizeof(buffer));
    tmp = string(buffer);
    cout << "First => " << tmp << endl;
    printf("==> %d\n", tmp == "2");

    do
    {
        in.getline(buffer, sizeof(buffer));
        tmp = string(buffer);
        cout << tmp << endl;
    } while (!in.eof());

    in.close();

    /* code */
    return 0;
}
