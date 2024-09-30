#pragma once
#ifndef __ASM_EXECUTE
#define __ASM_EXECUTE
#include <vector>
#ifndef __RUN_CORE
#define __RUN_CORE 0
#endif
using namespace std;
#ifndef _MAX_PROG_AT_ONCE
#define _MAX_PROG_AT_ONCE 10
#endif
executable executecmd;
TaskHandle_t __run_handles[_MAX_PROG_AT_ONCE];
volatile xSemaphoreHandle waitEndShow[_MAX_PROG_AT_ONCE];
; // string strcompile;
bool exeExist;
typedef struct
{
    vector<string> args;
    executable exe;

} _exe_args;
EventGroupHandle_t xCreatedEventGroup = xEventGroupCreate();
EventGroupHandle_t xCreatedEventGroup2 = xEventGroupCreate();
bool resetSync=false;
static void _run_task(void *pvParameters);
#include "esp_task_wdt.h"
static void feedTheDog()
{
    /*
    // feed dog 0
    TIMERG0.wdtwprotect = TIMG_WDT_WKEY_VALUE; // write enable
    TIMERG0.wdtfeed = 1;                       // feed dog
    TIMERG0.wdtwprotect = 0;                   // write protect
    // feed dog 1
    TIMERG1.wdtwprotect = TIMG_WDT_WKEY_VALUE; // write enable
    TIMERG1.wdtfeed = 1;                       // feed dog
    TIMERG1.wdtwprotect = 0;                   // write protect

    if(esp_task_wdt_reset()!=ESP_OK)
    {
        printf("impossnlie\r\n");
    }
    */
    vTaskDelay(1);
}
class _executablesClass
{
public:
    _executablesClass()
    {
        for (int i = 0; i < _MAX_PROG_AT_ONCE; i++)
        {
            __run_handles[i] = NULL;
            waitEndShow[i] = NULL;
        }
        nb_concurrent_programs = 0;
    }
    int getHandle()
    {
        if (nb_concurrent_programs < _MAX_PROG_AT_ONCE - 1)
        {
            for (int i = 0; i < _MAX_PROG_AT_ONCE; i++)
            {
                if (__run_handles[i] == NULL)
                {
                    nb_concurrent_programs++;
                    printf(" ndb prgh:%d position%d\r\n", nb_concurrent_programs, i);
                    waitEndShow[i] = xSemaphoreCreateBinary();
                    return i;
                }
            }
            return 9999;
        }
        else
        {
            return 9999;
        }
    }
    void suspendAll()
    {
        for (int i = 0; i < _MAX_PROG_AT_ONCE; i++)
        {
            if (__run_handles[i] != NULL)
            {
                vTaskSuspend(__run_handles[i]);
            }
        }
    }
    void restartAll()
    {
        for (int i = 0; i < _MAX_PROG_AT_ONCE; i++)
        {
            if (__run_handles[i] != NULL)
            {
                vTaskResume(__run_handles[i]);
            }
        }
    }
    void freeSync()
    {
        if(nb_concurrent_programs==0)
        return;
         int MASK = (1 << (nb_concurrent_programs)) - 1;
        xEventGroupSync(xCreatedEventGroup,
                        MASK,
                        MASK,
                        portMAX_DELAY);
        vTaskDelay(10);
        xEventGroupSync(xCreatedEventGroup2,
                        MASK,
                        MASK,
                        portMAX_DELAY);
    }
    TaskHandle_t *getHandleByIndex(int i)
    {
        if (i >= 0 and i < _MAX_PROG_AT_ONCE)
        {
            // waitEndShow[i]= xSemaphoreCreateBinary();
            return &__run_handles[i];
        }
        else
        {
            return NULL;
        }
    }
    void removeHandle(int i)
    {
        nb_concurrent_programs--;
        __run_handles[i] = NULL;
        waitEndShow[i] = NULL;
    }
    void setFunctionToSync(void (*function)())
    {
        functionToSync=function;
    }

    // private:
    int nb_concurrent_programs;
    int nb_concurrent_programs_current;
    void (*functionToSync)() = NULL;

};
_executablesClass runningPrograms = _executablesClass();
static void syncExt(int h)
{
    if (resetSync)
    return;
    // printf("on tente %d\r\n",h);
    int MASK = (1 << (runningPrograms.nb_concurrent_programs)) - 1;
    EventBits_t uxReturn;
    uxReturn = xEventGroupSync(xCreatedEventGroup,
                               1 << h,
                               MASK,
                               portMAX_DELAY);

    // printf("all \r\n");
        if (resetSync)
    return;
    if (h == 0)
    {
        if ((uxReturn & MASK) == MASK)
        {
            xEventGroupClearBits(xCreatedEventGroup, MASK);
            if(runningPrograms.functionToSync!=NULL)
            runningPrograms.functionToSync();
            xEventGroupSync(xCreatedEventGroup2,
                            1 << h,
                            MASK,
                            portMAX_DELAY);
        }
    }
    else
    {
        xEventGroupSync(xCreatedEventGroup2,
                        1 << h,
                        MASK,
                        portMAX_DELAY);
    }

    // printf("release %d\r\n",h);
}

