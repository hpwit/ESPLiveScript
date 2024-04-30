__ext__ CRGB *leds;
__ext__ CRGB *ope;

__ext__ void clear();
__ext__ void show();
__ext__ void display(int h);
__ext__ void hsv(uint8_t h, uint8_t s, uint8_t v);
int copy[9216];
int line1[96];
int linec[96];
int size;
int res;
CRGB palette[18];
int kl[18];
void count_n(int i, int j)
{
   res = 0;

   res = linec[i - 1] + linec[i + 1];
   res = res + line1[i - 1] + line1[i + 1] + line1[i];
   int pos = (j + 1) * size + i;

   res = res + copy[pos - 1] + copy[pos + 1] + copy[pos];
}

void create_leds(int o)
{
hsv(o,255,255);

   for (int j = 1; j < size - 1; j++)
   {
      for (int h = 0; h < size; h++)
      {

         int tmp = linec[h];
         line1[h] = tmp;

         tmp = copy[(j)*size + h];
         linec[h] = tmp;
      }
      for (int i = 1; i < size - 1; i++)
      {
         count_n(i, j);

         int po = 0;
         int gh = size * j + i;
         int l = copy[gh];
         int gh2 = j * 128 + i;
if(res==3)
{
po=1;
}

         if (l == 1)
         {
            
            if (res == 2)
            {
               po = 1;
            }

         }

         if (po == 1)
         {
            leds[gh2] = ope[0];
            copy[gh] = 1;
         }
         else
         {
            leds[gh2] = CRGB(0, 0, 0);
            copy[gh] = 0;
         }
      }
   }
}

int main()
{
   size = 96;
   for (int h = 0; h < size * size; h++)
   {
      copy[h] = 0;
   }
   for (int h = 0; h < size; h++)
   {
      int pos = h * size + size / 2;
      copy[pos] = 1;
   }

   clear();
   show();
   for (int i = 0; i < 500; i++)
   {
      create_leds(i);
      show();
   }
}