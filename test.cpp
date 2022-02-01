//
// Created by overlord on 2022/2/1.
//

#include <iostream>
#include <cstdio>
#include <string>
#include <codecvt>
#include <locale>
//#include <wincon.h>
//#include <WinNls.h>

#include <cstddef>

using namespace std;

int main() {
/*//    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
//    std::string str= reinterpret_cast<const char*>(u8"草你妈妈");
    std::string str = reinterpret_cast<const char*>(u8"草你妈妈");
//    std::cout << str << std::endl;
    std::wstring wstr = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(str);
    std::wcout << wstr << std::endl;*/

    long long stdZipHeader=0x04034b50LL;
    byte* bptr=static_cast<byte*>(static_cast<void*>(&stdZipHeader));
    printf("0x%0.8llX\n",stdZipHeader);
    byte* endPtr = bptr + (sizeof stdZipHeader);
    for(byte* nowPtr=bptr; nowPtr != endPtr; ++nowPtr){
        printf("%0.2x",*nowPtr);
    }
    printf("\n");

    return 0;
}

