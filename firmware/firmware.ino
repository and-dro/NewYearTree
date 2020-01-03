
// пример работы с лентой
#define LED_PIN 13       // пин ленты
#define BUZZER_PIN 12       // пин ленты
#define NUMLEDS_TREE 100     // кол-во светодиодов на елке
#define NUMLEDS_STAR 30      // кол-во светодиодов на звезде
#define NUMLEDS_STRIP 156    // кол-во светодиодов на переходной ленте
#define NUMLEDS 286 // кол-во светодиодов
#define STRIP_MAX 50 // максимум светодиодов на ленте одновремено
#define TREE_MAX 80 // максимум светодиодов на дереве
#define TREE_LINES 6 // количество вертикальных полосок на елке

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
  bool start;
  bool tuningMode;
  bool buzzerEnable;
  byte buzzerDouble;
  byte  MainLoop;
  bool pause;
  int currentma;
  byte treeMode;
  byte  fadeMode;
} treeState; // управляемое состояние елки

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);

  strip.setVoltage(5000); // светим на 5 вольтах

  StateInit();

  strip.setVoltage(5000); // светим на 5 вольтах
  strip.setMaxCurrent(treeState.currentma); // ограничение тока == ограничение светимости

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
  // получим управляющий сигнал от внешнего IR источника (сделан на отдельном чипе, т.к. здесь прерывания отключаются для работы с лентой)
  while (Wire.available()) // пройтись по всем до последнего
  {
    byte cmd = Wire.read();    // принять байт как символ
    // uart.println(cmd, HEX); // напечатать символ
    StateUpdate(cmd);
  }
}


void loop() {

  uartTick();
  buzzerTick();

  if (treeState.fadeMode != 0)
  {
    if (treeState.fadeMode == 2)
    {
      bool faded = true;
      for (uint16_t i = 0; i < NUMLEDS; i++)
      {
        if (strip.getColorHEX(i) != 0x0000)
        {
          strip.fade(i, 1);
          faded = false;
        }
      }
      if (faded) treeState.fadeMode = 1;
      strip.show();
    }
    if (treeState.fadeMode >= 4)
    {
      for (uint16_t i = 0; i < NUMLEDS_TREE; i++)
      {
        strip.setHSV(i, treeState.fadeMode * 5 + i * 2, 200, treeState.fadeMode);
      }
      for (uint16_t i = 0; i < NUMLEDS_STAR; i++)
      {
        strip.setHSV(NUMLEDS_TREE + NUMLEDS_STRIP + i, treeState.fadeMode * 2, 150, treeState.fadeMode);
      }
      treeState.fadeMode++;
      if (treeState.fadeMode == 28) {
        treeState.fadeMode = 0;
        if (treeState.start)
        {
          treeState.start = false;
          treeState.buzzerEnable = false;
        }
      }
      strip.show();
    }
  }
  else if ((millis() - main_timer > treeState.MainLoop) && !treeState.pause) {
    starTick(); // звезда

    stripTick(); // лента вокруг ствола под звездой

    treeTick(); // дерево елка

    strip.show();
    main_timer = millis();    // сбросить таймер
  }
}
