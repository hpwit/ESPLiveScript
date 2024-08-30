#pragma once

#include <string>
#include <vector>

using namespace std;

#include "string_function.h"

#include "tokenizer.h"

void pushToConsole(string str, bool force)
{
#ifdef __CONSOLE_ESP32
    LedOS.pushToConsole(str, force);
#else
#ifndef __TEST_DEBUG
    Serial.printf("%s\r\n", str.c_str());
#else
    printf("%s\r\n", str.c_str());
#endif
#endif
}

void pushToConsole(string str)
{
    pushToConsole(str, false);
}

#define _START_2 32
#define _STACK_SIZE (_START_2 + 4 * 4)
string __globalscript;
uint32_t __startmem;
uint32_t __maxMemUsage;
uint32_t __startStackMemory;
uint32_t __MaxStackMemory;
uint32_t __endtime;
uint32_t __starttime;

int stack_size = 0;

int for_if_num = 0;

int block_statement_num = 0;

int nb_argument = 0;

int local_var_num = 0;

int __sav_arg = 0;
bool _asPointer = false;

list<int> nb_args;

list<int> nb_sav_args;

// list<string> _header;

// list<string> _content;

// list<string> _target_stack;

// list<token> _other_tokens;

// list<varTypeEnum> _types;
vector<Token> sav_t;

int point_regnum = 4;

bool isExternal = false;

bool isPointer = true;

bool isASM = false;
bool safeMode = false;
bool saveReg = false;
bool saveRegAbs = false;
// list<int> _register_numl;

// list<int> _register_numr;
Stack<int> register_numr = Stack<int>(15);
Stack<int> register_numl = Stack<int>(15);

// list<int> _sp;

list<int> _compare;

// token *__current;
int __sav_pos;

void initMem()
{
#ifndef __COMPILER_TEST
    __startmem = esp_get_free_heap_size();
    TaskHandle_t loopTaskHandle = xTaskGetCurrentTaskHandle();

    // Get the minimum stack size left for the loop task
    UBaseType_t stackHighWaterMark = uxTaskGetStackHighWaterMark(loopTaskHandle);
    __startStackMemory = stackHighWaterMark * sizeof(StackType_t);
    __maxMemUsage = 0;
    __MaxStackMemory = 0;
    __starttime = ESP.getCycleCount();
    __exe_size = 0;
    // printf("We satrt with: %ld free and stack:%ld  \n", __startmem, __startStackMemory);
#endif
}
void updateMem()
{
#ifndef __COMPILER_TEST

    if (esp_get_free_heap_size() > __startmem)
        __startmem = esp_get_free_heap_size();
    uint32_t newdelta = __startmem - esp_get_free_heap_size();
    if (newdelta > __maxMemUsage)
        __maxMemUsage = newdelta;
    TaskHandle_t loopTaskHandle = xTaskGetCurrentTaskHandle();

    // Get the minimum stack size left for the loop task
    UBaseType_t stackHighWaterMark = uxTaskGetStackHighWaterMark(loopTaskHandle);

    // Print the remaining stack size in bytes
    if (__startStackMemory - stackHighWaterMark * sizeof(StackType_t) < 0)
        __startStackMemory = stackHighWaterMark * sizeof(StackType_t);

    newdelta = __startStackMemory - stackHighWaterMark * sizeof(StackType_t);
    if (newdelta > __MaxStackMemory)
        __MaxStackMemory = newdelta;

    __endtime = ESP.getCycleCount();

    // printf("max memory: %ld mem and stack:%ld free mem:%ld\n", __maxMemUsage, __MaxStackMemory, esp_get_free_heap_size());
#endif
}
void displayStat()
{
    pushToConsole(string_format("max used memory: %ld maxstack:%ld  started %d free mem:%ld consumed %ld exe size:%d time:%dms", __maxMemUsage, __MaxStackMemory, __startmem, esp_get_free_heap_size(), __startmem - esp_get_free_heap_size(), __exe_size, (__endtime - __starttime) / 240000), true);
}
string _numToBytes(uint32_t __num)
{
    string val = ".bytes 4";
    uint8_t c = __num & 0xff;
    val = val + " " + string_format("%02x", c);
    // val=val+'A';
    __num = __num / 256;
    c = __num & 0xff;
    val = val + " " + string_format("%02x", c);
    // val=val+'A';
    __num = __num / 256;
    c = __num & 0xff;
    val = val + " " + string_format("%02x", c);
    // val=val+'A';
    __num = __num / 256;
    c = __num & 0xff;
    val = val + " " + string_format("%02x", c);
    return val;
}
enum nodeType

{
    typeNode,
    numberNode,
    binOpNode,
    unitaryOpNode,
    operatorNode,
    globalVariableNode,
    localVariableNode,
    blockStatementNode,
    defFunctionNode,
    statementNode,
    programNode,
    assignementNode,
    comparatorNode,
    callFunctionNode,
    forNode,
    argumentNode,
    extGlobalVariableNode,
    extDefFunctionNode,
    extCallFunctionNode,
    returnArgumentNode,
    variableDeclarationNode,
    defExtFunctionNode,
    inputArgumentsNode,
    defExtGlobalVariableNode,
    defGlobalVariableNode,
    defLocalVariableNode,
    storeLocalVariableNode,
    storeGlobalVariableNode,
    storeExtGlocalVariableNode,
    ifNode,
    elseNode,
    whileNode,
    returnNode,
    defAsmFunctionNode,
    stringNode,
    changeTypeNode,
    importNode,
    continueNode,
    breakNode,
    UnknownNode

};

string nodeTypeNames[] =
    {

#ifdef __TEST_DEBUG
        "typeNode",
        "numberNode",
        "binOpNode",
        "unitaryOpNode",
        "operatorNode",
        "globalVariableNode",
        "localVariableNode",
        "blockStatementNode",
        "defFunctionNode",
        "statementNode",
        "programNode",
        "assignementNode",
        "comparatorNode",
        "callFunctionNode",
        "forNode",
        "argumentNode",
        "extGlobalVariableNode",
        "extDefFunctionNode",
        "extCallFunctionNode",
        "returnArgumentNode",
        "variableDeclarationNode",
        "defExtFunctionNode",
        "inputArgumentsNode",
        "defExtGlobalVariableNode",
        "defGlobalVariableNode",
        "defLocalVariableNode",
        "storeLocalVariableNode",
        "storeGlobalVariableNode",
        "storeExtGlocalVariableNode",
        "ifNode",
        "elseNode",
        "whileNode",
        "returnNode",
        "defAsmFunctionNode",
        "stringNode",
        "changeTypeNode",
        "importNode",
        "continueNode",
        "breakNode",
        "UnknownNode"

#endif

};

typedef struct

{
    int error;
    string error_message;

} f_error_struct;

int findMember(varType *v, string member)
{
    printf("zerk %d %s\n", v->size, v->varName.c_str());
    for (int i = 0; i < v->size; i++)
    {
        printf("look for %s %s\n", member.c_str(), v->membersNames[i].c_str());
        if (v->membersNames[i].compare(member) == 0)
        {
            return i;
        }
    }
    return -1;
}
int findMember(uint8_t _v, string member)
{

    varType *v = &_userDefinedTypes[_v];
    // printf("zerk dfin %d %s\n",v->size,v->varName.c_str());
    for (int i = 0; i < v->size; i++)
    {
        //      printf("look for %s %s\n",member.c_str(),v->membersNames[i].c_str());
        if (v->membersNames[i].compare(member) == 0)
        {
            return i;
        }
    }
    return -1;
}

int findStruct(char *structName)
{
    for (int i = 0; i < _userDefinedTypes.size(); i++)
    {
        if (strcmp(_userDefinedTypes[i].varName.c_str(), structName) == 0)
        {
            return i;
        }
    }
    return -1;
}
// class Context;

uint16_t stringToInt(char *str)

{
    uint16_t res;
    int i = 0;
    while (str[i] != 0)
    {
        res = 10 * res + (str[i] - 48);
        i++;
    }
    return res;
}

Text all_targets;
Text content;
Text header;

class NodeToken;

void _visittypeNode(NodeToken *nd);
void _visitnumberNode(NodeToken *nd);
void _visitbinOpNode(NodeToken *nd);
void _visitunitaryOpNode(NodeToken *nd);
void _visitoperatorNode(NodeToken *nd);
void _visitglobalVariableNode(NodeToken *nd);
void _visitlocalVariableNode(NodeToken *nd);
void _visitblockStatementNode(NodeToken *nd);
void _visitdefFunctionNode(NodeToken *nd);
void _visitstatementNode(NodeToken *nd);
void _visitprogramNode(NodeToken *nd);
void _visitassignementNode(NodeToken *nd);
void _visitcomparatorNode(NodeToken *nd);
void _visitcallFunctionNode(NodeToken *nd);
void _visitforNode(NodeToken *nd);
void _visitargumentNode(NodeToken *nd);
void _visitextGlobalVariableNode(NodeToken *nd);
// void _visitextDefFunctionNode(NodeToken *nd);
void _visitextCallFunctionNode(NodeToken *nd);
void _visitreturnArgumentNode(NodeToken *nd);
void _visitvariableDeclarationNode(NodeToken *nd);
void _visitdefExtFunctionNode(NodeToken *nd);
void _visitinputArgumentsNode(NodeToken *nd);
void _visitdefExtGlobalVariableNode(NodeToken *nd);
void _visitdefGlobalVariableNode(NodeToken *nd);
void _visitdefLocalVariableNode(NodeToken *nd);
void _visitstoreLocalVariableNode(NodeToken *nd);
void _visitstoreGlobalVariableNode(NodeToken *nd);
void _visitstoreExtGlocalVariableNode(NodeToken *nd);
void _visitifNode(NodeToken *nd);
void _visitelseNode(NodeToken *nd);
void _visitwhileNode(NodeToken *nd);
void _visitreturnNode(NodeToken *nd);
void _visitdefAsmFunctionNode(NodeToken *nd);
void _visitstringNode(NodeToken *nd);
void _visitchangeTypeNode(NodeToken *nd);
void _visitimportNode(NodeToken *nd);
void _visitcontinueNode(NodeToken *nd);
void _visitbreakNode(NodeToken *nd);
void _visitUnknownNode(NodeToken *nd);

class NodeToken
{

public:
    NodeToken() {}
    NodeToken(nodeType tt)
    {
        _nodetype = tt;
    }
    NodeToken(Token *t, nodeType tt)
    {
        type = t->type;
        textref = t->textref;
        _vartype = t->_vartype;
        _nodetype = tt;
    }
    NodeToken(Token *t)
    {
        type = t->type;
        textref = t->textref;
        _vartype = t->_vartype;
    }
    NodeToken(Token t, nodeType tt)
    {
        type = t.type;
        textref = t.textref;
        _vartype = t._vartype;
        _nodetype = tt;
    }
    NodeToken(NodeToken nd, nodeType tt)
    {

        type = nd.type;
        textref = nd.textref;
        _vartype = nd._vartype;
        _nodetype = tt;
        isPointer = nd.isPointer;
        _total_size = nd._total_size;
        target = nd.target;

        if (tt == defLocalVariableNode)
        {
            isPointer = nd.isPointer;
            _total_size = nd._total_size;
            // visitNode=visitNodeDefLocalVariable;
            int delta = 0;
            if (nd.isPointer)
            {
                if (stack_size % 4 != 0)
                    delta = 4 - stack_size % 4;
            }
            if (nd.getVarType()->_varType == __uint32_t__ || nd.getVarType()->_varType == __float__ || nd.getVarType()->_varType == __CRGB__)
            {
                if (stack_size % 4 != 0)
                {
                    if (nd.getVarType()->_varSize % 2 == 0)
                        delta = nd.getVarType()->_varSize - stack_size % 4;
                    else
                        delta = nd.getVarType()->_varSize - stack_size % 4 + 1;
                }
            }
            else if (nd.getVarType()->_varType == __uint16_t__ || nd.getVarType()->_varType == __int__)
            {
                if (stack_size % 2 != 0)
                {
                    delta = 1;
                }
            }
            stack_size += delta;
            nd.stack_pos = stack_size;
            if (nd.isPointer)
            {
                stack_size += 4;
            }
            else
            {
                stack_size += nd.getVarType()->_varSize;
            }

            stack_pos = nd.stack_pos;
        }
    }
    NodeToken(NodeToken *nd, nodeType tt)
    {

        type = nd->type;
        textref = nd->textref;
        _vartype = nd->_vartype;
        _nodetype = tt;
        isPointer = nd->isPointer;
        _total_size = nd->_total_size;
        target = nd->target;
    }
    NodeToken(NodeToken *nd)
    {

        type = nd->type;
        textref = nd->textref;
        _vartype = nd->_vartype;
        _nodetype = nd->_nodetype;
        isPointer = nd->isPointer;
        _total_size = nd->_total_size;
        target = nd->target;
    }
    NodeToken(string _target, nodeType tt)
    {
        _nodetype = tt;
        addTargetText(_target);
    }
    NodeToken(NodeToken nd, nodeType tt, string _target)
    {

        type = nd.type;
        textref = nd.textref;
        _vartype = nd._vartype;
        isPointer = nd.isPointer;
        _total_size = nd._total_size;
        _nodetype = tt;
        stack_pos = nd.stack_pos;
        addTargetText(_target);
    }
    NodeToken *addChild(NodeToken *j)
    {
        j->parent = this;
        children.push_back(j);
        return j;
    }
    void copyChildren(NodeToken *k)
    {
        for (NodeToken *lm : k->children)
        {
            addChild(lm);
        }
    }
    NodeToken *addChild(NodeToken j)
    {
        j.parent = this;
        NodeToken *tmp = (NodeToken *)malloc(sizeof(NodeToken));
        if (tmp == NULL)
        {
            // printf("impossinlme pour crear %s %lu\n",nodeTypeNames[j._nodetype].c_str(),sizeof(NodeToken));
            return NULL;
        }
        // printf("ok pour crear %s\n",nodeTypeNames[j._nodetype].c_str());
        memcpy(tmp, &j, sizeof(NodeToken));
        for (NodeToken *k : j.children)
        {
            // tmp-addChild(k);
        }

        // tmp->children.shrink_to_fit();
        // tmp->parent = this;
        children.push_back(tmp);
        return tmp;
    }
    void clear(bool all)
    {
        // printf("delete %s %d\r\n",getTokenText(),_nodetype);
        int i = 0;
        if (!all)
        {
            if (_nodetype == (int)callFunctionNode || _nodetype == (int)extCallFunctionNode || _nodetype == (int)defAsmFunctionNode || _nodetype == (int)defFunctionNode || _nodetype == (int)defExtFunctionNode)
            {
                // printf("on tente %s\r\n",getTokenText());
                i = 2;
            }
        }
        int j = 0;
        for (NodeToken *child : children)
        {

            // child->clear();
            if (j >= i)
            {

                child->clear();
                free(child);
                // children[j]=NULL;
            }
            j++;
        }

        children.clear();

        children.shrink_to_fit();
    }
    void clear()
    {
        clear(false);
    }
    void clearAll()
    {
        clear(true);
    }