class Executable
{

public:
#ifndef __TEST_DEBUG
    int __run_handle_index;
#endif
    bool exeExist;
    bool _isRunning = false;
    bool isHalted = false;
    _exe_args df;
    Executable()
    {
        exeExist = false;
        __run_handle_index = 9999;
    }
    Executable(executable _executable)
    {
        if (_executable.error.error == 0)
        {

            exeExist = true;
        }
        else
        {
            exeExist = false;
        }
        _executecmd = _executable;
    }
    void setExecutable(executable _executable)
    {
        _executecmd = _executable;
        if (_executable.error.error == 0)
        {

            exeExist = true;
        }
        else
        {
            exeExist = false;
        }
    }
    void createExecutable()
    {
#ifndef __TEST_DEBUG
        if (_isRunning)
        {
#ifdef __CONSOLE_ESP32
            LedOS.pushToConsole("Something Already running kill it first ...", true);
#else
            Serial.printf("Something Already running kill it first ...\r\n");
#endif
            _kill();
        }

        bool othercore = false;

        if (exeExist)
        {
            freeBinary(&_executecmd);
        }
        exeExist = false;

#endif
    }
    void setPrekill(void (*function)(), void (*function2)())
    {
        prekill = function;
        postkill = function2;
    }
    void suspend()
    {
#ifndef __TEST_DEBUG
        if (_isRunning and !isHalted)
        {
#ifdef __CONSOLE_ESP32
            LedOS.pushToConsole("Halting the program ...", true);
#else
            Serial.printf("Halting the program...\r\n");
#endif
            if (prekill != NULL)
                prekill();
            vTaskDelay(30);
            if (__run_handle_index != 9999)
            {
                vTaskSuspend(*runningPrograms.getHandleByIndex(__run_handle_index));
            }

            // _isRunning=false;
            isHalted = true;
            vTaskDelay(30);
            if (postkill != NULL)
                postkill();
                // vTaskDelay(20);
#ifdef __CONSOLE_ESP32
            LedOS.pushToConsole("Program Halted.", true);
#else
            Serial.printf("Program Halted.\r\n");
#endif
        }

        // freeBinary(&_executecmd);
#endif
    }
    void restart()
    {
#ifndef __TEST_DEBUG
        if (_isRunning and isHalted)
        {

            vTaskResume(*runningPrograms.getHandleByIndex(__run_handle_index));
            isHalted = false;
        }
#endif
    }
    void _kill()
    {
#ifndef __TEST_DEBUG
        if (_isRunning)
        {
#ifdef __CONSOLE_ESP32
            LedOS.pushToConsole("Stopping the program ...", true);
#else
            Serial.printf("Stopping the program...\r\n");
#endif
            if (prekill != NULL)
                prekill();
            vTaskDelay(30);

            if (__run_handle_index != 9999)
            {
                vTaskDelete(*runningPrograms.getHandleByIndex(__run_handle_index));
            }

            _isRunning = false;
            vTaskDelay(30);
            if (postkill != NULL)
                postkill();
                // vTaskDelay(20);
#ifdef __CONSOLE_ESP32
            LedOS.pushToConsole("Program stopped.", true);
#else
            Serial.printf("Program stopped.\r\n");
#endif
        }
        delay(10);
        runningPrograms.removeHandle(__run_handle_index);
        __run_handle_index = 9999;

        // freeBinary(&_executecmd);
#endif
    }

    void _run(vector<string> args, bool second_core)
    {
#ifndef __TEST_DEBUG
        if (_isRunning)
        {

            _kill();
        }

        if (exeExist == true)
        {

            df.args = args;
            df.exe = _executecmd;
            //
            // we free the sync
        resetSync=true;
           

            runningPrograms.freeSync();
            vTaskDelay(10);
             runningPrograms.suspendAll();

            __run_handle_index = runningPrograms.getHandle();
             resetSync=false;
              runningPrograms.restartAll();
            if (__run_handle_index == 9999)
            {
#ifdef __CONSOLE_ESP32
                LedOS.pushToConsole("too many programs at once", true);
#else
                Serial.printf("too many programs at once\r\n");
#endif
            }
            string taskname = string_format("_run_task_%d", __run_handle_index);
            xTaskCreateUniversal(_run_task, taskname.c_str(), 4096 * 2, this, 3, (TaskHandle_t *)runningPrograms.getHandleByIndex(__run_handle_index), __RUN_CORE);

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
#endif
    }
    void free()
    {
#ifndef __TEST_DEBUG
        if (exeExist)
        {
            freeBinary(&_executecmd);
        }
        exeExist = false;

#endif
    }

    void execute(string prog)
    {
#ifndef __TEST_DEBUG
        executeBinary("@_" + prog, _executecmd, 9999);
#endif
    }

    void executeAsTask(string prog)
    {
#ifndef __TEST_DEBUG
        vector<string> args;
        args.push_back("@_" + prog);
        _run(args, true);
#endif
    }
    bool isRunning()
    {
        return _isRunning;
    }

private:
    void (*prekill)() = NULL;
    void (*postkill)() = NULL;
    executable _executecmd;

    // bool exeExist=false;
};
static void _run_task(void *pvParameters)
{

#ifndef __TEST_DEBUG

    Executable *exec = ((Executable *)pvParameters);
    // esp_task_wdt_delete(NULL);
    //  _exe_args *_fg = exec->df;
    exec->_isRunning = true;
    if (exec->df.args.size() > 0)
    {
        executeBinary(exec->df.args[0], exec->df.exe, exec->__run_handle_index);
    }
    else
    {
        executeBinary("@_main", exec->df.exe, exec->__run_handle_index);
    }
    pushToConsole("Execution done.", true);
    // exec->__run_handle= NULL;
    exec->_isRunning = false;

    vTaskDelete(NULL);
#endif
}

Executable SCExecutable = Executable();
#endif
