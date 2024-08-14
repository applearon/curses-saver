#include "cmdargs.hpp"
#include <string>
#include<iostream>

Arguments::Arguments(int arg_count, char *arguments[]) {
    std::vector<std::string> arg(arg_count - 1); 
    for (int i = 0; i < arg_count - 1; ++i) {
        arg[i] = arguments[i + 1];
    }
    int args_parsed = 0;
    while ((int) arg.size() > args_parsed) {
        int next_arg = args_parsed;
        if (arg[next_arg] == "-c" || arg[next_arg] == "--config") {
            if ((int) arg.size() <= args_parsed + 1) {
                std::cout << "Error parsing -c" << std::endl;
                ++args_parsed;
                failed = true;
            } else {
                this->config_path = arg[next_arg + 1];
                args_parsed += 2;
            }
        } else if (arg[next_arg] == "-f" || arg[next_arg] == "--file") {
            if ((int) arg.size() <= args_parsed + 1) {
                std::cout << "Error parsing -f" << std::endl;
                ++args_parsed;
                failed = true;
            } else {
                this->logo_path = arg[next_arg+1];
                args_parsed += 2;
            }
        } else {
            std::cout << "invalid argument: " << arg[0] << std::endl;
            failed = true;
            args_parsed++;
        }
    }
}

bool Arguments::Failed() {
    return this->failed;
}
