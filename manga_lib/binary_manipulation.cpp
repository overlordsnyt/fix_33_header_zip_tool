//
// Created by overlord on 2022/2/4.
//

#include "binary_manipulation.h"

#include <fstream>
#include <array>

#define ZIP_ENTRY_SEARCH_BUF 64

static const uint32_t END_CENTRAL_DIR_SIGNATURE=0x06054B50;
static const uint32_t CENTRAL_DIR_SIGNATURE=0x02014B50;

dir_entry_list BinaryManipulation::fix33HeaderZip(const dir_entry_list& badHeaderFiles) {
    dir_entry_list fixedFiles{};
    for (const auto& entry:badHeaderFiles) {
        if (fix33HeaderZip(entry))
            fixedFiles.push_back(entry);
    }
    return fixedFiles;
}

static std::streampos
searchBackward4BytesPositionFromCurrentInPos(std::istream& searchFS, const uint32_t& searchedContent);
static std::list<std::streampos>
searchForwardAll4BytesPositionFromCurrentInPos(std::istream& searchFS, const size_t& searchCount, const uint32_t& searchedContent);

static bool modify(std::fstream& fixingFS, const std::streampos& pos, const int& x);
static bool deleteFileData(std::fstream& cutMiddleFS, const uint32_t& pos, const size_t& count, const std::streampos& endpos);

//ZIP define: https://pkware.cachefly.net/webdocs/casestudies/APPNOTE.TXT
bool BinaryManipulation::fix33HeaderZip(const std::filesystem::directory_entry& badHeaderFile) {
    using namespace std;
    streampos fileSize{0};
    fstream fixingFS{badHeaderFile, ios::binary | ios::in | ios::out | ios::ate};
    if (fixingFS.is_open()) {
        const streampos endCentralDirRecPos= searchBackward4BytesPositionFromCurrentInPos(fixingFS,
                                                                                          END_CENTRAL_DIR_SIGNATURE);
        if (endCentralDirRecPos == -1) return false;
        const auto centralDirStartPosIndicator = endCentralDirRecPos.operator+(16);
        uint32_t centralDirStartPos;
        fixingFS.seekg(centralDirStartPosIndicator).read(reinterpret_cast<char*>(&centralDirStartPos), sizeof(uint32_t));
        uint32_t centralDirSize;
        fixingFS.seekg(centralDirStartPosIndicator.operator-(4)).read(reinterpret_cast<char*>(&centralDirSize),
                                                                      sizeof(uint32_t));
        fixingFS.seekg(centralDirStartPos);
        const auto headerPosIndicators = searchForwardAll4BytesPositionFromCurrentInPos(fixingFS, centralDirSize,
                                                                                  CENTRAL_DIR_SIGNATURE);
        //searched all need modifying location: centralDirStartPosIndicator, headerPosIndicators.
        //then modify them.
        int x = -4;
        modify(fixingFS, centralDirStartPosIndicator, x);
        for (const auto& indicator: headerPosIndicators) {
            modify(fixingFS, indicator, x);
        }
        fixingFS.flush();
        //delete start 4 bytes.
        fileSize=fixingFS.seekg(0,ios::end).tellg();
        deleteFileData(fixingFS, 0, 4, fileSize);
        fixingFS.flush();
        fixingFS.close();
    }
    //discard tail 4 bytes.
    error_code err;
    filesystem::resize_file(badHeaderFile, fileSize.operator-=(4), err);
    if (err)
        return false;
    return true;
}

static std::streampos searchBackward4BytesPositionFromCurrentInPos(std::istream& searchFS, const uint32_t& searchedContent) {
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
            } else if (nowSearch != startSearch) {
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

std::list<std::streampos>
searchForwardAll4BytesPositionFromCurrentInPos(std::istream& searchFS, const size_t& searchCount,
                                               const uint32_t& searchedContent) {
    std::list<std::streampos> headerPosIndicators{};
    char* start= (char*) &searchedContent;
    char* end = start + sizeof(searchedContent);
    char* now = start;
    //keep search count accord with meaning.
    for (int i = 0; i < searchCount; ++i) {
        char nowByte = searchFS.get();
        while (nowByte == *now && now != end) {
            //once get increase i;
            nowByte = searchFS.get();
            ++i;
            ++now;
        }
        if (now == end) {
            //once unget decrease i;
            searchFS.unget();
            --i;
            auto cenEntry = searchFS.tellg().operator-(4);
            auto headerPosIndicator = cenEntry.operator+(42);
            headerPosIndicators.push_back(headerPosIndicator);
            now = start;
        } else if (now != start) {
            for (; now != start; --now) {
                searchFS.unget();
                --i;
            }
        }
    }
    return headerPosIndicators;
}

bool modify(std::fstream& fixingFS, const std::streampos& pos, const int& x) {
    uint32_t indicatorModifying;
    fixingFS.seekg(pos).read(reinterpret_cast<char*>(&indicatorModifying), sizeof(uint32_t));
    indicatorModifying += x;
    fixingFS.seekp(pos).write(reinterpret_cast<const char*>(&indicatorModifying), sizeof(uint32_t));
    if (fixingFS.good())
        return true;
    else
        return false;
}

bool deleteFileData(std::fstream& cutMiddleFS, const uint32_t& pos, const size_t& count, const std::streampos& endpos) {
    std::array<char,COPY_MEMORY> buf{};
    std::streampos readpos{pos+(uint32_t)count};
    std::streampos writepos{pos};
    while (readpos != endpos) {
        auto remainSize = endpos - readpos;
        auto readSize = buf.size() < remainSize ? buf.size() : remainSize;
        cutMiddleFS.seekg(readpos);
        cutMiddleFS.read(buf.data(), readSize);
        cutMiddleFS.seekp(writepos);
        cutMiddleFS.write(buf.data(), readSize);
        readpos += readSize;
        writepos += readSize;
    }
    if (cutMiddleFS.good())
        return true;
    else
        return false;
}
