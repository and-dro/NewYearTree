void StateInit()
{
  treeState.MainLoop = 5;
  treeState.currentma = 1251;
  treeState.buzzerEnable = true;
  treeState.buzzerDouble = 1;
  treeState.fadeMode = 4;
  treeState.start = true;
  
  buzzerOn(25);
}

unsigned long buzzer_timer;

void buzzerOn(byte duration)
{
  if(treeState.buzzerEnable)
  {
    digitalWrite(BUZZER_PIN, LOW);
    buzzer_timer = millis() + duration;
  }
}


void buzzerTick()
{
  if (buzzer_timer && buzzer_timer < millis())
  {
    digitalWrite(BUZZER_PIN, HIGH);
    buzzer_timer = 0;
    if (treeState.buzzerDouble == 1) {
      buzzer_timer = millis() + 1000;
      treeState.buzzerDouble = 2;
    }
    if (treeState.buzzerDouble == 2) {
      treeState.buzzerDouble = 0;
      buzzerOn(100);
    }
  }
}
void uartTick()
{
  if (uart.available())
  {
    char c = uart.read();
    switch (c)
    {
      case 'q':
        StateUpdate(0x11);
        break;
      case 'w':
        StateUpdate(0x12);
        break;
      case 'e':
        StateUpdate(0x13);
        break;
      case 'a':
        StateUpdate(0x21);
        break;
      case 's':
        StateUpdate(0x22);
        break;
      case 'd':
        StateUpdate(0x23);
        break;
      case 'z':
        StateUpdate(0x31);
        break;
      case 'x':
        StateUpdate(0x32);
        break;
      case 'c':
        StateUpdate(0x33);
        break;
    }
  }
}

void StateUpdate(uint8_t cmd)
{
  if(cmd != 0x11) buzzerOn(15);
  switch (cmd)
  {
    case 0x11: // выкл звук
      treeState.buzzerEnable = false;
      break;
    case 0x12: // вкл/выкл затухание
      if(treeState.fadeMode != 0) treeState.fadeMode = 4;
      else treeState.fadeMode = 2;
      break;
    case 0x13: // вкл звук
      treeState.buzzerEnable = true;
      buzzerOn(500);
      break;
    case 0x22: // двигаем быстрее
      if (treeState.MainLoop <= 5) treeState.MainLoop = 1;
      else if (treeState.MainLoop <= 30) treeState.MainLoop = 5;
      else treeState.MainLoop -= 25;
      treeState.pause = false;
      uart.println(treeState.MainLoop);
      if(treeState.MainLoop == 1) treeState.buzzerDouble = 1;
      break;

    case 0x21: // двигаем медленнее
      if (treeState.MainLoop < 75)
      {
        if (treeState.MainLoop == 1) treeState.MainLoop = 5;
        else treeState.MainLoop += 25;
      }
      else treeState.buzzerDouble = 1;
      treeState.pause = false;
      uart.println(treeState.MainLoop);
      break;

    case 0x23: // замри
      treeState.pause = !treeState.pause;
      treeState.tuningMode = false;
      break;

    case 0x31: // тусклее
    case 0x32: // ярче
      if (treeState.currentma == 1251) treeState.currentma = cmd == 0x32 ? 1251 : 351;
      else if (treeState.currentma == 351) treeState.currentma = cmd == 0x32 ? 1251 : 301;
      else if (treeState.currentma == 301) treeState.currentma = cmd == 0x32 ? 351 : 251;
      else if (treeState.currentma == 251) treeState.currentma = cmd == 0x32 ? 301 : 251;
      if(treeState.currentma == 251 || treeState.currentma == 1251) treeState.buzzerDouble = 1;
      strip.setMaxCurrent(treeState.currentma);
      uart.println(treeState.currentma);
      break;

    case 0x33: // режим настройки
      treeState.tuningMode = !treeState.tuningMode;
      treeState.pause = false;
      break;

    case 0x51:
    case 0x61:
    case 0x71:
      treeState.treeMode = 1;
      break;

    case 0x52:
    case 0x62:
    case 0x72:
      treeState.treeMode = 2;
      break;

    case 0x53:
    case 0x63:
    case 0x73:
      treeState.treeMode = 3;
      break;

    default:
      uart.print("case 0x");
      uart.println(cmd, HEX);
      break;
  }
}
