external CRGB *leds;
external void show();
external void clear();
external CRGB hsv(int h, int s, int v);
external float sin(float angle);
external void display(int g);
external void resetStat();
external void dp(float h);
import rand
define nb_balls 100
define rmax 6
define rmin 4
define width 128
define height 96
define panel_width 128 
int slider1;
struct ball {
   float vx;
   float vy;
   float xc;
   float yc;
   float r;
   int color;
   void drawBall( )
{
   int startx = xc - r;
   float r2 =r * r;
   float r4=r^4;
   int starty = yc - r;
   int _xc=xc;
   int _yc=yc;
   for (int i =startx; i <=_xc; i++)
   {
      for (int j = starty; j <= _yc; j++)
      {
         int v;

          float distance = (i - xc)^2+(j-yc)^2;
  
         if (distance <= r2)
         {
            v = 255 * (1 - distance^2 / (r4));
            CRGB cc=hsv(color,255,v);
            leds[i + j * panel_width] = cc;
           // int h=(int)(2 * xc - i) + j * panel_width;
            leds[(int)(2 * xc - i) + j * panel_width] = cc;
          // h=(int)(2 * xc - i) + (int)(2 * yc - j) * panel_widthsdfsdf;
            leds[(int)(2 * xc - i) + (int)(2 * yc - j) * panel_width] = cc;
           // h=i + (int)(2 * yc - j) * panel_width;
            leds[i + (int)(2 * yc - j) * panel_width] = cc;
         }
      }
   }
 }
 void updateBall()
{

//drawBall();
//return;   
   xc = xc + vx;
   yc = yc + vy;
   if (xc >= width - r - 1)
   {
      xc =width - r - 1.1;
       vx = -vx;
   }
   if (xc < r + 1)
   {
      xc=r + 1.1;
      vx = -vx;

   }
   if (yc >= height - r - 1)
   {
      yc=height - r - 1.1;
      vy = -vy;
   }
   if (yc < r + 1)
   {
      yc = r + 1.1;
      vy = -vy;
    }

   drawBall();
}

}


void updateParams(int num)
{
num=slider1;
 if(num>nb_balls)
   {
      
      num=nb_balls;
   }
   if(num<=0)
   {
      num=1;
   }
}

ball Balls[nb_balls];
ball tmpball;

void init()
{
   for(int i=0;i<nb_balls;i++)
   {
      tmpball.vx = rand(300)/255+0.5;
      dp(tmpball.vx);
      tmpball.vy = rand(280)/255+0.3;
      tmpball.r = (rmax-rmin)*(rand(280)/180) +rmin;
      tmpball.xc = width/2*(rand(280)/255+0.3)+15;
      tmpball.yc = height/2*(rand(280)/255+0.3)+15;
      
      tmpball.color = rand(255);
      Balls[i]=tmpball;
   }  
}

void main(int num)
{

resetStat();



init();


    
   uint32_t h = 1;

  
   while (h > 0)
   {

     for(int i=0;i<width;i++)
     {
        for(int j=0;j<height;j++)
        {
           leds[i+panel_width*j]=hsv(i+h+j,255,180);
        }
     }
     updateParams(2);
      for (int i = 0; i < num; i++)
      {



Balls[i].updateBall();
      }

      sync();
      h++;
   }
}