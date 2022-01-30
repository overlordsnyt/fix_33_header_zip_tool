#include <iostream>

#include "read_files.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    FileSelector fs{};
    std::list<std::filesystem::directory_entry> fileList=fs.listPathFiles(R"(D:\Manga\UID639803_Yami\fixed)", true,
                     FileSelector::FILE,L"(C98)*.zip");

    return 0;
}
