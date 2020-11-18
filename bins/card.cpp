#include "card.h"

Card::Card(int ssPin, int rstPin) : cardReader{MFRC522(ssPin, rstPin)} {
}

void Card::init() {
    SPI.begin();
    cardReader.PCD_Init();
}

String Card::getCard(){
    return card;
}

bool Card::isNewCard(){
    bool aux = newCard;
    newCard = false;
    return aux;
}

String Card::getIDfromReader() {
  String content= "";
  byte letter;
  for (byte i = 0; i < cardReader.uid.size; i++) {
      content.concat(String(cardReader.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(cardReader.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  return content;
}

bool Card::updatesAvailable() {
  return cardReader.PICC_IsNewCardPresent() &&
         cardReader.PICC_ReadCardSerial();
}

bool Card::processCard() {
  if ((newCard = updatesAvailable()) == false)
    return false;

  card = getIDfromReader();

  Serial.print("ID:");
  Serial.println(card);
  
  return true;
}   