#include <iostream>

using namespace std;

int main(int argc,char* argv[])
{
    setlocale(LC_ALL,"");
    wprintf(L"유니코드 테스트\n");

    cout<<"sizeof(wchar_t)="<<sizeof(wchar_t)<<endl;

    const wchar_t *pstr = L"더 킹.mp4";
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

    wstring ws((wchar_t*)pstr);
    wcout<<"ws="<<ws<<endl;
    wcout<<"ws[0]="<<ws[0]<<endl;
    wcout<<"ws[1]="<<ws[1]<<endl;
    wcout<<"ws[2]="<<ws[2]<<endl;
    wcout<<"ws[3]="<<ws[3]<<endl;
    wcout<<"ws[4]="<<ws[4]<<endl;
    wcout<<"ws[5]="<<ws[5]<<endl;
    wcout<<"ws[6]="<<ws[6]<<endl;

    wcout<<"sizeof(ws[0])="<<sizeof(ws[0])<<endl;

    unsigned char *p = (unsigned char*)&ws[0];
    for(int i=0;i<sizeof(wchar_t);i++) {
		wcout<<hex<<"0x"<<*(p+i)<<endl;
    }

    unsigned char netBuf[]={ 0x54,0xb3,0x20,0x00,0xb9,0xd0,0x2e,0x00,0x6d,0x00,0x70,0x00,0x34,0x00 };	//더 킹.mp4
    return 0;
}
