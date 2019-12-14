struct pixelData {
  byte index, h, v;
}; // тип - параметры одного элемента

static byte stripMode = 0; // режим свечения
static int stripLow = NUMLEDS_TREE; // нижняяя граница в общем массиве
static int stripHigh = NUMLEDS_TREE + NUMLEDS_STRIP - 1; // верхняя граница в общем массиве
static byte stripActive = 0; // сколько элементов уже светится
static byte stripTimeout = 0; // сколько осталось до начала зажигания следующего элемента
static pixelData stripPoints[STRIP_MAX]; // массив светящихся элементов
static byte stripLast[NUMLEDS_STRIP]; // счетчик последних зажигавшихся элементов (чтобы заставить светиться все)
static struct pixelData pixel; // параметры текущего элемента


void stripTick() // обработчик одного цикла для ленты
{
  if (stripMode == 0)
  {
    stripAddNewLamp();
    stripChageValue();
  }
}

void stripAddNewLamp() // если не все лампы горят, добавим одну
{
  if (stripActive < STRIP_MAX)
  {
    // зажгем один элемент
    if (stripTimeout > 0)
    {
      stripTimeout--;
    }
    else
    {
      byte ledIndex = getFreePoint();
      int newIndex = getNewPosition();

      stripPoints[ledIndex].index = newIndex;
      stripPoints[ledIndex].h = random(0, 127) * 2;
      stripPoints[ledIndex].v = 1;

      stripActive ++;
      stripTimeout = 5;
    }
  }
}

byte getFreePoint() //найдем свободное место в коллекции указателей состояний
{
  byte freePoint = 0;
  for (byte i = 0; i < STRIP_MAX; i++)
  {
    if (stripPoints[i].index == 0)
    {
      freePoint = i;
      break;
    }
  }
  return freePoint;
}

int getNewPosition() // определим совободную позицию на ленте
{
  int newPosition = 0;
  while (true)
  {
    byte shift = random(stripHigh - stripLow); // берем рандомайзер
    newPosition = stripLow + shift;
    bool checked = true;
    for (byte i = 0; i < STRIP_MAX; i++)
    {
      if (stripPoints[i].index != 0)
      {
        checked = checked && abs(stripPoints[i].index - newPosition) > 1 && stripLast[shift] == 0; // рядом нельзя, слишком часто нельзя
      }
    }
    if (checked)
    {
      stripLast[shift] = NUMLEDS_STRIP - STRIP_MAX; // этот массив не дает использовать элемент слишком часто
      for (byte j = 0; j < NUMLEDS_STRIP; j++)
      {
        if (stripLast[j]) stripLast[j]--;
      }
      break;
    }
  }
  return newPosition;
}

void stripChageValue() // изменим яркость активных элементов
{
  // раззожем и потушим
  for (byte ledIndex = 0; ledIndex < STRIP_MAX; ledIndex++)
  {
    if(stripPoints[ledIndex].index != 0)
    {
      pixel = stripPoints[ledIndex];
      if (pixel.v < 100)// разжигаем
      {
        stripPoints[ledIndex].v = pixel.v + (1 + (pixel.v > 20 ? 1 : 0) + (pixel.v > 40 ? 1 : 0));
        strip.setHSV(pixel.index, pixel.h, 255, pixel.v);
        if (pixel.v >= 61) stripPoints[ledIndex].v = 162;
      }
      else // розжиг завершен, пора тушить
      {
        // гасим
        if (pixel.v > 100)
        {
          stripPoints[ledIndex].v = pixel.v - ((1 + (pixel.v > 120 ? 1 : 0) + (pixel.v > 140 ? 1 : 0)));
          strip.setHSV(pixel.index, pixel.h, 255, pixel.v - 100);
        }
        else
        { 
          // выключаем
          strip.setColor(pixel.index, BLACK);
          // освобождаем элемент
          stripPoints[ledIndex].index = 0;
          stripActive --;
        }
      }
    }
  }
}
