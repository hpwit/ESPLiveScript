#pragma once
#ifndef __ASM_EXECUTE
#define __ASM_EXECUTE
#include <vector>
#include <variant>
#ifndef __RUN_CORE
#define __RUN_CORE 0
#endif
using namespace std;
#ifndef _MAX_PROG_AT_ONCE
#define _MAX_PROG_AT_ONCE 10
#endif
executable executecmd;
volatile bool useOfSync;

#ifdef __TEST_DEBUG
typedef struct _TaskHandle_t
{
    int h;
};
typedef _TaskHandle_t *TaskHandle_t;
typedef struct _xSemaphoreHandle
{
    int h;
};
typedef _xSemaphoreHandle *xSemaphoreHandle;


#endif
typedef struct
{
    uint16_t data_size;
    uint16_t binary_size;
    uint16_t total_size;
    bool isRunning;
    string name;
} exe_info;
TaskHandle_t __run_handles[_MAX_PROG_AT_ONCE];
volatile xSemaphoreHandle waitEndShow[_MAX_PROG_AT_ONCE];

; // string strcompile;
bool exeExist;
typedef struct
{
    vector<string> args;
    executable exe;

} _exe_args;
typedef struct
{
    string name;
    bool isRunning;
} exeInfo;

#ifndef __TEST_DEBUG
EventGroupHandle_t xCreatedEventGroup = xEventGroupCreate();
EventGroupHandle_t xCreatedEventGroup2 = xEventGroupCreate();
#endif
bool resetSync = false;
bool toResetSync = false;
bool isSyncalled = false;
static void _run_task(void *pvParameters);

#ifndef __TEST_DEBUG
#include "esp_task_wdt.h"

#endif
static void feedTheDog()
{
#ifndef __TEST_DEBUG
    vTaskDelay(1);
#endif
}
class Executable;
static void syncExt(int h);
class _executablesClass
{
public:
    _executablesClass()
    {
        for (int i = 0; i < _MAX_PROG_AT_ONCE; i++)
        {
            __run_handles[i] = NULL;
            waitEndShow[i] = NULL;
            execPtr[i] = NULL;
        }
        nb_concurrent_programs = 0;
        addExternal("_sync", externalType::function, (void *)syncExt);
        addExternal("feed", externalType::function, (void *)feedTheDog);
    }
    int getHandle(Executable *exec)
    {

        if (nb_concurrent_programs < _MAX_PROG_AT_ONCE - 1)
        {
#ifndef __TEST_DEBUG
            for (int i = 0; i < _MAX_PROG_AT_ONCE; i++)
            {
                if (__run_handles[i] == NULL)
                {
                    nb_concurrent_programs++;
                    // printf(" ndb prgh:%d position%d\r\n", nb_concurrent_programs, i);
                    waitEndShow[i] = xSemaphoreCreateBinary();
                    execPtr[i] = exec;
                    return i;
                }
            }
#endif
            return 9999;
        }
        else
        {
            return 9999;
        }
    }
    uint32_t getMask();
    void kill(int handle);
    void suspendAll()
    {
#ifndef __TEST_DEBUG
        for (int i = 0; i < _MAX_PROG_AT_ONCE; i++)
        {
            if (__run_handles[i] != NULL)
            {
                vTaskSuspend(__run_handles[i]);
            }
        }
#endif
    }
    void restartAll()
    {
#ifndef __TEST_DEBUG
        for (int i = 0; i < _MAX_PROG_AT_ONCE; i++)
        {
            if (__run_handles[i] != NULL)
            {
                vTaskResume(__run_handles[i]);
            }
        }
#endif
    }
    void freeSync()
    {
#ifndef __TEST_DEBUG
        if (nb_concurrent_programs == 0)
            return;
        uint32_t MASK = getMask();
        // TickType_t xTicksToWait = 2000 / portTICK_PERIOD_MS;
        xEventGroupSync(xCreatedEventGroup,
                        MASK,
                        MASK,
                        portMAX_DELAY);
        vTaskDelay(10);
        xEventGroupSync(xCreatedEventGroup2,
                        MASK,
                        MASK,
                        portMAX_DELAY);
#endif
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
        execPtr[i] = NULL;
    }
    void setFunctionToSync(void (*function)())
    {
        functionToSync = function;
    }

