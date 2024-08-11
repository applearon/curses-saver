#include "config.hpp"
#include<string>
#include<fstream>
#include<iostream>
#include<algorithm>
#ifdef DEBUG_BUILD
const bool debug = true;
#else
const bool debug = false;
#endif
int testFunc() {
    return 69;
}

Config::Config(std::string filename) {
    std::ifstream file;
    file.open(filename);
    if (file.fail()) {
        std::cout << "Failed to open file" << std::endl;
        failed = true;
    } else {
        std::string line;
        int line_num = 1;
        while (std::getline(file, line)) {
            if (line.size() > 0 && line[0] != '#') { // ignore comments
                std::string option;
                std::string val;
                try {
                    int x = line.find(delim);
                    option = line.substr(0, x);
                    option.erase(std::remove_if(option.begin(), option.end(), isspace), option.end()); // removes whitespace probably
                    val = line.substr(x + 1);
                    ++line_num;
                } catch(std::exception &err) {
                    std::cout << "Failed to parse line " << line_num << ": " << err.what() << std::endl;
                    failed = true;
                };
                if (option == "fps") {
                    try {
                        int framerate = std::stoi(val);
                        this->fps = framerate;
                    } catch (std::exception &err) {
                        std::cout << "Failed to parse value on line " << line_num << ": " << err.what() << std::endl;
                        std::cout << "Option: " << option << ", Value: " << val << std::endl;
                        failed = true;
                    } 
                } else if (false) { // other values

                }
            }
        }
    }
}

bool Config::hasLoaded() {
    return !failed;
}
