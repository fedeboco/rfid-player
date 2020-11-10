#include "player.h"
#include "SoftwareSerial.h"

Player::Player(int rx, int tx) : mySoftwareSerial{SoftwareSerial(rx,tx)}, timer{millis()} {
    
}

void Player::start() {
    mySoftwareSerial.begin(9600);
    Serial.println();
    Serial.println(F("DFRobot DFPlayer Mini Demo"));
    Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

    // el segundo false fuerza un reset que popea el DAC y el parlante
    while (!myDFPlayer.begin(mySoftwareSerial, true, false)) {  //Use softwareSerial to communicate with mp3.
        delay(1000);
        Serial.println(F("Unable to begin:"));
        Serial.println(F("1.Please recheck the connection!"));
        Serial.println(F("2.Please insert the SD card!"));
    }
    Serial.println(F("DFPlayer Mini online."));

    myDFPlayer.volume(5);  //Set volume value. From 0 to 30
    myDFPlayer.play(1);  //Play the first mp3
}

void Player::processPlayer() {
    if (millis() - timer > 150) {
        myDFPlayer.volume(100);
    }

    if (millis() - timer > 3000) {
        timer = millis();
        //myDFPlayer.next();  //Play next mp3 every 3 second.
        if (myDFPlayer.available()) {
        handleDFState(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
        }
    }
}

void Player::handleDFState(uint8_t type, int value) {
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