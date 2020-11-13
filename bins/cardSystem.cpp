#include "cardSystem.h"

CardSystem::CardSystem() :
                        card{Card(SS_PIN, RST_PIN)},   // Create MFRC522 instance.
                        config{Config(BUTTON_PIN)},
                        player{Player(RX_DF_PIN, TX_DF_PIN, config.readFoldersEEPROM())} {

}

void CardSystem::updateSystem() {
    card.processCard();
    player.processPlayer(card.getCard(), card.isNewCard());
    card.isNewCard() ? delay(2000) : delay(100);
}

void CardSystem::saveCardsToEEPROM(int pendingCards) {
    Serial.println(pendingCards);
    config.writeFoldersEEPROM(pendingCards - 2); //number of folders
    for (int i = 0; pendingCards > 0; i++, pendingCards--) {

        if (pendingCards == 2) {
            player.MP3Folder(11);
        } else if (pendingCards == 1) {
            player.MP3Folder(12);
        } else {
            player.MP3Folder(i + 1);
        }
        
        while(!card.isNewCard()){
            card.processCard();
            delay(1000);
        }

        //first bytes for general config
        config.writeEEPROM(card.getCard(), i + 1);
    }
}

void CardSystem::loadCardsFromEEPROM(int cardsNumber) {
    Serial.println(cardsNumber);

    for (int i = 0; i < cardsNumber; i++) {
        Serial.println(config.readEEPROM(i + 1));
    }
}

void CardSystem::initCardConfig() {
    int pendingCards = -1;
    player.start();
    //while para darle tiempo a que actualice la cantidad de carpetas
    while (pendingCards == -1){
        pendingCards = player.numberOfFolders();
        delay(1000);
    }

    pendingCards++; // +2 (play and next) -1 (MP3 folder)
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
    loadCardsFromEEPROM(player.getFolders() + 2);
}