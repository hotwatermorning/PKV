#include "../../include/PKV/PKV.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>

std::string trim(std::string str)
{
    auto first_index = std::find_if(str.begin(), str.end(), [](char c) { return std::isprint(c); }) - str.begin();
    auto last_rindex = std::find_if(str.rbegin(), str.rend(), [](char c) { return std::isprint(c); }) - str.rbegin();
    auto last_index = str.size() - last_rindex;
    auto len = last_index - first_index;
    return str.substr(first_index, len);
}

void testKey(std::string_view key)
{
    if(hwm::PKV::isValidKey(key)) {
        std::cout << "valid." << std::endl;
    } else {
        std::cout << "invalid." << std::endl;
    }
}

// usage:
// `pkv-validator [<key-string>]...`
//
// If pkv-validator is started without any command line arguments, it
// read a key string from standard input and tests if the key is valid.
// To quit the command, type `quit` and press the enter key.
//
// If command line arguments are passed, it tests each parameters for a valid key, and quit immediately.
int main(int argc, char ** argv)
{
    if(argc >= 2) {
        for(int i = 1; i < argc; ++i) {
            std::cout << "[" << i-1 << "]: " << argv[i] << std::endl;
            testKey(argv[i]);
        }
        return 0;
    }

    std::string line;
    for( ; ; ) {
        std::cout << "> " << std::flush;
        if(std::getline(std::cin, line).fail()) {
            std::cout << "failed to input." << std::endl;
            std::cin.clear();
        }

        line = trim(line);
        if(line == "quit") {
            std::cout << "Bye." << std::endl;
            return 0;
        }

        testKey(line);
        line.clear();
    }

    return 0;
}

