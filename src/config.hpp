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
        bool saver_mode = false;

        const char* delim = "=";
        bool hasLoaded();
    private:
        bool failed = false;
};

#endif
