# Introduction
For a very long time now I have been looking at being able to execute programs on the esp32 without having to load the code via any IDE(Arduino, VSCode ...).
I rapidly forgot that topic and focus my energy on writing leds driver and artnet libraries for the esp32. Once that done, I start thinking again about being able to write animations 'live'. Of course in my searches for existing previous implementations, I fell upon PixelBlaze and later Arti-Fx (part of the WLED MoonModules).
In both cases the scripting lanquage is a subset of JavaScript and to my knowkledge interpreted.
A number of impressive animations have already been written for both these environments and PixelBlaze has a full web based accessible examples library to which one can contribute.
WLED Arti-Fx and PixelBlaze both have a web interface to program the animations.
PixelBlaze is a closed/licensed product so no way to 'look' into it and I haven't installed Arti-Fx I have to admit :). Hence I do not have any real clue about the performance of these two implementations.
 
Several interpreted scripting lanquages are available (Lua, JavaScript, Gravity, ...) for the esp32, I have tried Lua and Gravity on my panel (48 16x16 ws2812 panels or 12288 leds) the refresh rate  of the panel is 128fps thanks to my VirtualLedDriver.
The results were quite a let down in term of performance. For a simple rainbow animation the global frame rate comparison is:
* 85 fps: for the C++ program uplaod via arduino
* 10 fps: for Gravity script
*  3 fps: for Lua script
 
This is indeed due do the large number of pixels needed to be calculated for my panel. Hence none of these language will make the cut for me.
 
**So I have decided also give it a go. Can I also conceive a 'language' to program led animations.**
 
# Which Language ?
Being old school, I know that assembly language is the way to get the most of performance out of a CPU (given that your skills at writing ML are quite good of course). So I've tried to duplicate the same program directly assembly (still using arduino) and it worked great.
 
So I wrote an assembly language compiler as well as a small terminal interface to edit/save/compile/execute these programs (It will later become [ledOS](https://www.github.com/hpwit/LedOS) )

Of course proposing an assembly parer is not a really something usable :) so I took my chance at writing a compiler which will produce assembly languages for the esp32 xtensa CPU. 

## C like language
I have chosen to go for a C like syntax which is closed to JavaScript with stronger typing. I have made a loose adaptation of the language. But you can write thing like this:
 
 ```C
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
    show();
  }
}
 ```

## DYI parser and compiler
I am not a compiler specialist and I wrote 'by hand' the parser and compiler using no existing grammar.

Later in this documentation you will see what is possible and not.

## Led manipulation oriented
Even if the language is made to be of general application, the target of this language is targeted at creating led animations.
As a consequence the scripting language has some limitations.

# Let's code

If you execute the following program:

 ```C
#include "ESPLiveScript.h"

string script="\
external void display(int h);\n\
void main()\n\
{\n\
for(int i=0;i<20;i++)\n\
    {\n\
        display(i);\n\
    }\n\
}";
void displayfunction(int h)
{
  Serial.printf("coming from ESPLiveScript :%d\n",h);
}
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
addExternal("display", externalType::function, (void *)&displayfunction);
Parser p;
Executable exec=p.parseScript(&script);
if(exec.isExeExists())
{
  exec.execute("main");
}

}

void loop() {
  // put your main code here, to run repeatedly:

}
 ```
the output will be:
 ```
***********PARSING DONE*********
***********COMPILING DONE*********
max used memory: 6548 maxstack:2112  started 266760 free mem:260536 consumed 6224 exe size:0 time:27ms
max used memory: 6548 maxstack:2112  started 266760 free mem:264292 consumed 2468 exe size:0 time:39ms
***********AFTER CLEAN*********
***********CREATE EXECUTABLE*********
Creation of an 184 bytes binary and 56 bytes data

Parsing 65 assembly lines ...

max used memory: 6548 maxstack:2112  started 266760 free mem:266128 consumed 632 exe size:0 time:94ms
coming from ESPLiveScript :0
coming from ESPLiveScript :1
coming from ESPLiveScript :2
coming from ESPLiveScript :3
coming from ESPLiveScript :4
coming from ESPLiveScript :5
coming from ESPLiveScript :6
coming from ESPLiveScript :7
coming from ESPLiveScript :8
coming from ESPLiveScript :9
coming from ESPLiveScript :10
coming from ESPLiveScript :11
coming from ESPLiveScript :12
coming from ESPLiveScript :13
coming from ESPLiveScript :14
coming from ESPLiveScript :15
coming from ESPLiveScript :16
coming from ESPLiveScript :17
coming from ESPLiveScript :18
coming from ESPLiveScript :19
 ```