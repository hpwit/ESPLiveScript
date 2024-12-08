#include "ESPLiveScript.h"

string script="\
Object new_type\n\
{\n\
  float f;\n\
  int index;\n\
  void display(int multi)\n\
  {\n\
    printfln(\"from Object:%d\",multi);\n\
  }\n\
  void func2() \n\
  {\n\
    display(f*index);\n\
  }\n\
}\n\
\n\
new_type var;\n\
void main()\n\
{\n\
 var.f=0.8;\n\
 var.index=12;\n\
 var.func2();\n\
 var.display(23)\n\
}";


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

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