    int getFirstHandle()
    {
        for (int i = 0; i < _MAX_PROG_AT_ONCE; i++)
        {
            if (__run_handles[i] != NULL)
            {
                return i;
            }
        }

        return _MAX_PROG_AT_ONCE;
    }
    void setPrekill(void (*function)(), void (*function2)())
    {
        _prekill = function;
        _postkill = function2;
    }
    void prekill()
    {
        if (nb_concurrent_programs == 1)
        {
            if (_prekill != NULL)
                _prekill();
        }
    }
    void postkill()
    {
        if (nb_concurrent_programs == 1)
        {
            if (_postkill != NULL)
                _postkill();
        }
    }

    // private:
    int nb_concurrent_programs;
    int nb_concurrent_programs_current;
    void (*functionToSync)() = NULL;
    Executable *execPtr[_MAX_PROG_AT_ONCE];
    void (*_prekill)() = NULL;
    void (*_postkill)() = NULL;
};
_executablesClass runningPrograms = _executablesClass();
static void syncExt(int h)
{
#ifndef __TEST_DEBUG
    isSyncalled = true;
    if (resetSync)
        return;
    // printf("on tente %d\r\n",h);
    uint32_t MASK = runningPrograms.getMask();
    EventBits_t uxReturn;
    // TickType_t xTicksToWait = 2000 / portTICK_PERIOD_MS;
    uxReturn = xEventGroupSync(xCreatedEventGroup,
                               1 << h,
                               MASK,
                               portMAX_DELAY);

    // printf("all \r\n");

    if (h == runningPrograms.getFirstHandle())
    {
        if ((uxReturn & MASK) == MASK)
        {
            xEventGroupClearBits(xCreatedEventGroup, MASK);
            //  xEventGroupClearBits(xCreatedEventGroup2, MASK);

            if (runningPrograms.functionToSync != NULL)
            {

                runningPrograms.functionToSync();
                if (resetSync)
                    return;
            }

            xEventGroupSync(xCreatedEventGroup2,
                            1 << h,
                            MASK,
                            portMAX_DELAY);

            if (toResetSync)
            {
                resetSync = true;
                toResetSync = false;
            }
            xEventGroupClearBits(xCreatedEventGroup2, MASK);
        }
    }
    else
    {
        xEventGroupSync(xCreatedEventGroup2,
                        1 << h,
                        MASK,
                        portMAX_DELAY);
    }
    isSyncalled = true;
// printf("release %d\r\n",h);
#endif
}

class Executable
{

public:
    int __run_handle_index;

    bool exeExist;
    bool _isRunning = false;
    bool isHalted = false;
    error_message_struct error;
    string name = "Unknown";
    Arguments args;
    _exe_args df;
    Executable()
    {
        exeExist = false;
        __run_handle_index = 9999;
        // args.clear();
        // args.shrink_to_fit();
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
    bool isFunctionInExecutable(string name)
    {
        for (int i = 0; i < _executecmd.functions.size(); i++)
        {
            if (_executecmd.functions[i].name.compare(name) == 0)
            {
                return true;
            }
        }
        return false;
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
            kill();
        }

        // bool othercore = false;

        if (exeExist)
        {
            freeBinary(&_executecmd);
        }
        exeExist = false;

#endif
    }

