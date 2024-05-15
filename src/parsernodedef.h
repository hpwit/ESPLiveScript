
#pragma once
#include <string>
#include "string_function.h"
#include "tokenizer.h"

using namespace std;

/*
Helper functions
*/
// #include "string_function.h"
/*
fin helper fiunctiopns
*/
int stack_size = 0;
int for_if_num = 0;
int block_statement_num = 0;
int nb_argument = 0;
int local_var_num = 0;

list<int> nb_args;
list<int> nb_sav_args;
list<string> _header;
list<string> _content;
list<string> _target_stack;
list<token> _other_tokens;
list<varTypeEnum> _types;
int point_regnum = 4;
bool isExternal = false;
bool isPointer = true;
bool isASM = false;
list<int> _register_numl;
list<int> _register_numr;

list<int> _sp;

class StackVarEnumType
{
public:
    StackVarEnumType(list<varTypeEnum> *l2)
    {
        l = l2;
    }
    void push(varTypeEnum a)
    {
        l->push_back(a);
    }
    varTypeEnum pop()
    {
        if (l->size() < 1)
            return __int__;
        varTypeEnum sav = l->back();
        l->pop_back();
        return sav;
    }
    void clear()
    {
        l->clear();
    }
    varTypeEnum get()
    {
        return l->back();
    }
    void duplicate()
    {
        l->push_back(l->back());
    }
    list<varTypeEnum> *l;

    void swap()
    {
        varTypeEnum sav = pop();
        varTypeEnum sav2 = pop();
        push(sav);
        push(sav2);
    }
};
class StackString
{
public:
    StackString(list<string> *l2)
    {
        l = l2;
    }
    void push(string a)
    {
        l->push_back(a);
    }
    string pop()
    {
        string sav = l->back();
        l->pop_back();
        return sav;
    }
    void clear()
    {
        l->clear();
    }
    string get()
    {
        return l->back();
    }
    void duplicate()
    {
        l->push_back(l->back());
    }
    list<string> *l;

    void swap()
    {
        string sav = pop();
        string sav2 = pop();
        push(sav);
        push(sav2);
    }
};

class StackInt
{
public:
    StackInt(list<int> *l2)
    {
        l = l2;
    }
    void push(int a)
    {
        l->push_back(a);
    }
    int pop()
    {
        int sav = l->back();
        l->pop_back();
        return sav;
    }
    void clear()
    {
        l->clear();
    }
    int get()
    {
        return l->back();
    }
    void duplicate()
    {
        l->push_back(l->back());
    }
    list<int> *l;
    void displaystack()
    {
        int i = 1;
        // printf("****DISPLAY STACK******\n");
        for (int g : *l)
        {
            // printf("%2d:%3d\n",i,g);
            i++;
        }
        // printf("****DISPLAY STACK END******\n");
    }

    void displaystack(string s)
    {
        int i = 1;
        // printf("****DISPLAY STACK %s******\n",s.c_str());
        for (int g : *l)
        {
            // printf("%2d:%3d\n",i,g);
            i++;
        }
        // printf("****DISPLAY STACK END %s******\n",s.c_str());
    }
    void increase()
    {
        int sav = pop();
        push(sav + 1);
    }
    void decrease()
    {
        int sav = pop();
        push(sav - 1);
    }
    void swap()
    {
        int sav = pop();
        int sav2 = pop();
        push(sav);
        push(sav2);
    }
};

class IteratorString
{
public:
    IteratorString(list<string> *l2)
    {
        l = l2;
        position = 0;
    }
    void addAfter(int pos, string s)
    {
        list<string>::iterator __it = getChildAtPos(pos);
        // printf(" on recupere %d:%s\n",pos,(*__it).c_str());
        // if((*_it).compare(s)!=0)
        //{
        l->insert(next(__it), s);
        _it = getChildAtPos(position);
        position++;
        // }
    }
    void addAfter(string s)
    {

        _it = l->insert(next(_it), s);
        position++;
    }
    void addAfterNoDouble(string s)
    {
        string str;
        if (_it == l->end())
        {
            str = "";
        }
        else
        {
            str = (*_it);
        }

        if (str.compare(s) != 0)
        {
            _it = l->insert(next(_it), s);
            position++;
        }
    }
    void addBefore(string s)
    {
        // if((*_it).compare(s)!=0)
        //{
        _it = l->insert(_it, s);
        _it++;
        position++;
        //}
    }
    void addBefore(list<string>::iterator it, string s)
    {
        l->insert(it, s);
        _it++;
    }
    int get()
    {
        // printf("position %d: %s\n",position,(*_it).c_str());

        return position - 1;
    }
    list<string>::iterator getChildAtPos(int pos)
    {
        int i = 0;
        if (pos >= l->size() || pos < 0)
        {
            return l->end();
        }
        for (list<string>::iterator it = l->begin(); it != l->end(); it++)
        {
            if (i == pos)
            {

                return it;
            }
            i++;
        }
        return l->end();
    }
    void begin()
    {

        _it = l->begin();
        position = 0;
    }
    void putIteratorAtPos(int pos)
    {
        _it = getChildAtPos(pos);
    }
    void end()
    {
        _it = getChildAtPos(l->size() - 1);
    }
    list<string> *l;
    list<string>::iterator _it;
    list<string>::iterator parent;
    StackInt sp = StackInt(&_sp);

    int position;
};

IteratorString content = IteratorString(&_content);
IteratorString header = IteratorString(&_header);
StackInt register_numr = StackInt(&_register_numr);
StackInt register_numl = StackInt(&_register_numl);
StackString targetList = StackString(&_target_stack);
StackVarEnumType globalType = StackVarEnumType(&_types);

enum statementType
{
    statementAssignment,
    statementDeclaration,
    statementDeclarationAndAssignment,
    statementIf,
    statementElse,
    statementFor,
    statementWhile,
    statementCall,
};

string statementTypeNames[] =
    {
        "statementAssignment",
        "statementDeclaration",
        "statementDeclarationAndAssignment",
        "statementIf",
        "statementElse",
        "statementFor",
        "statementWhile",
        "statementCall",
};

enum nodeType
{
    typeNode,
    numberNode,
    binOpNode,
    unitaryOpNode,
    operatorNode,
    Program,
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

};

string nodeTypeNames[] =
    {
        "typeNode",
        "numberNode",
        "binOpNode",
        "unitaryOpNode",
        "operatorNode",
        "Program",
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
        "changeTypeNode"};

typedef struct
{
    int error;
    string error_message;

} f_error_struct;

typedef struct
{
    int register_numl;
    int register_numr;
    string f;
    string header;
    f_error_struct error;
    string body[10];
} prt_r;

// class Context;
uint16_t stringToInt(string str)
{
    uint16_t res;
    for (int i = 0; i < str.size(); i++)
    {
        res = 10 * res + (str[i] - 48);
    }
    return res;
}

class Context;

class NodeToken
{
public:
    NodeToken()
    {
        parent = NULL;
        _link = NULL;
        _token = NULL;
    }

    NodeToken(token *t)
    {
        _token = t;
    }
    NodeToken *addChild(NodeToken t)
    {
        t.parent = this;
        // t._global = _global;
        children.push_back(t);
        list<NodeToken>::iterator it = children.end();
        it--;

        NodeToken *g = &*it;
        /*
        if (g->children.size() > 0)
        {
            for (list<NodeToken>::iterator it2 = g->children.begin(); it2 != g->children.end(); ++it2)
            {
                (&*it2)->parent = g;
            }
        }*/

        return g; //&*it;
    }
    // virtual string value();

    nodeType _nodetype;
    list<NodeToken> children;
    token *_token;
    // statementType stmttype;
    void (*visitNode)(NodeToken *nd) = NULL;

    NodeToken *_link;
    NodeToken *parent;
    string target;
    //  Context *cntx;
    uint8_t stack_pos = 0;
    bool isPointer = false;
    bool asPointer = false;
    uint16_t _total_size = 1;
    NodeToken *getChildAtPos(int pos)
    {
        int i = 0;
        if (pos >= children.size() || pos < 0)
        {
            return NULL;
        }
        for (list<NodeToken>::iterator it = children.begin(); it != children.end(); ++it)
        {
            if (i == pos)
            {

                return &*it;
            }
            i++;
        }
        return NULL;
    }
};

list<NodeToken> _node_token_stack;
int ggggg = 0;
void addTokenSup(NodeToken *nd)
{
    token t;
    t._vartype = NULL;

    ///// t.text=string_format("rr%d",ggggg);
    _other_tokens.push_back(t);
    int i = 0;

    int pos = _other_tokens.size() - 1;
    if (pos < 0)
        return;
    for (list<token>::iterator it = _other_tokens.begin(); it != _other_tokens.end(); ++it)
    {
        if (i == pos)
        {

            nd->_token = &*it;
            return;
        }
        i++;
    }
}

