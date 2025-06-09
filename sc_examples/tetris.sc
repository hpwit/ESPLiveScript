external CRGB leds[96, 128];
// external void printfln(char * s,Args args);
// external CRGB hsv(int h, int k, int v);
// external void clear();
// external void delay(uint32_t d);
define WIDTH 10 define HEIGHT 20

    import rand uint8_t board[HEIGHT][WIDTH];
uint8_t temp[4][4];
CRGB palette[28];

uint8_t currentShape[4][4];
int currentX = 0;
int currentY = 0;

uint8_t shapes[7, 4, 4] = {
    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2,
    0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 3, 3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 0, 0, 0, 5, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0,
    0, 6, 6, 0, 0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 7, 0, 0, 0, 0, 0};

void createPalette() {
  palette[0] = CRGB(0, 0, 0);
  palette[1] = CRGB(0, 0, 0);
  palette[2] = CRGB(0, 0, 0);
  for (int i = 1; i <= 7; i++) {
    int o = 255 * (i - 1) / 7;
    palette[i * 3] = hsv(o, 255, 255);
    palette[i * 3 + 1] = hsv(o, 255, 190);
    palette[i * 3 + 2] = hsv(o, 255, 128);
  }
  palette[24] = CRGB(255, 255, 255);
  palette[25] = CRGB(50, 50, 50);
  palette[26] = CRGB(255, 255, 255);
}

void displayTile(int x, int y, int pa) {
  if (x < -1)
    return;
  leds[83 - 4 * y, 4 * x + 4] = palette[3 * pa];
  leds[83 - 4 * y, 4 * x + 5] = palette[3 * pa];
  leds[83 - 4 * y, 4 * x + 6] = palette[3 * pa];
  leds[83 - 4 * y, 4 * x + 7] = palette[3 * pa];

  leds[83 - 4 * y - 1, 4 * x + 4] = palette[3 * pa];
  leds[83 - 4 * y - 1, 4 * x + 5] = palette[3 * pa + 1];
  leds[83 - 4 * y - 1, 4 * x + 6] = palette[3 * pa + 1];
  leds[83 - 4 * y - 1, 4 * x + 7] = palette[3 * pa + 2];

  leds[83 - 4 * y - 2, 4 * x + 4] = palette[3 * pa];
  leds[83 - 4 * y - 2, 4 * x + 5] = palette[3 * pa + 1];
  leds[83 - 4 * y - 2, 4 * x + 6] = palette[3 * pa + 1];
  leds[83 - 4 * y - 2, 4 * x + 7] = palette[3 * pa + 2];

  leds[83 - 4 * y - 3, 4 * x + 4] = palette[3 * pa];
  leds[83 - 4 * y - 3, 4 * x + 5] = palette[3 * pa + 2];
  leds[83 - 4 * y - 3, 4 * x + 6] = palette[3 * pa + 2];
  leds[83 - 4 * y - 3, 4 * x + 7] = palette[3 * pa + 2];
}
void init() {
  for (int i = 0; i < WIDTH * HEIGHT; i++)
    board[i] = 0;
  createPalette();
  // on dessine les bords
  for (int i = 0; i <= WIDTH + 1; i++) {
    displayTile(i - 1, HEIGHT, 8);
  }
  for (int i = 0; i < HEIGHT; i++) {
    displayTile(-1, i, 8);
    displayTile(WIDTH, i, 8);
  }
}

void mergeShapeToBoard() {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (currentShape[y][x]) {
        board[currentY + y][currentX + x] = currentShape[y][x];
      }
    }
  }
}

bool checkCollision(int dx, int dy) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (currentShape[y][x] > 0) {
        int newX = currentX + x + dx;
        // printfln("x:%d",newX);
        int newY = currentY + y + dy;
        // if (newX < 0 || newX>=WIDTH)
        //   return true;
        // if (newY >= HEIGHT)
        //  return true;
        if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || board[newY, newX]) {
          return true;
        }
      }
    }
  }
  return false;
}

void rotateShape() {

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      temp[y][x] = currentShape[3 - x][y];
    }
  }
  for (int i = 0; i < 16; i++)
    currentShape[i] = temp[i];
}

bool update;
void left() {
  if (update)
    return;
  if (!checkCollision(-1, 0)) {
    currentX--;
    update = true;
  }
}
void right() {
  if (update)
    return;
  if (!checkCollision(1, 0)) {
    currentX++;
    update = true;
  }
}
void rotate() {
  if (update)
    return;
  rotateShape();
  if (checkCollision(0, 0)) {
    rotateShape();
    //   update = true;
  }

  else {
    update = true;
  }
}
void spawnShape() {
  int shapeIndex = rand(7);
  for (int i = 0; i < 16; i++)
    currentShape[i] = shapes[shapeIndex * 16 + i];
  currentX = WIDTH / 2 - 2;
  currentY = 0;
}

// void putTetro(int x, int y, int tetro)
//{
//    for (int i = 0; i < 4; i++)
//       for (int j = 0; j < 4; j++)
//          board[y + j, x + i] = shapes[tetro, j, i];
// }
void drawBoard() {
  for (int i = 0; i < WIDTH; i++)
    for (int j = 0; j < HEIGHT; j++)
      displayTile(i, j, board[j, i]);

  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {

      if (currentShape[j, i])
        displayTile(i + currentX, j + currentY, currentShape[j, i]);
    }
}
int count = 0;
void clearFullLines() {
  for (int y = 0; y < HEIGHT; y++) {
    bool full = true;
    for (int x = 0; x < WIDTH; x++) {
      if (board[y][x] == 0) {
        full = false;
        break;
      }
    }
    if (full) {
      for (int ty = y; ty > 0; ty--) {
        for (int i = 0; i < WIDTH; i++) {
          board[ty, i] = board[ty - 1, i];
        }
        // memcpy(board[ty], board[ty - 1], sizeof(board[ty]));
      }
      for (int i = 0; i < WIDTH; i++) {
        board[0, i] = 0;
      }
    }
  }
}

int main() {
  pinInterrupt(_execaddr_, "left", 23);
  pinInterrupt(_execaddr_, "right", 2);
  pinInterrupt(_execaddr_, "rotate", 15);
  clear();
  show();
  init();
  spawnShape();
  while (true) {
    if (update) {

      drawBoard();
      show();
      update = false;
    }
    if (count == 0) {

      if (!checkCollision(0, 1)) {
        //     printfln("currnt:%d", currentY);
        currentY++;
      } else {
        mergeShapeToBoard();
        spawnShape();
        clearFullLines();
        if (checkCollision(0, 0)) {
          //               printfln("lost");
          break;
        }
      }
      update = true;
    }
    count = (count + 1) % 300;
    delay(1);
  }
}