    void replaceTargetText(string t)
    {
        all_targets.replaceText(textref, t);
    }
    NodeToken *getChildAtPos(int pos)
    {
        if (pos >= 0 and pos < children.size())
            return children[pos];
        else
            return NULL;
    }
    char *getTokenText()
    {
        return all_text.getText(textref);
    }
    void addTargetText(string t)
    {
        target = all_targets.addText(t);
    }
    char *getTargetText()
    {
        return all_targets.getText(target);
    }
    void setTargetText(string t)
    {
        target = all_targets.addText(t);
    }

    varType *getVarType()
    {
        if (_vartype == EOF_VARTYPE)

            return NULL;

        if (type == TokenUserDefinedVariable)
        {
            /// int k = findStruct(getTokenText());
            // if (k > -1)
            // {
            if (target != EOF_TEXTARRAY)
            {
                if(string(getTargetText()).compare(0,1,"@")==0)
                {
                     return &_userDefinedTypes[_vartype];
                }
                int i = findMember(_vartype, string(getTargetText()));
                if (i > -1)
                {
                    return &_varTypes[(_userDefinedTypes[_vartype]).types[i]];
                }
                else
                {
                    printf("member %s not foudn in %s\n", getTargetText(), _userDefinedTypes[_vartype].varName.c_str());
                    return NULL;
                }
            }

            return &_userDefinedTypes[_vartype];
            // }
            // else
            //    return &_varTypes[_vartype];
        }
        else
            return &_varTypes[_vartype];
    }
    nodeType getNodeTokenType()
    {
        return (nodeType)_nodetype;
    }
    int findMaxArgumentSize()
    {
        int cur_size = 0;
        if (_nodetype == extCallFunctionNode)
        {
            cur_size = getChildAtPos(1)->children.size();
            for (int i = 0; i < getChildAtPos(2)->children.size(); i++)
            {
                int cmp = getChildAtPos(2)->getChildAtPos(i)->findMaxArgumentSize();
                if (cmp > cur_size)
                    cur_size = cmp;
            }
        }
        else
        {
            if (children.size() > 0)
            {
                for (int i = 0; i < children.size(); i++)
                {
                    int cmp = getChildAtPos(i)->findMaxArgumentSize();
                    if (cmp > cur_size)
                        cur_size = cmp;
                }
            }
        }
        return cur_size;
    }
    void visitNode()
    {
        switch ((nodeType)_nodetype)
        {
        case typeNode:
            _visittypeNode(this);
            break;
        case numberNode:
            _visitnumberNode(this);
            break;

        case binOpNode:
            _visitbinOpNode(this);
            break;

        case unitaryOpNode:
            _visitunitaryOpNode(this);
            break;

        case operatorNode:
            _visitoperatorNode(this);
            break;

        case globalVariableNode:
            _visitglobalVariableNode(this);
            break;

        case localVariableNode:
            _visitlocalVariableNode(this);
            break;

        case blockStatementNode:
            _visitblockStatementNode(this);
            break;

        case defFunctionNode:
            _visitdefFunctionNode(this);
            break;

        case statementNode:
            _visitstatementNode(this);
            break;

        case programNode:
            _visitprogramNode(this);
            break;

        case assignementNode:
            _visitassignementNode(this);
            break;

        case comparatorNode:
            _visitcomparatorNode(this);
            break;

        case callFunctionNode:
            _visitcallFunctionNode(this);
            break;

        case forNode:
            _visitforNode(this);
            break;

        case argumentNode:
            _visitargumentNode(this);
            break;

        case extGlobalVariableNode:
            _visitextGlobalVariableNode(this);
            break;
            /*
                    case extDefFunctionNode:
                        _visitextDefFunctionNode(this);
                        break;
            */
        case extCallFunctionNode:
            _visitextCallFunctionNode(this);
            break;

        case returnArgumentNode:
            _visitreturnArgumentNode(this);
            break;

        case variableDeclarationNode:
            _visitvariableDeclarationNode(this);
            break;

        case defExtFunctionNode:
            _visitdefExtFunctionNode(this);
            break;

        case inputArgumentsNode:
            _visitinputArgumentsNode(this);
            break;

        case defExtGlobalVariableNode:
            _visitdefExtGlobalVariableNode(this);
            break;

        case defGlobalVariableNode:
            _visitdefGlobalVariableNode(this);
            break;

        case defLocalVariableNode:
            _visitdefLocalVariableNode(this);
            break;

        case storeLocalVariableNode:
            _visitstoreLocalVariableNode(this);
            break;

        case storeGlobalVariableNode:
            _visitstoreGlobalVariableNode(this);
            break;

        case storeExtGlocalVariableNode:
            _visitstoreExtGlocalVariableNode(this);
            break;

        case ifNode:
            _visitifNode(this);
            break;

        case elseNode:
            _visitelseNode(this);
            break;

        case whileNode:
            _visitwhileNode(this);
            break;

        case returnNode:
            _visitreturnNode(this);
            break;

        case defAsmFunctionNode:
            _visitdefAsmFunctionNode(this);
            break;

        case stringNode:
            _visitstringNode(this);
            break;

        case changeTypeNode:
            _visitchangeTypeNode(this);
            break;

        case importNode:
            _visitimportNode(this);
            break;

        case continueNode:
            _visitcontinueNode(this);
            break;

        case breakNode:
            _visitbreakNode(this);
            break;

        case UnknownNode:
            _visitUnknownNode(this);
            break;
        default:
            break;
        }
    }
    vector<NodeToken *> children;
    NodeToken *parent = NULL;
    uint16_t _total_size = 1;
    uint16_t target = EOF_TEXTARRAY;
    uint16_t textref = EOF_TEXTARRAY;
    uint16_t stack_pos = 0;
    bool isPointer = false;
    bool asPointer = false;
    uint8_t _nodetype = (int)UnknownNode;
   
    uint8_t type = (int)TokenUnknown;
    uint8_t _vartype = EOF_VARTYPE;
};

Script main_script;
NodeToken program = NodeToken(programNode);
NodeToken *current_node;
NodeToken *search_result;
vector<NodeToken> _functions;
class Context
{
public:
    Context() {}
    void clear()
    {
        variables.clear();
        variables.shrink_to_fit();
        for (Context child : children)
        {
            child.clear();
        }
        children.clear();
        children.shrink_to_fit();
    }
    Context *addChild(Context cntx)
    {
        cntx.parent = this;
        children.push_back(cntx);
        vector<Context>::iterator it = children.end();
        it--;
        return &*it;
    }
    Context *addChild()
    {
        // cntx.parent = this;
        children.push_back(Context());
        vector<Context>::iterator it = children.end();
        it--;
        (&*it)->parent = this;
        return &*it;
    }
    void addFunction(NodeToken *nd)
    {
        _functions.push_back(*nd);
    }
    void addVariable(NodeToken nd)
    {
        variables.push_back(nd);
    }
    void findFunction(Token *t)
    {
        // NodeTo

        search_result = NULL;
        if (t->getText() == NULL)
            return;
        for (vector<NodeToken>::iterator it = _functions.begin(); it != _functions.end(); ++it)
        {

            if (strcmp((*it).getTokenText(), t->getText()) == 0)
            {
                search_result = &*it;
                return;
            }
        }
        /*
        if (parent != NULL)
        {
            return parent->findFunction(t);
        }
       */
        search_result = NULL;
        return;
    }
    void findVariable(Token *t, bool isCreation)
    {
        search_result = NULL;

        if (t->getText() == NULL)
            return;
        // //printf("lokking for variable |%s| dans %s  already %d variables defined \n", t->text.c_str(),name.c_str(),variables.size());
        for (vector<NodeToken>::iterator it = variables.begin(); it != variables.end(); ++it)
        {

            if ((*it).getTokenText() != NULL)
            {
                if (strcmp((*it).getTokenText(), t->getText()) == 0)
                {
                    search_result = &*it;
                    return;
                }
            }
        }
        if (!isCreation)
        {
            Context *c_cntx = this->parent;
            while (c_cntx != NULL)
            {
                // ////printf("lokking for variable |%s| dans %s %d branches\n", t->text.c_str(),c_cntx->name.c_str(),c_cntx->variables.size());
                for (vector<NodeToken>::iterator it = c_cntx->variables.begin(); it != c_cntx->variables.end(); ++it)
                {
                    // ////printf("is equalt to |%s|\n", (*it)._token->text.c_str());
                    if ((*it).getTokenText() != NULL)
                    {
                        //  //////printf("is equalt to |%s|\n", (*it)._token->text.c_str());
                        if (strcmp((*it).getTokenText(), t->getText()) == 0)
                        {
                            search_result = &*it;
                            return;
                        }
                    }
                }
                c_cntx = c_cntx->parent;
            }
        }
        search_result = NULL;
        return;
    }
    Context *parent = NULL;
    vector<Context> children;
    vector<NodeToken> variables;
};
Context main_context;
Context *current_cntx;
f_error_struct Error;

Stack<NodeToken> nodeTokenList;
Stack<string> targetList;
list<NodeToken *> sav_token;
list<NodeToken *> _node_token_stack;
NodeToken _uniquesave;
void copyPrty(NodeToken *from, NodeToken *to)

{
    if (!to->isPointer)
        to->isPointer = from->isPointer;
    to->stack_pos = from->stack_pos;
    to->_vartype = from->_vartype;
    to->type = from->type;
    to->_total_size = to->_total_size * from->getVarType()->_varSize;
}

void createNodeVariable(Token *_var, bool isStore)

