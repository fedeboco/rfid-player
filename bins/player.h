#ifndef DFPLAYERH
#define DFPLAYERH

#include "DFRobotDFPlayerMini.h"
#include "SoftwareSerial.h"

#define DFRECONNECT_DELAY 1000 //ms
#define DFTIMEOUT 2000 //ms
#define VOLUME 30 // 0~30
#define BAUD_RATE 9600

#define CARD_DETECTED "Card detected"
#define INVALID_CARD "Invalid card"
#define DFSTARTUP "DFPlayer Startup. Wait some seconds (3-5)."
#define DF_NOT_DETECTED "DFPlayer or SD not detected."
#define DF_CONNECTED "DFPlayer started."

typedef enum{
    PLAY,
    NEXT,
    FOLDER,
    INVALID
} cardT;

class Player {
    private:
    DFRobotDFPlayerMini myDFPlayer;
    SoftwareSerial mySoftwareSerial; // RX, TX
    unsigned long timer;
    cardT cardType;
    int folders;
    String * identifiers;
    bool playing = false;
    int folderPlaying = 1;
    int currentFolderTracks;
    int currentTrack = 1;

    public:
    void handleDFState(uint8_t, int);
    void processPlayer(String, bool);
    void start();
    Player(int, int, int);
    void translateCard(int);
    int numberOfFolders();
    void MP3Folder(int);
    int getFolders();
    void loadIdentifiers(String[]);
    void setDFOptions();
    void next();
    int countFolderFiles(int);
    void folder();
    void play();
};



#endif