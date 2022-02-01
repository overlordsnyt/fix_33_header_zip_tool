//
// Created by overlord on 2022/1/31.
//

#ifndef FIX_33ZIP_READ_FILES_H
#define FIX_33ZIP_READ_FILES_H

#include <list>
#include <string>
#include <filesystem>

typedef std::list<std::filesystem::directory_entry> dir_entry_list;


class FileSelector {
private:
    FileSelector();
    ~FileSelector();
public:
    enum SEL_TYPE { FILE = 1, FOLDER = 1 << 1 };

    static dir_entry_list listPathFiles(const std::wstring_view& path, const bool recurFlag = false,
                                                                     const short selType = FILE, const std::wstring& wildcard = L"");
};

#endif //FIX_33ZIP_READ_FILES_H
