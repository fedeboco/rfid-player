#include "config.h"
#include <EEPROM.h>

Config::Config(int pin = 4) : configButton{pin} {
    pinMode(pin, INPUT_PULLUP);
}

bool Config::readButton() {
    return !digitalRead(configButton);
}

void Config::writeEEPROM(String card, int pos) {
    String id = card.substring(1);
    int startAddress = pos * ID_SIZE;

    for (int i = 0; i < ID_SIZE; i++)
        EEPROM.write(startAddress + i, id[i]);    
}

void Config::writeFoldersEEPROM(int num) {
    EEPROM.write(0, num >> 8);
    EEPROM.write(1, num & 0xFF);
}

int Config::readFoldersEEPROM() {
    return (EEPROM.read(0) << 8) + EEPROM.read(1);
}

String Config::readEEPROM(int pos) {
    char id[ID_SIZE + 1];
    int startAddress = pos * ID_SIZE;

    for (int i = 0; i < ID_SIZE; i++) {
        id[i] = EEPROM.read(startAddress + i);   
    }
    id[ID_SIZE] = '\0';
    return String(id);
}