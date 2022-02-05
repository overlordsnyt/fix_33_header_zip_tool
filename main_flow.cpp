//
// Created by overlord on 2022/2/2.
//

#include <list>

#include "file_util.h"
#include "check_header_util.h"
#include "binary_manipulation.h"

static const uint32_t ZIP_BAD_HEADER=0x03034b50;
int main(){

    std::list<std::filesystem::directory_entry> filesQueriedByWildcard = FileSelector::listPathFiles(
            LR"(D:\Manga\UID639803_Yami)", true, FileSelector::FILE, L" (C98)*.zip | (C97)*.zip ");

    std::list<std::filesystem::directory_entry> filesPrepared = FileSelector::listPathFiles(
            LR"(D:\Manga\UID639803_Yami)", true);

    const std::byte* badHeaderPtr = reinterpret_cast<const std::byte*>(&ZIP_BAD_HEADER);
    std::list<std::filesystem::directory_entry> filesBadHeader = FileHeader::pickMagicNumberFile(filesPrepared, 0,
                                                                                                 badHeaderPtr,
                                                                                                 sizeof ZIP_BAD_HEADER);

    std::list<std::filesystem::directory_entry> filesCopied = FileSelector::copyFiles(filesBadHeader,
                                                                                      LR"(D:\Manga\UID639803_Yami)",
                                                                                      LR"(..\Yami_backup)", true);

    std::list<std::filesystem::directory_entry> fixedFiles = BinaryManipulation::fix33HeaderZip(filesBadHeader);

    return 0;
}

