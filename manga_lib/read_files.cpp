
#include "read_files.h"

static bool match(wchar_t const* needle, wchar_t const* haystack);
static std::list<std::wstring_view> splitWildcardInput(wchar_t token, const std::wstring& wildcardMultiple);

namespace fs=std::filesystem;

static wchar_t* wildcardPtr;

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

        std::destroy(wildcardList.begin(), wildcardList.end());
        wildcardList.clear();
        free(wildcardPtr);
    }

    return entryList;
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
    wchar_t* splitPtr = startPtr;
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

FileSelector::FileSelector() {

}

FileSelector::~FileSelector() {

}
