#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 10

MD_MAX72XX matrix = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const int maxX = 16;
const int maxY = 16;
int snakeX[256] = {8};
int snakeY[256] = {8};
int snakeLength = 2;
int foodX = 5;
int foodY = 5;
int direction = 1;
#define VRX A1
#define VRY A0

void setup() {
  matrix.begin();
  matrix.control(MD_MAX72XX::INTENSITY, 3);
  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);
  for (int i = 0; i < 16; i++) {
    matrix.clear();  // clear display before lighting up a new module
    setRow(i);
    setColumn(i);
    delay(50);
  }
  for (int i = 15; i >= 0; i--) {
    matrix.clear();  // clear display before lighting up a new module
    setRow(i);
    setColumn(i);
    delay(50);
  }  
  
  delay(500);
  matrix.clear(); 

    for(int i = 0; i < 16; i++) {
    for(int j = 0; j < 16; j++) {
      setPoint(i, j);
      delay(5);
    }
  }
    delay(500);
  matrix.clear(); 

  randomSeed(analogRead(0));
  placeFood();

  
}

void loop() {
  readJoystick();
  moveSnake();
  checkCollision();
  drawGame();
  delay(200);
}

void readJoystick() {
  int xValue = analogRead(VRX);
  int yValue = analogRead(VRY);

  if (xValue < 400 && direction != 1) {
    direction = 1; // Left
  } else if (xValue > 600 && direction != 3) {
    direction = 3; // Right
  } else if (yValue < 400 && direction != 2) {
    direction = 0; // Up
  } else if (yValue > 600 && direction != 0) {
    direction = 2; // Down
  }
}

void moveSnake() {
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  if (direction == 0) snakeY[0]--;
  else if (direction == 1) snakeX[0]++;
  else if (direction == 2) snakeY[0]++;
  else if (direction == 3) snakeX[0]--;

  if (snakeX[0] < 0) snakeX[0] = maxX - 1;
  if (snakeX[0] >= maxX) snakeX[0] = 0;
  if (snakeY[0] < 0) snakeY[0] = maxY - 1;
  if (snakeY[0] >= maxY) snakeY[0] = 0;
}

void checkCollision() {
  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      gameOver();
    }
  }
  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    if (snakeLength < 256) snakeLength++;
    placeFood();
  }
}

void placeFood() {
  bool valid;
  do {
    valid = true;
    foodX = random(0, maxX);
    foodY = random(0, maxY);
    for (int i = 0; i < snakeLength; i++) {
      if (snakeX[i] == foodX && snakeY[i] == foodY) {
        valid = false;
        break;
      }
    }
  } while (!valid);
}

void drawGame() {
  matrix.clear();
  for (int i = 0; i < snakeLength; i++) {
    setPoint(snakeX[i], snakeY[i]);
  }
  setPoint(foodX, foodY);
  // matrix.clear();
}

void gameOver() {
  matrix.clear();
  delay(1000);
  snakeLength = 2;
  snakeX[0] = 8;
  snakeY[0] = 8;
  direction = 1;
  placeFood();
}

void setPoint(int col, int row) {

  if(col > 7) {
    col = 15 - col;
    row = 31 - row;
  }
  matrix.setPoint(col, row, 0xFF); 

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