    void suspend()
    {

#ifndef __TEST_DEBUG
#ifndef __TEST_DEBUG
        if (_isRunning and !isHalted)
        {
#ifdef __CONSOLE_ESP32
            LedOS.pushToConsole("Halting the program ...", true);
#else
            Serial.printf("Halting the program...\r\n");
#endif
            resetSync = true;

            runningPrograms.freeSync();
            vTaskDelay(10);
            if (__run_handle_index != 9999)
            {
                vTaskSuspend(*runningPrograms.getHandleByIndex(__run_handle_index));
            }

            // _isRunning=false;
            isHalted = true;
            vTaskDelay(10);

            // vTaskDelay(20);
#ifdef __CONSOLE_ESP32
            LedOS.pushToConsole("Program Halted.", true);
#else
            Serial.printf("Program Halted.\r\n");
#endif
        }

        // freeBinary(&_executecmd);
#endif
#endif
    }
    void restart()
    {
#ifndef __TEST_DEBUG
        if (_isRunning and isHalted)
        {
            resetSync = true;

            runningPrograms.freeSync();
            vTaskDelay(10);
            vTaskResume(*runningPrograms.getHandleByIndex(__run_handle_index));
            isHalted = false;
            resetSync = false;
        }
#endif
    }
    void kill()
    {
#ifndef __TEST_DEBUG
        if (_isRunning)
        {
#ifdef __CONSOLE_ESP32
            LedOS.pushToConsole("Stopping the program ...", true);
#else
            Serial.printf("Stopping the program...\r\n");
#endif
            // printf("old mask %d\r\n",runningPrograms.getMask());
            toResetSync = true;
            while (!toResetSync)
            {
            }
            vTaskDelay(10);
            runningPrograms.freeSync();
            // vTaskDelay(10);
            runningPrograms.suspendAll();

            runningPrograms.prekill();
            vTaskDelay(30);

            if (__run_handle_index != 9999)
            {
                vTaskDelete(*runningPrograms.getHandleByIndex(__run_handle_index));
            }

            _isRunning = false;
            vTaskDelay(30);

            runningPrograms.postkill();
            // vTaskDelay(20);
#ifdef __CONSOLE_ESP32
            LedOS.pushToConsole("Program stopped.", true);
#else
            Serial.printf("Program stopped.\r\n");
#endif
            vTaskDelay(10);
            runningPrograms.removeHandle(__run_handle_index);
            // printf("new mask %d\r\n",runningPrograms.getMask());
            resetSync = false;
            toResetSync = false;
            runningPrograms.restartAll();
            isSyncalled = false;
            __run_handle_index = 9999;
        }

        // freeBinary(&_executecmd);
#endif
    }

    int _run(vector<string> args, bool second_core, int core, Arguments arguments)
    {
        __run_handle_index = 9999;
#ifndef __TEST_DEBUG
        if (_isRunning)
        {

            kill();
        }

        if (exeExist == true)
        {

            // printf("rrrr\r\n");
            df.args = args;
            df.exe = _executecmd;
            //
            // we free the sync

            toResetSync = true;
            // vTaskDelay(30);
            if (isSyncalled)
                while (!resetSync)
                {
                }
            // printf("rrrr\r\n");
            // runningPrograms.freeSync();
            vTaskDelay(20);
            runningPrograms.suspendAll();

            __run_handle_index = runningPrograms.getHandle(this);
            _isRunning = true;
            toResetSync = false;
            resetSync = false;
            runningPrograms.restartAll();
            // vTaskDelay(10);

            if (__run_handle_index == 9999)
            {
#ifdef __CONSOLE_ESP32
                LedOS.pushToConsole("too many programs at once", true);
#else
                Serial.printf("too many programs at once\r\n");
#endif
            }
            string taskname;
            if (name.compare("Unknow") == 0)
                taskname = string_format("_run_task_%d", __run_handle_index);
            else
                taskname = string_format("%s_%d", name.c_str(), __run_handle_index);
            xTaskCreateUniversal(_run_task, taskname.c_str(), 4096 * 2, this, 3, (TaskHandle_t *)runningPrograms.getHandleByIndex(__run_handle_index), core);

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
        return __run_handle_index;
    }
    void free()
    {
#ifndef __TEST_DEBUG
        if (_isRunning)
        {
            kill();
        }
        if (exeExist)
        {
            uint32_t memb = esp_get_free_heap_size();
            printf("Free memory before:%ld\r\n", esp_get_free_heap_size());
            freeBinary(&_executecmd);
            _isRunning = false;
            uint32_t mema = esp_get_free_heap_size();
            printf("Free memory after:%ld freed:%ld\r\n", mema, mema - memb);
        }
        exeExist = false;

#endif
    }

    bool isExeExists()
    {
        return exeExist;
    }
#ifndef __TEST_DEBUG
    void execute(string prog)
    {
        args.clear();
#ifndef __TEST_DEBUG
        error_message_struct res = executeBinary("@_" + prog, _executecmd, 9999, args);
        if (res.error)
        {
            pushToConsole(res.error_message, true);
        }
#endif
    }

    void execute(string prog, Arguments arguments)
    {
        args.clear();
        for (int i = 0; i < arguments._args.size(); i++)
        {
            args.add(arguments._args[i]);
        }
#ifndef __TEST_DEBUG
        error_message_struct res = executeBinary("@_" + prog, _executecmd, 9999, args);
        if (res.error)
        {
            pushToConsole(res.error_message, true);
        }
#endif
    }

    void executeAsTask(string prog, int core, Arguments arguments)
    {
        args.clear();
        for (int i = 0; i < arguments._args.size(); i++)
        {
            args.add(arguments._args[i]);
        }
#ifndef __TEST_DEBUG
        if (core == 0 or core == 1)
        {
            vector<string> __args;
            __args.push_back("@_" + prog);
            _run(__args, true, core, arguments);
        }
        else
        {
            pushToConsole(string_format("Wrong core number %d should be 0 or 1", core));
        }
#endif
    }

    void executeAsTask(string prog, Arguments arguments)
    {
        executeAsTask(prog, __RUN_CORE, arguments);
    }
    void executeAsTask(string prog)
    {
        args.clear();
        executeAsTask(prog, __RUN_CORE, args);
    }

    void executeAsTask(string prog, int core)
    {
        args.clear();
        executeAsTask(prog, core, args);
    }
#endif
    bool isRunning()
    {
        return _isRunning;
    }

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
        error_message_struct res = executeBinary(exec->df.args[0], exec->df.exe, exec->__run_handle_index, exec->args);
        if (res.error)
        {
            pushToConsole(res.error_message, true);
        }
    }
    else
    {
        error_message_struct res = executeBinary("@_main", exec->df.exe, exec->__run_handle_index, exec->args);
        if (res.error)
        {
            pushToConsole(res.error_message, true);
        }
    }
    pushToConsole("Execution done.", true);
    // exec->__run_handle= NULL;
    exec->_isRunning = false;
    runningPrograms.removeHandle(exec->__run_handle_index);
    isSyncalled = false;
    vTaskDelete(NULL);
#endif
}

