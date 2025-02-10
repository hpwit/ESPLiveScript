// Original Code from Zranger1 adapted
// https://github.com/zranger1/PixelblazePatterns/blob/master/2D_and_3D/mandelbrot2D.js
//  Reference:
//  https://mathworld.wolfram.com/MandelbrotSet.html
//  Version  Author        Date
//   1.0.0    JEM(ZRanger1) 12/08/2020

//save_reg


#define maxIterations 15 
#define width 128 
#define height 96 

external CRGB leds[height,width];
#define scale 1
uint32_t __deltamillis[1];

uint32_t __baseTime[1];

__ASM__ uint32_t millis()
{
   "entry a1,32"
   "l32r a5,@___baseTime"
   "l32i a14,a5,0"
   "rsr a13,234"
   "l32r a4,@___deltamillis"
   "s32i a13,a4,0"
   "sub a13,a13,a14"
   "movi a14,240"
   "quou a13,a13,a14"
   "movi a14,1000"
   "quou a2,a13,a14"
   // "l32r a4,@_stackr"
   //"s32i a13,a4,0"
   "retw.n"
}

float cR = -0.94299;
float cI = 0.3162;

float cX, cY;//, fX, fY;

// timers used to animate movement and color
float t1, t2;

float fmod(float a, float b)
{
   int tmp = a / b;
   return a - b * tmp;
}

float triangle(float h)
{
   float res = fabs(fmod(2 * h, 2.0) - 1.0);
   return 1 - res;
}

float time(float par1)
{
   float myVal = millis();
   myVal = myVal / 10000 / par1;
   myVal = fmod(myVal, 1.0);
   return myVal;
}

void beforeRender()
{
   t1 = (triangle(time(0.2)) - 0.5) * 2.4;
   t2 = time(0.05);
   cX = cR + t1;
   cY = cI + (t1 / 2.5);
}

void render2D(int x1, int y1)
{
   float x0 = (x1*2.7)/ width-2.1 ;
   float y0 = (y1*2.2)/height - 1.1;
   float x = 0 ;
    float y = 0;
    float x2=0;
    float y2=0;
   int iter=0;
   while (iter < maxIterations  && x2+y2<=4)
   {

     float fX = x2 - y2 + x0;
      y = 2 * x * y + y0;
      x = fX;
       x2 = x *x ;
       y2 = y *y;
       iter++;
    
   }

   if (iter < maxIterations)
   {
      leds[y1, x1] = hsv(t2+(iter / maxIterations) * 255, 255, 255);
   }
   else
   {
      leds[y1,x1] = CRGB(0, 0, 0);
   }
}

void main()
{
  // clear();
   //resetStat();

   int h = 1;
   while (true)
   {
      beforeRender();
      for (int i = 0; i < width; i++)
      {
         for (int j = 0; j < height; j++)
         {
            render2D(i, j);
         }
      }
   show();
 //     sync();
   }
}