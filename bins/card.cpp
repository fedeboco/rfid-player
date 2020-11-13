#include "card.h"

Card::Card(int ssPin, int rstPin) : cardReader{MFRC522(ssPin, rstPin)} {
}

void Card::init() {
    SPI.begin();      // Initiate  SPI bus
    cardReader.PCD_Init();   // Initiate cardReader
}

String Card::getCard(){
    return card;
}

bool Card::isNewCard(){
    bool aux = newCard;
    newCard = false;
    return aux;
}

void Card::processCard() {
  // Look for new cards
  newCard = false;
  if ( ! cardReader.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! cardReader.PICC_ReadCardSerial()) 
  {
    return;
  }
  newCard = true;
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < cardReader.uid.size; i++) 
  {
     Serial.print(cardReader.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(cardReader.uid.uidByte[i], HEX);
     content.concat(String(cardReader.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(cardReader.uid.uidByte[i], HEX));
  }
  Serial.println();
  content.toUpperCase();
  card = content;
}   