void translateType(varTypeEnum to, varTypeEnum from, int regnum)
{
    // printf("to:%s from: %s\n",varTypeEnumNames[to].c_str(),varTypeEnumNames[from].c_str());

    if (to == __none__ or from == to)
        return;
    switch (to)
    {
    case __float__:
        content.sp.pop();
        content.addAfterNoDouble(string_format("float.s f%d,a%d,0", regnum, regnum));
        content.sp.push(content.get());
        break;
    case __int__:
        switch (from)
        {
        case __float__:
            content.sp.pop();
            content.addAfterNoDouble(string_format("trunc.s a%d,f%d,0", regnum, regnum));
            content.sp.push(content.get());
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

class StackNodeToken
{
public:
    StackNodeToken(list<NodeToken> *l2)
    {
        l = l2;
    }
    void push(NodeToken a)
    {
        l->push_back(a);
    }
    NodeToken pop()
    {
        NodeToken sav = l->back();
        l->pop_back();
        return sav;
    }
    void clear()
    {
        l->clear();
    }
    NodeToken get()
    {
        return l->back();
    }
    void duplicate()
    {
        l->push_back(l->back());
    }
    list<NodeToken> *l;

    void swap()
    {
        NodeToken sav = pop();
        NodeToken sav2 = pop();
        push(sav);
        push(sav2);
    }
};
StackNodeToken nodeTokenList = StackNodeToken(&_node_token_stack);
NodeToken *current_node;

NodeToken *search_result;

list<NodeToken *> sav_token;
void copyNodeToken(NodeToken from, NodeToken *to)
{
    to->_token = from._token;
}

list<NodeToken *> _functions;

class Context
{
public:
    Context()
    {
        name = " ";
    };
    list<NodeToken> functions;
    list<NodeToken> variables;
    // varTypeEnum current_varType;
    Context *parent = NULL;
    list<Context> children;
    string name;
    // Context *_global = NULL;

    Context *addChild(Context cntx)
    {

        cntx.parent = this;
        cntx.name = name + "_" + cntx.name;

        // cntx._global = _global;
        children.push_back(cntx);
        list<Context>::iterator it = children.end();
        it--;
        return &*it;
    }

    void display(string id)
    {
        // ////printf("%scontext:%s\n", id.c_str(), name.c_str());
        for (int i = 0; i < variables.size(); i++)
        {
            // ////printf("%s\t%s %s %d\n", id.c_str(), variables[i]._token.text.c_str(), nodeTypeNames[variables[i]._nodetype].c_str(), variables[i]._token._vartype.external_var);
        }
        for (int i = 0; i < children.size(); i++)
        {
            display(id + "   ");
        }
    }
    void addVariable(NodeToken nd)
    {
        nd._token->text = name + "_" + nd._token->text;
        ////printf("i have added %s \n", nd._token->text.c_str());
        variables.push_back(nd);
    }
    void addFunction(NodeToken *nd)
    {
        ////printf("adding function %s\n", nd._token->text.c_str());
        // nd._token->text = name + "_" + nd._token->text;
        ////printf("adding function %s\n", nd._token->text.c_str());
        _functions.push_back(nd);
    }
    void findVariable(token *t, bool isCreation)
    {
        search_result = NULL;
        // printf("lokking for variable |%s| dans %s  already %d variables defined \n", t->text.c_str(),name.c_str(),variables.size());
        for (list<NodeToken>::iterator it = variables.begin(); it != variables.end(); ++it)
        {
            // //printf("is equalt to |%s|\n", (*it)._token->text.c_str());
            if ((*it)._token != NULL)
            {
                //  ////printf("is equalt to |%s|\n", (*it)._token->text.c_str());
                if ((*it)._token->text.compare(name + "_" + t->text) == 0)
                {
                    // //printf("varianle %s found in %s\n", t->text.c_str(),name.c_str());
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
                for (list<NodeToken>::iterator it = c_cntx->variables.begin(); it != c_cntx->variables.end(); ++it)
                {
                    // //printf("is equalt to |%s|\n", (*it)._token->text.c_str());
                    if ((*it)._token != NULL)
                    {
                        //  ////printf("is equalt to |%s|\n", (*it)._token->text.c_str());
                        if ((*it)._token->text.compare(c_cntx->name + "_" + t->text) == 0)
                        {
                            // //printf("varianle %s found in %s\n", t->text.c_str(),c_cntx->name.c_str());
                            search_result = &*it;
                            return;
                        }
                    }
                }
                c_cntx = c_cntx->parent;
            }
        }
        /*
        if(!isCreation)
        {
                //printf("no variebale found\n");
        }
        else
        {
             //printf("no variebale found we will create\n");
        }*/
        search_result = NULL;
        return;
    }
    void findFunction(token *t)
    {

       // NodeTo
 search_result = NULL;
        for (list<NodeToken *>::iterator it = _functions.begin(); it != _functions.end(); ++it)
        {

            if ((*it)->_token->text.compare(t->text) == 0)
            {
                search_result= *it;
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
        return ;
    }
};
Context main_cntx = Context();
Context *current_cntx = &main_cntx;
typedef struct
{
    f_error_struct error;
    NodeToken _nd;
} resParse;

f_error_struct Error;

void copyPrty(NodeToken *from, NodeToken *to)
{
    if (!to->isPointer)
        to->isPointer = from->isPointer;
    to->stack_pos = from->stack_pos;
    to->_token->_vartype = from->_token->_vartype;
    // to->_token->text=from->_token->text;
    to->_total_size = to->_total_size * from->_token->_vartype->_varSize;
}

void _visitNodeProgramNode(NodeToken *nd)
{

    // printf("in programl\n");
    content.begin();
    header.begin();
    header.addAfter("stack:");
    header.addAfter(".bytes 60");
    header.addAfter("stackr:");
    header.addAfter(".bytes 60");
    register_numr.clear();
    register_numl.clear();
    register_numl.push(15);
    register_numr.push(15);
    for (int i = 0; i < nd->children.size(); i++)
    {
        if (nd->getChildAtPos(i)->visitNode != NULL)
        {
            nd->getChildAtPos(i)->visitNode(nd->getChildAtPos(i));
        }
    }
}

class NodeProgram : public NodeToken
{
public:
    NodeProgram()
    {
        _nodetype = programNode;
        _token = NULL;
        visitNode = _visitNodeProgramNode;
    }
};

NodeProgram program;

class NodeVariableDeclaration : public NodeToken
{
public:
    NodeVariableDeclaration()
    {
        _nodetype = variableDeclarationNode;
        _token = NULL;
    }
};



void _visitNodeExtGlobalVariable(NodeToken *nd)
{

    // int savreg = register_numl;
    register_numl.duplicate();
    if (nd->children.size() > 0)
    {
        register_numl.duplicate();
        nd->getChildAtPos(0)->visitNode(nd->getChildAtPos(0));
        register_numl.pop();
    }
    varType *v = nd->_token->_vartype;
    int start = nd->stack_pos;
    uint8_t regnum = 1;
    if (nd->isPointer)
    {
        // start = nd->stack_pos;
        regnum = point_regnum;
    }
    string body = "";

    // register_numl++;
    content.addAfter(string_format("movExt a%d,%s", point_regnum, nd->_token->text.c_str()));

    if (nd->isPointer)
    {
        // f=f+number.f;
        for (int i = 0; i < v->total_size; i++)
        {
            content.addAfter(string_format("add a%d,a%d,a%d", point_regnum, point_regnum, register_numl.get()));
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
    //    res.register_numl=register_numl;
    // res.register_numr=register_numr;
    return;
}

class NodeExtGlobalVariable : public NodeToken
{
public:
    NodeExtGlobalVariable()
    {
        _nodetype = extGlobalVariableNode;
        _token = NULL;
        visitNode = _visitNodeExtGlobalVariable;
    }
    NodeExtGlobalVariable(NodeToken nd)
    {
        _nodetype = extGlobalVariableNode;
        _token = nd._token;
        isPointer = nd.isPointer;
        _total_size = nd._total_size;
        visitNode = _visitNodeExtGlobalVariable;
    }
    NodeExtGlobalVariable(token *nd)
    {
        _nodetype = extGlobalVariableNode;
        _token = nd;

        visitNode = _visitNodeExtGlobalVariable;
    }
};

void _visitNodeElse(NodeToken *nd)
{
    content.addBefore(string_format("j %s", nd->target.c_str()));
    if (nd->getChildAtPos(0)->visitNode != NULL)
    {
        register_numl.duplicate();
        nd->getChildAtPos(0)->visitNode(nd->getChildAtPos(0));
        register_numl.pop();
    }
    content.addAfter(string_format("%s:", nd->target.c_str()));
}

class NodeElse : public NodeToken
{
public:
    NodeElse()
    {
        _nodetype = elseNode;
        _token = NULL;
        visitNode = _visitNodeElse;
    }
    NodeElse(NodeToken nd)
    {
        _nodetype = elseNode;
        _token = nd._token;
        visitNode = _visitNodeElse;
    }
};

void _visitNodeWhile(NodeToken *nd)
{
    content.addAfter(string_format("%s_while:", nd->target.c_str()));
    if (nd->getChildAtPos(0)->visitNode != NULL)
    {
        register_numl.duplicate();
        nd->getChildAtPos(0)->visitNode(nd->getChildAtPos(0));
        register_numl.pop();
    }
    // content.addAfter(  string_format("%s:\n",nd->target.c_str()));
    if (nd->getChildAtPos(1)->visitNode != NULL)
    {
        register_numl.duplicate();
        nd->getChildAtPos(1)->visitNode(nd->getChildAtPos(1));
        register_numl.pop();
    }
    content.addAfter(string_format("j %s_while", nd->target.c_str()));
    content.addAfter(string_format("%s_end:", nd->target.c_str()));
}

class NodeWhile : public NodeToken
{
public:
    NodeWhile()
    {
        _nodetype = whileNode;
        _token = NULL;
        visitNode = _visitNodeWhile;
    }
    NodeWhile(NodeToken nd)
    {
        _nodetype = whileNode;
        _token = nd._token;
        visitNode = _visitNodeWhile;
    }
};

void _visitNodeIf(NodeToken *nd)
{
    if (nd->getChildAtPos(0)->visitNode != NULL)
    {
        register_numl.duplicate();
        nd->getChildAtPos(0)->visitNode(nd->getChildAtPos(0));
        register_numl.pop();
    }
    // content.addAfter(  string_format("%s:\n",nd->target.c_str()));
    if (nd->getChildAtPos(1)->visitNode != NULL)
    {
        register_numl.duplicate();
        nd->getChildAtPos(1)->visitNode(nd->getChildAtPos(1));
        register_numl.pop();
    }

    content.addAfter(string_format("%s_end:", nd->target.c_str()));
}

class NodeIf : public NodeToken
{
public:
    NodeIf()
    {
        _nodetype = ifNode;
        _token = NULL;
        visitNode = _visitNodeIf;
    }
    NodeIf(NodeToken nd)
    {
        _nodetype = ifNode;
        _token = nd._token;
        visitNode = _visitNodeIf;
    }
};



void _visitNodeStoreExtGlobalVariable(NodeToken *nd)
{

    // int savreg = register_numl;
    register_numl.duplicate();
    varType *v = nd->_token->_vartype;
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
    for (int i = v->size - 1; i >= 0; i--)
    {
        content.addAfter(content.sp.pop(), string_format("%s %s%d,%s%d,%d", asmInstructionsName[v->store[i]].c_str(), getRegType(v->store[i], 0).c_str(), register_numl.get(), getRegType(v->store[i], 1).c_str(), point_regnum, start));
        // register_numl--;
        start -= v->sizes[i];
        // content.sp.push(content.get());
    }
    // res.f = f;
    // res.header = number.header + h;
    content.sp.push(content.get());
    content.sp.swap();
    content.putIteratorAtPos(content.sp.get());
    content.sp.displaystack("PILE");
    if (nd->children.size() > 0)
    {
        register_numl.duplicate();
        nd->getChildAtPos(0)->visitNode(nd->getChildAtPos(0));
        register_numl.pop();
    }

    content.addAfter(string_format("movExt a%d,%s",
                                   point_regnum, nd->_token->text.c_str()));

    if (nd->isPointer)
    {
        // f=f+number.f;
        for (int i = 0; i < v->total_size; i++)
        {
            content.addAfter(string_format("add a%d,a%d,a%d", point_regnum, point_regnum, register_numl.get()));
        }
    }
    content.end();
    content.sp.pop();
    point_regnum++;
    register_numl.pop();
    point_regnum--;

    //    res.register_numl=register_numl;
    // res.register_numr=register_numr;
    return;
}

class NodeStoreExtGlobalVariable : public NodeToken
{
public:
    NodeStoreExtGlobalVariable()
    {
        _nodetype = extGlobalVariableNode;
        _token = NULL;
        visitNode = _visitNodeStoreExtGlobalVariable;
    }
    NodeStoreExtGlobalVariable(NodeToken nd)
    {
        _nodetype = extGlobalVariableNode;
        _token = nd._token;
        isPointer = nd.isPointer;
        _total_size = nd._total_size;
        visitNode = _visitNodeStoreExtGlobalVariable;
    }
    NodeStoreExtGlobalVariable(token *nd)
    {
        _nodetype = extGlobalVariableNode;
        _token = nd;

        visitNode = _visitNodeStoreExtGlobalVariable;
    }
};


void _visitNodeGlobalVariable(NodeToken *nd)
{

    // int savreg = register_numl;
    register_numl.duplicate();
    if (nd->children.size() > 0)
    {
        register_numl.duplicate();
        nd->getChildAtPos(0)->visitNode(nd->getChildAtPos(0));
        register_numl.pop();
    }
    varType *v = nd->_token->_vartype;
    int start = nd->stack_pos;
    uint8_t regnum = 1;
    if (nd->asPointer or (nd->isPointer && nd->children.size() == 0))
        point_regnum++;
    if (nd->isPointer)
    {
        // start = nd->stack_pos;
        regnum = point_regnum;
    }
    string body = "";

    // register_numl++;
    content.addAfter(string_format("l32r a%d,%s", point_regnum, nd->_token->text.c_str()));

    if (nd->isPointer && nd->children.size() > 0) // leds[g];
    {
        // f=f+number.f;
        for (int i = 0; i < v->total_size; i++)
        {
            content.addAfter(string_format("add a%d,a%d,a%d", point_regnum, point_regnum, register_numl.get()));
        }
    }
    content.sp.push(content.get());
    if (nd->asPointer) //(&d)
    {
        content.addAfter(string_format("mov a%d,a%d", register_numl.get(), point_regnum));

        content.sp.push(content.get());
    }
    else if (nd->children.size() > 0 or !nd->isPointer) // leds[h] or h h being global)
    {

        for (int i = 0; i < v->size; i++)
        {
            // content.addAfter(string_format("%s %s%d,%s%d,%d", v->load[i].c_str(), v->reg_name.c_str(), register_numl.get(), v->reg_name.c_str(), point_regnum, start));
            asmInstruction asmInstr = v->load[i];
            content.addAfter(string_format("%s %s%d,%s%d,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), point_regnum, start));

            // register_numl--;
            start += v->sizes[i];
            content.sp.push(content.get());
        }
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
    if (nd->asPointer or (nd->isPointer && nd->children.size() == 0))
        point_regnum--;
    return;

}
class NodeGlobalVariable : public NodeToken
{
public:
    NodeGlobalVariable()
    {
        _nodetype = globalVariableNode;
        _token = NULL;
        visitNode = _visitNodeGlobalVariable;
    }
    NodeGlobalVariable(NodeToken nd)
    {
        _nodetype = globalVariableNode;
        _token = nd._token;
        isPointer = nd.isPointer;
        _total_size = nd._total_size;
        visitNode = _visitNodeGlobalVariable;
    }
    NodeGlobalVariable(token *nd)
    {
        _nodetype = globalVariableNode;
        _token = nd;

        visitNode = _visitNodeGlobalVariable;
    }
};



void _visitNodeStoreGlobalVariable(NodeToken *nd)
{

    // int savreg = register_numl;
    register_numl.duplicate();
    varType *v = nd->_token->_vartype;
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
    for (int i = v->size - 1; i >= 0; i--)
    {
        // content.addAfter(content.sp.pop(), string_format("%s %s%d,%s%d,%d", v->store[i].c_str(), v->reg_name.c_str(), register_numl.get(), v->reg_name.c_str(), point_regnum, start));
        content.addAfter(content.sp.pop(), string_format("%s %s%d,%s%d,%d", asmInstructionsName[v->store[i]].c_str(), getRegType(v->store[i], 0).c_str(), register_numl.get(), getRegType(v->store[i], 1).c_str(), point_regnum, start));
        // register_numl--;
        start -= v->sizes[i];
        // content.sp.push(content.get());
    }
    // res.f = f;
    // res.header = number.header + h;
    content.sp.push(content.get());
    content.sp.swap();
    content.putIteratorAtPos(content.sp.get());
    content.sp.displaystack("PILE");
    if (nd->children.size() > 0)
    {
        register_numl.duplicate();
        nd->getChildAtPos(0)->visitNode(nd->getChildAtPos(0));
        register_numl.pop();
    }

    content.addAfter(string_format("l32r a%d,%s", point_regnum, nd->_token->text.c_str()));

    if (nd->isPointer)
    {
        // f=f+number.f;
        for (int i = 0; i < v->total_size; i++)
        {
            content.addAfter(string_format("add a%d,a%d,a%d", point_regnum, point_regnum, register_numl.get()));
        }
    }
    content.end();
    content.sp.pop();
    //  point_regnum++;
    register_numl.pop();
    // point_regnum--;

    //    res.register_numl=register_numl;
    // res.register_numr=register_numr;
    return;
}

class NodeStoreGlobalVariable : public NodeToken
{
public:
    NodeStoreGlobalVariable()
    {
        _nodetype = storeGlobalVariableNode;
        _token = NULL;
        visitNode = _visitNodeStoreGlobalVariable;
    }
    NodeStoreGlobalVariable(NodeToken nd)
    {
        _nodetype = storeGlobalVariableNode;
        _token = nd._token;
        isPointer = nd.isPointer;
        _total_size = nd._total_size;
        visitNode = _visitNodeStoreGlobalVariable;
    }
    NodeStoreGlobalVariable(token *nd)
    {
        _nodetype = storeGlobalVariableNode;
        _token = nd;

        visitNode = _visitNodeStoreGlobalVariable;
    }
};



void _visitNodeLocalVariable(NodeToken *nd)
{

    if (nd->asPointer)
    {
        register_numl.duplicate();
        varType *v = nd->_token->_vartype;
        int start = nd->stack_pos;
        // content.addAfter(string_format("l32r a%d,stack", point_regnum));
        content.addAfter(string_format("addi a%d,a1,%d", register_numl.get(), start));
        content.sp.push(content.get());
        register_numl.pop();
        register_numl.decrease();
        return;
    }
    register_numl.duplicate();
    // if (nd->children.size() > 0)
    //{

    //  number = nd->getChildAtPos(0)->visitNode(nd->getChildAtPos(0), register_numl, register_numr);
    // }
    varType *v = nd->_token->_vartype;
    int start = nd->stack_pos;
    uint8_t regnum = 1;
    // point_regnum++;
    if (nd->isPointer)
    {
        // start = nd->stack_pos;
        regnum = point_regnum;
    }

    for (int i = 0; i < v->size; i++)
    {
        // content.addAfter(string_format("%s %s%d,%s%d,%d", v->load[i].c_str(), v->reg_name.c_str(), register_numl.get(), v->reg_name.c_str(), regnum, start));

        asmInstruction asmInstr = v->load[i];
        content.addAfter(string_format("%s %s%d,%s%d,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), regnum, start));
        translateType(globalType.get(), v->_varType, register_numl.get());
        // register_numl--;
        start += v->sizes[i];
        content.sp.push(content.get());
    }
    // register_numl++;
    //  content.sp.push(content.get());
    // content.sp.swap();
    // content.putIteratorAtPos(content.sp.get());
    content.sp.displaystack("PILE");
    if (nd->children.size() > 0)
    {
        register_numl.duplicate();
        nd->getChildAtPos(0)->visitNode(nd->getChildAtPos(0));
        register_numl.pop();
    }

    if (nd->isPointer)
    {
        for (int i = 0; i < v->total_size; i++)
        {
            content.addAfter(string_format("add a%d,a%d,a%d", point_regnum, point_regnum, register_numl.get()));
        }
    }
    // point_regnum--;

    // content.end();
    // content.sp.pop();
    register_numl.pop();
    register_numl.decrease();

    return;
}

class NodeLocalVariable : public NodeToken
{
public:
    NodeLocalVariable()
    {
        _nodetype = localVariableNode;
        _token = NULL;
        visitNode = _visitNodeLocalVariable;
    }
    NodeLocalVariable(NodeToken nd)
    {
        _nodetype = localVariableNode;
        _token = nd._token;
        isPointer = nd.isPointer;
        _total_size = nd._total_size;
        stack_pos = nd.stack_pos;
        visitNode = _visitNodeLocalVariable;
    }
    NodeLocalVariable(token *nd)
    {
        _nodetype = localVariableNode;
        _token = nd;

        visitNode = _visitNodeLocalVariable;
    }
};


void _visitNodeStoreLocalVariable(NodeToken *nd)
{
    // printf("jjjkkj\n");

    varType *v = nd->_token->_vartype;

    // globalType.push(__float__);//v->_varType);
    // printf("ona stocké:%d: %s\n",globalType.get(),varTypeEnumNames[globalType.get()].c_str());
    uint8_t regnum = 1;
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
    for (int i = v->size - 1; i >= 0; i--)
    {
        // printf("jjjkkj: %d\n",i);
        // list<string>::iterator l=content.sp.pop();

        // content.addAfter(  content.sp.pop(),  string_format("%s %s%d,%s%d,%d", v->store[i].c_str(), v->reg_name.c_str(), numl, v->reg_name.c_str(), regnum, start));
        content.addAfter(content.sp.pop(), string_format("%s %s%d,%s%d,%d", asmInstructionsName[v->store[i]].c_str(), getRegType(v->store[i], 0).c_str(), numl, getRegType(v->store[i], 1).c_str(), regnum, start));

        // numl++;
        start -= v->sizes[i];
    }
    if (nd->isPointer)
    {
        register_numl.duplicate();
        content.sp.push(content.get());
        content.sp.swap();
        content.putIteratorAtPos(content.sp.get());
        content.sp.displaystack("PILE");
        if (nd->children.size() > 0)
        {
            register_numl.duplicate();
            nd->getChildAtPos(0)->visitNode(nd->getChildAtPos(0));
            register_numl.pop();
        }
        content.addAfter(string_format("l32i a%d,a1,%d", point_regnum, nd->stack_pos));
        for (int i = 0; i < v->total_size; i++)
        {
            content.addAfter(string_format("add a%d,a%d,a%d", point_regnum, point_regnum, register_numl.get()));
        }
        content.end();
        content.sp.pop();
        point_regnum++;
        register_numl.pop();
        point_regnum--;
    }

    // globalType.pop();
}
class NodeStoreLocalVariable : public NodeToken
{
public:
    NodeStoreLocalVariable()
    {
        _nodetype = storeLocalVariableNode;
        _token = NULL;
        visitNode = _visitNodeStoreLocalVariable;
    }
    NodeStoreLocalVariable(NodeToken nd)
    {
        _nodetype = storeLocalVariableNode;
        _token = nd._token;
        isPointer = nd.isPointer;
        _total_size = nd._total_size;
        stack_pos = nd.stack_pos;
        visitNode = _visitNodeStoreLocalVariable;
    }
    NodeStoreLocalVariable(token *nd)
    {
        _nodetype = storeLocalVariableNode;
        _token = nd;

        visitNode = _visitNodeStoreLocalVariable;
    }
};


class NodeDefExtGlobalVariable : public NodeToken
{
public:
    NodeDefExtGlobalVariable()
    {
        _nodetype = defExtGlobalVariableNode;
        _token = NULL;
        // visitNode = visitNodeDefExtGlobalVariable;
    }
    NodeDefExtGlobalVariable(NodeToken nd)
    {
        _nodetype = defExtGlobalVariableNode;
        _token = nd._token;
        isPointer = nd.isPointer;
        _total_size = nd._total_size;
        // visitNode = visitNodeDefExtGlobalVariable;
    }
};


void _visitNodeDefGlobalVariable(NodeToken *nd)
{
    ////printf("in visitNodeDefGlobalVariable\n");
    string f = "";
    string h = "";
    header.addAfter(string_format("%s:", nd->_token->text.c_str()));
    header.addAfter(string_format(".bytes %d", nd->_total_size));

    return;
}

class NodeDefGlobalVariable : public NodeToken
{
public:
    NodeDefGlobalVariable()
    {
        _nodetype = defGlobalVariableNode;
        _token = NULL;
        visitNode = _visitNodeDefGlobalVariable;
    }
    NodeDefGlobalVariable(NodeToken nd)
    {
        _nodetype = defGlobalVariableNode;
        _token = nd._token;
        isPointer = nd.isPointer;
        _total_size = nd._total_size;
        visitNode = _visitNodeDefGlobalVariable;
    }
};


class NodeDefLocalVariable : public NodeToken
{
public:
    NodeDefLocalVariable()
    {
        _nodetype = defLocalVariableNode;
        _token = NULL;
        // visitNode=visitNodeDefLocalVariable;
    }
    NodeDefLocalVariable(NodeToken nd)
    {
        _nodetype = defLocalVariableNode;

        isPointer = nd.isPointer;
        _total_size = nd._total_size;
        // visitNode=visitNodeDefLocalVariable;
        int delta = 0;
        if (nd.isPointer)
        {
            if (stack_size % 4 != 0)
                delta = 4 - stack_size % 4;
        }
        if (nd._token->_vartype->_varType == __unit32_t__ || nd._token->_vartype->_varType == __float__)
        {
            if (stack_size % 4 != 0)
                delta = nd._token->_vartype->_varSize - stack_size % 4;
        }
        else if (nd._token->_vartype->_varType == __unit16_t__ || nd._token->_vartype->_varType == __int__)
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
            stack_size += nd._token->_vartype->_varSize;
        }
        _token = nd._token;
        stack_pos = nd.stack_pos;
    }
};

void _visitNodeCallFunction(NodeToken *nd)
{
    
    NodeToken *t = nd->_link; // cntx.findFunction(nd->_token);
    if (t == NULL)
    {

        return;
    }
    // if(t->getChildAtPos(1)->children.size()<1)
    // return;
    content.addAfter(string_format("l32r a%d,stack", point_regnum));
    for (int i = 0; i < t->getChildAtPos(1)->children.size(); i++)
    {
        // isPointer = false;
        if (t->getChildAtPos(1)->getChildAtPos(i)->isPointer)
        {
            // isPointer = true;
            register_numl.duplicate();
            nd->getChildAtPos(0)->getChildAtPos(i)->visitNode(nd->getChildAtPos(0)->getChildAtPos(i));
            register_numl.pop();

            int start = t->getChildAtPos(1)->getChildAtPos(i)->stack_pos - 16;
            content.addAfter(content.sp.pop(), string_format("s32i a%d,a%d,%d", register_numl.get(), point_regnum, start));
            // isPointer=false;
        }
        else
        {
            globalType.push(t->getChildAtPos(1)->getChildAtPos(i)->_token->_vartype->_varType);
            register_numl.duplicate();
            nd->getChildAtPos(0)->getChildAtPos(i)->visitNode(nd->getChildAtPos(0)->getChildAtPos(i));
            register_numl.pop();

            int start = t->getChildAtPos(1)->getChildAtPos(i)->stack_pos - 16 + t->getChildAtPos(1)->getChildAtPos(i)->_token->_vartype->total_size;
            int tot = t->getChildAtPos(1)->getChildAtPos(i)->_token->_vartype->size - 1;
            for (int j = 0; j < t->getChildAtPos(1)->getChildAtPos(i)->_token->_vartype->size; j++)
            {
                if (nd->getChildAtPos(0)->getChildAtPos(i)->_token->_vartype != NULL)
                    translateType(globalType.get(), nd->getChildAtPos(0)->getChildAtPos(i)->_token->_vartype->_varType, register_numl.get());
                else
                    translateType(globalType.get(), nd->getChildAtPos(0)->getChildAtPos(i)->_token->_varType, register_numl.get());
                start -= t->getChildAtPos(1)->getChildAtPos(i)->_token->_vartype->sizes[tot - j];
                asmInstruction asmInstr = t->getChildAtPos(1)->getChildAtPos(i)->_token->_vartype->store[tot - j];
                content.addAfter(content.sp.pop(), string_format("%s %s%d,%s%d,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), point_regnum, start));

                // start+=t->getChildAtPos(1)->getChildAtPos(i)->_token->_vartype->sizes[j];
            }
            globalType.pop();
        }
        // content.addAfter(string_format("addi a%d,a1,%d",11+i,t->getChildAtPos(1)->getChildAtPos(i)->stack_pos));
    }

    content.addAfter(string_format("mov a10,a2")); // neded to find the external variables !!!!!!

    content.addAfter(string_format("call8 %s", nd->_token->text.c_str()));

    int start = nd->stack_pos;
    varType *v = nd->_link->getChildAtPos(0)->_token->_vartype;
    content.addAfter(string_format("l32r a%d,stackr", point_regnum));
    for (int i = 0; i < v->size; i++)
    {
        // content.addAfter(string_format("mov a15,a10"));
        // content.addAfter(string_format("%s %s%d,%s%d,%d", v->load[i].c_str(), v->reg_name.c_str(), register_numl.get(), v->reg_name.c_str(), point_regnum, start));
        asmInstruction asmInstr = v->load[i];
        content.addAfter(string_format("%s %s%d,%s%d,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), point_regnum, start));

        // register_numl--;
        start += v->sizes[i];
        content.sp.push(content.get());
    }
}

class NodeCallFunction : public NodeToken
{
public:
    NodeCallFunction()
    {
        _nodetype = callFunctionNode;
        _token = NULL;
        visitNode = _visitNodeCallFunction;
    }
    NodeCallFunction(token *t)
    {
        _token = t;
        _nodetype = callFunctionNode;
        visitNode = _visitNodeCallFunction;
    }
    NodeCallFunction(NodeToken *t)
    {
        //_token = t->_token;
        _token = t->_token;
        ;
        _nodetype = callFunctionNode;
        _link = t;

        visitNode = _visitNodeCallFunction;
    }
};


void _visitNodeExtCallFunction(NodeToken *nd)
{

    NodeToken *t = nd->_link; // cntx.findFunction(nd->_token);

    if (t == NULL)
    {

        // globalType.pop();
        return;
    }

    // printf(" %s %d %d\n",nd->_token->text.c_str(),  t->children.size(),t->getChildAtPos(1)->children.size());

    for (int i = 0; i < t->getChildAtPos(1)->children.size(); i++)
    {
        register_numl.duplicate();
        globalType.push(t->getChildAtPos(1)->getChildAtPos(i)->_token->_vartype->_varType);
        nd->getChildAtPos(0)->getChildAtPos(i)->visitNode(nd->getChildAtPos(0)->getChildAtPos(i));
 register_numl.pop();
        if (nd->getChildAtPos(0)->getChildAtPos(i)->_token->_vartype != NULL)
            translateType(globalType.get(), nd->getChildAtPos(0)->getChildAtPos(i)->_token->_vartype->_varType, register_numl.get());
        else
            translateType(globalType.get(), nd->getChildAtPos(0)->getChildAtPos(i)->_token->_varType, register_numl.get());
       
        // translateType(t->getChildAtPos(1)->getChildAtPos(i)->_token->_vartype->_varType,globalType.get(),register_numl.get());
        if (t->getChildAtPos(1)->getChildAtPos(i)->_token->_vartype->_varType == __float__)
        {
            content.addAfter(string_format("rfr a%d,f%d", 10 + i, register_numl.get()));
        }
        else
        {
            content.addAfter(string_format("mov a%d,a%d", 10 + i, register_numl.get()));
        }
        globalType.pop();
    }

    content.addAfter(string_format("callExt a8,%s", nd->_token->text.c_str()));
    content.sp.push(content.get());
    int start = nd->stack_pos;
    varType *v = nd->_link->getChildAtPos(0)->_token->_vartype;

if(v->_varType == __float__)
{
content.addAfter(string_format("wfr f%d,a10",register_numl.get()));
register_numl.decrease();
 content.sp.push(content.get());
}
else
{
    for (int i = 0; i < v->size; i++)
    {
        // content.addAfter(string_format("mov a15,a10"));
        content.addAfter(string_format("extui a%d,a10,%d,%d", register_numl.get(),start * 8, v->sizes[i] * 8));
        // register_numl--;
        start += v->sizes[i];
        content.sp.push(content.get());
    }
}
    return;
}

class NodeExtCallFunction : public NodeToken
{
public:
    NodeExtCallFunction()
    {
        _nodetype = extCallFunctionNode;
        _token = NULL;
        visitNode = _visitNodeExtCallFunction;
    }
    NodeExtCallFunction(token *t)
    {
        _token = t;
        _nodetype = extCallFunctionNode;
        visitNode = _visitNodeExtCallFunction;
    }
    NodeExtCallFunction(NodeToken *t)
    {
        //_token = t->_token;
        _token = t->_token;
        _nodetype = extCallFunctionNode;
        _link = t;
        visitNode = _visitNodeExtCallFunction;
    }
};

void _visitNodeDefFunction(NodeToken *nd)
{

    header.addAfter(string_format(".global %s", nd->_token->text.c_str()));

    content.addAfter(string_format("%s:", nd->_token->text.c_str()));
    content.addAfter(string_format("entry a1,%d", ((nd->stack_pos) / 8 + 1) * 8+32)); // ((nd->stack_pos) / 8 + 1) * 8+20)
    for (int i = 1; i < nd->children.size(); i++)
    {
        if (nd->getChildAtPos(i)->visitNode != NULL)
        {
            nd->getChildAtPos(i)->visitNode(nd->getChildAtPos(i));
            // f = f + g.f;
            // h = h + g.header;
        }
    }
    content.addAfter(string_format("retw.n"));

}

class NodeDefFunction : public NodeToken
{
public:
    NodeDefFunction()
    {
        _nodetype = defFunctionNode;
        _token = NULL;
        visitNode = _visitNodeDefFunction;
    }
    NodeDefFunction(token *t)
    {
        _token = t;
        _nodetype = defFunctionNode;
        visitNode = _visitNodeDefFunction;
    }
    NodeDefFunction(NodeToken t)
    {
        _token = t._token;
        _nodetype = defFunctionNode;
        visitNode = _visitNodeDefFunction;
    }
};

void _visitNodeDefAsmFunction(NodeToken *nd)
{


    header.addAfter(string_format(".global %s", nd->_token->text.c_str()));

    content.addAfter(string_format("%s:", nd->_token->text.c_str()));
    //content.addAfter(string_format("entry a1,%d", 80)); // ((nd->stack_pos) / 8 + 1) * 8)

    if (nd->children.size() >= 3)
        if (nd->getChildAtPos(2)->visitNode != NULL)
        {
            nd->getChildAtPos(2)->visitNode(nd->getChildAtPos(2));
            // f = f + g.f;
            // h = h + g.header;
        }
    

    //content.addAfter(string_format("retw.n"));

}

class NodeDefAsmFunction : public NodeToken
{
public:
    NodeDefAsmFunction()
    {
        _nodetype = defAsmFunctionNode;
        _token = NULL;
        visitNode = _visitNodeDefAsmFunction;
    }
    NodeDefAsmFunction(token *t)
    {
        _token = t;
        _nodetype = defAsmFunctionNode;
        visitNode = _visitNodeDefAsmFunction;
    }
    NodeDefAsmFunction(NodeToken t)
    {
        _token = t._token;
        _nodetype = defAsmFunctionNode;
        visitNode = _visitNodeDefAsmFunction;
    }
};

void _visitNodeString(NodeToken *nd)
{
    content.addAfter(string_format("%s", nd->_token->text.substr(1, nd->_token->text.size() - 2).c_str()));
}
class NodeString : public NodeToken
{
public:
    NodeString()
    {
        _nodetype = stringNode;
        _token = NULL;
        visitNode = _visitNodeString;
    }
    NodeString(token *t)
    {
        _token = t;
        _nodetype = stringNode;
        visitNode = _visitNodeString;
    }
    NodeString(NodeToken t)
    {
        _token = t._token;
        _nodetype = stringNode;
        visitNode = _visitNodeString;
    }
};

class NodeDefExtFunction : public NodeToken
{
public:
    NodeDefExtFunction()
    {
        _nodetype = defExtFunctionNode;
        _token = NULL;
        // visitNode = visitNodeDefExtFunction;
    }
    NodeDefExtFunction(token *t)
    {
        _token = t;
        _nodetype = defExtFunctionNode;
        //  visitNode = visitNodeDefExtFunction;
    }
    NodeDefExtFunction(NodeToken t)
    {
        _token = t._token;
        _nodetype = defExtFunctionNode;
        //  visitNode = visitNodeDefExtFunction;
    }
};

void _visitNodeInputArguments(NodeToken *nd)
{
    // if(nd->children.size()<1)
    // return;
    content.addAfter(string_format("l32r a%d,stack", point_regnum));

    for (int i = 0; i < nd->children.size(); i++)
    {
        int start = nd->getChildAtPos(i)->stack_pos;
        if (nd->getChildAtPos(i)->isPointer)
        {
            int start = nd->getChildAtPos(i)->stack_pos;
            content.addAfter(string_format("l32i a15,a%d,%d", point_regnum, start - 16));
            content.addAfter(string_format("s32i a15,a1,%d", start));
        }
        else
        {

            for (int j = 0; j < nd->getChildAtPos(i)->_token->_vartype->size; j++)
            {

                asmInstruction asmInstr = nd->getChildAtPos(i)->_token->_vartype->load[0];
                content.addAfter(string_format("%s %s15,%s%d,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), getRegType(asmInstr, 1).c_str(), point_regnum, start - 16));
                asmInstr = nd->getChildAtPos(i)->_token->_vartype->store[0];
                content.addAfter(string_format("%s %s15,%s1,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), getRegType(asmInstr, 1).c_str(), start));
                start += nd->getChildAtPos(i)->_token->_vartype->sizes[j];
            }
        }
    }
}

class NodeInputArguments : public NodeToken
{
public:
    NodeInputArguments()
    {
        _nodetype = inputArgumentsNode;
        _token = NULL;
        visitNode = _visitNodeInputArguments;
    }
    NodeInputArguments(token *t)
    {
        _token = t;
        _nodetype = inputArgumentsNode;
        _token = NULL;
        visitNode = _visitNodeInputArguments;
    }
};



void _visitNodeBlockStatement(NodeToken *nd)
{
    // printf("in block statement");
    for (int i = 0; i < nd->children.size(); i++)
    {
        if (nd->getChildAtPos(i)->visitNode != NULL)
        {
            nd->getChildAtPos(i)->visitNode(nd->getChildAtPos(i));
            // f = f + g.f;
            // h = h + g.header;
        }
    }
}

class NodeBlockStatement : public NodeToken
{
public:
    NodeBlockStatement()
    {
        _nodetype = blockStatementNode;
        _token = NULL;
        visitNode = _visitNodeBlockStatement;
    }
    NodeBlockStatement(token *t)
    {
        _token = t;
        _nodetype = blockStatementNode;
        visitNode = _visitNodeBlockStatement;
    }
};



void _visitNodeOperator(NodeToken *nd)
{

    // printf("operator %s\n",tokenNames[nd->_token->type].c_str());
    // register_numl.pop();
    register_numl.displaystack();
    varTypeEnum l = __none__;
    varTypeEnum r = __none__;
    // printf("kk\n");
    if (nd->parent->getChildAtPos(0)->_token != NULL)
    {
        if (nd->parent->getChildAtPos(0)->_token->_vartype != NULL)
        {
            l = nd->parent->getChildAtPos(0)->_token->_vartype->_varType;
        }
        else
        {
            l = nd->parent->getChildAtPos(0)->_token->_varType;
        }
    }
    // printf("kk2 :%d\n",nd->parent->children.size());
    if (nd->parent->children.size() >= 3)
    {
        if (nd->parent->getChildAtPos(2) == NULL)
        {
            // printf("WFT %d %s\n",nd->parent->children.size(),nodeTypeNames[nd->parent->_nodetype].c_str());
        }
        if (nd->parent->getChildAtPos(2)->_token != NULL)
        {

            if (nd->parent->getChildAtPos(2)->_token->_vartype != NULL)
            {
                // printf("kk32 %s\n",nodeTypeNames[nd->parent->_nodetype].c_str());
                r = nd->parent->getChildAtPos(2)->_token->_vartype->_varType;
            }
            else
            {
                // printf("kk33\n");
                r = nd->parent->getChildAtPos(2)->_token->_varType;
            }
        }
    }
    // printf("kk3\n");
    bool ff = false;

    if ((nd->parent->_token == NULL))
    {
        addTokenSup(nd->parent);
        if (globalType.get() == __float__)
        {
            nd->parent->_token->_varType = __float__;
        }
        else
        {
            nd->parent->_token->_varType = __none__;
        }
    }
    else if (nd->parent->_token->_vartype == NULL)
    {
        if (globalType.get() == __float__)
        {
            nd->parent->_token->_varType = __float__;
        }
        else
        {
            nd->parent->_token->_varType = __none__;
        }
    }
    // printf("kk4\n");
    if (globalType.get() == __float__)
    {
        ff = true;
        // nd->parent->_token->_vartype=&_varTypes[__float__];
    }

    // printf("kk5\n");
    asmInstruction asmInstr;

    translateType(globalType.get(), r, register_numr.get());
    translateType(globalType.get(), l, register_numl.get());
    switch (nd->_token->type)
    {
    case TokenAddition:

        if (ff)
        {
            asmInstr = adds;

        }
        else
        {
            asmInstr = add;
            
        }
        content.addAfter(string_format("%s %s%d,%s%d,%s%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), register_numl.get(), getRegType(asmInstr, 2).c_str(), register_numr.get()));

        return;
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

        return;
        break;
    case TokenSlash:
        // content.addAfter(string_format("quou a%d,a%d,a%d", register_numl.get(), register_numl.get(), register_numr.get()));
        if (ff)
        {
            content.addAfter(string_format("mov.s f1,f%d",register_numl.get()));
            content.addAfter(string_format("mov.s f2,f%d",register_numr.get()));
            content.addAfter("call8 __div");
           content.addAfter(string_format("mov.s f%d,f0",register_numl.get()));
        }
        else
        {
            asmInstr = quou;
            content.addAfter(string_format("%s %s%d,%s%d,%s%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), register_numl.get(), getRegType(asmInstr, 2).c_str(), register_numr.get()));
        }
        

        return;
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
        return;
        break;
    case TokenPlusPlus:

        content.addAfter(string_format("addi a%d,a%d,1", register_numl.get(), register_numl.get()));
        return;
        break;
    case TokenModulo:

        content.addAfter(string_format("remu a%d,a%d,a%d", register_numl.get(), register_numl.get(), register_numr.get()));
        return;

    default:
        return;
        break;
    }
}

class NodeOperator : public NodeToken
{
public:
    NodeOperator()
    {
        _nodetype = operatorNode;
        _token = NULL;
        visitNode = _visitNodeOperator;
    }
    NodeOperator(token *t)
    {
        _nodetype = operatorNode;
        _token = t;
        visitNode = _visitNodeOperator;
    }
};

void _visitNodeChangeType(NodeToken *nd)
{
    register_numl.duplicate();
    globalType.push(nd->_token->_vartype->_varType);
    for (int i = 0; i < nd->children.size(); i++)
    {
        if (nd->getChildAtPos(i)->visitNode != NULL)
        {
            register_numl.duplicate();
            nd->getChildAtPos(i)->visitNode(nd->getChildAtPos(i));
            // f = f + g.f;
            // h = h + g.header;
            register_numl.pop();
            if (nd->getChildAtPos(i)->_token->_vartype != NULL)
                translateType(globalType.get(), nd->getChildAtPos(i)->_token->_vartype->_varType, register_numl.get());
            else
                translateType(globalType.get(), nd->getChildAtPos(i)->_token->_varType, register_numl.get());
        }
    }
    varTypeEnum s = globalType.pop();
    // globalType.pop();
    register_numl.pop();
    translateType(globalType.get(), s, register_numl.get());
}
class NodeChangeType : public NodeToken
{
public:
    NodeChangeType()
    {
        _nodetype = changeTypeNode;
        _token = NULL;
        visitNode = _visitNodeChangeType;
    }
    NodeChangeType(token *t)
    {
        _nodetype = changeTypeNode;
        _token = t;
        _token->_vartype = t->_vartype;
        visitNode = _visitNodeChangeType;
    }
};


void _visitNodeBinOperator(NodeToken *nd)
{
    // printf("bin operator\n");
    register_numl.displaystack();
    register_numl.duplicate();
    // register_numr.duplicate();
    if (nd->getChildAtPos(0)->visitNode != NULL)
        nd->getChildAtPos(0)->visitNode(nd->getChildAtPos(0));
    register_numl.displaystack();
    register_numl.duplicate();
    // register_numr.duplicate();
    if (nd->getChildAtPos(2)->visitNode != NULL)
        nd->getChildAtPos(2)->visitNode(nd->getChildAtPos(2));
    // register_numr.pop();
    register_numl.swap();
    register_numr.push(register_numl.pop());
    register_numl.swap();

    register_numl.displaystack();

    if (nd->getChildAtPos(1)->visitNode != NULL)
        nd->getChildAtPos(1)->visitNode(nd->getChildAtPos(1));
    register_numl.pop();
    if (nd->getChildAtPos(1)->_token->type == TokenAddition || nd->getChildAtPos(1)->_token->type == TokenSubstraction)
    {
        register_numl.increase();
    }
    content.sp.pop();
    content.sp.pop();
    content.sp.push(content.get());
    // register_numl.pop();

    register_numr.pop();
    // register_numr.pop();
}
class NodeBinOperator : public NodeToken
{
public:
    NodeBinOperator(NodeToken left, NodeOperator t, NodeToken right)
    {
        children.push_back(left);
        children.push_back(t);
        children.push_back(right);
        _nodetype = binOpNode;
        _token = NULL;
        visitNode = _visitNodeBinOperator;
    }
    NodeBinOperator()
    {
        _nodetype = binOpNode;
        _token = NULL;
        visitNode = _visitNodeBinOperator;
    }
};

void _visitNodeUnitary(NodeToken *nd)
{
    // printf("bin operator\n");
    register_numl.displaystack();
    // register_numl.duplicate();

    // register_numr.duplicate();
    if (nd->getChildAtPos(0)->_token->type == TokenUppersand)
    {
        nd->getChildAtPos(1)->asPointer = true;
    }
    else
    {
        if (nd->getChildAtPos(0)->visitNode != NULL)
            nd->getChildAtPos(0)->visitNode(nd->getChildAtPos(0));
        register_numl.displaystack();
        register_numl.increase();
    }
    //

    if (nd->getChildAtPos(1)->visitNode != NULL)
        nd->getChildAtPos(1)->visitNode(nd->getChildAtPos(1));
    // register_numl.pop();
    // content.sp.pop();
    content.sp.push(content.get());
    // register_numl.pop();

    // register_numr.pop();
    //  register_numr.pop();
}

class NodeUnitary : public NodeToken
{
public:
    NodeUnitary(NodeOperator t, NodeToken right)
    {
        children.push_back(t);
        children.push_back(right);
        _nodetype = unitaryOpNode;
        _token = NULL;
        visitNode = _visitNodeUnitary;
    }
    NodeUnitary()
    {

        _nodetype = unitaryOpNode;
        _token = NULL;
        visitNode = _visitNodeUnitary;
    }
    NodeUnitary(NodeOperator t)
    {
        children.push_back(t);
        _nodetype = unitaryOpNode;
        _token = NULL;
        visitNode = _visitNodeUnitary;
    }
};


void _visitNodeNumber(NodeToken *nd)
{
    // printf("in number\n");
    register_numl.displaystack();
    // register_numl.duplicate();

    if (nd->children.size() > 0)
    {

        for (int i = 0; i < nd->children.size(); i++)
        {
            register_numl.duplicate();
            nd->getChildAtPos(i)->visitNode(nd->getChildAtPos(i));
            // register_numl.pop();
            // content.sp.push(content.get());
            register_numl.pop();
        }
    }
    else
    {
        if (nd->_token->_vartype->_varType == __float__)
        {
            float __f = 0;
            sscanf(nd->_token->text.c_str(), "%f", &__f);
            header.addAfter(string_format("%s_%d:", "local_var", local_var_num));
            // local_var_num++;
            string val = ".bytes 4 ";
            uint32_t __num = (uint32_t)(*((uint32_t *)&__f));
            // Serial.printf(" on a float  %4x\r\n",__num);
            char c = __num & 0xff;
            val = val + c;
            // val=val+'A';
            __num = __num / 256;
            c = __num & 0xff;
            val = val + c;
            // val=val+'A';
            __num = __num / 256;
            c = __num & 0xff;
            val = val + c;
            // val=val+'A';
            __num = __num / 256;
            c = __num & 0xff;
            val = val + c;
            // val=val+'A';

            header.addAfter(val);

            point_regnum++;
            content.addAfter(string_format("l32r a%d,%s_%d", point_regnum, "local_var", local_var_num));
            content.addAfter(string_format("lsi f%d,a%d,0", register_numl.get(), point_regnum));
            content.sp.push(content.get());
            point_regnum--;
            local_var_num++;
            register_numl.decrease();
        }
        else
        {
            int __num = 0;
            sscanf(nd->_token->text.c_str(), "%d", &__num);
            if (__num >= 2048 or __num < -2046)
            {
                header.addAfter(string_format("%s_%d:", "local_var", local_var_num));
                string val = ".bytes 4 ";
                char c = __num & 0xff;
                val = val + c;
                // val=val+'A';
                __num = __num / 256;
                c = __num & 0xff;
                val = val + c;
                // val=val+'A';
                __num = __num / 256;
                c = __num & 0xff;
                val = val + c;
                // val=val+'A';
                __num = __num / 256;
                c = __num & 0xff;
                val = val + c;
                // val=val+'A';

                header.addAfter(val);

                point_regnum++;
                content.addAfter(string_format("l32r a%d,%s_%d", point_regnum, "local_var", local_var_num));
                content.addAfter(string_format("l32i a%d,a%d,0", register_numl.get(), point_regnum));
                content.sp.push(content.get());
                point_regnum--;
                local_var_num++;
                register_numl.decrease();
            }

            else
            {


                content.addAfter(string_format("movi a%d,%s", register_numl.get(), nd->_token->text.c_str()));
                content.sp.push(content.get());
                register_numl.decrease();
            }
        }
;
    }


}
class NodeNumber : public NodeToken
{
public:
    NodeNumber(token *t)
    {
        _token = t;
        _nodetype = numberNode;
        visitNode = _visitNodeNumber;
    }
};

void createNodeVariable(token *_var, NodeToken *nd, bool isStore)
{
    // printf("***************create cariavbla %d %s\n", isStore ,nd->_token->text.c_str());
    // NodeToken var = NodeToken(_var);
    switch (nd->_nodetype)
    {
    case extGlobalVariableNode:
    {

        if (isStore)
        {
            // NodeStoreExtGlobalVariable v = NodeStoreExtGlobalVariable(var);
            NodeStoreExtGlobalVariable v = NodeStoreExtGlobalVariable(_var);
            copyPrty(nd, &v);
            current_node = current_node->addChild(v);
            return;
        }
        else
        {
            NodeExtGlobalVariable v = NodeExtGlobalVariable(_var);
            copyPrty(nd, &v);
            current_node = current_node->addChild(v);
            return;
        }
    }
    break;
    case defExtGlobalVariableNode:
    {

        if (isStore)
        {
            NodeStoreExtGlobalVariable v = NodeStoreExtGlobalVariable(_var);
            copyPrty(nd, &v);
            current_node = current_node->addChild(v);
            return;
        }
        else
        {
            NodeExtGlobalVariable v = NodeExtGlobalVariable(_var);
            copyPrty(nd, &v);
            current_node = current_node->addChild(v);
            return;
        }
    }
    break;
    case defLocalVariableNode:
    {

        if (isStore == true)
        {
            NodeStoreLocalVariable v = NodeStoreLocalVariable(_var);
            copyPrty(nd, &v);
            current_node = current_node->addChild(v);
            return;
        }
        else
        {
            NodeLocalVariable v = NodeLocalVariable(_var);
            copyPrty(nd, &v);
            current_node = current_node->addChild(v);
            return;
        }
    }
    break;
    case localVariableNode:
    {

        if (isStore == true)
        {
            NodeStoreLocalVariable v = NodeStoreLocalVariable(_var);
            copyPrty(nd, &v);
            current_node = current_node->addChild(v);
            return;
        }
        else
        {
            NodeLocalVariable v = NodeLocalVariable(_var);
            copyPrty(nd, &v);
            current_node = current_node->addChild(v);
            return;
        }
    }
    break;
    case defGlobalVariableNode:
    {

        if (isStore)
        {
            NodeStoreGlobalVariable v = NodeStoreGlobalVariable(_var);
            copyPrty(nd, &v);
            v._token->text = nd->_token->text;
            current_node = current_node->addChild(v);
            return;
        }
        else
        {
            NodeGlobalVariable v = NodeGlobalVariable(_var);
            copyPrty(nd, &v);
            v._token->text = nd->_token->text;
            current_node = current_node->addChild(v);
            return;
        }
    }
    break;
    case globalVariableNode:
    {

        if (isStore)
        {
            NodeStoreGlobalVariable v = NodeStoreGlobalVariable(_var);
            copyPrty(nd, &v);
            current_node = current_node->addChild(v);
            return;
        }
        else
        {
            NodeGlobalVariable v = NodeGlobalVariable(_var);
            copyPrty(nd, &v);
            current_node = current_node->addChild(v);
            return;
        }
    }
    break;
    default:
    {

        if (isStore)
        {
            NodeStoreLocalVariable v = NodeStoreLocalVariable(_var);
            copyPrty(nd, &v);
            current_node = current_node->addChild(v);
            return;
        }
        else
        {
            NodeLocalVariable v = NodeLocalVariable(_var);
            copyPrty(nd, &v);
            current_node = current_node->addChild(v);
            return;
        }
    }
    break;
    }
}

NodeToken createNodeLocalVariableForCreation(NodeToken *var, NodeToken *nd)
{
    
    switch (var->_nodetype)
    {
    case defGlobalVariableNode:
    {

        NodeDefGlobalVariable v = NodeDefGlobalVariable(*var);
        copyPrty(nd, &v);
        return v;
    }
    break;
    case defLocalVariableNode:
    {

      
        NodeDefLocalVariable v = NodeDefLocalVariable(*var);
        copyPrty(nd, &v);
        return v;
    }
    break;

        break;
    default:
    {

        copyPrty(nd, var);
        NodeDefLocalVariable v = NodeDefLocalVariable(*var);

        return v;
    }
    break;
    }
}

NodeToken createNodeLocalVariableForStore(NodeToken var)
{

    switch (var._nodetype)
    {
    case defGlobalVariableNode:
    {
      
        NodeStoreGlobalVariable v = NodeStoreGlobalVariable(var);

        return v;
    }
    break;
    case defLocalVariableNode:
    {

        // copyPrty(*nd, var);
        NodeStoreLocalVariable v = NodeStoreLocalVariable(var);

        return v;
    }
    break;

        break;
    default:
    {

        // copyPrty(*nd, var);
        NodeStoreLocalVariable v = NodeStoreLocalVariable(var);

        return v;
    }
    break;
    }
}



void _visitNodeAssignement(NodeToken *nd)
{
    // printf("in Assignment Node\n");
    point_regnum = 4;
    content.sp.push(content.get());
    register_numl.duplicate();
    content.sp.displaystack("PILE");
    globalType.push(nd->getChildAtPos(0)->_token->_vartype->_varType);
    if (nd->getChildAtPos(1)->visitNode != NULL)
    {
        register_numl.duplicate();
        nd->getChildAtPos(1)->visitNode(nd->getChildAtPos(1));
        register_numl.pop();
        if (nd->getChildAtPos(1)->_token->_vartype != NULL)
            translateType(globalType.get(), nd->getChildAtPos(1)->_token->_vartype->_varType, register_numl.get());
        else
            translateType(globalType.get(), nd->getChildAtPos(1)->_token->_varType, register_numl.get());
    }
    content.sp.displaystack("PILE");
    // printf("before store\n");
    point_regnum++;
    register_numl.displaystack();
    // register_numl.pop();
    // register_numl.displaystack();
    if (nd->getChildAtPos(0)->visitNode != NULL)
    {
        register_numl.duplicate();
        nd->getChildAtPos(0)->visitNode(nd->getChildAtPos(0));
        register_numl.pop();
    }
    register_numl.pop();
    content.sp.pop();
    globalType.pop();
}

class NodeAssignement : public NodeToken
{
public:
    NodeAssignement()
    {
        _nodetype = assignementNode;
        _token = NULL;
        visitNode = _visitNodeAssignement;
    }
    NodeAssignement(token *t)
    {
        _nodetype = assignementNode;
        _token = t;
        visitNode = _visitNodeAssignement;
    }
};


void visitNodeStatement(NodeToken *nd)
{
    point_regnum = 4;

    for (int i = 0; i < nd->children.size(); i++)
    {
        if (nd->getChildAtPos(i)->visitNode != NULL)
        {
            register_numl.duplicate();
            nd->getChildAtPos(i)->visitNode(nd->getChildAtPos(i));
            register_numl.pop();
        }
    }

    return;
}
class NodeStatement : public NodeToken
{
public:
    NodeStatement()
    {
        _nodetype = statementNode;
        _token = NULL;
        visitNode = visitNodeStatement;
    }
    NodeStatement(token *t)
    {
        _nodetype = statementNode;
        _token = t;
        visitNode = visitNodeStatement;
    }
};


void _visitNodeFor(NodeToken *nd)
{

    if (nd->getChildAtPos(0)->visitNode != NULL)
    {
        register_numl.duplicate();
        nd->getChildAtPos(0)->visitNode(nd->getChildAtPos(0));
        register_numl.pop();
    }
    content.addAfter(string_format("%s:", nd->target.c_str()));
    if (nd->getChildAtPos(1)->visitNode != NULL)
    {
        register_numl.duplicate();
        nd->getChildAtPos(1)->visitNode(nd->getChildAtPos(1));
        register_numl.pop();
    }

    if (nd->getChildAtPos(3)->visitNode != NULL)
    {
        register_numl.duplicate();
        nd->getChildAtPos(3)->visitNode(nd->getChildAtPos(3));
        register_numl.pop();
    }

    if (nd->getChildAtPos(2)->visitNode != NULL)
    {
        register_numl.duplicate();
        nd->getChildAtPos(2)->visitNode(nd->getChildAtPos(2));
        register_numl.pop();
    }

    content.addAfter(string_format("j %s", nd->target.c_str()));
    content.addAfter(string_format("%s_end:", nd->target.c_str()));


    return;
}

class NodeFor : public NodeToken
{
public:
    NodeFor()
    {
        _token = NULL;

        _nodetype = forNode;
        visitNode = _visitNodeFor;
    };

    NodeFor(token *t) // token th
    {
        // children.push_back(right);
        // children.push_back(right);
        _token = t;

        _nodetype = forNode;
        //_token._keyword = th._keyword;

        visitNode = _visitNodeFor;
    }
        NodeFor(token *t,string _target) // token th
    {
        // children.push_back(right);
        // children.push_back(right);
        _token = t;

        _nodetype = forNode;
        target=_target;
        //_token._keyword = th._keyword;

        visitNode = _visitNodeFor;
    }
};



void _visitNodeComparatorFunction(NodeToken *nd)
{
    string f = "";
    prt_r fleft;
    prt_r fright;
    prt_r f1;
    int numl = register_numl.get();
    if (nd->getChildAtPos(0)->visitNode != NULL)
        nd->getChildAtPos(0)->visitNode(nd->getChildAtPos(0));
    int leftl = register_numl.get();
    if (nd->getChildAtPos(1)->visitNode != NULL)
        nd->getChildAtPos(1)->visitNode(nd->getChildAtPos(1));

    ////printf("compare %s %s\n",tokenNames[nd->_token->type ].c_str(),nd->_token->text.c_str());
    string compop = "";
    // to compose
    int h;
    switch (nd->_token->type)
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
    content.addAfter(string_format("%s a%d,a%d,%s_if", compop.c_str(), numl, leftl, nd->target.c_str()));
    content.addAfter(string_format("j %s_end", nd->target.c_str()));
    content.addAfter(string_format("%s_if:", nd->target.c_str()));

    register_numl.increase();

    return;
}

class NodeComparator : public NodeToken
{
public:
    NodeComparator()
    {
        _token = NULL;
        _nodetype = comparatorNode;
        visitNode = _visitNodeComparatorFunction;
    };

    NodeComparator(token *t) // token th
    {

        _token = t;
        _nodetype = comparatorNode;
        visitNode = _visitNodeComparatorFunction;
    }
};
void _visitNodeReturn(NodeToken *nd)
{

    NodeToken *t = nd;
    while (t->_nodetype != defFunctionNode and t->parent != NULL)
    {
        t = t->parent;
    }

    content.addAfter(string_format("l32r a%d,stackr", point_regnum));
    for (int i = 0; i < nd->children.size(); i++)
    {
        globalType.push(t->_token->_vartype->_varType);
        register_numl.duplicate();
        nd->getChildAtPos(i)->visitNode(nd->getChildAtPos(i));

        register_numl.pop();

        int start = nd->stack_pos + t->_token->_vartype->total_size;

        int tot = t->_token->_vartype->size - 1;
        for (int j = 0; j < t->_token->_vartype->size; j++)
        {
            start -= t->_token->_vartype->sizes[tot - j];
            asmInstruction asmInstr = t->_token->_vartype->store[tot - j];
            content.addAfter(content.sp.pop(), string_format("%s %s%d,%s%d,%d", asmInstructionsName[asmInstr].c_str(), getRegType(asmInstr, 0).c_str(), register_numl.get(), getRegType(asmInstr, 1).c_str(), point_regnum, start));


        }
        globalType.pop();
    }

    content.addAfter("retw.n");
}

class NodeReturn : public NodeToken
{
public:
    NodeReturn()
    {
        _token = NULL;
        _nodetype = returnNode;
        visitNode = _visitNodeReturn;
    };
};
void clearContext(Context *_cntx)
{
    ////printf("clearContect\n");
    for (list<Context>::iterator it = _cntx->children.begin(); it != _cntx->children.end(); ++it)
    {
        clearContext(&*it);
    }


    _cntx->functions.clear();
    _cntx->variables.clear();
    _cntx->children.clear();
    _cntx->parent = NULL;
    _cntx->name = " ";

}

void clearNodeToken(NodeToken *nd)
{

    _functions.clear();
    for (list<NodeToken>::iterator it = nd->children.begin(); it != nd->children.end(); ++it)
    {
        if (&*it != NULL)
            clearNodeToken(&*it);
    }

    nd->children.clear();

}

void buildParents(NodeToken *nd)
{
    if (nd->children.size() > 0)
    {
        for (list<NodeToken>::iterator it2 = nd->children.begin(); it2 != nd->children.end(); ++it2)
        {
            (&*it2)->parent = nd;
            buildParents(&*it2);
        }
    }
}