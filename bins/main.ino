#include "Arduino.h"
#include "player.h"
#include "card.h"
 
#define SS_PIN 10
#define RST_PIN 9
#define RX_DF_PIN 5
#define TX_DF_PIN 6

Card card(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Player player(RX_DF_PIN, TX_DF_PIN);

void setup()
{
  card.init();
  Serial.begin(115200);
  player.start();
}

void loop()
{
  player.processPlayer();
  card.processCard();
}
