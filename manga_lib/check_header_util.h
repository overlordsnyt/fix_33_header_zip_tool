//
// Created by overlord on 2022/2/1.
//

#ifndef FIX_33ZIP_CHECK_HEADER_UTIL_H
#define FIX_33ZIP_CHECK_HEADER_UTIL_H

#include <cstddef>
#include <list>
#include <filesystem>

typedef std::list<std::filesystem::directory_entry> dir_entry_list;

class FileHeader {
public:
    static dir_entry_list
    pickMagicNumberFile(const dir_entry_list& dirEntryList, size_t offset, const std::byte*& assume, size_t assumeSize);
    static bool isMagicNumberFile(std::filesystem::directory_entry& dirEntry, size_t offset, const std::byte*& assume,
                           size_t assumeSize);
};


#endif //FIX_33ZIP_CHECK_HEADER_UTIL_H
