//safe_mode
//external CRGB   leds[12288];
//external void clear();
//external void show();
//external uint8_t * font;
//external void display(int h);
define led_width 128
//external CRGB hsv(int h,int s,int v);
char str[]="HERE IS A SCROLLING ....";

void displayChar(int posx,int posy, int charpos)
{
//display(999);
//display(posx);
//display(888);
//display(charpos);
//int posy=20;
 //    int h=0;
  int   h=str[charpos]-32;
//display(h);
     h=h*8;
     if(posx<0)
     {
        return;
     }
     if(posx>led_width)
     {
     return;
     }
     for(int y=0;y<8;y++)
        {
        uint8_t d=font[h+y];
//        d=255;
//     display(d);
           for (int  x=0;x<8;x++)
           {
           if(posx+x<led_width)
           {
              
//               display(d and (1<<(8-x)));
           if( ( d  and (1<<(8-x) ) ) >  0 )
              {
             int _h=posx+x+led_width*(posy+8-y);
//             display(_h);
                 leds[_h]=CRGB(255,255,0);
              }
           }
           }
        }
}

void main()
{
printfln("dd %d",-1);
   clear();
//   for( int i=0;i<128;i++)
//   {
//      for(int j=0;j<96;j++)
//      {
//         leds[i+j*128]=hsv(i+j+k,255,255);
//      }
//   }
   int k=0;
   while(k>=0)
   {
   // k++;
   for( int i=0;i<128;i++)
   {
      for(int j=0;j<96;j++)
      {
         leds[i+j*128]=hsv(i+j+k,255,100);
      }
   }
    
//    clear();  
   int h=0;
   while(str[h]>0)
   {
   int gg=led_width+8*h-k%250;
         displayChar(gg,20,h);
         h++;
    }
    k++;
    k=k%300;
    show();
    }   

}