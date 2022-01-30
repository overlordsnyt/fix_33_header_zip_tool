
#include "read_files.h"

#include <iostream>
static bool match(wchar_t const* needle, wchar_t const* haystack);
namespace fs=std::filesystem;

FileSelector::FileSelector() {

}

FileSelector::~FileSelector() {

}

std::list<fs::directory_entry> FileSelector::listPathFiles(const std::string_view& path, const bool recurFlag, const short selType,
                                                   const std::wstring& wildcard) {

    std::cout << path << std::endl;
    const fs::path searchRootPath{path};
    std::list<fs::directory_entry> entryList{};
    if (recurFlag)
        for (const fs::directory_entry& dir_entry: fs::recursive_directory_iterator(searchRootPath))
            entryList.push_back(dir_entry);
    else
        for (const fs::directory_entry& dir_entry: fs::directory_iterator(searchRootPath))
            entryList.push_back(dir_entry);

    if (!(selType&FILE))
        entryList.remove_if([](fs::directory_entry entry) {
            return entry.is_regular_file();
        });
    if (!(selType&FOLDER))
        entryList.remove_if([](fs::directory_entry entry){
           return entry.is_directory();
        });
    if (!wildcard.empty())
        entryList.remove_if([&](fs::directory_entry entry) {
            return !match(wildcard.data(), entry.path().filename().wstring().data());
        });
    return entryList;
}

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
