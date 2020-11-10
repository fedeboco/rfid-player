#ifndef DFPLAYERH
#define DFPLAYERH

#include "DFRobotDFPlayerMini.h"
#include "SoftwareSerial.h"

class Player {
    private:
    DFRobotDFPlayerMini myDFPlayer;
    SoftwareSerial mySoftwareSerial; // RX, TX
    unsigned long timer;

    public:
    void handleDFState(uint8_t, int);
    void processPlayer();
    void start();
    Player(int, int);
};



#endif