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
