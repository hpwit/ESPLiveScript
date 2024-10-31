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
I am not a compiler specialist and I wrote 'by hand' the parser and compiler using no existing grammar. Please do not throw me into the fire for this. The entire exercice has been a learning experience also and the result is not half cooked (according to me).

Later in this documentation you will see what is possible and not.

## Not a development environment
This libray doesn't provide an environment to write scripts. Nevertheless it has been included in:
- [LedOS](https://github.com/hpwit/ledOS) : an environment in a console terminal to edit, save and execute programs

<img title="Screenshot" alt="Alt text" src="pictures/screen1.png" style="width:400px">

- [Moon Modules](https://github.com/MoonModules/StarLight) : A web environment for led manipulation based on WLED which contains several modules (artnet, DMX, powerful mapping tool ....) [Live Scripts doc](https://ewowi.github.io/StarDocs/UserMod/UserModLiveScripts)


## Led manipulation oriented
Even if the language is made to be of general application, the target of this language is targeted at creating led animations.
As a consequence the scripting language has some limitations that 

# Let's code

 ## First light :)

- To create a new instance of a parser : ``Parser _parser;`` 
- To parse a script and create an executable : ``Executable exec=_parser.parseScript(&script);``
- To execute the code assuming there is function called main in you code: ``exec.execute("main");``


If you execute the following program [firstlight.ino](examples/firstlight/firstlight.ino):

 ```C
#include "ESPLiveScript.h"

string script="\
void main()\n\
{\n\
for(int i=0;i<20;i++)\n\
    {\n\
        printfln(\"i:%2d 3xi:%2d\",i,3*i);\n\
    }\n\
}";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Parser _parser;
  Executable exec=_parser.parseScript(&script);
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
max used memory: 7064 maxstack:2156  started 265960 free mem:258796 consumed 7164 time:30ms
max used memory: 7064 maxstack:2156  started 265960 free mem:262940 consumed 3020 time:41ms
***********AFTER CLEAN*********
***********CREATE EXECUTABLE*********
Creation of an 208 bytes binary and 70 bytes data
Parsing 75 assembly lines ...
max used memory: 7064 maxstack:2156  started 265960 free mem:265000 consumed 960 time:104ms
i: 0 3*i: 0
i: 1 3*i: 3
i: 2 3*i: 6
i: 3 3*i: 9
i: 4 3*i:12
i: 5 3*i:15
i: 6 3*i:18
i: 7 3*i:21
i: 8 3*i:24
i: 9 3*i:27
i:10 3*i:30
i:11 3*i:33
i:12 3*i:36
i:13 3*i:39
i:14 3*i:42
i:15 3*i:45
i:16 3*i:48
i:17 3*i:51
i:18 3*i:54
i:19 3*i:57
```

**NB: if you have several functions it the same script you can call any of the function**


## The function you call can have input parameters

To add parameter to the exection call
```C
Arguments args;
args.add(1);
args.add(2.3);
...
exec.execute("main", args);
```
**NB: For the moment you can only have interger and float entry parameters**

In this example we do calculate the factorial:
```C
#include "ESPLiveScript.h"

string script="\
int fact(int h)\n\
{\n\
   if(h==1)\n\
   {  \n\
     return 1;\n\
   }  \n\
  return h*fact(h-1);\n\
}\n\
\n\
void main(int g)\n\
{  \n\
   printfln(\"factorial of %d is %d\",g,fact(g));\n\
}";

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

Parser p;
Executable exec=p.parseScript(&script);
if(exec.isExeExists())
{
  Arguments args;
  args.add(5);
  exec.execute("main",args);
  args.clear();
   args.add(6);
  exec.execute("main",args);
   args.clear();
   args.add(7);
  exec.execute("main",args);
}

}

void loop() {
  // put your main code here, to run repeatedly:

}
```


result:
```
***********PARSING DONE*********
***********COMPILING DONE*********
max used memory: 8468 maxstack:2304  started 265896 free mem:257328 consumed 8568 time:33ms
max used memory: 8468 maxstack:2304  started 265896 free mem:262196 consumed 3700 time:44ms
***********AFTER CLEAN*********
***********CREATE EXECUTABLE*********
Creation of an 272 bytes binary and 90 bytes data
Parsing 98 assembly lines ...
max used memory: 8468 maxstack:2304  started 265896 free mem:264808 consumed 1088 time:121ms
factorial of 5 is 120
factorial of 6 is 720
factorial of 7 is 5040
 ```

## Interaction with pre compiled functions

### Calling/accessing 'pre compiled' functions/variables from ESPScript

With the ESPScript is not able to code everything with the same efficiency as the espressif compiler plus it doesn't gfive you accès to WiFi, bluetooth, SPI, I2C, ... Futhermore, it will not be concievable to rewrite functions like the one the the FastLED library or any other library. Hence the ESPScript can call pre-compiled functions. In other case you can need to access a 'precompile' variable which is changed by another process for instance.


### Access to 'pre compiled' variables
You need in your sketch that your variable needs to be accessible from the scripts:
```
addExternal("name_of_the_variable_int_the_script", externalType::value, (void *)&address_to_the_variable);
```

In your script you need to declare your variable as external:
```
external type name_of_the_variable_int_the_script;
```
You  read and write the variables. 
NB: a variable can also be an array.

Example:
```C
#include "ESPLiveScript.h"

string script="\
external int value;\n\
external uint16_t *array;\n\
//external uint16_t array[10]; is the same \n\
void fillArray()\n\
{\n\
  for(int i=0;i<10;i++)\n\
  {\n\
    array[i]=i*3;\n\
  }\n\
}\n\
void change()\n\
{\n\
value=value+2;\n\
}\n\
void main()\n\
{  \n\
   printfln(\"value: %d \",value);\n\
}";

int variable=0;
uint16_t _array[10];
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

Parser p;
addExternal("value", externalType::value, (void *)&variable);
addExternal("array", externalType::value, (void *)_array);
Executable exec=p.parseScript(&script);
if(exec.isExeExists())
{
  
 variable=5;
 exec.execute("main");
  variable=240;
 exec.execute("main");
 variable=15;
 printf("old value:%d ",variable);
 exec.execute("change");
 printf("new value:%d\n",variable);
 exec.execute("fillArray");
 for (int i=0;i<10;i++)
 {
  printf("%d:%d\n",i,_array[i]);
 }
}

}

void loop() {
  // put your main code here, to run repeatedly:

}
```

NB: here we did call three different functions all defined in the script.

Result:
```
***********PARSING DONE*********
***********COMPILING DONE*********
max used memory: 9172 maxstack:2112  started 265376 free mem:256100 consumed 9276 time:40ms
max used memory: 9172 maxstack:2112  started 265376 free mem:261868 consumed 3508 time:52ms
***********AFTER CLEAN*********
***********CREATE EXECUTABLE*********
Creation of an 264 bytes binary and 75 bytes data

Parsing 99 assembly lines ...

max used memory: 9172 maxstack:2112  started 265376 free mem:264168 consumed 1208 time:129ms
 value: 5 
value: 240 
old value:15 new value:17
0:0
1:3
2:6
3:9
4:12
5:15
6:18
7:21
8:24
9:27
```

### Safe mode and arrays

Let's consider the following Use case:

In your c++ code
```C
uint16_t _array[10];
addExternal("array", externalType::value, (void *)_array);
```

and the script
```C
external uint16_t *array; //or external uint16_t array[10];

void main()
{
  for(int i=0;i<200;i++)
  {
    array[i]=200;
  }
}
```
the script will write out of bounds of the array with unintended consequences.

To avoid this you can use the safemode:
```C
safe_mode
external uint16_t array[10];

void main()
{
  for(int i=0;i<200;i++)
  {
    array[i]=200;
  }
}
