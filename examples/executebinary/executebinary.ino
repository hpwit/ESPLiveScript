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
