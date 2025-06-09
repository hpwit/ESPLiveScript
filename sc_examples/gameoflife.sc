

#define width 128
#define height 128
external CRGB leds[height, width];
uint8_t copy[height, width];
uint8_t line1[width];
uint8_t linec[width];

void create_leds(int o) {
  CRGB color = hsv(o, 255, 255);
  for (int j = 1; j < height - 1; j++) {
    for (int h = 0; h < width; h++) {
      line1[h] = linec[h];
      linec[h] = copy[j, h];
    }
    for (int i = 1; i < width - 1; i++) {

      int res = linec[i - 1] + linec[i + 1];
      res = res + line1[i - 1] + line1[i + 1] + line1[i];
      res = res + copy[j + 1, i - 1] + copy[j + 1, i + 1] + copy[j + 1, i];
      bool alive = false;
      int l = copy[j, i];

      if (res == 3)
        alive = true;
      else
        alive = (l == 1 && res == 2) ? true : false;
      if (alive) {
        leds[j, i] = color;
        copy[j, i] = 1;
      } else {
        leds[j, i] = CRGB(0, 0, 0);
        copy[j, i] = 0;
      }
    }
  }
}

int main() {
  for (int h = 0; h < width * height; h++) {
    copy[h] = 0;
  }
  for (int h = 0; h < height; h++) {
    copy[h, width / 2] = 1;
  }
  clear();
  show();
  for (int i = 0; i < 50; i++) {
    create_leds(i);
    show();
  }
}