#include "IRLremote.h"

#define pinIR 2
CHashIR IRLremote;

int row;
int column;

void setup() {
  Serial.begin(9600);
  Serial.println("Start IR tester (and code generator)");
  if (!IRLremote.begin(pinIR))
    Serial.println("IR pin error");
  row = 0;
  column = 0;
  Serial.println("\t\t\t\tswitch(data.command){");
}

void loop() {
  if (IRLremote.available()) {
    auto data = IRLremote.read();
    if (data.command != 0x117697CD) {
      bool mapped = true;
      switch(data.command){       
        case 0x4E5BA3AD: //ch+
          row = 1;
          column = 1;
          break;
        case 0xE51CA6AD: //ch
          row = 1;
          column = 2;
          break;
        case 0xE207E1AD: //ch-
          row = 1;
          column = 3;
          break;
        case 0x517068AD: //backward
          row = 2;
          column = 1;
          break;
        case 0x1B92DDAD: //forward
          row = 2;
          column = 2;
          break;
        case 0xAC2A56AD: //play-pause
          row = 2;
          column = 3;
          break;
        case 0x5484B6AD: //volume down
          row = 3;
          column = 1;
          break;
        case 0xD22353AD: //volume up
          row = 3;
          column = 2;
          break;
        case 0xDF3F4BAD:  //EQ
          row = 3;
          column = 3; 
          break;
        case 0x68E456AD: //0
          row = 4;
          column = 1;
          break;
        case 0xF08A26AD: //100+
          row = 4;
          column = 2;
          break;
        case 0x151CD6AD: //200+
          row = 4;
          column = 3;
          break;
        case 0x18319BAD: //number 1
          row = 5;
          column = 1;
          break;
        case 0xF39EEBAD: //number 2
          row = 5;
          column = 2;
          break;
        case 0x4AABDFAD: //number 3
          row = 5;
          column = 3;
          break;
        case 0xE25410AD: //number 4
          row = 6;
          column = 1;
          break;
        case 0x297C76AD: //number 5
          row = 6;
          column = 2;
          break;
        case 0x14CE54AD: //number 6
          row = 6;
          column = 3;
          break;
        case 0xAF3F1BAD: //number 7
          row = 7;
          column = 1;
          break;
        case 0xC089F6AD: //number 8
          row = 7;
          column = 2;
          break;
        case 0x38379AD: //number 9
          row = 7;
          column = 3;
          break;
        default:
          mapped = false;
          column = column == 3 ? 1 : column + 1 ;
          if(column == 1) row++;
          
          Serial.print("\t\t\t\tcase 0x");
          Serial.print(data.command, HEX);
          Serial.println(":");
          Serial.print("\t\t\t\t\trow = ");
          Serial.print(row);
          Serial.println(";");
          Serial.print("\t\t\t\t\tcolumn = ");
          Serial.print(column);
          Serial.println(";");
          Serial.println("\t\t\t\t\tbreak;");
        break;
      }
      if(mapped)
      {
          Serial.print("row");
          Serial.print(row);
          Serial.print(", column:");
          Serial.println(column);
      }
    }
  }
}
