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

