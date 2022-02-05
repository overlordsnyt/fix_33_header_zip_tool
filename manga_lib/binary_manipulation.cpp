//
// Created by overlord on 2022/2/4.
//

#include "binary_manipulation.h"

#include <fstream>

#define ZIP_ENTRY_SEARCH_BUF 64

static const unsigned int FIXED_SIGNATURE=0x03034B50;
static const unsigned int END_CENTRAL_DIR_SIGNATURE=0x06054B50;
static const unsigned int CENTRAL_DIR_SIGNATURE=0x02014B50;

dir_entry_list BinaryManipulation::fix33HeaderZip(const dir_entry_list& badHeaderFiles) {
    dir_entry_list fixedFiles{};
    for (const auto& entry:badHeaderFiles) {
        if (fix33HeaderZip(entry))
            fixedFiles.push_back(entry);
    }
    return fixedFiles;
}

static std::streampos search4BytesPositionFromCurrentInPos(std::istream& searchFS, const unsigned int& searchedContent);

bool BinaryManipulation::fix33HeaderZip(const std::filesystem::directory_entry& badHeaderFile) {
    using namespace std;
    fstream fixingFS{badHeaderFile, ios::binary | ios::in | ios::out | ios::ate};
    if (fixingFS.is_open()) {
        const streampos centralDirPos= search4BytesPositionFromCurrentInPos(fixingFS, END_CENTRAL_DIR_SIGNATURE);
        if (centralDirPos==-1) return false;
    }
    return false;
}

static std::streampos search4BytesPositionFromCurrentInPos(std::istream& searchFS, const unsigned int& searchedContent) {
    searchFS.seekg(-ZIP_ENTRY_SEARCH_BUF, std::ios::cur);
    char* startSearch = (char*) &searchedContent;
    char* endSearch = startSearch + (sizeof searchedContent);
    char* nowSearch = startSearch;
    while (!searchFS.fail()) {
        for (int i = 0; i < ZIP_ENTRY_SEARCH_BUF; ++i) {
            char nowByte = searchFS.get();
            while (nowByte == *nowSearch && nowSearch != endSearch) {
                nowByte = searchFS.get();
                ++nowSearch;
            }
            if (nowSearch == endSearch) {
                return searchFS.tellg() -= (sizeof searchedContent + 1);
            } else {
                for (; nowSearch != startSearch; --nowSearch) {
                    searchFS.unget();
                }
            }
        }
        //back forward search continue.
        //but back forward whole file what would happen?
        //stream state will set fail() true, and tellg() returned -1.
        searchFS.seekg(-ZIP_ENTRY_SEARCH_BUF * 2, std::ios::cur); //one buffer length already read, so look back upon double.
    }
    return searchFS.tellg(); //always -1.
}
