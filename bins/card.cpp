#include "card.h"

Card::Card(int ssPin, int rstPin) : cardReader{MFRC522(ssPin, rstPin)} {
}

void Card::init() {
    SPI.begin();      // Initiate  SPI bus
    cardReader.PCD_Init();   // Initiate cardReader
}

void Card::processCard() {
  // Look for new cards
  if ( ! cardReader.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! cardReader.PICC_ReadCardSerial()) 
  {
    return;
  }
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
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "BD 31 15 2B") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(3000);
  }
 
 else   {
    Serial.println(" Access denied");
    delay(3000);
  }
}   