#include <iostream>

using namespace std;

int main(int argc,char* argv[])
{
    setlocale(LC_ALL,"");
    wprintf(L"유니코드 테스트\n");

    cout<<"sizeof(wchar_t)="<<sizeof(wchar_t)<<endl;

    wchar_t *pstr = L"더 킹.mp4";
    wcout<<"pstr="<<pstr<<endl;

    cout<<hex;
    cout<<"pstr[0] 0x"<<pstr[0]<<endl;
    cout<<"pstr[1] 0x"<<pstr[1]<<endl;
    cout<<"pstr[2] 0x"<<pstr[2]<<endl;
    cout<<"pstr[3] 0x"<<pstr[3]<<endl;
    cout<<"pstr[4] 0x"<<pstr[4]<<endl;
    cout<<"pstr[5] 0x"<<pstr[5]<<endl;
    cout<<"pstr[6] 0x"<<pstr[6]<<endl;
    cout<<"pstr[7] 0x"<<pstr[7]<<endl;
    return 0;
}
