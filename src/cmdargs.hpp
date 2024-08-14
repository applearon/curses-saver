#ifndef CMDARGS_H
#define CMDARGS_H
#include<vector>
#include<string>
#include<limits.h>

class Arguments {
    public:
        Arguments(int arg_count, char*arguments[]);
        const int total_arg_types = 2; // config location, logo location
        std::string logo_path = "";
        std::string config_path = "";
        bool Failed();
    private:
        std::vector<std::string> args;
        bool failed = false;
};

#endif
