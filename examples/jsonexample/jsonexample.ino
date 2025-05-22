#include "ESPLiveScript.h"

string script = R"EOF(
json dp.value as int g2;
json dp.test as int g;
json dp2 as float x;
int fib(int h)
{
   if(h<=1)
   return h;
   return fib(h-1)+fib(h-2);
}

void main()
{  

  int d=x*5;
  printfln("%d",d);
   printfln("fib of %d is %d",g2,fib(g2));
   printfln("fib of %d is %ld",g,fib(g));
   
}
)EOF";

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);

    Parser p;
    Executable exec = p.parseScript(&script);
    printf("copmùpile done\n");
    if (exec.isExeExists())
    {

        exec.execute("main", "{\"dp\":{\"value\":6,\"test\":12},\"dp2\":4.5}");
        exec.execute("main", "{\"dp\":{\"value\":10,\"test\":20},\"dp2\":7.5854}");
    }
    else
    {
        printf("%s\n", exec.error.error_message.c_str());
    }
}

void loop()
{
    // put your main code here, to run repeatedly:
}
