
// пример работы с лентой
#define LED_PIN 13       // пин ленты
#define NUMLEDS_TREE 100      // кол-во светодиодов на елке
#define NUMLEDS_STAR 30      // кол-во светодиодов на звезде
#define NUMLEDS_STRIP 156      // кол-во светодиодов на переходной ленте
#define NUMLEDS 286 // кол-во светодиодов
#define STRIP_MAX 50 // максимум светодиодов на ленте одновремено

#define ORDER_RGB      // порядок цветов ORDER_GRB / ORDER_RGB / ORDER_BRG

#define COLOR_DEBTH 3   // цветовая глубина: 1, 2, 3 (в байтах)
// на меньшем цветовом разрешении скетч будет занимать в разы меньше места,
// но уменьшится и количество оттенков и уровней яркости!

// ВНИМАНИЕ! define настройки (ORDER_GRB и COLOR_DEBTH) делаются до подключения библиотеки!
#include <microLED.h> // https://github.com/AlexGyver/GyverLibs/tree/master/microLED

#include <IRremote.h> // https://github.com/z3t0/Arduino-IRremote

LEDdata leds[NUMLEDS];  // буфер ленты типа LEDdata (размер зависит от COLOR_DEBTH)
microLED strip(leds, NUMLEDS, LED_PIN);  // объект лента

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  Serial.println("start");

  strip.setBrightness(200);    // яркость (0-255)
  // яркость применяется при выводе .show() !
  strip.clear();   // очищает буфер
  strip.show(); // выводим изменения на ленту
  
  treeInit();
  
  irrecv.enableIRIn(); // Start the receiver

  randomSeed(millis());
}

void loop() {
    
    starTick(); // звезда
    
    stripTick(); // лента вокруг ствола под звездой
    
    treeTick(); // дерево елка

    strip.show();
    delay(10);

  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
}
