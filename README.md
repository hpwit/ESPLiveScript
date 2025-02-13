# Introduction
For a very long time now I have been looking at being able to execute programs on the esp32 without having to load the code via any IDE(Arduino, VSCode ...).
I rapidly forgot that topic and focus my energy on writing leds drivers and artnet libraries for the esp32. Once that done, I start thinking again about being able to write animations 'live'. Of course in my searches for existing previous implementations, I fell upon PixelBlaze and later Arti-Fx (part of the WLED MoonModules).
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

In the sc_examples directory you will find examples of complexe scripts.
 
**So I have decided also give it a go. Can I also conceive a 'language' to program led animations.**
 

<!-- TOC start (generated with https://github.com/derlin/bitdowntoc) -->

- [Which Language ?](#which-language-)
   * [C like language](#c-like-language)
   * [DYI parser and compiler](#dyi-parser-and-compiler)
   * [Not a development environment](#not-a-development-environment)
   * [Led manipulation oriented](#led-manipulation-oriented)

- [First Light](#first-light)
  * [Error object](#error-object)
  * [Deleting an executable](#deleting-an-executable)

- [The function you call can have input parameters](#the-function-you-call-can-have-input-parameters)

- [Interaction with pre compiled functions](#interaction-with-pre-compiled-functions)
  * [Calling/accessing 'pre compiled' functions/variables from ESPScript](#callingaccessing-pre-compiled-functionsvariables-from-espscript)
  * [Access to 'pre compiled' variables](#access-to-pre-compiled-variables)
  * [Calling 'pre-compiled' functions](#calling-pre-compiled-functions)
- [Safe mode and arrays](#safe-mode-and-arrays)

- [Variables types](#variables-types)
  * [Arrays and multidimensional arrays](#arrays-and-multidimensionnal-arrays)
  * [Structures](#structures)

- [Saving Executables](#saving-executables)
  * [Saving/executing the binary](#savingexecuting-the-binary)
  * [Executing the binary](#executing-the-binary)
  * [Binded functions](#binded-functions)
- [What you can do with the language](#what-you-can-do-with-the-language)
  * [Use of define](#use-of-define)

- [Running scripts in the background](#running-scripts-in-the-background)
  * [How to cope with several binaries](#how-to-cope-with-several-binaries)
  * [Task synchronization](#task-synchronization)
  * [Pre and post kill](#pre-and-post-kill)

- [Advanced stuff](#advanced-stuff)
  * [Pointer to the executable](#pointer-to-the-executable)
      * [Use of the executable pointer  for interrupts](#use-the-exectubale-pointer-for-interrupts)

- [Conclusion](#conclusion)

<!-- TOC end -->


 
<!-- TOC --><a name="which-language-"></a>
# Which Language ?
Being old school, I know that assembly language is the way to get the most of performance out of a CPU (given that your skills at writing ML are quite good of course). So I've tried to duplicate the same program directly assembly (still using arduino) and it worked great.
 
So I wrote an assembly language compiler as well as a small terminal interface to edit/save/compile/execute these programs (It will later become [ledOS](https://www.github.com/hpwit/LedOS) )

Of course proposing an assembly language compiler is not a really something usable :) so I took my chance at writing a compiler which will produce assembly languages for the esp32 xtensa CPU. 

<!-- TOC --><a name="c-like-language"></a>
## C like language
I have chosen to go for a C like syntax which is closed to JavaScript with stronger typing. I have made a loose adaptation of the language. But you can write programs like this:
 
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

<!-- TOC --><a name="dyi-parser-and-compiler"></a>
## DYI parser and compiler
I am not a compiler specialist and I wrote 'by hand' the parser and compiler using no existing grammar. Please do not throw me into the fire for this. The entire exercice has been a learning experience also and the result is not half cooked (according to me).

Later in this documentation you will see what is possible and not.

<!-- TOC --><a name="not-a-development-environment"></a>
## Not a development environment
This libray doesn't provide an environment to write scripts. Nevertheless it has been included in:
- [LedOS](https://github.com/hpwit/ledOS) : an environment in a console terminal to edit, save and execute programs

<img title="Screenshot" alt="Alt text" src="pictures/screen1.png" style="width:400px">

- [MoonModules StarLight](https://github.com/MoonModules/StarLight) : Web enabled ESP32 application for LED manipulation based on WLED which contains several modules (artnet, DMX, powerful mapping tool ....) [Live Scripts doc](https://ewowi.github.io/StarDocs/UserMod/UserModLiveScripts)


<!-- TOC --><a name="led-manipulation-oriented"></a>
## Led manipulation oriented
Even if the language is made to be of general application, the target of this language is targeted at creating led animations.
As a consequence the scripting language has some limitations that 


<!-- TOC --><a name="first-light"></a>
# First light :)

- To create a new instance of a parser : ``Parser _parser;`` 
- To parse a script and create an executable : ``Executable exec=_parser.parseScript(&script);``
- To execute the code assuming there is function called main in you code: ``exec.execute("main");``


If you execute the following program [firstlight.ino](examples/firstlight/firstlight.ino):

 ```C
#include "ESPLiveScript.h"

string script = R"EOF(
void main()
{
for(int i=0;i<20;i++)
    {
        printfln("i:%2d  3*i:%2d",i,3*i);
    }
}
)EOF";
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  Parser p;
  Executable exec = p.parseScript(&script);
  if (exec.isExeExists())
  {
    exec.execute("main");
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
}
 ```
the output will be:
```
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

**NB 1 : if you have several functions it the same script you can call any of the function**

**NB 2 : You can have information about the compiling process by activating the debug mode during the compilation** 

## Error object
the `Executable` class has an `error` member
- `error`: 1 if an error occured during the compiling
- `error_message` : the error message
- `line` and `pos` : line and position in the script where the error was found (this information is also in the `error_message`)


## Deleting an executable

To delete an executable hence freeing the used memory by the binary use: `exec.free()`

<!-- TOC --><a name="the-function-you-call-can-have-input-parameters"></a>
# The function you call can have input parameters

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

string script = R"EOF(
int fact(int h)
{
   if(h==1)
   {  
     return 1;
   } 
  return h*fact(h-1);
}

void main(int g)
{  
   printfln("factorial of %d is %d",g,fact(g));
}
)EOF";

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);

    Parser p;
    Executable exec = p.parseScript(&script);
    if (exec.isExeExists())
    {
        Arguments args;
        args.add(5);
        exec.execute("main", args);
        args.clear();
        args.add(6);
        exec.execute("main", args);
        args.clear();
        args.add(7);
        exec.execute("main", args);
    }
}

void loop()
{
    // put your main code here, to run repeatedly:
}

```


result:
```
factorial of 5 is 120
factorial of 6 is 720
factorial of 7 is 5040
 ```

<!-- TOC --><a name="interaction-with-pre-compiled-functions"></a>
# Interaction with pre compiled functions

<!-- TOC --><a name="callingaccessing-pre-compiled-functionsvariables-from-espscript"></a>
## Calling/accessing 'pre compiled' functions/variables from ESPScript

With the ESPScript is not able to code everything with the same efficiency as the espressif compiler plus it doesn't gfive you access to WiFi, bluetooth, SPI, I2C, ... Futhermore, it will not be concievable to rewrite functions like the one the the FastLED library or any other library. Hence the ESPScript can call pre-compiled functions. In other case you can need to access a 'precompile' variable which is changed by another process for instance.


<!-- TOC --><a name="access-to-pre-compiled-variables"></a>
## Access to 'pre compiled' variables
You need in your sketch that your variable needs to be accessible from the scripts:
```
addExternalVariable("name_of_the_variable_int_the_script", type,other details, (void *)&address_to_the_variable);
```

You  read and write the variables. 
NB: a variable can also be an array.

Example:
```C
#include "ESPLiveScript.h"

string script = R"EOF(
void fillArray()
{
  for(int i=0;i<10;i++)
  {
    array[i]=i*3;
  }
}
void change()
{
value=value+2;
}
void main()
{  
   printfln("value: %d ",value);
}
)EOF";

int variable = 0;
uint16_t _array[10];
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  Parser p;
  addExternalVariable("value", "int", "", (void *)&variable);
  addExternalVariable("array", "int *", "", (void *)_array);
  Executable exec = p.parseScript(&script);
  if (exec.isExeExists())
  {

    variable = 5;
    exec.execute("main");
    variable = 240;
    exec.execute("main");
    variable = 15;
    printf("old value:%d ", variable);
    exec.execute("change");
    printf("new value:%d\n", variable);
    exec.execute("fillArray");
    for (int i = 0; i < 10; i++)
    {
      printf("%d:%d\n", i, _array[i]);
    }
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
}

```

NB: here we did call three different functions all defined in the script.

Result:
```
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

<!-- TOC --><a name="calling-pre-compiled-functions"></a>
## Calling 'pre-compiled' functions
You can call 'core' functions which would be to complicated to reproduced in scripting (like fft , showing leds ...)

```
addExternalFunction("funtion name", "outputtype","signature", (void *)function);
```


Example
```C
#include "ESPLiveScript.h"

string script = R"EOF(
void main()
{
 float h=calc(52);
 displayfloat(h);
 otherfunction();
}
)EOF";

void displayfloat(float nb) {
  printf("from pre-compiled %f\n", nb);
}

float calcul(int pos) {
  return (float)(pos / 34.0);
}
void otherfunction() {
  printf("from other function\n");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  addExternalFunction("calc", "float", "int", (void *)calcul);
  addExternalFunction("displayfloat", "void", "float", (void *)displayfloat);
  addExternalFunction("otherfunction", "void", "", (void *)otherfunction);
  Parser p;
  Executable exec = p.parseScript(&script);

  if (exec.isExeExists()) {
    exec.execute("main");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}

```

Result:
```
 from pre-compiled 1.529412
from other function
```


<!-- TOC --><a name="safe-mode-and-arrays"></a>
# Safe mode and arrays

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
```

you will get

```
Overflow error line 0 max size: 10 got 11
```

NB: As the check will be done everytime a write is done then it will slow the script down.

<!-- TOC --><a name="variables-types"></a>
# Variables types

Here are the default types:
 * `uint8_t`
 * `char`
 * `bool` : `true`, `false`
 * `int` : int over 4 bytes
 * `s_int` : int over 2 bytes
 * `uint16_t`
 * `uint32_t`
 * `float`
 * `CRGB`
 * `CRGBW`

## Arrays and multidimensionnal arrays
 
You can manage arrays:
```
//example of definitions
int array[23]; 
int array2D[y][x]; //you can also write int arrayx_y[y,x];
int array3D[z][y][x];  //or int arrayx_y_x[z,y,x]; 

//usage

int h=array3D[23][12][2];

```


## Structures

You can define new types call `struct`

example:
```C
struct new_type
{
  float k;
  int l;
}
```
 
The structures can have methods

```C
struct new_type
{
  float h;
  int l;
  void display()
  {
    printf("l :%d\n",l);
  }
}
```


Example:
```C
#include "ESPLiveScript.h"

string script = R"EOF(
struct new_type
{
  float f;
  int index;
  void display(int multi)
  {
    printfln("from Object:%d",multi);
  }
  void func2() 
  {
    display(f*index);
  }
}

new_type var;
void main()
{
 var.f=0.8;
 var.index=12;
 var.func2();
 var.display(23);
}
)EOF";

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  Parser p;
  Executable exec = p.parseScript(&script);
  if (exec.isExeExists())
  {
    exec.execute("main");
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
}

```

Result:
```
from object:9
from structobjecture:23
```
NB 1: you can have arrays of objects : `new_type arr[10] `

NB 2: the functions needs to be in order i.e you cannot all a function which has not be defined previously. (for the moment)

NB 4: You can have structure constructors

```C
struct varname
{
  float h;
  uint16_t k;
  char c;
  varname()
  {
    ...
  }
  varname(int j)
  {
    h=j/23;
  }
}

....

varname d; //it is equivalent to varname d=varname();
varname d2=varname(23);
```


NB 4: Due to some memory alignment concern, for the moment you need to order the variable by order of size. Here is the order of size:
  - `float, uint32_t, int`
  - `s_int, uint16_t`
  - `uint8_t, CRGB, CRGBW`


i.e :
```C
struct varname
{
char c;
uint16_t k;
float h;
}

will not work. you need to arrange your data as such

struct varname
{
  float h;
  uint16_t k;
  char c;
}
```

# Saving executables
You can save the executables into SPIFF or SD card to re execute afterwards or recompile.

## Saving/Executing the binary
```C
#include "ESPLiveScript.h"
#include <LittleFS.h> 
#include "FS.h"
string script = R"EOF(
void main()
{
for(int i=0;i<20;i++)
    {
        printfln("i:%2d  3*i:%2d",i,3*i);
    }
}
)EOF";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Parser p;
  Binary bin = p.parseScriptBinary(&script);
  Binary bin2;
  LittleFS.begin(true);
  if (!bin.error.error) {

    saveBinary("/test.bin", LittleFS, &bin);
    freeBinary(&bin);

    loadBinary("/test.bin", LittleFS, &bin2);
    Executable ex;
   
    ex.createExecutableFromBinary(&bin2);
    if (ex.isExeExists())
      ex.execute("main");


  } else {
    printf("%s\n\r", bin.error.error_message.c_str());
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
```

## Executing the binary

If you need to only execute the binary. You do no need the entire library but only the runtime. `#include "ESPLivescriptRuntime.h"`

```C
#include "ESPLivescriptRuntime.h"
#include <LittleFS.h>
#include "FS.h"

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);

    Binary bin2;
    LittleFS.begin(true);

    loadBinary("/test.bin", LittleFS, &bin2);
    Executable ex;
    if (bin2.error.error)
    {
        Serial.printf("%s\n", bin2.error.error_message.c_str());
    }
    else
    {
        ex.createExecutableFromBinary(&bin2);
        if (ex.isExeExists())
            ex.execute("main");
    }
}

void loop()
{
    // put your main code here, to run repeatedly:
}

```
## Binded functions

The binded functions/variables are not exported with the binary. If the orginal binary has been compile with binded functions or variables using `addExternalFunction` or `addExternalVariable` to be able to execute the binary you will need to add the same bindings.

ie : If you have this script
```C
#include "ESPLiveScript.h"
#include <LittleFS.h> 
#include "FS.h"
string script = R"EOF(
void main()
{
....
show();

}
)EOF";

....

void show()
{
  displaygraphics();
}

addExternalFunction("show","void","",(void *)show())

saveBinary("/test.bin", LittleFS, &bin);

```
then the executing script should be

```C
#include "ESPLivescriptRuntime.h"
#include <LittleFS.h>
#include "FS.h"

void show()
{
  displaygraphics();
}
....


addExternalFunction("show","void","",(void *)show())
....

loadBinary("/test.bin", LittleFS, &bin2);

```



# What you can do with the language

Like any normal language you can have:
- loops (`while` , `for`)
- `break` , `continue`
- testing : `if` ,`else` ,`(test)?true_statement:false_statement`
- `++`,`--`: for integers and pointers
- `+=`,`-=`,`/=`,`*=`
- pointers
- `^` for power
- `>>` and `<<`
- type convertion `(float)` , `(int)`
    NB: the convertion float <-> int is automatic
- `define` : see below
- predefined functions:
  * `printf` , `pritnfln` only for intergers for now.
  * `fabs`, `abs` other to come

## Use of define

More or less like in C you can have pre-process instructions with a limited scope for now

```C
define TOKEN 25

if(i<TOKEN)
{
  ...
}

vill be compiled as

if(i<25)
{
  ...
}
```
Of course you can have this
```C
define p printf
...
p("here %d\n",25);
```

NB: there is no macro yet



# Running scripts in the background

You have the possibility of running scripts as task in the background (interesting when you want several tasks at once):

- `exec.executeAsTask("function_name")`
- `exec.executeAsTask("function_name",arguments)`
- `exec.executeAsTask("function_name", __RUN_CORE, args)`

When using this you can also do the following

- `exec.suspend()`
- `bool exec.isRunning()`
- `exec.estart()`
- `exec.kill()` : will not delete the binary hence will not free the memory use `exec.free()`

## How to cope with several binaries

When you deal with several binaries, it could get complicated to follow a lot of different variables.
You can 'register' your executable in the Script runtime:

`scriptRuntime.addExe(executable exec,string exename)`

```C
string script1="...";
string scrip2="...";

 Executable exec=p.parseScript(&script1);
 if(exec.isExeExists())
{
  scriptRuntime.addExe(exec,"script1");
}
exec=p.parseScript(&script2);
 if(exec.isExeExists())
{
  scriptRuntime.addExe(exec,"script2");
}

...

scriptRuntime.execute("script1");
scriptRuntime.execute("script2");
```

Here are all the functions of the scriptRuntime:
- `scriptRuntime.addExe(executable exec,string exename)`
- `scriptRuntime.excute(string execname)` : the runtime will execute the "main" function by default
- `scriptRuntime.excute(string execname,string function_name)`
- `scriptRuntime.excute(string execname,Arguments args)`:: the runtime will execute the "main" function by default
- `scriptRuntime.excute(string execname,Arguments args,string function_name)`
- `scriptRuntime.excuteAsTask(string execname)`: : the runtime will execute the "main" function by default
- `scriptRuntime.excuteAsTask(string execname,string function_name)`
- `scriptRuntime.excuteAsTask(string execname,Arguments args)`: the runtime will execute the "main" function by default
- `scriptRuntime.excuteAsTask(string execname,Arguments args,string function_name)`
- `scriptRuntime.excuteAsTask(string execname,Arguments args)` : the runtime will execute the "main" function by default
- `scriptRuntime.excuteAsTask(string execname,int core,Arguments args)` : the runtime will execute the "main" function by default
- `scriptRuntime.kill(string execname)` : this will not free the memory
- `scriptRuntime.deleteExe(string execname)` : will free the memory of the binary
- `Executable *findExecutable(string execname)`
- `vector<exe_info> scriptRuntime.getListExecutables()` : will list all the executables:
- `exe_info getExecutableInfo(string execname)`
  * exe_info:
    + `uint16_t data_size`
    + `uint16_t binary_size`
    + `uint16_t total_size`
    + `bool isRunning`
    + `string name`

## Tasks synchronization
As discussed earlier  this scripting language aims at being used primarily for leds animations. when you have several scripts running at the same time it is important to synchronize the script to avoid artifacts:
![Sync](/pictures/VideoToGif_GIF.GIF)


Let's say that you need to synchronize the show function which is an 'pre-compiled' function.
in your C/C++ program add this:
```C
void show()
{
  ...
  driver.showPixel();
  ...
}


string script1="
...

while(2>1)
{
rainbow();
sync(); //put this instead of show();
}";
string script2="
...

while(2>1)
{
gameoflife();
sync(); //put this instead of show();
}";
void setup()
{
runningPrograms.setFunctionToSync(show);
Parser p;
scriptRuntime.addExe(p.parseScript(&script1,"exe1"));
scriptRuntime.addExe(p.parseScript(&script2,"exe2"));
scriptRuntime.executeAsTask("exe1");
scriptRuntime.executeAsTask("exe2");
}

```

## Pre and post kill
When you kill a task you cannot control where exactly in the program it will be stopped. As a consequence it could be stopped when calling an external function with interupts or calling another background task.

For instance in the case of the I2SClockLessDriver and the I2SVirtualClocklessDriver it is necessary to stop the driver from displaying anything before killing the script.

this is done as such :
```C
void pre() {
  driver.__enableDriver = false;
  while (driver.isDisplaying) {};
}

void post() {
  driver.__enableDriver = true;
}

..
void setup()
{
  ...
 runningPrograms.setPrekill(pre, post);
 ...
}
```

# Advanced stuff

## Pointer to the executable

The compiler/runtime has an internal variable `_execaddr_` which contains the pointer to the executable itself.

### Use the exectubale pointer for interrupts
You can use this variable to register interrupt.

```C
#include "ESPLiveScript.h"
#include "driver/gpio.h"

typedef struct
{
  Executable *ptr;
  char *name;
  uint32_t last;
  uint32_t _new;
} call_isr;
call_isr isr_struct;


static void IRAM_ATTR gpio_isr_handler(void *args) {
  call_isr *ll = (call_isr *)args;
  unsigned long currentTime = millis();

  if ((currentTime - ll->last) > 100) {
    ll->ptr->executeOnly(string(ll->name));
    ll->last = currentTime;  // Update the last interrupt time
  }
}
void setup_gpio_interrupt(Executable *ptr, char *str, int pin_num) {
  printf("Setting up the interrupt\n");
  gpio_config_t io_conf = {
    .pin_bit_mask = (1ULL << pin_num),
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,  // Disable pull-down
    .intr_type = GPIO_INTR_NEGEDGE,
  };

  gpio_config(&io_conf);
  gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
  call_isr *_isr_struct = &isr_struct;
  _isr_struct->ptr = ptr;
  _isr_struct->name = str;
  _isr_struct->last = millis();
  gpio_isr_handler_add((gpio_num_t)pin_num, gpio_isr_handler, _isr_struct);
}

string script = R"EOF(
int number=0;
void increase()
{
  number++;
}

void main()
{  
   pinInterrupt(_execaddr_, "increase", 23);
   while(true)
   {
     printfln("number:%d",number);
   }
}
)EOF";

void setup() {
  Serial.begin(115200);
  addExternalFunction("pinInterrupt", "void", "uint32_t,char *,int", (void *)setup_gpio_interrupt);
  Parser p;
  Executable exec = p.parseScript(&script);
  if (exec.isExeExists()) {
    exec.execute("main");
  }
  else {
  printf("Error:%s\n",exec.error.error_message.c_str());
  }
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}

```
[![Tetris](http://img.youtube.com/vi/nx-Y8qRdbSQ/0.jpg)](https://www.youtube.com/shorts/nx-Y8qRdbSQ)


# Conclusion

This is my first try at creating such a thing. You can see a video of me talking about it [live on youtube](https://www.youtube.com/watch?v=iOzKHQxdNJM) where I use the compiler with ledOS.

Please have a look at [Starlight](https://github.com/MoonModules/StarLight/commit/42cb1b8db689b097774bf7640c768d880337567c)

I will add an issue for all the functionalities you want to see added in the; compiler.

As always enjoy and have fun.

New version coming with some improvements. look at the [project board](https://github.com/users/hpwit/projects/1/views/1)









