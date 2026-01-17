define maxIterations 16
#define width 800
#define height 480 
#define x_min -2.5
#define x_max 1.5
#define y_min -1.2
#define y_max 1.2
external CRGB leds[height,width]; 
#define s 32
int offset;
void render2D(int x1, int y1)
{
   float x0 = x_min+(x_max-x_min)*x1/width;  //(x1*2.7)/ width-2.1 ;
   float y0 = y_min+(y_max-y_min)*y1/height; //(y1*2.2)/height - 1.1;
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
      int g=(iter*s) & 0xff;   
 //int g=34;
      leds[y1*width+ x1] = CRGB(g,(2*g),(3*g));
   }
   else
   {
      leds[y1*width+x1] = CRGB(0, 0, 0);
   }
}

void main()
{
offset=0;
sizex= width;
sizey= height;
  // while (true)
  // {
      for (int i = 0; i < width; i++)
      {
         for (int j = 0; j < height; j++)
         {
  //       printfln("%d %d",i,j);
            render2D(i, j);
         }
      }
   show();
   offset++;
  // }
}