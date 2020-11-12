#include "Arduino.h"
#include "player.h"
#include "card.h"
#include "config.h"

#define SS_PIN 10
#define RST_PIN 9
#define RX_DF_PIN 5
#define TX_DF_PIN 6
#define BUTTON_PIN 4

Card card(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Config config(BUTTON_PIN);
Player player(RX_DF_PIN, TX_DF_PIN, config.readFoldersEEPROM());

void setup()
{
  card.init();
  Serial.begin(115200);
  int pendingConfigurations = -1;

  if(config.readButton()){
    player.start();
    //while para darle tiempo a que actualice la cantidad de carpetas
    while (pendingConfigurations == -1){
      pendingConfigurations = player.numberOfFolders();
      delay(1000);
    }

    pendingConfigurations += 2;
    Serial.println(pendingConfigurations);
    
    config.writeFoldersEEPROM(pendingConfigurations - 2);


    for (int i = 0; pendingConfigurations > 0; i++, pendingConfigurations--) {
      if (pendingConfigurations == 2)
        i = 10;
      player.MP3Folder(i + 1);
      while(!card.isNewCard()){
        card.processCard();
        delay(1000);
      }
      //first bytes for general config
      config.writeEEPROM(card.getCard(), i + 1);
    }
    for (int i = 0; i < player.getFolders(); i++) {
      Serial.println(config.readEEPROM(i + 1));
    }
    
  } else {
    player.start();
    for (int i = 0; i < player.getFolders(); i++) {
      Serial.println(config.readEEPROM(i + 1));
    }
  } 

}

void loop()
{
  card.processCard();
  player.processPlayer(card.getCard(), card.isNewCard());
  card.isNewCard() ? delay(2000) : delay(100);
}
