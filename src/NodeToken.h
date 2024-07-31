#pragma once

#include <string>
#include <vector>

using namespace std;

#include "string_function.h"

#include "tokenizer.h"

#define _STACK_SIZE 32

string __globalscript;
uint32_t __startmem;
uint32_t __maxMemUsage;
uint32_t __startStackMemory;
uint32_t __MaxStackMemory;

int stack_size = 0;

int for_if_num = 0;

int block_statement_num = 0;

int nb_argument = 0;

int local_var_num = 0;

int __sav_arg = 0;
bool _asPointer = false;

list<int> nb_args;

list<int> nb_sav_args;

list<string> _header;

list<string> _content;

list<string> _target_stack;

list<token> _other_tokens;

list<varTypeEnum> _types;
vector<Token> sav_t;

int point_regnum = 4;

bool isExternal = false;

bool isPointer = true;

bool isASM = false;
bool safeMode = false;
bool saveReg = false;
bool saveRegAbs = false;
list<int> _register_numl;

list<int> _register_numr;

list<int> _sp;

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
    printf("We satrt with: %ld free and stack:%ld \n", __startmem, __startStackMemory);
#endif
}
void updateMem()
{
#ifndef __COMPILER_TEST
    uint32_t newdelta = __startmem - esp_get_free_heap_size();
    if (newdelta > __maxMemUsage)
        __maxMemUsage = newdelta;
    TaskHandle_t loopTaskHandle = xTaskGetCurrentTaskHandle();

    // Get the minimum stack size left for the loop task
    UBaseType_t stackHighWaterMark = uxTaskGetStackHighWaterMark(loopTaskHandle);

    // Print the remaining stack size in bytes

    newdelta = __startStackMemory - stackHighWaterMark * sizeof(StackType_t);
    if (newdelta > __MaxStackMemory)
        __MaxStackMemory = newdelta;
    printf("max memory: %ld mem and stack:%ld \n", __maxMemUsage, __MaxStackMemory);
#endif
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
    for (int i = 0; i < v->size; i++)
    {
        if (member.compare(v->membersNames[i]) == 0)
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
void _visitextDefFunctionNode(NodeToken *nd);
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
            if (nd.getVarType()->_varType == __uint32_t__ || nd.getVarType()->_varType == __float__)
            {
                if (stack_size % 4 != 0)
                    delta = nd.getVarType()->_varSize - stack_size % 4;
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
    NodeToken(string _target,nodeType tt)
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
    NodeToken *addChild(NodeToken j)
    {
        j.parent = this;
        children.push_back(j);
        return &(children[children.size() - 1]);
    }
    void clear()
    {
        for (NodeToken child : children)
        {
            child.clear();
        }
        children.clear();
        children.shrink_to_fit();
    }
    void replaceTargetText(string t)
    {
        all_targets.replaceText(textref, t);
    }
    NodeToken *getChildAtPos(int pos)
    {
        if (pos >= 0 and pos < children.size())
            return &(children[pos]);
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
        if (type == TokenUserDefinedVariable)
        {
            int k = findStruct(getTokenText());
            if (k > -1)
            {
                return &_userDefinedTypes[_vartype];
            }
            else
                return &_varTypes[_vartype];
        }
        else
            return &_varTypes[_vartype];
    }
    nodeType getNodeTokenType()
    {
        return (nodeType)_nodetype;
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

        case extDefFunctionNode:
            _visitextDefFunctionNode(this);
            break;

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
    vector<NodeToken> children;
    NodeToken *_link = NULL;
    NodeToken *parent = NULL;
    uint16_t _total_size = 1;
    uint16_t target = EOF_TEXTARRAY;
    uint16_t textref = EOF_TEXTARRAY;
    bool isPointer = false;
    bool asPointer = false;
    uint8_t _nodetype = (int)UnknownNode;
    uint8_t stack_pos = 0;
    uint8_t type = (int)TokenUnknown;
    uint8_t _vartype = EOF_VARTYPE;
};

void _visittypeNode(NodeToken *nd) {}
void _visitnumberNode(NodeToken *nd) {}
void _visitbinOpNode(NodeToken *nd) {}
void _visitunitaryOpNode(NodeToken *nd) {}
void _visitoperatorNode(NodeToken *nd) {}
void _visitProgram(NodeToken *nd) {}
void _visitglobalVariableNode(NodeToken *nd) {}
void _visitlocalVariableNode(NodeToken *nd) {}
void _visitblockStatementNode(NodeToken *nd) {}
void _visitdefFunctionNode(NodeToken *nd) {}
void _visitstatementNode(NodeToken *nd) {}
void _visitprogramNode(NodeToken *nd) {}
void _visitassignementNode(NodeToken *nd) {}
void _visitcomparatorNode(NodeToken *nd) {}
void _visitcallFunctionNode(NodeToken *nd) {}
void _visitforNode(NodeToken *nd) {}
void _visitargumentNode(NodeToken *nd) {}
void _visitextGlobalVariableNode(NodeToken *nd) {}
void _visitextDefFunctionNode(NodeToken *nd) {}
void _visitextCallFunctionNode(NodeToken *nd) {}
void _visitreturnArgumentNode(NodeToken *nd) {}
void _visitvariableDeclarationNode(NodeToken *nd) {}
void _visitdefExtFunctionNode(NodeToken *nd) {}
void _visitinputArgumentsNode(NodeToken *nd) {}
void _visitdefExtGlobalVariableNode(NodeToken *nd) {}
void _visitdefGlobalVariableNode(NodeToken *nd) {}
void _visitdefLocalVariableNode(NodeToken *nd) {}
void _visitstoreLocalVariableNode(NodeToken *nd) {}
void _visitstoreGlobalVariableNode(NodeToken *nd) {}
void _visitstoreExtGlocalVariableNode(NodeToken *nd) {}
void _visitifNode(NodeToken *nd) {}
void _visitelseNode(NodeToken *nd) {}
void _visitwhileNode(NodeToken *nd) {}
void _visitreturnNode(NodeToken *nd) {}
void _visitdefAsmFunctionNode(NodeToken *nd) {}
void _visitstringNode(NodeToken *nd) {}
void _visitchangeTypeNode(NodeToken *nd) {}
void _visitimportNode(NodeToken *nd) {}
void _visitcontinueNode(NodeToken *nd) {}
void _visitbreakNode(NodeToken *nd) {}
void _visitUnknownNode(NodeToken *nd) {}

Script main_script;
NodeToken program = NodeToken(programNode);
NodeToken *current_node;
NodeToken *search_result;
vector<NodeToken *> _functions;
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
        _functions.clear();
        _functions.shrink_to_fit();
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
        _functions.push_back(nd);
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
        for (vector<NodeToken *>::iterator it = _functions.begin(); it != _functions.end(); ++it)
        {
            if (strcmp((*it)->getTokenText(), t->getText()) == 0)
            {
                search_result = *it;
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
        // printf("lokking for variable |%s| dans %s  already %d variables defined \n", t->text.c_str(),name.c_str(),variables.size());
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
                // //printf("lokking for variable |%s| dans %s %d branches\n", t->text.c_str(),c_cntx->name.c_str(),c_cntx->variables.size());
                for (vector<NodeToken>::iterator it = c_cntx->variables.begin(); it != c_cntx->variables.end(); ++it)
                {
                    // //printf("is equalt to |%s|\n", (*it)._token->text.c_str());
                    if ((*it).getTokenText() != NULL)
                    {
                        //  ////printf("is equalt to |%s|\n", (*it)._token->text.c_str());
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

template <class T>
class Stack
{
public:
    Stack() {}
    void push(T a)
    {
        _stack.push_back(a);
    }
    T pop()
    {
        T sav = _stack.back();
        _stack.pop_back();
        return sav;
    }
    T get()
    {
        return _stack.back();
    }
    void duplicate()
    {
        _stack.push_back(_stack.back());
    }
    void swap()
    {
        T sav = pop();
        T sav2 = pop();
        push(sav);
        push(sav2);
    }
    void clear()
    {
        _stack.clear();
        _stack.shrink_to_fit();
    }
    vector<T> _stack;
};

Stack<NodeToken> nodeTokenList;
Stack<string> targetList;
list<NodeToken *> sav_token;
list<NodeToken> _node_token_stack;
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
    // printf("***************create cariavbla %d %s\n", isStore ,nd->_token->text.c_str());
    // NodeToken var = NodeToken(_var);
    // printf("%s %d\n",_var->text.c_str(),_asPointer);

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
    copyPrty(search_result, &v);
    v.asPointer = _asPointer;
    current_node = current_node->addChild(v);
}



NodeToken createNodeLocalVariableForCreation(NodeToken var, NodeToken nd)

{
    switch (var._nodetype)
    {
    case defGlobalVariableNode:
    {
        NodeToken v = NodeToken(var,defGlobalVariableNode);
        copyPrty(&nd, &v);
        return v;
    }
    break;
    case defLocalVariableNode:
    {
         NodeToken v = NodeToken(var,defLocalVariableNode);
        copyPrty(&nd, &v);
        return v;
    }
    break;
        break;
    default:
    {
        
        copyPrty(&nd, &var);
        NodeToken v = NodeToken(var,defLocalVariableNode);
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
        // printf("jkd\n");
        res = string(p->getTargetText());
    }
    return res;
}