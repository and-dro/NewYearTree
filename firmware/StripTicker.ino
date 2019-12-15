static byte stripMode = 0; // режим свечения
static int stripLow = NUMLEDS_TREE; // нижняяя граница в общем массиве
static int stripHigh = NUMLEDS_TREE + NUMLEDS_STRIP - 1; // верхняя граница в общем массиве
static byte stripActive = 0; // сколько элементов уже светится
static byte stripTimeout = 0; // сколько осталось до начала зажигания следующего элемента

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
      int newIndex = getNewPosition();
      ledPoints[newIndex].value = random(0, 255);
      ledPoints[newIndex].mode = 0x81;

      stripActive ++;
      stripTimeout = 5;
    }
  }
}

int getNewPosition() // определим совободную позицию на ленте
{
  int newPosition = 0;
  while (true)
  {
    byte shift = random(NUMLEDS_STRIP); // берем рандомайзер
    newPosition = stripLow + shift;
    bool checked = true;
    for (byte i = 0; i < NUMLEDS_STRIP; i++)
    {
      if (ledPoints[stripLow + i].mode > 0)
      {
        checked = checked && abs(i - shift) > 1; // рядом нельзя, слишком часто нельзя
      }
    }
    if (checked)
    {
      // сделаем другие элементы достпными для выбора (т.к. при гашении они блокируются для выбора)
      for (byte j = 0; j < NUMLEDS_STRIP; j++)
      {
        // Ограничение частоты выбора случайного элемента, устанавливается после гашения в макс(128, 255 - число активных элементов)
        if (ledPoints[stripLow + j].mode == 0xFF) {
          ledPoints[stripLow + j].value--;
          if (ledPoints[stripLow + j].value == 0) ledPoints[stripLow + j].mode = 0;
        }
      }
      break;
    }
  }
  return newPosition;
}

void stripChageValue() // изменим яркость активных элементов
{
  // раззожем и потушим
  for (int index = stripLow; index <= stripHigh; index++)
  {
    if (ledPoints[index].mode)
    {
      uint8_t val = ledPoints[index].mode & 0x3F;
      if ((ledPoints[index].mode & 0xC0) == 0x40)
      {
        // гасим
        val -= (1 + (val > 20 ? 1 : 0) + (val > 40 ? 1 : 0));
        if (val > 1)
        {
          strip.setHSV(index, ledPoints[index].value, 255, val);
          ledPoints[index].mode = 0x40 | val;
        }
        else
        {
          // отправляем в очередь запрета розжига
          strip.setColor(index, BLACK);
          stripActive --;
          ledPoints[index].mode = 0xFF;
          ledPoints[index].value = NUMLEDS_STAR / 3;
        }
      }
      if ((ledPoints[index].mode & 0xC0) == 0x80)
      {
        // разжигаем
        val += (1 + (val > 20 ? 1 : 0) + (val > 40 ? 1 : 0));
        strip.setHSV(index, ledPoints[index].value, 255, val);
        ledPoints[index].mode = 0x80 | val;
        if (val >= 63) {
          ledPoints[index].mode = 0x40 | 0x3F;
        }
      }
    }
  }
}
