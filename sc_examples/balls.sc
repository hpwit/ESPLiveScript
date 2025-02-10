import rand 
#define max_nb_balls 200 
#define rmax 8 
#define rmin 8 
#define width 128 
#define height 96 
#define panel_width 128 
#define panel_height 96 


struct ball
{
   float vx, vy, xc, yc, r;
   int color;
   void drawBall()
   {
      int startx = xc - r;
      int r2 = r * r;
      float r4 = r ^ 4;
      int starty = yc - r;
      int _xc = xc;
      int _yc = yc;
      for (int i = startx; i <= _xc; i++)
         for (int j = starty; j <= _yc; j++)
         {
            int v;
            int distance = (i - xc) ^ 2 + (j - yc) ^ 2;
            if (distance <= r2)
            {
               v = 255 * (1 - distance ^ 2 / (r4));
               CRGB cc = hsv(color, 255, v);
               leds[j*panel_width +i] = cc;
               leds[j*panel_width+(int)(2 * xc - i)] = cc;
               leds[(int)(2 * xc - i) + (int)(2 * yc - j) * panel_width] = cc;
               leds[i + (int)(2 * yc - j) * panel_width] = cc;
            }
         }
   }
   void updateBall()
   {
      xc += vx;
      yc += vy;
      if (xc >= (width - r - 1))
      {
         xc = width - r - 1;
         vx = -vx;
      }
      if (xc < (r + 1))
      {
         xc = r + 1;
         vx = -vx;
      }
      if (yc >= height - r - 1)
      {
         yc = height - r - 1;
         vy = -vy;
      }
      if (yc < r + 1)
      {
         yc = r + 1;
         vy = -vy;
      }

      drawBall();
   }
   ball()
   {
      vx = rand(300) / 255 + 0.7;
      vy = rand(280) / 255 + 0.5;
      r = (rmax - rmin) * (rand(280) / 180) + rmin;
      xc = width / 2 * (rand(280) / 255 + 0.3) + 15;
      yc = height / 2 * (rand(280) / 255 + 0.3) + 15;
      color = rand(255);
   }
}

ball Balls[max_nb_balls];

void main(int num)
{
   if (num > max_nb_balls)
   {
      num = max_nb_balls;
   }
   if (num <= 0)
   {
      num = 1;
   }
   printfln("numberof balls:%d",num);
   resetStat();
   uint32_t h;
   while (true)
   {
      for (int i = 0; i < width; i++)
         for (int j = 0; j < height; j++)
            leds[j*panel_width+ i] = hsv(i + h + j, 255, 180);

      for (int i = 0; i < num; i++)
         Balls[i].updateBall();
      h++;
      sync();
   }
}