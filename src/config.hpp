#ifndef CONFIG_H
#define CONFIG_H
#include<string>
int testFunc();

class Config {
    public:
        Config(std::string filename);
        Config();

        // Default Values
        int fps = 15;
        std::string logo_path = "examples/arch";


        const char* delim = "=";
        bool hasLoaded();
    private:
        bool failed = false;
};

#endif
