#ifndef CARDH
#define CARDH

#include <SPI.h>
#include <MFRC522.h>

class Card {
    private:
    MFRC522 cardReader;
    bool newCard = false;
    String card = "";

    public:
    Card(int, int);
    void processCard();
    void init();
    String getCard();
    bool isNewCard();

};

#endif