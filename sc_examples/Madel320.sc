#define maxIterations 100
#define width  800
#define height 480
#define x_min -0.1
#define x_max 1.5
#define y_min -0.1
#define y_max 1

external CRGB leds[height,width]; 
void render2D(int x1, int y1)
{
   float x0 = x_min+(x_max-x_min)*x1/width;
   float y0 = y_min+(y_max-y_min)*y1/height;
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
      int g=(iter*255 / maxIterations);
      leds[y1*width+ x1] = hsv(g,255,255); //CRGB(g,g,g);
   }
   else
   {
      leds[y1*width+x1] = CRGB(0, 0, 0);
   }
}

void main()
{
sizex= width;
sizey= height;
      for (int i = 0; i < width; i++)
      {
         for (int j = 0; j < height; j++)
         {
            render2D(i, j);
         }
      }
   show();
   
}