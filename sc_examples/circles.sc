//external CRGB *leds;
//external CRGB *ope;

//external void clear();
//external void show();
//external CRGB hsv(uint8_t h, uint8_t s, uint8_t v);

void PixelOn(int x,int y,int color)
{
if(x<0)
{
   return;
}
if(y<0)
{
   return;
}
leds[(y%96)*128+(x%128)]=hsv(color,255,255);
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
    
   
    PixelOn(xc+x,yc-y,xc);
    PixelOn(xc-x,yc-y,xc);
    PixelOn(xc+x,yc+y,xc);
    PixelOn(xc-x,yc+y,xc);
    PixelOn(xc+y,yc-x,xc);
    PixelOn(xc-y,yc-x,xc);
    PixelOn(xc+y,yc+x,xc);
    PixelOn(xc-y,yc+x,xc);
    
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
        
        PixelOn(xc+x,yc-y,xc);
        PixelOn(xc-x,yc-y,xc);
        PixelOn(xc+x,yc+y,xc);
        PixelOn(xc-x,yc+y,xc);
        PixelOn(xc+y,yc-x,xc);
        PixelOn(xc-y,yc-x,xc);
        PixelOn(xc+y,yc+x,xc); 
        PixelOn(xc-y,yc+x,xc);
    }
    
}


void main()
{

int i=0;   
while(true)
{
clear();
printfln("i:%d",i);
BresenhamCircle(i,48,40);
show();
i++;
}
   
}