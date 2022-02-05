//
// Created by overlord on 2022/2/4.
//

#ifndef FIX_33ZIP_BINARY_MANIPULATION_H
#define FIX_33ZIP_BINARY_MANIPULATION_H

#include <list>
#include <filesystem>

#define COPY_MEMORY 4*1024*1024

typedef std::list<std::filesystem::directory_entry> dir_entry_list;

class BinaryManipulation {
public:
    static dir_entry_list fix33HeaderZip(const dir_entry_list& badHeaderFiles);
    static bool fix33HeaderZip(const std::filesystem::directory_entry& badHeaderFile);
};


#endif //FIX_33ZIP_BINARY_MANIPULATION_H
