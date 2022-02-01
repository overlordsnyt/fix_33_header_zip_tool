//
// Created by overlord on 2022/1/31.
//

#ifndef FIX_33ZIP_READ_FILES_H
#define FIX_33ZIP_READ_FILES_H

#include <list>
#include <string>
#include <filesystem>


class FileSelector {
public:
    enum SEL_TYPE { FILE = 1, FOLDER = 1 << 1 };
    FileSelector();
    ~FileSelector();

    std::list<std::filesystem::directory_entry> listPathFiles(const std::wstring_view& path, const bool recurFlag,
                                                              const short selType = FILE | FOLDER, const std::wstring& wildcard = L"");
};

#endif //FIX_33ZIP_READ_FILES_H
