#ifndef CARDH
#define CARDH
#include <SPI.h>

#include <MFRC522.h>

class Card {
    private:
    MFRC522 cardReader;

    public:
    Card(int, int);
    void processCard();
    void init();
};

#endif