{
    // //printf("***************create cariavbla %d %s\n", isStore ,nd->_token->text.c_str());
    // NodeToken var = NodeToken(_var);
    // //printf("%s %d\n",_var->text.c_str(),_asPointer);

    NodeToken v = NodeToken(_var);
    switch (search_result->getNodeTokenType())
    {
    case extGlobalVariableNode:
    {
        if (isStore)
        {
            // NodeStoreExtGlobalVariable v = NodeStoreExtGlobalVariable(var);
            // NodeStoreExtGlobalVariable v = NodeStoreExtGlobalVariable(_var);
            v._nodetype = (int)storeExtGlocalVariableNode;
            // current_node->asPointer=asPointer;
            //  return;
        }
        else
        {
            // NodeExtGlobalVariable v = NodeExtGlobalVariable(_var);
            v._nodetype = (int)extGlobalVariableNode;

            // current_node->asPointer=asPointer;
            // return;
        }
    }
    break;
    case defExtGlobalVariableNode:
    {
        if (isStore)
        {
            // NodeStoreExtGlobalVariable v = NodeStoreExtGlobalVariable(_var);
            v._nodetype = (int)storeExtGlocalVariableNode;
            // current_node->asPointer=asPointer;
            // return;
        }
        else
        {
            // NodeExtGlobalVariable v = NodeExtGlobalVariable(_var);
            v._nodetype = (int)extGlobalVariableNode;
            // current_node->asPointer=asPointer;
            // return;
        }
    }
    break;
    case defLocalVariableNode:
    {
        if (isStore == true)
        {
            // NodeStoreLocalVariable v = NodeStoreLocalVariable(_var);
            v._nodetype = (int)storeLocalVariableNode;
            // current_node->asPointer=asPointer;
            // asPointer=false;
            // return;
        }
        else
        {
            // NodeLocalVariable v = NodeLocalVariable(_var);
            v._nodetype = (int)localVariableNode;
            // current_node->asPointer=asPointer;
            //  return;
        }
    }
    break;
    case localVariableNode:
    {
        if (isStore == true)
        {
            //  NodeStoreLocalVariable v = NodeStoreLocalVariable(_var);
            v._nodetype = (int)storeLocalVariableNode;
            // current_node->asPointer=asPointer;
            //            return;
        }
        else
        {
            // NodeLocalVariable v = NodeLocalVariable(_var);
            v._nodetype = (int)localVariableNode;
            // current_node->asPointer=asPointer;
            // §            return;
        }
    }
    break;
    case defGlobalVariableNode:
    {

        if (isStore)
        {
            // NodeStoreGlobalVariable v = NodeStoreGlobalVariable(_var);
            v._nodetype = (int)storeGlobalVariableNode;
            // v.addTargetText(search_result->getTargetText());
            // current_node->asPointer=asPointer;
            //  return;
        }
        else
        {
            // NodeGlobalVariable v = NodeGlobalVariable(_var);
            v._nodetype = (int)globalVariableNode;
            // current_node->asPointer=asPointer;
            //            return;
        }
    }
    break;
    case globalVariableNode:
    {
        if (isStore)
        {
            // NodeStoreGlobalVariable v = NodeStoreGlobalVariable(_var);
            v._nodetype = (int)storeGlobalVariableNode;
           //v.addTargetText(search_result->getTargetText());
            // current_node->asPointer=asPointer;
            //  return;
        }
        else
        {
            // NodeGlobalVariable v = NodeGlobalVariable(_var);
            v._nodetype = (int)globalVariableNode;
            // current_node->asPointer=asPointer;
            //            return;
        }
    }
    break;
    default:
    {
        if (isStore == true)
        {
            //  NodeStoreLocalVariable v = NodeStoreLocalVariable(_var);
            v._nodetype = (int)storeLocalVariableNode;
            // current_node->asPointer=asPointer;
            //            return;
        }
        else
        {
            // NodeLocalVariable v = NodeLocalVariable(_var);
            v._nodetype = (int)localVariableNode;
            // current_node->asPointer=asPointer;
            // §            return;
        }
    }
    break;
    }
    if(search_result->getTargetText()[0]=='@')
    {
     v.addTargetText(search_result->getTargetText());
    }
    copyPrty(search_result, &v);
    v.asPointer = _asPointer;
    if(isPointer)
         v.isPointer = isPointer;
    current_node = current_node->addChild(v);
}

NodeToken createNodeLocalVariableForCreation(NodeToken var, NodeToken nd)

{
    switch (var._nodetype)
    {
    case defGlobalVariableNode:
    {
        NodeToken v = NodeToken(var, defGlobalVariableNode);
        copyPrty(&nd, &v);
        return v;
    }
    break;
    case defLocalVariableNode:
    {
        NodeToken v = NodeToken(var, defLocalVariableNode);
        copyPrty(&nd, &v);
        return v;
    }
    break;
        break;
    default:
    {

        copyPrty(&nd, &var);
        NodeToken v = NodeToken(var, defLocalVariableNode);
        //  NodeDefLocalVariable v = NodeDefLocalVariable(var);

        return v;
    }
    break;
    }
}

string findForWhile()
{
    string res = "__";
    NodeToken *p = current_node;
    while (p->_nodetype != forNode and p->_nodetype != whileNode)
    {
        p = p->parent;
        if (p == NULL)
            break;
    }
    if (p != NULL)
    {
        // //printf("jkd\n");
        res = string(p->getTargetText());
    }
    return res;
}

void buildParents(NodeToken *nd)

{
    // return; //new
    if (nd->children.size() > 0)
    {
        for (int i = 0; i < nd->children.size(); i++)
        {
            (nd->children[i])->parent = nd;
            buildParents(nd->children[i]);
        }
    }
}

void translateType(varTypeEnum to, varTypeEnum from, int regnum)

