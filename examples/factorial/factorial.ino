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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Parser p;
  Executable exec = p.parseScript(&script);
  if (exec.isExeExists()) {
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

void loop() {
  // put your main code here, to run repeatedly:
}
