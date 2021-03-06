#include <iostream>
#include <assert.h>

using namespace std;

wchar_t toLinUnicode(unsigned char uc1,unsigned char uc2)
{
    return (uc2<<8)|uc1;
}

std::wstring buf2wstr(unsigned char *buf,unsigned int bufSize)
{
    std::wstring s;
    for(int i=0;i<bufSize;i+=2) s.append(1,toLinUnicode(buf[i],buf[i+1]));
    return s;
}

unsigned int wstr2buf(std::wstring s,unsigned char *buf,unsigned int bufSize)
{
    assert(2*s.length()<=bufSize);
    for(size_t i=0;i<s.length();i++) {
        buf[2*i] = s[i]&0xff;
        buf[2*i+1] = (s[i]&0xff00)>>8;
    }
    return 2*s.length();
}

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
	wcout<<hex<<"0x"<<(int)ws[0]<<endl;

    unsigned char netBuf[]={ 0x54,0xb3,0x20,0x00,0xb9,0xd0,0x2e,0x00,0x6d,0x00,0x70,0x00,0x34,0x00 };	//더 킹.mp4
    wcout<<"wchar0="<<toLinUnicode(netBuf[0],netBuf[1])<<endl;  //더
    wcout<<"wchar1="<<toLinUnicode(netBuf[2],netBuf[3])<<endl;  //
    wcout<<"wchar2="<<toLinUnicode(netBuf[4],netBuf[5])<<endl;  //킹
    wcout<<"wchar3="<<toLinUnicode(netBuf[6],netBuf[7])<<endl;  //.
    wcout<<"wchar4="<<toLinUnicode(netBuf[8],netBuf[9])<<endl;  //m
    wcout<<"wchar5="<<toLinUnicode(netBuf[10],netBuf[11])<<endl;//p
    wcout<<"wchar6="<<toLinUnicode(netBuf[12],netBuf[13])<<endl;//4

    //Simplify using loop
    for(int i=0;i<sizeof(netBuf);i+=2){
        wcout<<"wchar"<<i/2<<"="<<toLinUnicode(netBuf[i],netBuf[i+1])<<endl;
    }

    //Using buf2wstr function
    wcout<<"obtained string from buffer="<<buf2wstr(netBuf,sizeof(netBuf))<<endl;

    //generate stream from unicode string by using wstr2buf function
    unsigned char *genBuf = new unsigned char[128];
    unsigned int len = wstr2buf(ws,genBuf,128);
    wcout<<dec<<"genBuf actual length="<<len<<endl;
    for(unsigned int i=0;i<len;i++) wcout<<hex<<"genBuf["<<i<<"]=0x"<<genBuf[i]<<endl;
    delete [] genBuf;

    return 0;
}
