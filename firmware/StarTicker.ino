static byte starMode = 2;
static int starModeCounter = 300;

static byte starSaturation = 200;
static byte starSaturationCnt = 6;
static byte starSaturationDif = -1;

static byte starValue = 48;

static int rnbCounter = 0;

static int starLow = NUMLEDS_TREE + NUMLEDS_STRIP; // нижняяя граница в общем массиве
static int starHigh = NUMLEDS_TREE + NUMLEDS_STRIP + NUMLEDS_STAR - 1;


void starTick() {
  // обновление базового цвета
  rnbCounter = rnbCounter + 3;

  if (starMode == 0)
  {
    // волны цвета от центра к краям
    for (byte rnd = 0; rnd < 6; rnd++) { // цикл по кругам
      int colorId = rnbCounter + rnd * 10;
      LEDdata starColor = mHSV(colorId, starSaturation, starValue);
      for (byte line = 0; line < 5; line++) { // цикл по лучам
        strip.setLED(starHigh - rnd - line * 6, starColor);
      }
    }
  }
  else if (starMode == 1)
  {
    // волны цвета по часовой стрелке
    for (byte line = 0; line < 5; line++) { // цикл по лучам
      int colorId = rnbCounter + line * 28;
      LEDdata starColor = mHSV(colorId, starSaturation, starValue);
      for (byte rnd = 0; rnd < 6; rnd++) { // цикл по кругам
        strip.setLED(starHigh - rnd - line * 6, starColor);
      }
    }
  }
  else if (starMode == 2)
  {
    // волны цвета по часовой стрелке
    for (byte line = 0; line < 5; line++) { // цикл по лучам
      for (byte rnd = 0; rnd < 6; rnd++) { // цикл по кругам
        strip.setHSV(starHigh - rnd - line * 6, rnbCounter + line * 28 + rnd * 10, starSaturation, starValue);
      }
    }
  }

  // переключение режима
  starModeCounter--;
  if (!starModeCounter)
  {
    starMode = starMode == 0 ? 2 : starMode-1;
    starModeCounter = 300;
  }
  // переключение насыщенности
  starSaturationCnt--;
  if (!starSaturationCnt)
  {
    starSaturationCnt = 5;
    starSaturation = starSaturation + starSaturationDif;
    if (starSaturation > 200)
    {
      starSaturationDif = -1;
    }
    else if (starSaturation < 120)
    {
      starSaturationDif = 1;
    }

  }
}
