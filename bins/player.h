#ifndef DFPLAYERH
#define DFPLAYERH

#include "DFRobotDFPlayerMini.h"
#include "SoftwareSerial.h"

typedef enum{
    PLAY,
    NEXT,
    PREVIOUS,
    VOLUP,
    VOLDOWN
} cardT;

class Player {
    private:
    DFRobotDFPlayerMini myDFPlayer;
    SoftwareSerial mySoftwareSerial; // RX, TX
    unsigned long timer;
    cardT cardType;
    int folders;
    String * identifiers;

    public:
    void handleDFState(uint8_t, int);
    void processPlayer(String, bool);
    void start();
    Player(int, int, int);
    void translateCard(String);
    int numberOfFolders();
    void MP3Folder(int);
    int getFolders();
    void loadIdentifiers(String[]);
};



#endif