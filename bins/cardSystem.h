#ifndef SYSTEMH
#define SYSTEMH

#include "card.h"
#include "config.h"
#include "cardSystem.h"
#include "player.h"

#define SS_PIN 10
#define RST_PIN 9
#define RX_DF_PIN 5
#define TX_DF_PIN 6
#define BUTTON_PIN 4

#define ACTIONS 2 // play, next
#define MAX_CARDS 10

class CardSystem {
    private:
    Card card;
    Config config;
    Player player;
    
    public:
    CardSystem();
    void init();
    void updateSystem();
    void saveCardsToEEPROM(int);
    void loadCardsFromEEPROM(int);
    void initCardConfig();
    int findCard(String);
    void updateCardType(int) ;
    void printCardsFromEEPROM(int);
};

#endif