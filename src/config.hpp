#ifndef CONFIG_H
#define CONFIG_H
#include<string>
int testFunc();

class Config {
    public:
        Config(std::string filename);
        int fps = 15;
        const char* delim = "=";
        bool hasLoaded();
    private:
        bool failed = false;
};

#endif
