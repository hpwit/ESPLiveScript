#define width  384
#define height 168

uint8_t copy[height,width];
uint8_t line1[width];
uint8_t linec[width];

void create_leds(int o)
{
  
   for (int j = 1; j < height - 1; j++)
   {
      for (int h = 0; h < width; h++)
      {
         line1[h] = linec[h];
      linec[h]=copy[j,h];
      }
      for (int i = 1; i < width - 1; i++)
      {
   
   int res = linec[i - 1] + linec[i + 1];
   res = res + line1[i - 1] + line1[i + 1] + line1[i];
   res=res+copy[j+1,i-1]+copy[j+1,i+1]+copy[j+1,i];
   bool alive = false;
   int l = copy[j,i];

if(res==3)
   alive=true;
else
   alive=(l==1 && res==2)?true:false;
         if (alive)
         {
            setPixel(i,j,1);
            copy[j,i] = 1;
         }
         else
         {
             setPixel(i,j,0);
            copy[j,i] = 0;
         }
      }
   }

}


int main()
{
   for (int h = 0; h < width * height; h++)
 {
   copy[h]=0;
 }
   for (int h = 2; h < height-2; h++)
   {
      copy[h,width/2]=1;

   }
   clear();
   show();
   for (int i = 0; i <100; i++)
   {
      create_leds(i);
      show();
   }
}