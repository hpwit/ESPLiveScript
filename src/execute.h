#pragma once
#ifndef __ASM_EXECUTE
#define __ASM_EXECUTE
using namespace std;
static volatile TaskHandle_t __run_handle = NULL;
executable executecmd;
// string strcompile;
bool exeExist;
typedef struct
{
    vector<string> args;
    executable exe;
} _exe_args;

static void _run_task(void *pvParameters)
{

    _exe_args *_fg = ((_exe_args *)pvParameters);
    if (_fg->args.size() > 0)
    {
        executeBinary(_fg->args[0], _fg->exe);
    }
    else
    {
        executeBinary("main", _fg->exe);
    }
    pushToConsole("Execution done.", true);
    __run_handle = NULL;
    vTaskDelete(NULL);
}
static void feedTheDog()
{
    // feed dog 0
    TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE; // write enable
    TIMERG0.wdt_feed = 1;                       // feed dog
    TIMERG0.wdt_wprotect = 0;                   // write protect
    // feed dog 1
    TIMERG1.wdt_wprotect = TIMG_WDT_WKEY_VALUE; // write enable
    TIMERG1.wdt_feed = 1;                       // feed dog
    TIMERG1.wdt_wprotect = 0;                   // write protect
}
class Executable
{

public:
    Executable() {}

    void createExecutable()
    {
        if (__run_handle != NULL)
        {
#ifdef __CONSOLE_ESP32
            LedOS.pushToConsole("Something Already running kill it first ...", true);
#else
            Serial.printf("Something Already running kill it first ...\r\n");
#endif
            _kill();
        }

        bool othercore = false;
        exeExist = false;
        freeBinary(&executecmd);
    }
    void setPrekill(void (*function)(), void (*function2)())
    {
        prekill = function;
        postkill = function2;
    }

    void _kill()
    {
        if (__run_handle != NULL)
        {

            if (prekill != NULL)
                prekill();
            delay(20);
            if (__run_handle != NULL)
                vTaskDelete(__run_handle);
            __run_handle = NULL;
            delay(10);
            if (postkill != NULL)
                postkill();
        }
    }

    void _run(vector<string> args, bool second_core)
    {
        if (__run_handle != NULL)
        {

            _kill();
        }

        if (exeExist == true)
        {

            _exe_args df;
            df.args = args;
            df.exe = executecmd;

            xTaskCreateUniversal(_run_task, "_run_task", 4096 * 2, &df, 3, (TaskHandle_t *)&__run_handle, 0);
#ifdef __CONSOLE_ESP32
            LedOS.pushToConsole("Execution on going CTRL + k to stop", true);
#else
            Serial.printf("Execution on going CTRL + k to stop\r\n");
#endif
        }
        else
        {
#ifdef __CONSOLE_ESP32
            LedOS.pushToConsole("Nothing to execute.", true);
#else
            Serial.printf("Nothing to execute\r\n");
#endif
        }
    }
void free()
{
    exeExist = false;
     freeBinary(&executecmd);
}

void execute(string prog)
{
executeBinary(prog, executecmd);
}
private:
    void (*prekill)() = NULL;
    void (*postkill)() = NULL;
    // bool exeExist=false;
};
Executable SCExecutable = Executable();
#endif
