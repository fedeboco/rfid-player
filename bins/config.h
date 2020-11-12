#ifndef CONFIGH
#define CONFIGH

#include "Arduino.h"

#define ID_SIZE 11

class Config {
    private:
    int configButton;

    public:
    Config(int);
    bool readButton();
    void writeEEPROM(String, int);
    String readEEPROM(int);
    bool configCards(int);
    void writeFoldersEEPROM(int);
    int readFoldersEEPROM();
};

#endif