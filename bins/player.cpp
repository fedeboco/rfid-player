#include "player.h"
#include "SoftwareSerial.h"

Player::Player(int rx, int tx, int folders) : 
                mySoftwareSerial{SoftwareSerial(rx,tx)}, 
                timer{millis()},
                folders{folders} {
}

int Player::getFolders() {
    return folders;
}

void Player::start() {
    mySoftwareSerial.begin(BAUD_RATE);
    Serial.println();
    Serial.println(F(DFSTARTUP));

    // second boolean forces restart and pops speaker
    while (!myDFPlayer.begin(mySoftwareSerial, true, true)) {
        delay(DFRECONNECT_DELAY);
        Serial.println(F(DF_NOT_DETECTED));
    }
    Serial.println(F(DF_CONNECTED));
    currentFolderTracks = -1;
    while (currentFolderTracks == -1) {
        delay(DFRECONNECT_DELAY);
        currentFolderTracks = myDFPlayer.readFileCountsInFolder(1);
    };
    setDFOptions();
}

void Player::setDFOptions() {
    myDFPlayer.setTimeOut(DFTIMEOUT);
    myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
    myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
    myDFPlayer.volume(VOLUME); //0 to 30
}

int Player::numberOfFolders() {
    return myDFPlayer.readFolderCounts();
}

void Player::MP3Folder(int fileNumber) {
    myDFPlayer.playMp3Folder(fileNumber);
    delay(DFRECONNECT_DELAY);
}

void Player::translateCard(int index){
    if (index == folders + 1) {
        cardType = PLAY;
    } else if (index == folders + 2) {
        cardType = NEXT;
    } else if (index <= folders) {
        cardType = FOLDER;
        folderPlaying = index;
    } else {
        cardType = INVALID;
    }
}

int Player::countFolderFiles(int index) {
    return myDFPlayer.readFileCountsInFolder(index);
}

// asumes device turns on in folder 1
void Player::next() {
    Serial.print("Tracks:");
    Serial.println(currentFolderTracks);
    if (currentTrack == currentFolderTracks) {
        currentTrack = 1;
        myDFPlayer.loopFolder(folderPlaying);
    } else {
        currentTrack++;
        myDFPlayer.next();
    }
    playing = true;
}

void Player::play() {
    if (playing == true)
        myDFPlayer.pause();
    else
        myDFPlayer.start();
    playing = !playing;
}

void Player::folder() {
    currentTrack = 1;
    currentFolderTracks = countFolderFiles(folderPlaying);
    myDFPlayer.loopFolder(folderPlaying);
}

void Player::processPlayer(String card, bool updatedCard) {
    if (updatedCard) {
        if (cardType == NEXT) {
            next();
        } else if (cardType == PLAY) {
            play();
        } else if (cardType == FOLDER) {
            folder();
        } else {
            Serial.println(INVALID_CARD);
        }

        if (myDFPlayer.available())
            handleDFState(myDFPlayer.readType(), myDFPlayer.read());
    }
}

 // Print the detail message from DFPlayer to handle 
 // different errors and states.
void Player::handleDFState(uint8_t type, int value) {
    Serial.println(type);
    switch (type) {
        case TimeOut:
        Serial.println(F("Time Out!"));
        break;
        case WrongStack:
        Serial.println(F("Stack Wrong!"));
        break;
        case DFPlayerCardInserted:
        Serial.println(F("Card Inserted!"));
        break;
        case DFPlayerCardRemoved:
        Serial.println(F("Card Removed!"));
        break;
        case DFPlayerCardOnline:
        Serial.println(F("Card Online!"));
        break;
        case DFPlayerPlayFinished:
        Serial.print(F("Number:"));
        Serial.print(value);
        Serial.println(F(" Play Finished!"));
        break;
        case DFPlayerError:
        Serial.print(F("DFPlayerError:"));
        switch (value) {
            case Busy:
            Serial.println(F("Card not found"));
            break;
            case Sleeping:
            Serial.println(F("Sleeping"));
            break;
            case SerialWrongStack:
            Serial.println(F("Get Wrong Stack"));
            break;
            case CheckSumNotMatch:
            Serial.println(F("Check Sum Not Match"));
            break;
            case FileIndexOut:
            Serial.println(F("File Index Out of Bound"));
            break;
            case FileMismatch:
            Serial.println(F("Cannot Find File"));
            break;
            case Advertise:
            Serial.println(F("In Advertise"));
            break;
            default:
            break;
        }
        break;
        default:
        break;
    }
}