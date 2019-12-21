static byte treeMode = 1; // режим свечения

static int treeLow = 0; // нижняяя граница в общем массиве
static int treeHigh = NUMLEDS_TREE - 1; // верхняя граница в общем массиве

// всего в гирлянде 100 светодиодов
// вешается на елку в фроме UUU - получаем 4 вершины и 6 лучей, которые снизу соединены 3 отрезками
static byte tops[4] = {0, 33, 66, 99}; // индексы вершин
// центры - 16, 49, 82

static byte treeActive = 0; // сколько элементов уже светится
static byte treeTimeout = 0; // сколько осталось до начала зажигания следующего элемента


void treeInit()
{
}

void treeTick()
{
  if(treeState.tuningMode)
  {
      treeModeZero();
  }
  else switch (treeMode)
  {
    case 1:
      treeModeBlink();
      break;
    case 2:
      treeModeWaterFall();
      break;
  }
}

void treeModeZero()
{
  //зажжем вершины
  for (byte i = 4; i > 0;)strip.setColor(tops[--i], RED);
  // зажжем нижние отрезки
  for (byte i = 0; i < 3; i++)
  {
    byte center = tops[i] + (tops[i + 1] - tops[i]) / 2;
    //uart.println(String(center));
    byte btmLow = center - 1;
    byte btmHigh = center + 2;

    for (byte j = btmLow; j <= btmHigh; j++)
    {
      strip.setColor(j, GREEN);
    }
    for (byte j = 1; j <= 14; j++)
    {
      strip.setHSV(tops[i] + j, 50 + j * 5, 255, 255);
      strip.setHSV(tops[i + 1] - j, 50 + j * 5, 255, 255);
    }
  }
}

void treeModeBlink()
{
  treeAddNewLamp();
  treeChangeValue();
}

void treeAddNewLamp()
{
  if (treeActive < TREE_MAX)
  {
    // зажгем один элемент
    if (treeTimeout > 0)
    {
      treeTimeout--;
    }
    else
    {
      int newIndex = getNewTreePosition();
      ledPoints[newIndex].value = random(0, 255);
      ledPoints[newIndex].mode = 0x81;

      treeActive ++;
      treeTimeout = 1;
    }
  }
}

int getNewTreePosition() // определим совободную позицию на ленте
{
  int newPosition = 0;
  while (true)
  {
    byte shift = random(NUMLEDS_TREE); // берем рандомайзер
    newPosition = treeLow + shift;
    bool checked = ledPoints[newPosition].mode == 0;

    if (checked)
    {
      // сделаем другие элементы достпными для выбора (т.к. при гашении они блокируются для выбора)
      for (byte j = 0; j < NUMLEDS_TREE; j++)
      {
        // Ограничение частоты выбора случайного элемента, устанавливается после гашения в макс(128, 255 - число активных элементов)
        if (ledPoints[treeLow + j].mode == 0xFF) {
          ledPoints[treeLow + j].value--;
          if (ledPoints[treeLow + j].value == 0) ledPoints[treeLow + j].mode = 0;
        }
      }
      break;
    }
  }
  return newPosition;
}

void treeChangeValue()
{
  // раззожем и потушим
  for (int index = treeLow; index <= treeHigh; index++)
  {
    if (ledPoints[index].mode == 0 || ledPoints[index].mode == 0xFF)
    {
      strip.setColor(index, BLACK);
    }
    else
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
          treeActive --;
          ledPoints[index].mode = 0xFF;
          ledPoints[index].value = NUMLEDS_TREE / 3;
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

void treeModeWaterFall()
{
//static byte tops[4] = {0, 33, 66, 99}; // индексы вершин
  //зажжем вершины
  for (byte i = 4; i > 0;)strip.setColor(tops[--i], RED);
  // зажжем нижние отрезки
  for (byte i = 0; i < 3; i++)
  {
    byte center = tops[i] + (tops[i + 1] - tops[i]) / 2;
    //uart.println(String(center));
    byte btmLow = center - 1;
    byte btmHigh = center + 2;

    for (byte j = btmLow; j <= btmHigh; j++)
    {
      strip.setColor(j, GREEN);
    }
    for (byte j = 1; j <= 14; j++)
    {
      strip.setHSV(tops[i] + j, 50 + j * 5, 255, 255);
      strip.setHSV(tops[i + 1] - j, 50 + j * 5, 255, 255);
    }
  }

  
}
