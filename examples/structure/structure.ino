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
