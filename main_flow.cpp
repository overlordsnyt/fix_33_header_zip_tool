//
// Created by overlord on 2022/2/2.
//

#include <list>

#include "file_util.h"
#include "check_header_util.h"

int main(){

    std::list<std::filesystem::directory_entry> filesQueriedByWildcard = FileSelector::listPathFiles(
            LR"(D:\Manga\UID639803_Yami)", true, FileSelector::FILE, L" (C98)*.zip | (C97)*.zip ");

    std::list<std::filesystem::directory_entry> filesPrepared = FileSelector::listPathFiles(
            LR"(D:\Manga\UID639803_Yami\fixed)", true);

    long zipBadHeader = 0x03034b50L;
    const std::byte* badHeaderPtr = static_cast<std::byte*>(static_cast<void*>(&zipBadHeader));
    std::list<std::filesystem::directory_entry> filesBadHeader = FileHeader::pickMagicNumberFile(filesPrepared, 0,
                                                                                                 badHeaderPtr,
                                                                                                 sizeof zipBadHeader);

    std::list<std::filesystem::directory_entry> filesCopied = FileSelector::copyFiles(filesBadHeader,
                                                                                      LR"(D:\Manga\UID639803_Yami\fixed)",
                                                                                      LR"(..\fix_output)", true);
    std::filesystem::remove_all(LR"(D:\Manga\UID639803_Yami\fixed\..\fix_output)");
    return 0;
}

