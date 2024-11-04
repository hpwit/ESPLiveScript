//save_reg
//safe_mode
external void show();
external CRGB *leds;
external CRGB hsv(int h, int s, int v);
external void dp(float h);
external void display(int h);
external void resetStat();
external void clear();
external float sin(float h);
external float log(float h);
external float atan2(float x,float y );

define width 128
define height 96
define scale 1
define speed  5
define nSquares 4

define cosT 0.999998
define sinT 0.0001745
float t1;
float t2;
float timebase;
//var cosT = cos(0.1), sinT = sin(0.1);
uint32_t __baseTime[1];
__ASM__ void setTime()
{
   "entry a1,32"
   "rsr a14,234"
   "l32r a5,@___baseTime"
   "s32i a14,a5,0"
   "retw.n"
}
uint32_t __deltamillis[1];
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
 //  "l32r a4,@_stackr"
  // "s32i a13,a4,0"
   "retw.n"
}
__ASM__ uint32_t elapseMillis()
{
   "entry a1,32"
   "l32r a5,@___deltamillis"
   "l32i a14,a5,0"
   "rsr a13,234"
   "sub a13,a13,a14"
   "movi a14,240"
   "quou a13,a13,a14"
   "movi a14,1000"
   "quou a2,a13,a14"
 //  "l32r a4,@_stackr"
//   "s32i a13,a4,0"
   "retw.n"
}

float fmod(float a, float b)
{
  int tmp = (float)(a / b);
  return a - b * tmp;
}

float time(float par1) 
{
  float myVal = millis();
  myVal = myVal / 65535 / par1; 
  myVal = fmod(myVal, 1.0);     
  return myVal;
}


float signum(float a) {
a=a*1000;
if((int)(a)>=0)
{
   return 1.0;//abs(sin(time(0.3)));
}
else
{
   return -1.0;
}

}

//float fmod(float a, float b)
//{
//  int tmp = (float)(a / b);
//  return a - b * tmp;
//}
//
//float time(float par1) 
//{
//  float myVal = millis();
//  myVal = myVal / 65535 / par1; 
//  myVal = fmod(myVal, 1.0);     
//  return myVal;
//}


void beforeRender() {
uint32_t delta=elapseMillis();
  timebase = (timebase + delta/1000) % 3600;
  t2 = time(0.08);
  t1 = speed * timebase;
}

// squared spiral expression adapted from https://www.shadertoy.com/view/4tlfRB  
void render2D(int x,int y) {
float x0=x/width/scale -0.5;
float y0=y/height/scale -0.5;
  float x1 = signum(x0); 
  float y1 = signum(y0);
  float sx = x1 * cosT + y1 * sinT;
 float  sy = y1 * cosT - x1 * sinT;

 float  dx = abs(sin(nSquares*log(x0 * sx + y0 * sy) + atan2(y0,x0) - t1));
  
// leds[128 * y + x]=hsv((float)((t2 + x0*sx + y0*sy)*255), 255, (float)((dx * dx * dx)*255));
leds[128 * y + x]=hsv((t2 + x0*sx + y0*sy)*255, 255,(dx * dx * dx)*255);
}

void main()
{
setTime();
resetStat(); 
  int h = 1 ;
  while (h > 0)
  {
    beforeRender();
    for (int i = 0; i < width; i++)
    {
      for (int j = 0; j < height; j++)
      {
        render2D(i, j);
      }
    }
    sync();
  }
}