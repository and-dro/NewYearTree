
// пример работы с лентой
#define LED_PIN 13       // пин ленты
#define BUZZER_PIN 12       // пин ленты
#define NUMLEDS_TREE 100     // кол-во светодиодов на елке
#define NUMLEDS_STAR 30      // кол-во светодиодов на звезде
#define NUMLEDS_STRIP 156    // кол-во светодиодов на переходной ленте
#define NUMLEDS 286 // кол-во светодиодов
#define STRIP_MAX 50 // максимум светодиодов на ленте одновремено
#define TREE_MAX 80 // максимум светодиодов на дереве

struct pixelData {
  uint8_t mode, value;
}; // тип - параметры одного элемента
static pixelData ledPoints[NUMLEDS_TREE + NUMLEDS_STRIP]; // массив параметров светящихся элементов

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

#include "stripHSV.h"

unsigned long main_timer;

static struct treeState {
  bool tuningMode;
  byte buzzerDouble;
  byte  MainLoop;
  bool pause;
  int currentma;
  byte treeMode;
} treeState; // управляемое состояние елки

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);

  buzzerOn(25);
  
  strip.setVoltage(5000); // светим на 5 вольтах
  
  StateInit();

  strip.setVoltage(5000); // светим на 5 вольтах
  strip.setMaxCurrent(treeState.currentma); // ограничение тока == ограничение светимости
  
  uart.begin(9600);
  uart.println("start-nyt2");

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
    byte cmd = Wire.read();    // принять байт как символ
    // uart.println(cmd, HEX);         // напечатать символ
    StateUpdate(cmd);
  }
}


void loop() {

  uartTick();
  
  if ((millis() - main_timer > treeState.MainLoop) && !treeState.pause) {
    starTick(); // звезда

    stripTick(); // лента вокруг ствола под звездой

    treeTick(); // дерево елка

    strip.show();
    main_timer = millis();    // сбросить таймер
  }
}
