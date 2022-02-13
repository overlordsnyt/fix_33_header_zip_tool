#include <iostream>
#include <cstddef>
#include <codecvt>

#include "read_files.h"
#include "check_file_magic.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

//    std::list<std::filesystem::directory_entry> fileList=fslt.listPathFiles(LR"(D:\Manga\UID639803_Yami\fixed)", true,
//                                                                            FileSelector::FILE, L"(C98)*.zip | (C97)*.zip");
    std::list<std::filesystem::directory_entry> fileList=FileSelector::listPathFiles(LR"(D:\Manga\UID639803_Yami\fixed)", true);
//    std::locale::global(std::locale("C"));
//    std::wcout.imbue(std::locale("C"));
//    std::wcout<<fileList.front()<<std::endl;

    long badZipHeader=0x03034b50L;
    const std::byte* bptr=static_cast<std::byte*>(static_cast<void*>(&badZipHeader));
//    auto u8str=u8R"(D:\Manga\UID639803_Yami\fixed\[MANA] ��Υ���ԭ��.zip)";
//    auto u8ts=u8"[MANA] ��Υ���ԭ��.zip";
//    auto path1=std::filesystem::path{wstr,std::locale{"zh_CN.UTF-8"}};
//    std::filesystem::directory_entry fileEntry1{std::filesystem::path{LR"(D:\Manga\UID639803_Yami\fixed\[MANA] ��Υ���ԭ��.zip)"}};
//    std::filesystem::directory_entry fileEntry2{std::filesystem::path{LR"(D:\Manga\UID639803_Yami\fixed\(ͬ��CG��) (patreon) [MANA] ���� 1 [����] [�o����].zip)"}};
//    bool judge1=fhd.isMagicNumberFile(fileEntry1,0,bptr,sizeof badZipHeader);
//    bool judge2=fhd.isMagicNumberFile(fileEntry2,0,bptr,sizeof badZipHeader);

    FileHeader::pickMagicNumberFile(fileList,0,bptr,sizeof badZipHeader);

    return 0;
}
