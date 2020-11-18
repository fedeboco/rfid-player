#include "cardSystem.h"

CardSystem::CardSystem() :
                        card{Card(SS_PIN, RST_PIN)},   // Create MFRC522 instance.
                        config{Config(BUTTON_PIN)},
                        player{Player(RX_DF_PIN, TX_DF_PIN, config.readFoldersEEPROM())} {

}

void CardSystem::updateSystem() {
    bool newCard = card.processCard();
    if (newCard)
        updateCardType(findCard(card.getCard()));
    player.processPlayer(card.getCard(), card.isNewCard());
    newCard ? delay(CARD_READ_DELAY) : delay(NO_NEW_CARD_DELAY);
}

void CardSystem::saveCardsToEEPROM(int pendingCards) {
    Serial.println(pendingCards);
    config.writeFoldersEEPROM(pendingCards - ACTIONS); //number of folders
    for (int i = 0; pendingCards > 0; i++, pendingCards--) {

        if (pendingCards == ACTIONS) {
            player.MP3Folder(MAX_CARDS + 1);
        } else if (pendingCards == ACTIONS - 1) {
            player.MP3Folder(MAX_CARDS + 2);
        } else {
            player.MP3Folder(i + 1);
        }
        
        while(!card.isNewCard()){
            card.processCard();
            delay(CARD_CHECK_EEPROM_DELAY);
        }

        //first bytes for general config
        config.writeEEPROM(card.getCard(), i + 1);
    }
}

// returns index if found, -1 otherwise
int CardSystem::findCard(String card) {
    int i = 1;
    String candidate;

    while (i <= player.getFolders() + ACTIONS) {
        if (config.readEEPROM(i) == card.substring(1))
            return i;
        i++;
    }
    return -1;
}

void CardSystem::updateCardType(int index) {
    player.translateCard(index);
}

void CardSystem::printCardsFromEEPROM(int cardsNumber) {
    Serial.println(cardsNumber);

    for (int i = 0; i < cardsNumber; i++) {
        Serial.println(config.readEEPROM(i + 1));
    }
}

void CardSystem::initCardConfig() {
    int pendingCards = -1;

    player.start();
    // takes some time to update folders
    while (pendingCards == -1){
        pendingCards = player.numberOfFolders();
        delay(1000);
    }

    pendingCards + ACTIONS - 1; // +2 (play and next) -1 (MP3 folder)
    saveCardsToEEPROM(pendingCards);
}

void CardSystem::init() {
    card.init();
    Serial.begin(115200);

    if(config.readButton()){
        initCardConfig();
    } else {
        player.start();
    } 
}