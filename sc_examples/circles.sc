__ext__ CRGB *leds;
__ext__ CRGB *ope;

__ext__ void clear();
__ext__ void show();
__ext__ void hsv(uint8_t h, uint8_t s, uint8_t v);

void PixelOn(int x,int y)
{
if(x<0)
{
   return;
}
if(x>=128)
{
   return;
}
if(y<0)
{
   return;
}
if(y>=96)
{
   return;
}
leds[y*128+x]=ope[0];
}
void  BresenhamCircle(int xc,int yc,int r)
{
   
    int x;
    int y;
    int p;
    if (r<0)
    {
        return;
    }
    x=0;
    y=r;
    
   
    PixelOn(xc+x,yc-y);
    PixelOn(xc-x,yc-y);
    PixelOn(xc+x,yc+y);
    PixelOn(xc-x,yc+y);
    PixelOn(xc+y,yc-x);
    PixelOn(xc-y,yc-x);
    PixelOn(xc+y,yc+x);
    PixelOn(xc-y,yc+x);
    
    p=3-(2*r);
    
    for(x=0;x<=y;x++)
    {
        if (p<0)
        {
            p=(p+(4*x)+6);
        }
        else
        {
            y=y-1;
            p=p+((4*(x-y)+10));
        }
        
        PixelOn(xc+x,yc-y);
        PixelOn(xc-x,yc-y);
        PixelOn(xc+x,yc+y);
        PixelOn(xc-x,yc+y);
        PixelOn(xc+y,yc-x);
        PixelOn(xc-y,yc-x);
        PixelOn(xc+y,yc+x); 
        PixelOn(xc-y,yc+x);
    }
    
}


void main()
{

   
for(int i=0;i<200;i++)
{
clear();
hsv(i,255,255);
BresenhamCircle(i,48,40);
show();
}
   
}