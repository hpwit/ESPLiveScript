
#define USE_FASTLED
#include "FastLED.h"
#include "ESPLiveScript.h"
#define NUM_LEDS 256
#define DATA_PIN 15
CRGB leds[NUM_LEDS];
void show() {
  FastLED.show();
}
string script = R"EOF(
void main()
{
   int k=0;
   while(true)
   {
    for (int i=0;i<128;i++)
      for(int j=0;j<96;j++)
          leds[j*16+i]=hsv(i+j+k,255,255);
    k++;
    show();
   }
}
)EOF";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(30);
  addExternalVariable("leds", "CRGB *", "", (void *)leds);
  addExternalFunction("show", "void", "", (void *)show);
  Parser p;
  Executable exec = p.parseScript(&script);
  if (exec.isExeExists()) {
    exec.execute("main");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