Executable SCExecutable = Executable();

void _executablesClass::kill(int handle_number)
{
    if (handle_number >= 0 and handle_number < _MAX_PROG_AT_ONCE)
    {
        if (execPtr[handle_number] != NULL)
        {
            execPtr[handle_number]->kill();
        }
    }
}

uint32_t _executablesClass::getMask()
{

    int mask = 0;
    for (int i = 0; i < _MAX_PROG_AT_ONCE; i++)
    {
        if (__run_handles[i] != NULL)
        {
            if (execPtr[i]->isRunning())
            {
                mask = mask | (1 << i);
            }
        }
    }
    return mask;
}

class _ScriptRuntime
{
public:
    _ScriptRuntime() {}
    void addExe(Executable df)
    {
        if (df.name.size() > 0)
        {
            _scExecutables.push_back(df);
        }
        else
        {
            pushToConsole("please add a name to the executable", true);
        }
    }
    void addExe(Executable df, string name)
    {
        df.name = name;
        addExe(df);
    }
    Executable *findExecutable(string name)
    {
        for (int i = 0; i < _scExecutables.size(); i++)
        {
            if (_scExecutables[i].name.compare(name) == 0)
            {
                return &_scExecutables[i];
            }
        }
        pushToConsole(string_format("Executable %s not found", name.c_str()), true);
        return NULL;
    }
    void execute(string name)
    {
        Executable *exec = findExecutable(name);
        if (exec != NULL)
        {
#ifndef __TEST_DEBUG
            exec->execute("main");
#endif
        }
    }
    void execute(string name, string function)
    {
        Executable *exec = findExecutable(name);
        if (exec != NULL)
        {
#ifndef __TEST_DEBUG
            exec->execute(function);
#endif
        }
    }
    void execute(string name, Arguments arguments)
    {
        Executable *exec = findExecutable(name);
        if (exec != NULL)
        {

#ifndef __TEST_DEBUG
            exec->execute("main", arguments);
#endif
        }
    }
    void execute(string name, string function, Arguments arguments)
    {
        Executable *exec = findExecutable(name);
        if (exec != NULL)
        {
#ifndef __TEST_DEBUG
            exec->execute(function, arguments);
#endif
        }
    }

