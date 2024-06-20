
#define NUM_LEDS_PER_STRIP 256
#define NUMSTRIPS 1
#define NUM_LEDS (NUM_LEDS_PER_STRIP * NUMSTRIPS)
#include "FastLED.h"
// #include "I2SClocklessLedDriver.h"
#include "parser.h"

CRGB leds[NUMSTRIPS * NUM_LEDS_PER_STRIP];

int pins[NUMSTRIPS] = {23};
#define DATA_PIN 23
// I2SClocklessLedDriver driver;

static void clearleds()
{
    memset(leds, 0, NUM_LEDS * 3);
}
long time1;
static float _min = 9999;
static float _max = 0;
static uint32_t _nb_stat = 0;
static float _totfps;
static void show()
{
    // SKIPPED: check nargs (must be 3 because arg[0] is self)
    long time2 = ESP.getCycleCount();
    // driver.showPixels(WAIT);
    FastLED.show();
    float k = (float)(time2 - time1) / 240000000;
    float fps = 1 / k;
    _nb_stat++;
    if (_min > fps && fps > 10 && _nb_stat > 10)
        _min = fps;
    if (_max < fps && fps < 200 && _nb_stat > 10)
        _max = fps;
    if (_nb_stat > 10)
        _totfps += fps;
    Serial.printf("current fps:%.2f  average:%.2f min:%.2f max:%.2f\r\n", fps, _totfps / (_nb_stat - 10), _min, _max);
    time1 = time2;

    // SKIPPED: check that both v1 and v2 are int numbers
    // RETURN_VALUE(VALUE_FROM_INT(0), rindex);
}
static CRGB POSV(uint8_t h, uint8_t s, uint8_t v)
{
    return CHSV(h, s, v);
}
static void resetShowStats()
{
    float min = 999;
    float max = 0;
    _nb_stat = 0;
    _totfps = 0;
}
string script = "\
external void show();\
external CRGB hsv(int h,int s,int v); \
external CRGB leds[256]; \
external void clear(); \
define LED_WIDTH 16 \n\
define LED_HEIGHT 16 \n\
define panel_width 16 \n\
\
void main()\
{\
int h=1;\
clear();\
   while(h>0)\
   {\
      for(int i=0;i<LED_WIDTH;i++)\
  {\
    for(int j=0;j<LED_HEIGHT;j++)\
    {\
      leds[i+j*panel_width]= hsv(((i+j+h)),255,255);\
    }\
}\
show();\
h++;\
 \     
   }\
}\
";
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(460800);
    while (!Serial)
    {
    };
    // driver.initled((uint8_t*)leds,pins,NUMSTRIPS,NUM_LEDS_PER_STRIP,ORDER_GRB);
    // driver.setBrightness(30);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(30);
    addExternal("leds", externalType::value, (void *)leds);
    addExternal("show", externalType::function, (void *)&show);
    addExternal("hsv", externalType::function, (void *)POSV);
    addExternal("clear", externalType::function, (void *)clearleds);
    addExternal("resetStat", externalType::function, (void *)&resetShowStats);

    Parser p = Parser();
    if (p.parse_c(&script))
    {

        SCExecutable.execute("main");
    }
    Serial.printf("done");
}

void loop()
{
    // put your main code here, to run repeatedly:
}