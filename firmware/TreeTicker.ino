static byte treeMode = 1; // режим свечения

static int treeLow = 0; // нижняяя граница в общем массиве
static int treeHigh = NUMLEDS_TREE - 1; // верхняя граница в общем массиве

// всего в гирлянде 100 светодиодов
// вешается на елку в фроме UUU - получаем 4 вершины и 6 лучей, которые снизу соединены 3 отрезками
static byte tops[4] = {0,33,66,99}; // индексы вершин
// центры - 16, 49, 82

void treeInit()
{
  //зажжем вершины
  for(byte i=4;i>0;)strip.setColor(tops[--i], RED);
  // зажжем нижние отрезки
  for(byte i=0;i<3;i++)
  {
    byte center = tops[i] + (tops[i+1] - tops[i])/ 2;
    //uart.println(String(center));
    byte btmLow = center-1;
    byte btmHigh = center+2;
    
    for(byte j = btmLow; j <= btmHigh; j++)
    {
      strip.setColor(j, GREEN);
    }

    
    for(byte j = 1; j <= 14; j++)
    {
      strip.setHSV(tops[i] + j, 50 + j * 5, 255, 255);
      strip.setHSV(tops[i+1] - j, 50 + j * 5, 255, 255);
    }
    
  }
}

void treeTick()
{
  if (treeMode == 0)
  {
  }
}
