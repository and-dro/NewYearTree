// получение команд из IR пульта
#include "IRLremote.h" // https://github.com/NicoHood/IRLremote
#include <Wire.h>

#define pinIR 2
CHashIR IRLremote;


int IR_row;
int IR_column;

void setup() {
  uart.begin(9600);
  uart.println("start");
  
  IRControlInit();

  Wire.begin();
}

void IRControlInit() {
  if (!IRLremote.begin(pinIR))
    uart.println("IR pin error");
  IR_row = 0;
  IR_column = 0;
}

void loop() {
  IRControlCheck();
}

void IRControlCheck() {
  if (IRLremote.available()) {
    auto data = IRLremote.read();
    if (data.command == 0x117697CD) {
      // off or repeat
    }
    else
    {
      bool mapped = mapButton(data.command);
      if (mapped)
      {
        uart.print("row: ");
        uart.print(IR_row);
        uart.print(", column: ");
        uart.println(IR_column);
        
        uint8_t toSend = IR_row << 4 | (IR_column & 0x0F);
        // uart.println(toSend, HEX);
        
        Wire.beginTransmission(42); 
        Wire.write(toSend);
        Wire.endTransmission();         
      }
      else
      {
        uart.print("case 0x");
        uart.print(data.command, HEX);
        uart.println(":");
      }
    }
  }
}

bool mapButton(long command) {
  bool mapped = true;
  switch (command) {
    case 0x4E5BA3AD: //ch+
      IR_row = 1;
      IR_column = 1;
      break;
    case 0xE51CA6AD: //ch
      IR_row = 1;
      IR_column = 2;
      break;
    case 0xE207E1AD: //ch-
      IR_row = 1;
      IR_column = 3;
      break;
    case 0x517068AD: //backward
      IR_row = 2;
      IR_column = 1;
      break;
    case 0x1B92DDAD: //forward
      IR_row = 2;
      IR_column = 2;
      break;
    case 0xAC2A56AD: //play-pause
      IR_row = 2;
      IR_column = 3;
      break;
    case 0x5484B6AD: //volume down
      IR_row = 3;
      IR_column = 1;
      break;
    case 0xD22353AD: //volume up
      IR_row = 3;
      IR_column = 2;
      break;
    case 0xDF3F4BAD:  //EQ
      IR_row = 3;
      IR_column = 3;
      break;
    case 0x68E456AD: //0
      IR_row = 4;
      IR_column = 1;
      break;
    case 0xF08A26AD: //100+
      IR_row = 4;
      IR_column = 2;
      break;
    case 0x151CD6AD: //200+
      IR_row = 4;
      IR_column = 3;
      break;
    case 0x18319BAD: //number 1
      IR_row = 5;
      IR_column = 1;
      break;
    case 0xF39EEBAD: //number 2
      IR_row = 5;
      IR_column = 2;
      break;
    case 0x4AABDFAD: //number 3
      IR_row = 5;
      IR_column = 3;
      break;
    case 0xE25410AD: //number 4
      IR_row = 6;
      IR_column = 1;
      break;
    case 0x297C76AD: //number 5
      IR_row = 6;
      IR_column = 2;
      break;
    case 0x14CE54AD: //number 6
      IR_row = 6;
      IR_column = 3;
      break;
    case 0xAF3F1BAD: //number 7
      IR_row = 7;
      IR_column = 1;
      break;
    case 0xC089F6AD: //number 8
      IR_row = 7;
      IR_column = 2;
      break;
    case 0x38379AD: //number 9
      IR_row = 7;
      IR_column = 3;
      break;
    default:
      mapped = false;
      break;
  }
  return mapped;
}
