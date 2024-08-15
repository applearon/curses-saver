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

bool trim(std::string &str) {
    int first = str.find_first_not_of(' ');
    int last = str.find_last_not_of(' ');
    try {
        str = str.substr(first, last - first + 1);
        return true;
    } catch (std::exception &err ) {
        std::cout << "Failed to parse string: " << str << std::endl;
        std::cout << err.what() << std::endl;
        return false;
    }
}

bool truthy(const std::string &str) {
    return (str == "1" || str == "true");
}
bool falsey(const std::string &str) {
    return (str == "0" || str == "false");
}

Config::Config() {
    // use default values;
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
                } else if (option == "logo") { // other values
                    if (trim(val)) {
                        this->logo_path = val;
                    } else {
                        failed = true;
                    }
                } else if (option == "saver_mode") {
                    if (!trim(val)) {
                        failed = true;
                    } else {
                        if (truthy(val)) {
                            this->saver_mode = true;
                        } else if (falsey(val)) {
                            this->saver_mode = false;
                        } else {
                            std::cout << "Failed to understand value: " << val << " for saver_mode" << std::endl;
                            failed = true;
                        }
                    }
                }
            }
        }
    }
}

bool Config::hasLoaded() {
    return !failed;
}
