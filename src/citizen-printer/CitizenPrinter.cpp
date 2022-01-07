#include "CitizenPrinter.hpp"

bool initialized = false;

CitizenPrinter::CitizenPrinter(int rx_pin, int tx_pin, int dtr_pin, int fault_pin) {
  this->dtr_pin = dtr_pin;
  this->fault_pin = fault_pin;
  
  pinMode(dtr_pin, INPUT_PULLUP);

  // Yeah I know this is bad because pin 0 exists, but bear with me.
  if (fault_pin != NULL) pinMode(fault_pin, INPUT_PULLUP);

  this->port = new SoftwareSerial(rx_pin, tx_pin);
}

void CitizenPrinter::begin() {
  if (initialized) return;
  initialized = true;
  this->port->begin(9600);
}

void CitizenPrinter::print(const char chr) {
  while (digitalRead(this->dtr_pin) == HIGH) {}
  port->print(chr);
}

void CitizenPrinter::print(const char *str) {
  if (str == nullptr) {
    return;
  }
  
  for (int i = 0; i < strlen(str); i++) {
    while (digitalRead(this->dtr_pin) == HIGH) {}
    port->print(str[i]);
  }
}

void CitizenPrinter::println(const char *str = nullptr) {
  print(str);
  print("\n");
}

void CitizenPrinter::indent(int spaces, const char c) {
  if (spaces <= 0) return;
  for(int i = 0; i < spaces; i++) {
    print(c);
  }
}

void CitizenPrinter::hr() {
  indent(40, '-');
  println();
}

void CitizenPrinter::printCenter(const char *str) {
  indent((40 - strlen(str)) / 2);
  println(str);
}

void CitizenPrinter::printRight(const char *str) {
  indent(40 - strlen(str));
  println(str);
}

void CitizenPrinter::printTable(const char *strL, const char *strR) {
  int space = 40 - strlen(strL) - strlen(strR);
  print(strL);
  indent(space);
  println(strR);
}

void CitizenPrinter::formFeed() {
  print(0x0C);
}

void CitizenPrinter::online() {
  print(0x11);
}

void CitizenPrinter::offline() {
  print(0x13);
}

void CitizenPrinter::red() {
  print(0x12);
}

void CitizenPrinter::reverse() {
  print(0x14);
}

void CitizenPrinter::flush() {
  print(0x18);
}

void CitizenPrinter::graphicStart() {
  print(0x1b);
  print('C');
}

void CitizenPrinter::shortLines() {
  print(0x1b);
  print("1");
}

void CitizenPrinter::normalLines() {
  print(0x1b);
  print("2");
}