{
    // printf("to:%s from: %s\n",varTypeEnumNames[to].c_str(),varTypeEnumNames[from].c_str());
    // if(to==__none__)
    //       to=__int__;
    if (to == __none__ or from == to)
        return;
    switch (to)
    {
    case __float__:
        // content.sp.pop();
        content.addAfterNoDouble(string_format("float.s f%d,a%d,0", regnum, regnum));
        // content.sp.push(content.get());
        break;
    case __int__:
        switch (from)
        {
        case __float__:
            //  content.sp.pop();
            content.addAfterNoDouble(string_format("trunc.s a%d,f%d,0", regnum, regnum));
            // content.sp.push(content.get());
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

string _data_sav;
Stack<varTypeEnum> globalType = Stack<varTypeEnum>(__int__);

void _visittypeNode(NodeToken *nd) {}
void _visitnumberNode(NodeToken *nd)
{
    // //printf("in number\n");

    // register_numl.duplicate();
    if (nd->children.size() > 0)
    {
        for (int i = 0; i < nd->children.size(); i++)
        {
            register_numl.duplicate();
            nd->getChildAtPos(i)->visitNode();
            // register_numl.pop();
            // content.sp.push(content.get());
            register_numl.pop();
        }
    }
    else
    {
        if (nd->getVarType()->_varType == __float__)
        {
            float __f = 0;
            sscanf(nd->getTokenText(), "%f", &__f);
            header.addAfter(string_format("@_%s_%d:", "local_var", local_var_num));
            // local_var_num++;
            string val = ".bytes 4";
            uint32_t __num = (uint32_t)(*((uint32_t *)&__f));
            // Serial.//printf(" on a float  %4x\r\n",__num);
            uint8_t c = __num & 0xff;
            val = val + " " + string_format("%02x", c);
            // val=val+'A';
            __num = __num / 256;
            c = __num & 0xff;
            val = val + " " + string_format("%02x", c);
            // val=val+'A';
            __num = __num / 256;
            c = __num & 0xff;
            val = val + " " + string_format("%02x", c);
            // val=val+'A';
            __num = __num / 256;
            c = __num & 0xff;
            val = val + " " + string_format("%02x", c);
            // val=val+'A';
            header.addAfter(val);
            // point_regnum++;
            content.addAfter(string_format("l32r a%d,@_%s_%d", 8, "local_var", local_var_num));
            content.addAfter(string_format("lsi f%d,a%d,0", register_numl.get(), 8));
            content.sp.push(content.get());
            // point_regnum--;
            local_var_num++;
            register_numl.decrease();
        }
        else
        {
            uint32_t __num = 0;
            if (string(nd->getTokenText()).find("x") != string::npos)
            {
                sscanf(nd->getTokenText(), "%x", &__num);
            }
            else
            {
                sscanf(nd->getTokenText(), "%d", &__num);
            }
            if (__num >= 2048)
            {
                header.addAfter(string_format("@_%s_%d:", "local_var", local_var_num));
                string val = ".bytes 4";
                uint8_t c = __num & 0xff;
                val = val + " " + string_format("%02x", c);
                // val=val+'A';
                __num = __num / 256;
                c = __num & 0xff;
                val = val + " " + string_format("%02x", c);
                // val=val+'A';
                __num = __num / 256;
                c = __num & 0xff;
                val = val + " " + string_format("%02x", c);
                // val=val+'A';
                __num = __num / 256;
                c = __num & 0xff;
                val = val + " " + string_format("%02x", c);
                // val=val+'A';
                header.addAfter(val);
                // point_regnum++;
                content.addAfter(string_format("l32r a%d,@_%s_%d", 8, "local_var", local_var_num));
                content.addAfter(string_format("l32i a%d,a%d,0", register_numl.get(), 8));
                content.sp.push(content.get());
                // point_regnum--;
                local_var_num++;
                register_numl.decrease();
            }
            else
            {
                content.addAfter(string_format("movi a%d,%d", register_numl.get(), __num)); // nd->_token->text.c_str()));
                content.sp.push(content.get());
                register_numl.decrease();
            }
        };
    }
}

void _visitbinOpNode(NodeToken *nd)
{

    // //printf("bin operator\n");
    // register_numl.displaystack();
    register_numl.duplicate();
    // register_numr.duplicate();
    // if (nd->getChildAtPos(0)->visitNode != NULL)
    nd->getChildAtPos(0)->visitNode();
    // register_numl.displaystack();
    register_numl.duplicate();
    // register_numr.duplicate();
    if (nd->getChildAtPos(1)->type != TokenPower)
    {
        // if (nd->getChildAtPos(2)->visitNode != NULL)
        nd->getChildAtPos(2)->visitNode();
    }
    else
    {
        content.addAfter("");
        content.sp.push(content.get());
    }
    // register_numr.pop();
    register_numl.swap();
    register_numr.push(register_numl.pop());
    register_numl.swap();
    // register_numl.displaystack();
    // if (nd->getChildAtPos(1)->visitNode != NULL)
    nd->getChildAtPos(1)->visitNode();
    register_numl.pop();
    if (nd->getChildAtPos(1)->type == TokenAddition || nd->getChildAtPos(1)->type == TokenSubstraction)
    {
        register_numl.increase();
    }
    // nex
    if (nd->getChildAtPos(1)->type == TokenSlash || nd->getChildAtPos(1)->type == TokenStar)
    {
        register_numl.pop();
        register_numl.push(register_numr.get());
    }
    // end new
    content.sp.pop();
    content.sp.pop();
    content.sp.push(content.get());
    // register_numl.pop();
    register_numr.pop();
}
void _visitunitaryOpNode(NodeToken *nd)
{
    // register_numl.displaystack();
    register_numl.duplicate();
    // register_numr.duplicate();
    if (nd->getChildAtPos(1)->type == TokenUppersand)
    {
        // //printf("node UNitary operator2\n");
        // nd->getChildAtPos(0)->asPointer = true;
        // addTokenSup(nd);
        // nd->_token->_varType = __none__;
        nd->_vartype = (int)__none__;
        nd->isPointer = true;
        nd->getChildAtPos(0)->visitNode();
        register_numl.pop();
        // content.sp.pop();
        content.sp.push(content.get());
        return;
    }
    else if (nd->getChildAtPos(1)->type == TokenSubstraction)
    {
        nd->getChildAtPos(1)->type = TokenNegation;

        nd->getChildAtPos(0)->visitNode();
        // register_numl.displaystack();
        register_numl.pop();
        // content.sp.push(content.get());

        nd->getChildAtPos(1)->visitNode();
        // register_numl.pop();
        register_numl.decrease();
    }
    else
    {
        // if (nd->getChildAtPos(0)->visitNode != NULL)
        nd->getChildAtPos(0)->visitNode();
        // register_numl.displaystack();
        register_numl.pop();

        nd->getChildAtPos(1)->visitNode();
        register_numl.decrease();
        content.sp.push(content.get());
    }
}

void _visitoperatorNode(NodeToken *nd)
{
    // printf("operator %s\n",tokenNames[nd->type].c_str());
    //  register_numl.pop();
    // register_numl.displaystack();
    varTypeEnum l = __none__;
    varTypeEnum r = __none__;
    // //printf("kk\n");

    if (nd->parent->getChildAtPos(0)->getVarType() != NULL)
    {
        l = nd->parent->getChildAtPos(0)->getVarType()->_varType;
    }

    // //printf("kk2 :%d\n",nd->parent->children.size());
    if (nd->parent->children.size() >= 3)
    {
        if (nd->parent->getChildAtPos(2) == NULL)
        {
            // printf("WFT %d %s\n",nd->parent->children.size(),nodeTypeNames[nd->parent->_nodetype].c_str());
        }

        if (nd->parent->getChildAtPos(2)->getVarType() != NULL)
        {
            // //printf("kk32 %s\n",nodeTypeNames[nd->parent->_nodetype].c_str());
            r = nd->parent->getChildAtPos(2)->getVarType()->_varType;
        }
    }
    // //printf("kk3\n");
    bool ff = false;
    if (nd->parent->getVarType() == NULL)
    {
        // addTokenSup(nd->parent);
        if (globalType.get() == __float__)
        {
            // nd->parent->_token->_varType = __float__;
            nd->parent->_vartype = (int)__float__;
        }
        else
        {
            // nd->parent->_token->_varType = __none__;
            nd->parent->_vartype = __none__;
        }
    }

    // //printf("kk4\n");
    if (globalType.get() == __float__)
    {
        ff = true;
        nd->parent->_vartype = __float__;
    }
    // //printf("kk5\n");
    asmInstruction asmInstr;
    if (nd->parent->children.size() >= 3)
        translateType(globalType.get(), r, register_numr.get());
    translateType(globalType.get(), l, register_numl.get());
    switch (nd->type)
    {
    case TokenAddition:
    {
          if (ff)
        {
            asmInstr = adds;
        }
        else
        {
            asmInstr = add;
        }
        string new_line=string_format("%s %s%d,%s%d,%s%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), register_numl.get(), getRegType(asmInstr, 2).c_str(), register_numr.get());
     
    string _last=content.back();
    string tocmp=string_format("movi a%d,",register_numr.get());
    //printf("to found %s\r\n",_last,tocmp);
    if(_last.compare(0,tocmp.size(),tocmp)==0)
    {
      //   printf("to found %s\r\n",_last,tocmp);
        int a,b;
        sscanf(_last.c_str(),"movi a%d,%d",&a,&b);
        if(b>=-128 and b<=127)
        {
            content.pop();
            content.addAfter(string_format("addi a%d,a%d,%d",register_numl.get(),register_numl.get(),b));
        }
        else
        {
            content.addAfter(new_line);
        }
    }
    else
    {
        content.addAfter(new_line);
    }

         // return;
    }
        break;
    case TokenShiftLeft:
        // content.addAfter("movi a8,32");
        // content.addAfter(string_format("sub a%d,a8,a%d",register_numr.get(),register_numr.get()).c_str());
        content.addAfter(string_format("ssl a%d", register_numr.get()).c_str());
        content.addAfter(string_format("sll a%d,a%d", register_numl.get(), register_numl.get()).c_str());
        break;
    case TokenShiftRight:
        // content.addAfter("movi a8,32");
        // content.addAfter(string_format("sub a%d,a8,a%d",register_numr.get(),register_numr.get()).c_str());
        content.addAfter(string_format("wsr a%d,3", register_numr.get()).c_str());
        content.addAfter(string_format("srl a%d,a%d", register_numl.get(), register_numl.get()).c_str());
        break;
    case TokenSubstraction:
        if (ff)
        {
            asmInstr = subs;
        }
        else
        {
            asmInstr = sub;
        }
        content.addAfter(string_format("%s %s%d,%s%d,%s%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), register_numl.get(), getRegType(asmInstr, 2).c_str(), register_numr.get()));
        // content.addAfter(string_format("sub a%d,a%d,a%d", register_numl.get(), register_numl.get(), register_numr.get()));
        // return;
        break;
    case TokenSlash:
        // content.addAfter(string_format("quou a%d,a%d,a%d", register_numl.get(), register_numl.get(), register_numr.get()));
        if (ff)
        {
            content.addAfter(string_format("mov.s f1,f%d", register_numl.get()));
            content.addAfter(string_format("mov.s f2,f%d", register_numr.get()));
            content.addAfter("call8  @___div");
            content.addAfter(string_format("mov.s f%d,f0", register_numl.get()));
        }
        else
        {
            asmInstr = quou;
            content.addAfter(string_format("%s %s%d,%s%d,%s%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), register_numl.get(), getRegType(asmInstr, 2).c_str(), register_numr.get()));
        }
        // return;
        break;
    case TokenStar:
        if (ff)
        {
            asmInstr = muls;
        }
        else
        {
            asmInstr = mull;
        }
        content.addAfter(string_format("%s %s%d,%s%d,%s%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), register_numl.get(), getRegType(asmInstr, 2).c_str(), register_numr.get()));
        // content.addAfter(string_format("mull a%d,a%d,a%d", register_numl.get(), register_numl.get(), register_numr.get()));
        // return;
        break;
    case TokenPlusPlus:
        if (nd->parent->getChildAtPos(0)->isPointer && nd->parent->getChildAtPos(0)->children.size() == 0)
        {
            content.addAfter(string_format("addi a%d,a%d,%d", register_numl.get(), register_numl.get(), nd->parent->getChildAtPos(0)->getVarType()->total_size));
        }
        else
        {
            content.addAfter(string_format("addi a%d,a%d,1", register_numl.get(), register_numl.get()));
        }
        // return;
        break;
    case TokenModulo:
        content.addAfter(string_format("remu a%d,a%d,a%d", register_numl.get(), register_numl.get(), register_numr.get()));
        //  return;
        break;
    case TokenPower:
    {
        // comment supprimer ce qu'il y a avant
        int __num = 0;
        if (nd->parent->getChildAtPos(2)->_nodetype == numberNode)
        {

            sscanf(nd->parent->getChildAtPos(2)->getTokenText(), "%d", &__num);

            if (ff)
            {
                content.addAfter(string_format("mov.s f10,f%d", register_numl.get()));
                asmInstr = muls;
            }
            else
            {
                content.addAfter(string_format("mov a10,a%d", register_numl.get()));
                asmInstr = mull;
            }
            for (int k = 1; k < __num; k++)
            {
                content.addAfter(string_format("%s %s%d,%s%d,%s10", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), register_numl.get(), getRegType(asmInstr, 2).c_str()));
            }
        }
    }
    break;
    case TokenKeywordAnd:
        content.addAfter(string_format("and a%d,a%d,a%d", register_numl.get(), register_numl.get(), register_numr.get()));
        break;
    case TokenKeywordOr:
        content.addAfter(string_format("or a%d,a%d,a%d", register_numl.get(), register_numl.get(), register_numr.get()));
        break;
    case TokenKeywordFabs:
        content.addAfter(string_format("abs.s f%d,f%d", register_numl.get(), register_numl.get()));
        break;
    case TokenKeywordAbs:
        content.addAfter(string_format("abs a%d,a%d", register_numl.get(), register_numl.get()));
        break;
    case TokenNegation:
        if (ff)
        {
            asmInstr = negs;
        }
        else
        {
            asmInstr = neg;
        }
        content.addAfter(string_format("%s %s%d,%s%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), register_numl.get()));
        content.sp.pop();
        content.sp.push(content.get());
        // return;
    default:
        // return;
        break;
    }
}
// void _visitProgram(NodeToken *nd) {}
void _visitglobalVariableNode(NodeToken *nd)
{

    // printf("comopiline glmobalvar  %s\n",nd->getTokenText());
    register_numl.duplicate();
    if (nd->children.size() > 0)
    {
        vector<string> tile;
        int nb=0;
        string sd=string(nd->getTargetText());
        if(sd.compare(0,1,"@")==0)
        {
            tile=split(sd," ");
            
            sscanf(tile[0].c_str(),"@%d",&nb);
            
        }
        if(nb>1)
        {
         content.addAfter("movi a11,1");
        }
        for(int i=0;i<nd->children.size();i++)
        {
        globalType.push(__int__);
        register_numl.duplicate();
        nd->getChildAtPos(i)->visitNode();
        register_numl.pop();
        if(nb>1)
        { 
        if(i<nd->children.size()-1)
        {
        
         content.addAfter(string_format("movi a10,%d",stringToInt((char*)tile[i+2].c_str())));
         content.addAfter(string_format("mull a11,a10,a11"));
         content.addAfter(string_format("mull a11,a%d,a11",register_numl.get()));
        }
        else
        {
            content.addAfter(string_format("add a%d,a11,a%d",register_numl.get(),register_numl.get()));
        }
        }
        
        
        globalType.pop();
        }
    }
    varType *v = nd->getVarType();
    int start = nd->stack_pos;
    uint8_t regnum = 1;
    if (nd->asPointer or (nd->isPointer)) // && nd->children.size() == 0))
        point_regnum++;
    if (nd->isPointer)
    {
        // start = nd->stack_pos;
        regnum = point_regnum;
    }
    string body = "";
    // register_numl++;
    // content.addAfter(string_format("l32r a%d,%s", point_regnum, nd->_token->text.c_str()));
    if (nd->isPointer && nd->children.size() > 0) // leds[g];
    {
        // f=f+number.f;
        if (v->total_size > 4)
        {
            // string tmp=content.l->back();
            // content.l->pop_back();
            content.addAfter(string_format("movi a%d,%d", point_regnum, v->total_size));
            content.addAfter(string_format("mull a%d,a%d,a%d", register_numl.get(), register_numl.get(), point_regnum));
            content.addAfter(string_format("l32r a%d,@_%s", point_regnum, nd->getTokenText()));
            content.addAfter(string_format("add a%d,a%d,a%d", point_regnum, point_regnum, register_numl.get()));
        }
        else
        {
            content.addAfter(string_format("l32r a%d,@_%s", point_regnum, nd->getTokenText()));
            for (int i = 0; i < v->total_size; i++)
            {
                content.addAfter(string_format("add a%d,a%d,a%d", point_regnum, point_regnum, register_numl.get()));
            }
        }
    }
    else
    {
        content.addAfter(string_format("l32r a%d,@_%s", point_regnum, nd->getTokenText()));
    }
    content.sp.push(content.get());
    if (nd->asPointer) //(&d)
    {
        content.addAfter(string_format("mov a%d,a%d", register_numl.get(), point_regnum));
        content.sp.push(content.get());
    }
    else if (nd->children.size() > 0 or !nd->isPointer) // leds[h] or h h being global)
    {
        // if (nd->target == EOF_TEXTARRAY)
        //  {
        for (int i = 0; i < v->size; i++)
        {
            // content.addAfter(string_format("%s %s%d,%s%d,%d", v->load[i].c_str(), v->reg_name.c_str(), register_numl.get(), v->reg_name.c_str(), point_regnum, start));
            asmInstruction asmInstr = v->load[i];
            content.addAfter(string_format("%s %s%d,%s%d,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), point_regnum, start));
            // register_numl--;
            start += v->sizes[i];
            content.sp.push(content.get());
        }
        // }
        /*
         else
         {

             int i = findMember(nd->_vartype, string(nd->getTargetText()));
             int pos = 0;
             v = &_userDefinedTypes[nd->_vartype];
             // printf(" we try to find %s %d\r\n", nd->getTargetText(), i);
             if (i > -1)
             {
                 // nd->getVarType() = &_varTypes[v->types[i]];
                 // nd->_vartype=(uint8_t)v->types[i];
                 start = nd->stack_pos + v->starts[i];
                 for (int h = 0; h < i; h++)
                 {
                     pos += v->memberSize[h];
                     // //printf("pos %d\r\n", pos);
                 }

                 for (int l = pos; l <= v->memberSize[i] - 1 + pos; l++)
                 {
                     // //printf("start gloabl %d\r\n", start);
                     content.addAfter(string_format("%s %s%d,%s%d,%d", asmInstructionsName[v->load[l]].c_str(), getRegType(v->load[l], 0).c_str(), register_numl.get(), getRegType(v->load[l], 1).c_str(), point_regnum, start));
                     start += v->sizes[l];
                     content.sp.push(content.get());
                 }
             }
             else
             {
                 return;
             }
         }
         */
        // if(v->size==1)
        // content.sp.pop();
    }
    else // s(leds)
    {
        content.addAfter(string_format("mov a%d,a%d", register_numl.get(), point_regnum));
        content.sp.push(content.get());
    }
    // res.f = f;
    // res.header = number.header + h;
    // point_regnum++;
    register_numl.pop();
    //    res.register_numl=register_numl;
    // res.register_numr=register_numr;
    register_numl.decrease();
    if (nd->asPointer or (nd->isPointer)) // && nd->children.size() == 0))
        point_regnum--;
    return;
}

void _visitlocalVariableNode(NodeToken *nd)
{
    // printf("in lcoall\n");

    if (nd->asPointer)
    {
        register_numl.duplicate();
        varType *v = nd->getVarType();
        int start = nd->stack_pos;
        // //printf("kzlekmze\n");
        // content.addAfter(string_format("l32r a%d,stack", point_regnum));

        //if( if (nd->isPointer && nd->children.size() > 0))

        if(nd->type==TokenUserDefinedVariableMember)
        {
             content.addAfter(string_format("addi a%d,a1,%d", register_numl.get(), start-(int)(start/1000)*1000));
             content.addAfter(string_format("l32i a%d,a%d,%d", register_numl.get(), register_numl.get(),(start/1000)));
        }
        else
        {
        content.addAfter(string_format("addi a%d,a1,%d", register_numl.get(), start));
        }
        register_numl.decrease();
        if (nd->isPointer && nd->children.size() > 0)
        {
            register_numl.duplicate();
            nd->getChildAtPos(0)->visitNode();
            register_numl.pop();
            for (int i = 0; i < v->total_size; i++)
            {
                content.addAfter(string_format("add a%d,a%d,a%d", register_numl.get() + 1, register_numl.get() + 1, register_numl.get()));
            }
            register_numl.increase();
        }
        content.sp.push(content.get());
        register_numl.pop();
        register_numl.decrease();
        // //printf("kzlekmze2\n");
        return;
    }
    register_numl.duplicate();
    // if (nd->children.size() > 0)
    //{
    //  number = nd->getChildAtPos(0)->visitNode(nd->getChildAtPos(0), register_numl, register_numr);
    // }

    if (nd->children.size() > 0)
    {
        globalType.push(__int__);
        register_numl.duplicate();
        nd->getChildAtPos(0)->visitNode();
        register_numl.pop();
        globalType.pop();
    }
    varType *v = nd->getVarType();
    int start = nd->stack_pos;
    uint8_t regnum = 1;
    if (nd->asPointer or (nd->isPointer && nd->children.size() == 0))
        point_regnum++;
    uint8_t save_reg;
    // point_regnum++;
    if (nd->isPointer)
    {
        // start = nd->stack_pos;
        regnum = point_regnum;
    }
    if (nd->isPointer)
    {
        int start = nd->stack_pos;

        // content.addAfter(string_format("addi a%d,a1,%d", point_regnum, start));

        if (nd->children.size() == 0)
        {
            content.addAfter(string_format("l32i a%d,a1,%d", register_numl.get(), start));
            // content.addAfter(string_format("mov a%d,a%d", register_numl.get(), point_regnum));
            content.sp.push(content.get());
        }
        else
        {
            content.addAfter(string_format("l32i a%d,a1,%d", point_regnum, start));
            start = 0;
            for (int i = 0; i < v->total_size; i++)
            {
                content.addAfter(string_format("add a%d,a%d,a%d", point_regnum, point_regnum, register_numl.get()));
            }
            content.sp.push(content.get());
            for (int i = 0; i < v->size; i++)
            {
                // content.addAfter(string_format("%s %s%d,%s%d,%d", v->load[i].c_str(), v->reg_name.c_str(), register_numl.get(), v->reg_name.c_str(), regnum, start));
                asmInstruction asmInstr = v->load[i];
                content.addAfter(string_format("%s %s%d,%s%d,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), point_regnum, start));
                translateType(globalType.get(), v->_varType, register_numl.get());
                // register_numl--;
                start += v->sizes[i];
                content.sp.push(content.get());
            }
        }
        if (nd->asPointer or (nd->isPointer && nd->children.size() == 0))
            point_regnum--;
        register_numl.pop();
        register_numl.decrease();
    }
    else
    {
        // printf("jj\n");
        for (int i = 0; i < v->size; i++)
        {
            // content.addAfter(string_format("%s %s%d,%s%d,%d", v->load[i].c_str(), v->reg_name.c_str(), register_numl.get(), v->reg_name.c_str(), regnum, start));
            asmInstruction asmInstr = v->load[i];
            content.addAfter(string_format("%s %s%d,%s%d,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), regnum, start));
            // printf("jj2\n");
            translateType(globalType.get(), v->_varType, register_numl.get());
            // printf("jj3\n");
            // register_numl--;
            start += v->sizes[i];
            content.sp.push(content.get());
            // printf("jj4\n");
        }
        register_numl.pop();
        // printf("jj5\n");
        register_numl.decrease();
        // printf("jj6\n");

        return;
    }
}

void _visitblockStatementNode(NodeToken *nd)
{
    register_numr.clear();
    register_numl.clear();
    register_numl.push(15);
    register_numr.push(15);

    register_numl.push(15);
    register_numr.push(15);
    for (int i = 0; i < nd->children.size(); i++)
    {

        nd->getChildAtPos(i)->visitNode();
        // f = f + g.f;
    }
    // nd->clear();
}
void _visitdefFunctionNode(NodeToken *nd)
{
    // printf("compiling %s\n",nd->getTokenText());
    header.addAfter(string_format(".global @_%s", nd->getTokenText()));
    header.addAfter(string_format("@_stack_%s:", nd->getTokenText()));
    header.addAfter(string_format(".bytes %d", (nd->getChildAtPos(1)->children.size() + 1) * 4));
    content.addAfter(string_format("@_%s:", nd->getTokenText()));
    content.addAfter(string_format("entry a1,%d", ((nd->stack_pos) / 8 + 1) * 8 + 16 + _STACK_SIZE)); // ((nd->stack_pos) / 8 + 1) * 8+20)
    if (saveReg)
    {
        content.addAfter("ssi f15,a1,16");
        content.addAfter("ssi f14,a1,20");
        content.addAfter("ssi f13,a1,24");
    }
    if (saveRegAbs)
    {
        content.addAfter("s32i a15,a1,16");
        content.addAfter("s32i a14,a1,20");
        content.addAfter("s32i a13,a1,24");
    }
    for (int i = 1; i < nd->children.size(); i++)
    {

        nd->getChildAtPos(i)->visitNode();
        // f = f + g.f;
        // h = h + g.header;
    }

    if (saveReg)
    {
        content.addAfter("lsi f15,a1,16");
        content.addAfter("lsi f14,a1,20");
        content.addAfter("lsi f13,a1,24");
    }
    if (saveRegAbs)
    {
        content.addAfter("l32i a15,a1,16");
        content.addAfter("l32i a14,a1,20");
        content.addAfter("l32i a13,a1,24");
    }
    content.addAfter(string_format("retw.n"));
}
void _visitstatementNode(NodeToken *nd)
{
    point_regnum = 4;
    // printf("visit statement\n");
    register_numr.clear();
    register_numl.clear();
    register_numl.push(15);
    register_numr.push(15);

    register_numl.push(15);
    register_numr.push(15);
    for (int i = 0; i < nd->children.size(); i++)
    {

        register_numl.duplicate();
        nd->getChildAtPos(i)->visitNode();
        register_numl.pop();
    }
    // nd->clear();
    // printf("end statement\n");
    //_node_token_stack.clear();
}
void _visitprogramNode(NodeToken *nd)
{
    // printf("visit program\n");
    point_regnum = 4;
    content.begin();
    header.begin();
    // header.addAfter("@_stack:");
    // header.addAfter(".bytes 60");
    header.addAfter("@_stackr:");
    header.addAfter(".bytes 16");

    // header.addAfter("__basetime:");
    // header.addAfter(".bytes 4");
    register_numr.clear();
    register_numl.clear();
    register_numl.push(15);
    register_numr.push(15);
    for (int i = 0; i < nd->children.size(); i++)
    {

#ifndef __MEM_PARSER
        if (nd->getChildAtPos(i)->_nodetype != defFunctionNode && nd->getChildAtPos(i)->_nodetype != defAsmFunctionNode)
        {
#endif
            nd->getChildAtPos(i)->visitNode();
#ifndef __MEM_PARSER
        } // NEW
#endif
    }
}
void _visitassignementNode(NodeToken *nd)
{
    // printf("entre assignemen\n") ;
    point_regnum = 4;
    content.sp.clear();
    content.sp.push(content.get());
    register_numl.duplicate();

    /*
      if (string(nd->getChildAtPos(0)->getTargetText()).size() > 0)
      {
          int i = findMember(nd->getChildAtPos(0)->getVarType(), string(nd->getChildAtPos(0)->getTargetText()));
          if (i > -1)
          {
              globalType.push(nd->getChildAtPos(0)->getVarType()->types[i]);
          }
      }
      else
      {*/
    // printf("on oass on push\n") ;
    globalType.push(nd->getChildAtPos(0)->getVarType()->_varType);
    // printf("retour on push\n") ;
    // }

    register_numl.duplicate();
    nd->getChildAtPos(1)->visitNode();
    register_numl.pop();
    if (nd->getChildAtPos(1)->getVarType() != NULL)
    {
        // printf("retour translate\n") ;
        /*        if (string(nd->getChildAtPos(1)->getTargetText()).size() > 0)
       {
           int i = findMember(nd->getChildAtPos(1)->getVarType(), string(nd->getChildAtPos(1)->getTargetText()));
           if (i > -1)
           {
              // globalType.push(nd->getChildAtPos(0)->getVarType()->types[i]);
               printf("retour translate2\n") ;
                translateType(globalType.get(), nd->getChildAtPos(1)->getVarType()->types[i], register_numl.get());
           }
       }
       else
       {*/
        // printf("on oass on push\n") ;
        // globalType.push(nd->getChildAtPos(0)->getVarType()->_varType);
        translateType(globalType.get(), nd->getChildAtPos(1)->getVarType()->_varType, register_numl.get());
        // printf("retour on push\n") ;
        // }
    }
    else
    {
        //   translateType(globalType.get(), nd->getChildAtPos(1)->_token->_varType, register_numl.get());
    }
    // printf("retour assignemen\n") ;
    // new
    content.sp.pop();
    content.sp.push(content.get());
    // end en

    // content.sp.displaystack("PILE");
    //  //printf("before store\n");
    point_regnum++;
    // register_numl.displaystack();
    //  register_numl.pop();
    //  register_numl.displaystack();

    register_numl.duplicate();
    nd->getChildAtPos(0)->visitNode();
    register_numl.pop();

    register_numl.pop();
    content.sp.pop();
    globalType.pop();
    // clearNodeToken(nd); // new
    // nd->clear();
    register_numl.clear();
    register_numl.push(15);
    register_numl.push(15);

    register_numr.clear();
    register_numr.push(15);
    register_numr.push(15);
}
void _visitcomparatorNode(NodeToken *nd)
{
    // printf("in comparator\n");
    int numl = register_numl.get();

    // register_numl.duplicate();
    nd->getChildAtPos(0)->visitNode();
    // register_numl.pop();

    int leftl = register_numl.get();

    // register_numl.duplicate();
    nd->getChildAtPos(1)->visitNode();
    // register_numl.pop();

    //////printf("compare %s %s\n",tokenNames[nd->_token->type ].c_str(),nd->_token->text.c_str());
    string compop = "";
    // to compose
    int h;
    if (nd->_total_size > 116)
    {
        switch (nd->type)
        {
        case TokenLessThan:
            compop = "blt"; // greater or equal
            //  content.addAfter( string_format("%s_end:\n",nd->target.c_str()));
            break;
        case TokenDoubleEqual:
            compop = "beq"; // not equal
            break;
        case TokenNotEqual:
            compop = "bne"; // equal
            break;
        case TokenMoreOrEqualThan:
            compop = "bge"; // less then
            break;
        case TokenMoreThan:
            compop = "blt"; // not equal
            h = numl;
            numl = leftl;
            leftl = h;
            break;
        case TokenLessOrEqualThan:
            compop = "bge"; // not equal
            h = numl;
            numl = leftl;
            leftl = h;
            break;
        default:
            break;
        }
        content.addAfter(string_format("%s a%d,a%d,%s_if", compop.c_str(), numl, leftl, nd->getTargetText()));
        content.addAfter(string_format("j %s_end", nd->getTargetText()));
        content.addAfter(string_format("%s_if:", nd->getTargetText()));
        register_numl.increase();
    }
    else
    {
        switch (nd->type)
        {
        case TokenLessThan:
            compop = "bge"; // greater or equal blt
            //  content.addAfter( string_format("%s_end:\n",nd->target.c_str()));
            break;
        case TokenDoubleEqual:
            compop = "bne"; // not equal beq
            break;
        case TokenNotEqual:
            compop = "beq"; // equal
            break;
        case TokenMoreOrEqualThan:
            compop = "blt"; // less then
            break;
        case TokenMoreThan:
            compop = "bge"; // not equal
            h = numl;
            numl = leftl;
            leftl = h;
            break;
        case TokenLessOrEqualThan:
            compop = "blt"; // not equal
            h = numl;
            numl = leftl;
            leftl = h;
            break;
        default:
            break;
        }
        content.addAfter(string_format("%s a%d,a%d,%s_end", compop.c_str(), numl, leftl, nd->getTargetText()));
        // content.addAfter(_compare.back()+1,string_format("j %s_end", nd->target.c_str()));
        // content.addAfter(_compare.back()+2,string_format("%s_if:", nd->target.c_str()));
        register_numl.increase();
    }
    // printf("out comparator\n");
}

void _visitcallFunctionNode(NodeToken *nd)
{
    // printf("compiling  call function %s\n",nd->getTokenText());
    NodeToken *t = nd; // cntx.findFunction(nd->_token);
                       //  printf("token type %d\r\n",nd->_link->getChildAtPos(0)->_token->_vartype->_varType);
    if (t == NULL)
    {
        return;
    }

    // if(t->getChildAtPos(1)->children.size()<1)
    // return;point_regnum
    int save = 9; // point_regnum;
                  // content.addAfterNoDouble(string_format("l32r a%d,@_stack_%s", save,nd->getTokenText())); // point_regnum
    for (int i = 0; i < t->getChildAtPos(1)->children.size(); i++)
    {
        // isPointer = false;
        if (t->getChildAtPos(2)->getChildAtPos(i)->isPointer)
        {
            // isPointer = true;
            register_numl.duplicate();
            nd->getChildAtPos(2)->getChildAtPos(i)->visitNode();
            register_numl.pop();
            int start = t->getChildAtPos(1)->getChildAtPos(i)->stack_pos - _STACK_SIZE;
            content.addAfter(content.sp.pop(), string_format("s32i a%d,a%d,%d", register_numl.get(), save, start)); // point_regnum
            content.addBefore(string_format("l32r a%d,@_stack_%s", save, nd->getTokenText()));                      // point_regnum
                                                                                                                    // isPointer=false;
        }
        else
        {

            globalType.push(t->getChildAtPos(1)->getChildAtPos(i)->getVarType()->_varType);
            register_numl.duplicate();
            nd->getChildAtPos(2)->getChildAtPos(i)->visitNode();
            register_numl.pop();
            int start = t->getChildAtPos(1)->getChildAtPos(i)->stack_pos - _STACK_SIZE + t->getChildAtPos(1)->getChildAtPos(i)->getVarType()->total_size;
            int tot = t->getChildAtPos(1)->getChildAtPos(i)->getVarType()->size - 1;
            for (int j = 0; j < t->getChildAtPos(1)->getChildAtPos(i)->getVarType()->size; j++)
            {
                if (nd->getChildAtPos(2)->getChildAtPos(i)->getVarType() != NULL)
                    translateType(globalType.get(), nd->getChildAtPos(2)->getChildAtPos(i)->getVarType()->_varType, register_numl.get());
                else
                {
                    //   translateType(globalType.get(), nd->getChildAtPos(0)->getChildAtPos(i)->_token->_varType, register_numl.get());
                }
                start -= t->getChildAtPos(1)->getChildAtPos(i)->getVarType()->sizes[tot - j];
                asmInstruction asmInstr = t->getChildAtPos(1)->getChildAtPos(i)->getVarType()->store[tot - j];
                // content.addAfter(content.sp.pop(), string_format("%s %s%d,%s%d,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), point_regnum, start));
                int sav;
                if (j == t->getChildAtPos(1)->getChildAtPos(i)->getVarType()->size - 1)
                    sav = content.sp.get();
                content.addAfter(content.sp.pop(), string_format("%s %s%d,%s%d,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), save, start));
                if (j == t->getChildAtPos(1)->getChildAtPos(i)->getVarType()->size - 1)
                {
                    // content.sp.push(content.get());

                    content.addAfter(sav, string_format("l32r a%d,@_stack_%s", save, nd->getTokenText())); // point_regnum
                }
                // start+=t->getChildAtPos(1)->getChildAtPos(i)->_token->_vartype->sizes[j];
            }
            globalType.pop();
        }
        // content.addAfter(string_format("addi a%d,a1,%d",11+i,t->getChildAtPos(1)->getChildAtPos(i)->stack_pos));
    }
    content.addAfter(string_format("mov a10,a2")); // neded to find the external variables !!!!!!
    content.addAfter(string_format("call8 @_%s", nd->getTokenText()));
    int start = nd->stack_pos;
    // printf("ini\r\n");
    varType *v = nd->getChildAtPos(0)->getVarType();
    // printf("ini\r\n");
    if (v == NULL)
    {
        printf("nodeToken %d\r\n", nd->getChildAtPos(0)->type);
        printf("NULL\r\n");
    }

    if (v->size > 0)
    {
        //   printf("ini size\r\n");
        content.addAfter(string_format("l32r a%d,@_stackr", 8)); // point_regnum
        for (int i = 0; i < v->size; i++)
        {
            // content.addAfter(string_format("mov a15,a10"));
            // content.addAfter(string_format("%s %s%d,%s%d,%d", v->load[i].c_str(), v->reg_name.c_str(), register_numl.get(), v->reg_name.c_str(), point_regnum, start));
            asmInstruction asmInstr = v->load[i];
            // printf("tryin to get %d %d\r\n",i,asmInstr);
            content.addAfter(string_format("%s %s%d,%s%d,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), 8, start)); // point_regnum
            // register_numl--;
            start += v->sizes[i];
            content.sp.push(content.get());
        }
        register_numl.decrease();
    }
}
void _visitforNode(NodeToken *nd)
{
    // printf("ente for\n") ;
    point_regnum = 4;

    register_numl.duplicate();
    nd->getChildAtPos(0)->visitNode();
    register_numl.pop();

    content.addAfter(string_format("%s:", nd->getTargetText()));
    _compare.push_back(content.get());

    register_numl.duplicate();
    nd->getChildAtPos(3)->visitNode();
    register_numl.pop();

    content.addAfter(string_format("%s_continue:", nd->getTargetText()));

    register_numl.duplicate();
    nd->getChildAtPos(2)->visitNode();
    register_numl.pop();

    int jumpsize = (content.get() - _compare.back()) * 3;
    nd->getChildAtPos(1)->_total_size = jumpsize;
    content.putIteratorAtPos(_compare.back());

    register_numl.duplicate();
    nd->getChildAtPos(1)->visitNode();
    register_numl.pop();

    _compare.pop_back();
    content.putIteratorAtPos(content.get());
    content.addAfter(string_format("j %s", nd->getTargetText()));
    content.addAfter(string_format("%s_end:", nd->getTargetText()));
    // clearNodeToken(nd);
    return;
}
void _visitargumentNode(NodeToken *nd) {}
void _visitextGlobalVariableNode(NodeToken *nd)
{
    register_numl.duplicate();
    if (nd->children.size() > 0)
    {
        register_numl.duplicate();
        nd->getChildAtPos(0)->visitNode();
        register_numl.pop();
    }
    varType *v = nd->getVarType();
    int start = nd->stack_pos;
    uint8_t regnum = 1;
    if (nd->isPointer)
    {
        // start = nd->stack_pos;
        regnum = point_regnum;
    }
    string body = "";
    // register_numl++;

    if (!nd->isPointer)
    {
        content.addAfter(string_format("movExt a%d,%s", point_regnum, nd->getTokenText()));
    }
    else
    {
        if (nd->children.size() > 0)
        {
            content.addAfter(string_format("movExt a%d,%s", point_regnum, nd->getTokenText()));
            // f=f+number.f;
            for (int i = 0; i < v->total_size; i++)
            {
                content.addAfter(string_format("add a%d,a%d,a%d", point_regnum, point_regnum, register_numl.get()));
            }
        }
        else
        {
            content.addAfter(string_format("movExt a%d,%s", register_numl.get(), nd->getTokenText()));
            content.sp.push(content.get());
            content.sp.push(content.get());
            register_numl.pop();
            register_numl.decrease();
            return;
        }
    }
    content.sp.push(content.get());
    for (int i = 0; i < v->size; i++)
    {
        // content.addAfter(string_format("%s %s%d,%s%d,%d", v->load[i].c_str(), v->reg_name.c_str(), register_numl.get(), v->reg_name.c_str(), point_regnum, start));
        asmInstruction asmInstr = v->load[i];
        content.addAfter(string_format("%s %s%d,%s%d,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), point_regnum, start));
        // register_numl--;
        start += v->sizes[i];
        content.sp.push(content.get());
    }
    // res.f = f;
    // res.header = number.header + h;
    // point_regnum++;
    register_numl.pop();
    register_numl.decrease();
    //    res.register_numl=register_numl;
    // res.register_numr=register_numr;
    return;
}
// void _visitextDefFunctionNode(NodeToken *nd){}

void _visitextCallFunctionNode(NodeToken *nd)
{

    bool saveinstack[5];
    for (int i = 0; i < 5; i++)
    {
        saveinstack[i] = false;
    }

    NodeToken *t = nd; // cntx.findFunction(nd->_token);
    if (t == NULL)
    {
        // globalType.pop();
        return;
    }
    // printf(" %s %d %d\n",nd->_token->text.c_str(),  t->children.size(),t->getChildAtPos(1)->children.size());
    // for (int i = 0; i < t->getChildAtPos(1)->children.size(); i++)

    // printf("number of arg %s %d\r\n", nd->getTokenText(), nd->findMaxArgumentSize());
    for (int i = t->getChildAtPos(1)->children.size() - 1; i >= 0; i--)
    {
        // printf("***number of arg %d %d\r\n", i, nd->getChildAtPos(2)->getChildAtPos(i)->findMaxArgumentSize());
        bool save_in_stack = false;
        for (int j = 0; j < i; j++)
        {
            if (nd->getChildAtPos(2)->getChildAtPos(j)->findMaxArgumentSize() - 1 >= i)
            {
                save_in_stack = true;
            }
        }
        if (i == 0)
            save_in_stack = false;
        saveinstack[i] = save_in_stack;
        register_numl.duplicate();
        globalType.push(t->getChildAtPos(1)->getChildAtPos(i)->getVarType()->_varType);
        nd->getChildAtPos(2)->getChildAtPos(i)->visitNode();
        register_numl.pop();
        if (nd->getChildAtPos(2)->getChildAtPos(i)->getVarType() != NULL)
            translateType(globalType.get(), nd->getChildAtPos(2)->getChildAtPos(i)->getVarType()->_varType, register_numl.get());
        else
        {
            // translateType(globalType.get(), nd->getChildAtPos(0)->getChildAtPos(i)->_token->_varType, register_numl.get());
        }
        // translateType(t->getChildAtPos(1)->getChildAtPos(i)->_token->_vartype->_varType,globalType.get(),register_numl.get());
        if (t->getChildAtPos(1)->getChildAtPos(i)->getVarType()->_varType == __float__)
        {
            content.addAfter(string_format("rfr a%d,f%d", 10 + i, register_numl.get()));
        }
        else if (t->getChildAtPos(2)->getChildAtPos(i)->getVarType()->_varType == __CRGB__)
        {
            // content.addAfter( content.sp.pop(),string_format("mov a%d,a%d", 10 + i, register_numl.get()));
            if (t->getChildAtPos(2)->getChildAtPos(i)->_nodetype == numberNode)
            {

                if (save_in_stack == true)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        content.addAfter(content.sp.pop(), string_format("s8i a%d,a1,%d", register_numl.get(), i * 4 + _START_2 + k));
                    }
                    // content.addAfter(string_format("l32i a%d,a1,%d",10+i,i * 4 + _START_2));
                }

                else
                {
                    for (int k = 2; k >= 0; k--)
                    {
                        content.addAfter(content.sp.pop(), string_format("s8i a%d,a1,%d", register_numl.get(), i * 4 + _START_2 + k));
                    }
                    content.addAfter(string_format("l32i a%d,a1,%d", 10 + i, i * 4 + _START_2));
                }
            }
            else
            {

                for (int k = 0; k < t->getChildAtPos(2)->getChildAtPos(i)->getVarType()->size; k++)
                {
                    // content.addAfter(string_format("mov a15,a10"));
                    // content.addAfter(content.sp.pop(),string_format("slli a%d,a%d,%d", 10+i,register_numl.get(),  k* 8));
                    // register_numl--;
                    content.pop();
                }
                if (t->getChildAtPos(2)->getChildAtPos(i)->_nodetype == callFunctionNode)
                {
                    if (save_in_stack == true)
                    {
                        content.addAfter(content.sp.pop(), string_format("l32i a%d,a8,0", 10 + i));
                        content.addAfter(string_format("s32i a%d,a1,%d", 10 + i, i * 4 + _START_2));
                    }
                    else
                    {
                        content.addAfter(content.sp.pop(), string_format("l32i a%d,a8,0", 10 + i));
                    }
                }
                if (t->getChildAtPos(2)->getChildAtPos(i)->_nodetype == extCallFunctionNode)
                {
                    if (save_in_stack == true)
                    {
                        // content.addAfter(content.sp.pop(), string_format("mov a%d,a10", 10 + i));
                        content.addAfter(string_format("s32i a10,a1,%d", i * 4 + _START_2));
                    }
                    else
                    {
                        content.addAfter(content.sp.pop(), string_format("mov a%d,a10", 10 + i));
                    }
                }
                else if (t->getChildAtPos(2)->getChildAtPos(i)->_nodetype == localVariableNode)
                {
                    if (save_in_stack == true)
                    {
                        content.addAfter(content.sp.pop(), string_format("l32i a%d,a1,%d", 10 + i, t->getChildAtPos(2)->getChildAtPos(i)->stack_pos));
                        content.addAfter(string_format("s32i a%d,a1,%d", 10 + i, i * 4 + _START_2));
                    }
                    else
                    {
                        content.addAfter(content.sp.pop(), string_format("l32i a%d,a1,%d", 10 + i, t->getChildAtPos(2)->getChildAtPos(i)->stack_pos));
                    }
                }
                else if (t->getChildAtPos(2)->getChildAtPos(i)->_nodetype == globalVariableNode)
                {
                    // tobe done
                    if (save_in_stack == true)
                    {
                        content.addAfter(content.sp.pop(), string_format("l32i a%d,a5,0", 10 + i));
                        content.addAfter(string_format("s32i a%d,a1,%d", 10 + i, i * 4 + _START_2));
                    }
                    else
                    {
                        content.addAfter(content.sp.pop(), string_format("l32i a%d,a5,0", 10 + i));
                    }
                }
            }
        }

        else
        {
            if (save_in_stack == true)
            {
                content.addAfter(string_format("s32i a%d,a1,%d", register_numl.get(), i * 4 + _START_2));
            }
            else
            {
                content.addAfter(string_format("mov a%d,a%d", 10 + i, register_numl.get()));
            }
        }

        globalType.pop();
    }
    for (int i = 0; i < 5; i++)
    {
        if (saveinstack[i] == true)
        {
            content.addAfter(string_format("l32i a%d,a1,%d", 10 + i, i * 4 + _START_2));
        }
    }
    content.addAfter(string_format("callExt a8,%s", nd->getTokenText()));
    content.sp.push(content.get());
    int start = nd->stack_pos;
    varType *v = t->getChildAtPos(0)->getVarType();
    if (v->_varType == __float__)
    {
        content.addAfter(string_format("wfr f%d,a10", register_numl.get()));

        content.sp.push(content.get());
        // globalType.push(__float__);
    }
    else
    {
        for (int i = 0; i < v->size; i++)
        {
            // content.addAfter(string_format("mov a15,a10"));
            content.addAfter(string_format("extui a%d,a10,%d,%d", register_numl.get(), start * 8, v->sizes[i] * 8));
            // register_numl--;
            start += v->sizes[i];
            content.sp.push(content.get());
        }
    }
    register_numl.decrease();
    return;
}

void _visitreturnArgumentNode(NodeToken *nd) {}
void _visitvariableDeclarationNode(NodeToken *nd) {}
void _visitdefExtFunctionNode(NodeToken *nd)
{
    // printf("visit externazl function %s\n", nd->getTokenText());
}
void _visitinputArgumentsNode(NodeToken *nd)
{

    if (nd->children.size() < 1)
        return;
    int sav = 9;                                                                                     // point_regnum;
    content.addAfterNoDouble(string_format("l32r a%d,@_stack_%s", sav, nd->parent->getTokenText())); // point_regnum
    for (int i = 0; i < nd->children.size(); i++)
    {
        int start = nd->getChildAtPos(i)->stack_pos;
        if (nd->getChildAtPos(i)->isPointer)
        {
            int start = nd->getChildAtPos(i)->stack_pos;
            content.addAfter(string_format("l32i a15,a%d,%d", sav, start - _STACK_SIZE)); // point reg_bnum
            content.addAfter(string_format("s32i a15,a1,%d", start));
        }
        else
        {
            for (int j = 0; j < nd->getChildAtPos(i)->getVarType()->size; j++)
            {
                asmInstruction asmInstr = nd->getChildAtPos(i)->getVarType()->load[0];
                content.addAfter(string_format("%s %s15,%s%d,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), getRegType(asmInstr, 1).c_str(), sav, start - _STACK_SIZE)); // point_regnum
                asmInstr = nd->getChildAtPos(i)->getVarType()->store[0];
                content.addAfter(string_format("%s %s15,%s1,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), getRegType(asmInstr, 1).c_str(), start));
                start += nd->getChildAtPos(i)->getVarType()->sizes[j];
            }
        }
    }
}
void _visitdefExtGlobalVariableNode(NodeToken *nd)
{
    if (safeMode)
    {
        if (nd->isPointer)
        {
            header.addAfter(string_format("@_size_%s:", nd->getTokenText()));
            header.addAfter(_numToBytes(nd->_total_size / nd->getVarType()->total_size).c_str());
        }
    }
}
void _visitdefGlobalVariableNode(NodeToken *nd)
{

    if (safeMode)
    {
        if (nd->isPointer)
        {
            header.addAfter(string_format("@_size_%s:", nd->getTokenText()));
            header.addAfter(_numToBytes(nd->_total_size / nd->getVarType()->total_size).c_str());
        }
    }
    header.addAfter(string_format("@_%s:", nd->getTokenText()));
    if (nd->children.size() == 0)
    {
        header.addAfter(string_format(".bytes %d", nd->_total_size));
    }
    else
    {
        _data_sav = "";
        if (nd->getVarType()->_varType == __CRGB__)
        {
            for (NodeToken *ndt : nd->children)
            {
                for (NodeToken *ndtc : ndt->children)
                {
                    int __num = 0;
                    sscanf(ndtc->getTokenText(), "%d", &__num);
                    _data_sav = _data_sav + " " + string_format("%02x", __num);
                }
            }
        }
        else if (nd->getVarType()->_varType == __char__)
        {
            string str = string(nd->getChildAtPos(0)->getTokenText());
            nd->_total_size = str.size() - 1; // we remove the size taken by " " adn we add 0 fo \0
            for (int i = 1; i < str.size() - 1; i++)
            {
                _data_sav = _data_sav + " " + string_format("%02x", str.c_str()[i]);
            }
            _data_sav = _data_sav + " 00";
        }
        else
        {
            uint32_t __num;
            __num = 0;
            uint8_t c;
            for (NodeToken *ndt : nd->children)
            {
                if (nd->getVarType()->_varType == __float__)
                {
                    float __f = 0;
                    sscanf(ndt->getTokenText(), "%f", &__f);
                    __num = (uint32_t)(*((uint32_t *)&__f));
                }
                else
                {
                    __num = 0;
                    sscanf(ndt->getTokenText(), "%d", &__num);
                }
                for (int i = 0; i < nd->getVarType()->total_size; i++)
                {
                    c = __num & 0xff;
                    _data_sav = _data_sav + " " + string_format("%02x", c);
                    // val=val+'A';
                    __num = __num / 256;
                }
            }
        }
        header.addAfter(string_format(".bytes %d%s", nd->_total_size, _data_sav.c_str()));
    }
}
void _visitdefLocalVariableNode(NodeToken *nd) {}

void _visitstoreLocalVariableNode(NodeToken *nd)
{
    // //printf("jjjkkj\n");
    varType *v = nd->getVarType();
    // globalType.push(__float__);//v->_varType);
    // //printf("ona stocké:%d: %s\n",globalType.get(),varTypeEnumNames[globalType.get()].c_str());
    uint8_t regnum = 1;
    if (nd->asPointer or (nd->isPointer && nd->children.size() == 0))
        point_regnum++;
    if (nd->isPointer)
    {
        // start = nd->stack_pos;
        regnum = point_regnum;
    }
    int start = nd->stack_pos;
    if (nd->isPointer)
    {
        start = 0;
    }
    int numl = register_numl.get();
    for (int h = 0; h < v->size - 1; h++)
    {
        start += v->sizes[h];
    }
    if (nd->children.size() > 0 or !nd->isPointer or nd->asPointer)
    {
        for (int i = v->size - 1; i >= 0; i--)
        {
            // //printf("jjjkkj: %d\n",i);
            // list<string>::iterator l=content.sp.pop();
            // content.addAfter(  content.sp.pop(),  string_format("%s %s%d,%s%d,%d", v->store[i].c_str(), v->reg_name.c_str(), numl, v->reg_name.c_str(), regnum, start));
            content.addAfter(content.sp.pop(), string_format("%s %s%d,%s%d,%d", asmInstructionsName[v->store[i]].c_str(), getRegType(v->store[i], 0).c_str(), numl, getRegType(v->store[i], 1).c_str(), regnum, start));
            // numl++;
            start -= v->sizes[i];
        }
    }
    else
    {
        content.addAfter(content.sp.pop(), string_format("s32i a%d,a%d,%d", numl, regnum, 0)); // start
        // content.addAfter(content.sp.pop(), string_format("s32i a%d,a%d,%d", numl, 1,nd->stack_pos));
    }
    if (nd->isPointer)
    {
        register_numl.duplicate();
        content.sp.push(content.get());
        content.sp.swap();
        content.putIteratorAtPos(content.sp.get());
        // content.sp.displaystack("PILE");
        if (nd->children.size() > 0)
        {
            globalType.push(__int__);
            register_numl.duplicate();
            nd->getChildAtPos(0)->visitNode();
            register_numl.pop();
            globalType.pop();
        }
        if (nd->children.size() > 0)
        {
            content.addAfter(string_format("l32i a%d,a1,%d", point_regnum, nd->stack_pos));
            for (int i = 0; i < v->total_size; i++)
            {
                content.addAfter(string_format("add a%d,a%d,a%d", point_regnum, point_regnum, register_numl.get()));
            }
        }
        else if (nd->asPointer)
        {
            if(nd->type== TokenUserDefinedVariableMember )
            {
                content.addAfter(string_format("l32i a%d,a1,%d", point_regnum,nd->stack_pos-(int)(nd->stack_pos/1000)*1000));
                 content.addAfter(string_format("addi a%d,a%d,%d", point_regnum, point_regnum,(int)(nd->stack_pos/1000)));
             
                         
            // content.addAfter(string_format("l32i a%d,a%d,%d", register_numl.get(), register_numl.get(),(start/1000)));
            }
            else

            content.addAfter(string_format("l32i a%d,a1,%d", point_regnum, nd->stack_pos));
        }
        else
        {
            // a remettre
            content.addAfter(string_format("addi a%d,a1,%d", point_regnum, nd->stack_pos));
        }
        content.end();
        content.sp.pop();
        // point_regnum++;
        register_numl.pop();
        // point_regnum--;
        if (nd->asPointer or (nd->isPointer && nd->children.size() == 0))
            point_regnum--;
    }
}
void _visitstoreGlobalVariableNode(NodeToken *nd)
{

    register_numl.duplicate();
    varType *v = nd->getVarType();
    int start = nd->stack_pos;
    uint8_t regnum = 1;
    // int savreg_num=point_regnum;
    if (nd->asPointer or (nd->isPointer)) // && nd->children.size() == 0))
        point_regnum++;
    int savreg_num = point_regnum;
    point_regnum = 3;
    if (nd->isPointer)
    {
        // start = nd->stack_pos;
        regnum = point_regnum;
    }
    string body = "";
    // register_numl++;
    for (int h = 0; h < v->size - 1; h++)
    {
        start += v->sizes[h];
    }
    if (nd->children.size() > 0 or !nd->isPointer)
    {
        //  if (nd->target == EOF_TEXTARRAY)
        // {
        for (int i = v->size - 1; i >= 0; i--)
        {
            // printf("here in store %d\r\n",i);
            // content.addAfter(content.sp.pop(), string_format("%s %s%d,%s%d,%d", v->store[i].c_str(), v->reg_name.c_str(), register_numl.get(), v->reg_name.c_str(), point_regnum, start));
            content.addAfter(content.sp.pop(), string_format("%s %s%d,%s%d,%d", asmInstructionsName[v->store[i]].c_str(), getRegType(v->store[i], 0).c_str(), register_numl.get(), getRegType(v->store[i], 1).c_str(), point_regnum, start));
            // register_numl--;
            // printf("here in store %d\r\n",i);
            start -= v->sizes[i - 1];
            // content.sp.push(content.get());
        }
        /*
    }
    else
    {

        int i = findMember(nd->_vartype, string(nd->getTargetText()));
        int pos = 0;
        // printf(" we try to find %s %d\r\n", nd->getTargetText(), i);
        if (i > -1)
        {
            v = &_userDefinedTypes[nd->_vartype];
            // nd->_vartype =v->types[i];
            start = nd->stack_pos + v->starts[i];
            for (int h = 0; h < i; h++)
            {
                pos += v->memberSize[h];
                // printf("pos %d\r\n", pos);
            }
            for (int h = 0; h < v->memberSize[i] - 1; h++)
            {
                // pos+=v->memberSize[i];
                start += v->sizes[h + pos];
            }
            for (int l = v->memberSize[i] - 1 + pos; l >= pos; l--)
            {
                // printf("start stire %d\r\n", start);
                content.addAfter(content.sp.pop(), string_format("%s %s%d,%s%d,%d", asmInstructionsName[v->store[l]].c_str(), getRegType(v->store[l], 0).c_str(), register_numl.get(), getRegType(v->store[l], 1).c_str(), point_regnum, start));
                start -= v->sizes[l];
            }
        }
        else
        {
            return;
        }
    }
    */
    }
    else
    {
        content.addAfter(content.sp.pop(), string_format("s32i a%d,a%d,%d", register_numl.get(), point_regnum, start));
    }
    // res.f = f;
    // res.header = number.header + h;
    content.sp.push(content.get());
    content.sp.swap();
    content.putIteratorAtPos(content.sp.get());

    if (nd->children.size() > 0)
    {
        vector<string> tile;
        int nb=0;
        string sd=string(nd->getTargetText());
        if(sd.compare(0,1,"@")==0)
        {
            tile=split(sd," ");
            
            sscanf(tile[0].c_str(),"@%d",&nb);
            
        }
        if(nb>1 and sd.compare(0,1,"@")==0)
        {
         content.addAfter("movi a11,1");
        }
        for(int i=0;i<nd->children.size();i++)
        {
        globalType.push(__int__);
        register_numl.duplicate();
        nd->getChildAtPos(i)->visitNode();
        register_numl.pop();
        if(nb>1)
        { 
        if(i<nd->children.size()-1)
        {
        
         content.addAfter(string_format("movi a10,%d",stringToInt((char*)tile[i+2].c_str())));
         content.addAfter(string_format("mull a11,a10,a11"));
         content.addAfter(string_format("mull a11,a%d,a11",register_numl.get()));
        }
        else
        {
            content.addAfter(string_format("add a%d,a11,a%d",register_numl.get(),register_numl.get()));
        }
        }
        
        
        globalType.pop();
        }
    }

    if (safeMode && nd->isPointer)
    {
        content.addAfter(string_format("l32r a%d,@_size_%s", point_regnum, nd->getTokenText()));
        content.addAfter(string_format("l32i a%d,a%d,0", point_regnum, point_regnum));
        content.addAfter(string_format("bge a%d,a%d,__test_safe_%d", point_regnum, register_numl.get(), for_if_num));
        content.addAfter(string_format("movi a10,%d", 0));
        content.addAfter(string_format("mov a11,a%d", point_regnum));
        content.addAfter(string_format("mov a12,a%d", register_numl.get()));
        content.addAfter("callExt a8,error");
        content.addAfter("retw.n");
        content.addAfter(string_format("__test_safe_%d:", for_if_num));
        for_if_num++;
    }

    if (nd->isPointer && nd->children.size() > 0)
    {
        if (v->total_size > 4)
        {
            // string tmp=content.l->back();
            // content.l->pop_back();
            content.addAfter(string_format("movi a%d,%d", point_regnum, v->total_size));
            content.addAfter(string_format("mull a%d,a%d,a%d", register_numl.get(), register_numl.get(), point_regnum));
            content.addAfter(string_format("l32r a%d,@_%s", point_regnum, nd->getTokenText()));
            content.addAfter(string_format("add a%d,a%d,a%d", point_regnum, point_regnum, register_numl.get()));
        }
        else
        {
            content.addAfter(string_format("l32r a%d,@_%s", point_regnum, nd->getTokenText()));
            for (int i = 0; i < v->total_size; i++)
            {
                content.addAfter(string_format("add a%d,a%d,a%d", point_regnum, point_regnum, register_numl.get()));
            }
        }
    }
    else
    {
        content.addAfter(string_format("l32r a%d,@_%s", point_regnum, nd->getTokenText()));
    }
    content.end();
    content.sp.pop();
    // if(!nd->isPointer)
    // point_regnum++;
    register_numl.pop();
    point_regnum = savreg_num;
    if (nd->asPointer or (nd->isPointer)) // && nd->children.size() == 0))
        point_regnum--;
    // point_regnum--;
    //    res.register_numl=register_numl;
    // res.register_numr=register_numr;

    return;
}

void _visitstoreExtGlocalVariableNode(NodeToken *nd)
{

    register_numl.duplicate();
    varType *v = nd->getVarType();
    int start = nd->stack_pos;
    uint8_t regnum = 1;
    if (nd->isPointer)
    {
        // start = nd->stack_pos;
        regnum = point_regnum;
    }
    string body = "";
    // register_numl++;
    for (int h = 0; h < v->size - 1; h++)
    {
        start += v->sizes[h];
    }
    if (nd->children.size() > 0 or !nd->isPointer or nd->asPointer)
    {
        for (int i = v->size - 1; i >= 0; i--)
        {
            content.addAfter(content.sp.pop(), string_format("%s %s%d,%s%d,%d", asmInstructionsName[v->store[i]].c_str(), getRegType(v->store[i], 0).c_str(), register_numl.get(), getRegType(v->store[i], 1).c_str(), point_regnum, start));
            // register_numl--;
            start -= v->sizes[i];
            // content.sp.push(content.get());
        }
    }
    else
    {
        content.addAfter(content.sp.pop(), string_format("s32i a%d,a%d,%d", register_numl.get(), point_regnum, start));
    }
    // res.f = f;
    // res.header = number.header + h;
    // content.sp.push(content.get());
    // content.sp.swap();
    // content.putIteratorAtPos(content.sp.get());
    // content.sp.displaystack("PILE");
    // content.addAfter(content.sp.l->front(),"");
    content.putIteratorAtPos(content.sp.front());
    if (nd->children.size() == 1)
    {
        register_numl.duplicate();
        nd->getChildAtPos(0)->visitNode();
        register_numl.pop();
        if (nd->getChildAtPos(0)->getVarType() != NULL)
        {
            translateType(__int__, nd->getChildAtPos(0)->getVarType()->_varType, register_numl.get());
        }
        else
        {
            // translateType(__int__, nd->getChildAtPos(0)->_token->_varType, register_numl.get());
        }
    }
    else if (nd->children.size() > 1)
    {
        for (int par = 0; par < nd->children.size(); par++)
        {
            register_numl.duplicate();
            nd->getChildAtPos(par)->visitNode();
            register_numl.pop();
            content.addAfter(string_format("mov a%d,a%d", 10 + par, register_numl.get()));
        }
        content.addAfter(string_format("callExt a%d,map%dD", register_numl.get(), nd->children.size()));
        content.addAfter(string_format("mov a%d,a10", register_numl.get()));
    }

    if (safeMode && nd->isPointer)
    {
        content.addAfter(string_format("l32r a%d,@_size_%s", regnum, nd->getTokenText()));
        content.addAfter(string_format("l32i a%d,a%d,0", regnum, regnum));
        content.addAfter(string_format("bge a%d,a%d,__test_safe_%d", regnum, register_numl.get(), for_if_num));
        content.addAfter(string_format("movi a10,%d", 0)); // nd->_token->line
        content.addAfter(string_format("mov a11,a%d", regnum));
        content.addAfter(string_format("mov a12,a%d", register_numl.get()));
        content.addAfter("callExt a8,error");
        content.addAfter("retw.n");
        content.addAfter(string_format("__test_safe_%d:", for_if_num));
        for_if_num++;
    }
    content.addAfter(string_format("movExt a%d,%s",
                                   point_regnum, nd->getTokenText()));
    if (nd->isPointer && nd->children.size() > 0)
    {
        // f=f+number.f;
        for (int i = 0; i < v->total_size; i++)
        {
            content.addAfter(string_format("add a%d,a%d,a%d", point_regnum, point_regnum, register_numl.get()));
        }
    }
    content.end();
    content.sp.pop();
    //;
    register_numl.pop();
    // point_regnum--;
    //    res.register_numl=register_numl;
    // res.register_numr=register_numr;
    return;
}

void _visitifNode(NodeToken *nd)
{
    // printf("oo\n");
    point_regnum = 4;
    _compare.push_back(content.get());
    // printf("oo1\n");
    //  content.addAfter(  string_format("%s:\n",nd->target.c_str()));

    register_numl.duplicate();
    // printf("oo2bis\n");
    nd->getChildAtPos(1)->visitNode();
    register_numl.pop();
    // printf("oo2\n");

    nd->getChildAtPos(0)->_total_size = (content.get() - _compare.back()) * 3; // on estime que toutes les instructions ont la meme taille
    content.addAfter(string_format("%s_end:", nd->getTargetText()));
    // printf("oo3\n");
    content.putIteratorAtPos(_compare.back());
    // printf("oo4\n");
    register_numl.duplicate();
    nd->getChildAtPos(0)->visitNode();
    register_numl.pop();
    // printf("oo5n");
    content.putIteratorAtPos(content.get());
    _compare.pop_back();
}

void _visitelseNode(NodeToken *nd)
{
    point_regnum = 4;
    content.addBefore(string_format("j %s", nd->getTargetText()));
    // register_numl.duplicate();

    register_numl.duplicate();
    nd->getChildAtPos(0)->visitNode();
    register_numl.pop();

    content.addAfter(string_format("%s:", nd->getTargetText()));
}

void _visitwhileNode(NodeToken *nd)
{
    point_regnum = 4;
    content.addAfter(string_format("%s_while:", nd->getTargetText()));
    content.addAfter(string_format("%s_continue:", nd->getTargetText()));
    _compare.push_back(content.get());

    // content.addAfter(  string_format("%s:\n",nd->target.c_str()));

    register_numl.duplicate();
    nd->getChildAtPos(1)->visitNode();
    register_numl.pop();

    content.addAfter(string_format("j %s_while", nd->getTargetText()));
    nd->getChildAtPos(0)->_total_size = (content.get() - _compare.back()) * 3; // on estime que toutes les instructions ont la meme taille

    content.addAfter(string_format("%s_end:", nd->getTargetText()));
    content.putIteratorAtPos(_compare.back());

    register_numl.duplicate();
    nd->getChildAtPos(0)->visitNode();
    register_numl.pop();

    content.putIteratorAtPos(content.get());
    _compare.pop_back();
}
void _visitreturnNode(NodeToken *nd)
{
    NodeToken *t = nd;
    while (t->_nodetype != defFunctionNode and t->parent != NULL)
    {
        t = t->parent;
    }
    t = t->getChildAtPos(0);
    content.addAfter(string_format("l32r a%d,@_stackr", 9)); // point_regnum
    for (int i = 0; i < nd->children.size(); i++)
    {
        globalType.push(t->getVarType()->_varType);
        register_numl.duplicate();
        nd->getChildAtPos(i)->visitNode();
        register_numl.pop();
        int start = nd->stack_pos + t->getVarType()->total_size;
        int tot = t->getVarType()->size - 1;
        for (int j = 0; j < t->getVarType()->size; j++)
        {
            start -= t->getVarType()->sizes[tot - j];
            asmInstruction asmInstr = t->getVarType()->store[tot - j];
            content.addAfter(content.sp.pop(), string_format("%s %s%d,%s%d,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), 9, start)); // point_regnum
        }
        globalType.pop();
    }

    if (saveReg)
    {
        content.addAfter("lsi f15,a1,16");
        content.addAfter("lsi f14,a1,20");
        content.addAfter("lsi f13,a1,24");
    }
    if (saveRegAbs)
    {
        content.addAfter("l32i a15,a1,16");
        content.addAfter("l32i a14,a1,20");
        content.addAfter("l32i a13,a1,24");
    }
    content.addAfter("retw.n");
}

void _visitdefAsmFunctionNode(NodeToken *nd)
{
    header.addAfter(string_format(".global @_%s", nd->getTokenText()));
    content.addAfter(string_format("@_%s:", nd->getTokenText()));
    header.addAfter(string_format("@_stack_%s:", nd->getTokenText()));
    header.addAfter(string_format(".bytes %d", (nd->getChildAtPos(1)->children.size() + 1) * 4));

    // content.addAfter(string_format("entry a1,%d", 80)); // ((nd->stack_pos) / 8 + 1) * 8)
    if (nd->children.size() >= 3)
    {
        nd->getChildAtPos(2)->visitNode();
    } // f = f + g.f;
      // h = h + g.header;
}

void _visitstringNode(NodeToken *nd)
{
    string target = string(nd->getTokenText());
    content.addAfter(string_format("%s", target.substr(1, target.size() - 2).c_str()));
}
void _visitchangeTypeNode(NodeToken *nd)
{

    globalType.push(nd->getVarType()->_varType);
    for (int i = 0; i < nd->children.size(); i++)
    {

        // register_numl.duplicate();
        nd->getChildAtPos(i)->visitNode();
        // f = f + g.f;
        // h = h + g.header;
        // register_numl.pop();
        if (nd->getChildAtPos(i)->getVarType() != NULL)
            translateType(globalType.get(), nd->getChildAtPos(i)->getVarType()->_varType, register_numl.get());
        else
        {
            // translateType(globalType.get(), nd->getChildAtPos(i)->_token->_varType, register_numl.get());
        }
    }
    // varTypeEnum s = globalType.pop();
    globalType.pop();
}
void _visitimportNode(NodeToken *nd) {}
void _visitcontinueNode(NodeToken *nd)
{
    content.addAfter(string_format("j %s_continue", nd->getTargetText()));
}
void _visitbreakNode(NodeToken *nd)
{

    content.addAfter(string_format("j %s_end", nd->getTargetText()));
}
void _visitUnknownNode(NodeToken *nd) {}

void optimize(Text *text)
{
    // return;
    int regnum;
    for (int regnum = 3; regnum < 11; regnum++)
    {
        // regnum=9;
        string str = "__";
        string registername = string_format("a%d", regnum);
      // printf("on test register name %s\r\n", registername.c_str());
        for (int i = 0; i < text->size(); i++)
        {
            string tmp = string((*text->getChildAtPos(i)));
            
            if (tmp.size() > 0)
            {
                if (tmp.compare(0, 2, "@_") == 0)
                {
                   // printf("restart \r\n");
                    str = "__";
                }
                else if (tmp.find(":") != -1)
                {
                    str = "__";
                }

                else
                {
                    vector<string> d = split(tmp, " ");
                    if (d[0].compare("call8") == 0 and regnum >= 8)
                    {
                        str = "_";
                    }
                    else if (d.size() > 1)
                    {
                        if (d[1].compare(0, registername.size(), registername) == 0)
                        {
                           // printf("on a %s %s\r\n", d[1].c_str(), registername.c_str());
                            // if (!(str.compare("__")== 0))
                            //{
                            if (str.compare(tmp) == 0)
                            {
                              //  printf("%s found \r\n", tmp.c_str());
                                text->replaceText(i, " ");
                                //")
                            }
                            //}
                            else
                            {
                                if ( d[0].compare("movi") == 0 or d[0].compare("l32i") == 0 or d[0].compare("l32r") == 0 or d[0].compare("l16i") == 0 or d[0].compare("l16ui") == 0 or d[0].compare("l8ui") == 0 or d[0].compare("movExt") == 0)
                                {
                                    str = tmp;
                                }
                                else
                                {
                                    str = "__";
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    
}