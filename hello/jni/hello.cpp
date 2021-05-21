#include <iostream>

using namespace std;

int main(int argc,char* argv[])
{
    cout<<"hello ndk"<<endl;

    setlocale(LC_ALL,"");
    wprintf(L"한글\n");
    return 0;
}
