#include <MD_MAX72xx.h>  // include MajicDesigns MAX72xx LED matrix library
#include <SPI.h>         // include Arduino SPI library

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW  // define dot matrix hardware type (FC-16)
#define MAX_DEVICES 4                      // define number of cascaded modules
#define CS_PIN 10                          // define Chip Select pin

MD_MAX72XX matrix = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup() {
  matrix.begin();                            // initialize the display
  matrix.control(MD_MAX72XX::INTENSITY, 3);  // set brightness level
  matrix.clear();                            // clear the display
}

void loop() {
  for (int i = 0; i < 16; i++) {
    matrix.clear();  // clear display before lighting up a new module
    setRow(i);
    setColumn(i);
    delay(50);
  }
  for (int i = 15; i > 0; i--) {
    matrix.clear();  // clear display before lighting up a new module
    setRow(i);
    setColumn(i);
    delay(50);
  }
}

void setRow(int row) {
  matrix.setColumn(row, 0xFF);       // light up all columns in the current column
  matrix.setColumn(31 - row, 0xFF);  // light up all columns in the current column
}

void setColumn(int column) {
  //matrix.setPoint(col, row, 0xFF);
  if (column > 7) {
    column = 15 - column;
    for (int i = 16; i < 32; i++) {
      matrix.setPoint(column, i, 0xFF);
    }
  } else {
    for (int i = 0; i < 16; i++) {
      matrix.setPoint(column, i, 0xFF);
    }
  }
}