// получение команд из IR пульта
#include <Wire.h>

bool modeBIL;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); 
  
  uart.begin(9600);
  uart.println("start1");
  
  Wire.begin(42);
  Wire.onReceive(receiveEvent); // зарегистрировать обработчик события
}

//   функция, которая будет выполняться всякий раз, когда от мастера принимаются данные
//   данная функция регистрируется как обработчик события, смотрите setup()
void receiveEvent(int howMany) 
{
  modeBIL = !modeBIL;
  while (Wire.available()) // пройтись по всем до последнего
  { 
    byte c = Wire.read();    // принять байт как символ
    uart.println(c, HEX);         // напечатать символ
  }
}

void loop() {
  delay(500);
  digitalWrite(LED_BUILTIN, modeBIL ? LOW : HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, modeBIL ? HIGH : LOW);
}