    void executeAsTask(string name, Arguments arguments)
    {
        Executable *exec = findExecutable(name);
        if (exec != NULL)
        {
#ifndef __TEST_DEBUG
            exec->executeAsTask("main", arguments);
#endif
        }
    }
    void executeAsTask(string name)
    {
        Executable *exec = findExecutable(name);
        if (exec != NULL)
        {
#ifndef __TEST_DEBUG
            exec->executeAsTask("main");
#endif
        }
    }

    void executeAsTask(string name, string function, Arguments arguments)
    {
        Executable *exec = findExecutable(name);
        if (exec != NULL)
        {
#ifndef __TEST_DEBUG
            exec->executeAsTask(function, arguments);
#endif
        }
    }
    void executeAsTask(string name, string function)
    {
        Executable *exec = findExecutable(name);
        if (exec != NULL)
        {
#ifndef __TEST_DEBUG
            exec->executeAsTask(function);
#endif
        }
    }
    void executeAsTask(string name, int core, Arguments args)
    {
        Executable *exec = findExecutable(name);
        if (exec != NULL)
        {
#ifndef __TEST_DEBUG
            exec->executeAsTask("main", core, args);
#endif
        }
    }
    void executeAsTask(string name, int core)
    {
        Executable *exec = findExecutable(name);
        if (exec != NULL)
        {
#ifndef __TEST_DEBUG
            exec->executeAsTask("main", core);
#endif
        }
    }
    void kill(string name)
    {
        Executable *exec = findExecutable(name);
        if (exec != NULL)
        {
#ifndef __TEST_DEBUG
            if (exec->isRunning())
                exec->kill();
#endif
        }
    }

    void deleteExe(string name)
    {
        Executable *exec = findExecutable(name);
        if (exec != NULL)
        {
            free(name);
            for (vector<Executable>::iterator it = _scExecutables.begin(); it != _scExecutables.end(); it++)
            {
                if ((*it).name.compare(name) == 0)
                {
                    // pushToConsole("get it");
                    _scExecutables.erase(it);
                    // pushToConsole("get it2");
                    return;
                }
            }
        }
    }
    void free(string name)
    {
        Executable *exec = findExecutable(name);
        if (exec != NULL)
        {
#ifndef __TEST_DEBUG
            exec->free();
#endif
        }
    }

    void listExec()
    {
        for (int i = 0; i < _scExecutables.size(); i++)
        {
            pushToConsole(string_format(" %2d | %12s isRunning:%d", i + 1, _scExecutables[i].name.c_str(), _scExecutables[i].isRunning()), true);
        }
    }

        vector<exe_info> getListExecutables()
        {
            vector<exe_info> res;
            for(int i=0;i<_scExecutables.size();i++)
            {
                    res.push_back(getExecutableInfo(i));
            }
            return res;
        }
    void killAndFreeRunningProgram()
    {
        for (int i = 0; i < _scExecutables.size(); i++)
        {
            if (_scExecutables[i].isRunning())
            {
                kill(_scExecutables[i].name);
                free(_scExecutables[i].name);
                deleteExe(_scExecutables[i].name);
            }
        }
    }
    exe_info getExecutableInfo(string name)
    {
        Executable *f = findExecutable(name);

        exe_info inf;
        if (f != NULL)
        {
            inf.data_size = f->_executecmd.data_size;
            inf.binary_size = f->_executecmd.binary_size;
            inf.total_size = f->_executecmd.data_size + f->_executecmd.binary_size;
            inf.isRunning = f->_isRunning;
            inf.name = f->name;
        }
        return inf;
    }
    exe_info getExecutableInfo(int pos)
    {
       exe_info inf;
        if(pos<0 or pos>=_scExecutables.size())
        {
                return inf;
        }
          Executable *f =&_scExecutables[pos];
        if (f != NULL)
        {
            inf.data_size = f->_executecmd.data_size;
            inf.binary_size = f->_executecmd.binary_size;
            inf.total_size = f->_executecmd.data_size + f->_executecmd.binary_size;
            inf.isRunning = f->_isRunning;
            inf.name = f->name;
        }
        return inf;
    }
    vector<Executable> _scExecutables;
};
_ScriptRuntime scriptRuntime;

#endif
