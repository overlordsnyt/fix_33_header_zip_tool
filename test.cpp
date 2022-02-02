//
// Created by overlord on 2022/2/1.
//

#include <filesystem>

using namespace std;

namespace fs = std::filesystem;

int main() {
    fs::path relative{"./d/tt3.txt"};
    fs::path absolute{"D:/manga/code_test"};
    bool judge1=relative.is_relative();
    bool judge2=absolute.is_relative();

    auto conpath=absolute/relative;
    auto normalize=conpath.lexically_normal();
    auto filename=relative.filename();

    return 0;
}

