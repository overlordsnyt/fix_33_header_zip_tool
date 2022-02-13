//
// Created by overlord on 2022/1/31.
//

#ifndef FIX_33ZIP_FILE_UTIL_H
#define FIX_33ZIP_FILE_UTIL_H

#include <list>
#include <string>
#include <filesystem>

typedef std::list<std::filesystem::directory_entry> dir_entry_list;


class FileSelector {
private:
    FileSelector()=default;
    ~FileSelector()=default;
public:
    enum SEL_TYPE { FILE = 1, FOLDER = 1 << 1 };

    static dir_entry_list listPathFiles(const std::wstring_view& path, bool recurFlag = false,
                                                                     short selType = FILE, const std::wstring& wildcard = L"");
    static const dir_entry_list
    copyFiles(const dir_entry_list& fileList, const std::filesystem::path& commonParentDir,
              const std::filesystem::path& dstPath, bool prefixFolderFlag = false);
};

#endif //FIX_33ZIP_FILE_UTIL_H
