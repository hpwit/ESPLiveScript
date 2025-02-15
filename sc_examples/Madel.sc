#define maxIterations 50 
#define width 128 
#define height 96 

external CRGB leds[height,width]; 
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
       x2 = x^2;
       y2 = y^2;
       iter++;
    
   }
   if (iter < maxIterations)
   {
      leds[y1*width+ x1] = hsv((iter*255 / maxIterations) , 255, 255);
   }
   else
   {
      leds[y1*width+x1] = CRGB(0, 0, 0);
   }
}

void main()
{
   while (true)
   {
      for (int i = 0; i < width; i++)
      {
         for (int j = 0; j < height; j++)
         {
            render2D(i, j);
         }
      }
   show();
   }
}