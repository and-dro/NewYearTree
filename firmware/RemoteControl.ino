void StateInit()
{
  treeState.MainLoop = 5;
}

void uartTick()
{
  if(uart.available())
  {
    char c = uart.read();
    switch(c)
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
  switch(cmd)
  {
    case 0x22:
      if(treeState.MainLoop > 1) treeState.MainLoop--;
      uart.println(treeState.MainLoop);
      break;
      
    case 0x21:
      treeState.MainLoop++;
      uart.println(treeState.MainLoop);
      break;

    case 0x23:
      treeState.pause = !treeState.pause;
      break;
   default:
      uart.print("case 0x");
      uart.println(cmd, HEX);
      break;
  }
}
