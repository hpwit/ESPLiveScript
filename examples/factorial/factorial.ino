#include "ESPLiveScript.h"

string script = "\
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
   printf(\"factorial of %d is %d\",g,fact(g));\n\
}";

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
