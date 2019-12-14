
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

LEDdata leds[NUMLEDS];  // буфер ленты типа LEDdata (размер зависит от COLOR_DEBTH)
microLED strip(leds, NUMLEDS, LED_PIN);  // объект лента

// получение команд из IR пульта по I2C с другого ардуино
#include <Wire.h>

byte  MainLoop;
unsigned long main_timer;

void setup() {
  MainLoop = 5;
  
  uart.begin(9600);
  uart.println("start-nyt");

  strip.setBrightness(200);    // яркость (0-255)
  // яркость применяется при выводе .show() !
  strip.clear();   // очищает буфер
  strip.show(); // выводим изменения на ленту

  treeInit();

  Wire.begin(42);
  Wire.onReceive(receiveEvent); // зарегистрировать обработчик события

  randomSeed(millis());
}

void receiveEvent(int howMany) 
{
  while (Wire.available()) // пройтись по всем до последнего
  { 
    byte c = Wire.read();    // принять байт как символ
    uart.println(c, HEX);         // напечатать символ
  }
}


void loop() {

  if (millis() - main_timer > MainLoop) {
    starTick(); // звезда

    stripTick(); // лента вокруг ствола под звездой

    treeTick(); // дерево елка

    strip.show();
    main_timer = millis();    // сбросить таймер
  }
}
