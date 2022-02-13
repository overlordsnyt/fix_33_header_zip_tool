//
// Created by overlord on 2022/1/31.
//

#include <set>
#include <utility>

#ifndef _MSC_VER
#include <cstring>
#endif

#include "file_util.h"

static bool match(wchar_t const* needle, wchar_t const* haystack);
static std::list<std::wstring_view> splitWildcardInput(wchar_t token, const std::wstring& wildcardMultiple);

namespace fs = std::filesystem;

static wchar_t* wildcardPtr = nullptr;

dir_entry_list FileSelector::listPathFiles(const std::wstring_view& path, const bool recurFlag, const short selType,
                                                           const std::wstring& wildcard) {
    const fs::path searchRootPath{path};
    dir_entry_list entryList{};
    if (recurFlag)
        for (const fs::directory_entry& dir_entry: fs::recursive_directory_iterator(searchRootPath))
            entryList.push_back(dir_entry);
    else
        for (const fs::directory_entry& dir_entry: fs::directory_iterator(searchRootPath))
            entryList.push_back(dir_entry);

    if (!(selType&FILE))
        entryList.remove_if([](const fs::directory_entry& entry) {
            return entry.is_regular_file();
        });
    if (!(selType&FOLDER))
        entryList.remove_if([](const fs::directory_entry& entry){
           return entry.is_directory();
        });
    if (!wildcard.empty()){
        std::list<std::wstring_view> wildcardList=splitWildcardInput(L'|',wildcard);

        entryList.remove_if([&](const fs::directory_entry& entry) {
            const std::wstring filename=entry.path().filename().wstring();
            bool flag=true;
            for(std::wstring_view wildcardItem:wildcardList){
                if (match(wildcardItem.data(),filename.data()))
                    flag = false;
            }
            return flag;
        });
        if (!wildcardList.empty()) {
            std::destroy(wildcardList.begin(), wildcardList.end());
            wildcardList.clear();
        }
        if (wildcardPtr != nullptr)
            free(wildcardPtr);
    }

    return entryList;
}

const dir_entry_list
FileSelector::copyFiles(const dir_entry_list& fileList, const std::filesystem::path& commonParentDir,
                        const std::filesystem::path& dstPath, bool prefixFolderFlag) {
    if (!commonParentDir.empty() && !commonParentDir.is_absolute())
        throw fs::filesystem_error("Common parent directory must be absolutely.", commonParentDir,
                                   std::make_error_code(std::errc::bad_address));
    //create dst folder
    fs::directory_entry dst{dstPath};
    if (dstPath.is_relative())
        dst.assign((commonParentDir/dstPath).lexically_normal());
    if (!dst.exists()) {
        fs::create_directories(dst.path());
    } else if (!dst.is_directory()) {
        throw fs::filesystem_error("Copy files, set destination directory error.", dst.path(),
                                   make_error_code(std::errc::not_a_directory));
    }
    //generate dst list according prefix flag
    std::list<std::pair<fs::directory_entry,fs::directory_entry>> copyingFiles{};
    if (prefixFolderFlag) {
        for(const auto& orgEntry:fileList){
            char flag=0;
            for (auto orgPath = orgEntry.path().parent_path(); orgPath.has_parent_path(); orgPath = orgPath.parent_path()) {
                if (orgPath == commonParentDir) {
                    flag = 1;
                    break;
                }
            }
            if (flag) {
                auto relate2FilePath = orgEntry.path().lexically_relative(commonParentDir);
                auto dstFileWholePath = (dst.path() / relate2FilePath).lexically_normal();
                copyingFiles.emplace_back(std::pair{orgEntry, dstFileWholePath});
            } else {
                throw fs::filesystem_error("Common parent path must match copied file path.", orgEntry.path(),
                                           std::make_error_code(std::errc::function_not_supported));
            }
        }
    } else {
        for (const auto& orgEntry: fileList) {
            auto filename = orgEntry.path().filename();
            auto dstFileWholePath = dst.path() / filename;
            copyingFiles.emplace_back(std::pair{orgEntry, dstFileWholePath});
        }
    }
    //create folder
    if (prefixFolderFlag) {
        std::set<fs::path> uniqueFolderPath{};
        for (const auto& entryPair: copyingFiles) {
            uniqueFolderPath.insert(entryPair.second.path().parent_path());
        }
        uniqueFolderPath.erase(dst.path());
        for (const auto& path: uniqueFolderPath) {
            if (!fs::exists(path))
                fs::create_directories(path);
        }
    }
    //copy file
    dir_entry_list copiedFiles{};
    const fs::copy_options copyingOptions{fs::copy_options::update_existing};
    for (const auto& entryPair: copyingFiles) {
        bool copiedFlag = fs::copy_file(entryPair.first, entryPair.second, copyingOptions);
        if (copiedFlag)
            copiedFiles.push_back(entryPair.second);
    }
    return copiedFiles;
}

static void trimString(wchar_t*& beginPtr,wchar_t* endPtr);

std::list<std::wstring_view> splitWildcardInput(wchar_t token, const std::wstring& wildcardMultiple) {
    std::list<std::wstring_view> wildcardList{};
    wildcardPtr = static_cast<wchar_t*>(malloc(sizeof(wchar_t)*(wildcardMultiple.size()+1)));
    memset(wildcardPtr,0, sizeof(wchar_t)*(wildcardMultiple.size()+1));
//    wchar_t* wildcardPtr= const_cast<wchar_t*>(wildcardMultiple.data());
    wildcardMultiple.copy(wildcardPtr, wildcardMultiple.size());
    wchar_t* startPtr = wildcardPtr;
    wchar_t* endPtr = wildcardPtr + wildcardMultiple.size();
    wchar_t* splitPtr;
    while ((splitPtr = wcschr(wildcardPtr, token)) != nullptr) {
        *splitPtr = L'\0';
        //trim string
        trimString(startPtr,splitPtr);
        wildcardList.emplace_back(startPtr);
        startPtr = splitPtr + 1;
    }
    trimString(startPtr,endPtr);
    wildcardList.emplace_back(startPtr);
//    free(wildcardPtr);
    return wildcardList;
}

void trimString(wchar_t*& beginPtr,wchar_t* endPtr){
    while(*(endPtr - 1) == L' ')
        *(--endPtr) = L'\0';
    while(*(beginPtr)==L' ')
        ++beginPtr;
}

// https://stackoverflow.com/questions/3300419/file-name-matching-with-wildcard
bool match(wchar_t const* needle, wchar_t const* haystack) {
    for (; *needle != '\0'; ++needle) {
        switch (*needle) {
            case '?':
                if (*haystack == '\0')
                    return false;
                ++haystack;
                break;
            case '*': {
                if (needle[1] == '\0')
                    return true;
                size_t max = wcslen(haystack);
                for (size_t i = 0; i < max; i++)
                    if (match(needle + 1, haystack + i))
                        return true;
                return false;
            }
            default:
                if (*haystack != *needle)
                    return false;
                ++haystack;
        }
    }
    return *haystack == '\0';
}
