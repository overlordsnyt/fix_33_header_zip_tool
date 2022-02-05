//
// Created by overlord on 2022/2/1.
//

#include "check_header_util.h"

#include <fstream>

dir_entry_list
FileHeader::pickMagicNumberFile(const dir_entry_list& dirEntryList, size_t offset,
                                const std::byte*& assume, size_t assumeSize) {
    /*dirEntryList.remove_if([&](std::filesystem::directory_entry entry){
        return !isMagicNumberFile(entry, offset, assume, assumeSize);
    });
    return dirEntryList;*/
    dir_entry_list pickedFiles{};
    for (auto entry: dirEntryList) {
        if (isMagicNumberFile(entry,offset,assume,assumeSize))
            pickedFiles.push_back(entry);
    }
    return pickedFiles;
}

bool FileHeader::isMagicNumberFile(std::filesystem::directory_entry& dirEntry, size_t offset, const std::byte*& assume,
                                   size_t assumeSize) {
    std::ifstream inFileStm{dirEntry.path(), std::ios::in | std::ios::binary};
    if (inFileStm.is_open()) {
        inFileStm.seekg(offset);
        std::byte* readBuf = static_cast<std::byte*>(malloc(assumeSize));
        inFileStm.read(reinterpret_cast<char*>(readBuf), assumeSize);
        inFileStm.close();
        if (inFileStm.eof() | inFileStm.fail()) {
            free(readBuf);
            return false;
        }
        for (size_t pos = 0; pos < assumeSize; ++pos) {
            if (assume[pos] != readBuf[pos]) {
                free(readBuf);
                return false;
            }
        }
        free(readBuf);
        return true;
    }
    return false;
}
