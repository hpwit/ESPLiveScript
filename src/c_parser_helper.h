
#include <string>
#include "tokenizer.h"
#pragma once
using namespace std;

/*
Helper functions
*/

/*
fin helper fiunctiopns
*/
int stack_size = 0;
int forif_num=0;
int block_statement_num=0;
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
    callfunctionNode,
    forNode,
};

string nodeTypeNames[] =
    {
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
        "callfunctionNode",
        "forNode",        
};

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

class Context;

class NodeToken
{
public:
    NodeToken() {}

    NodeToken(token *t)
    {
        _token = t;
    }
    NodeToken * addChild(NodeToken t)
    {
        t.parent = this;
       // t._global = _global;
        children.push_back(t);
        list<NodeToken>::iterator it=children.end();
        it--;
        return &*it;
    }
    // virtual string value();

    nodeType _nodetype;
    list<NodeToken> children;
    token *_token;
    //statementType stmttype;
    prt_r (*visitNode)(NodeToken nd, string ident, int register_numl, int register_numr) = NULL;
    NodeToken *target;
    NodeToken *parent;
    //Context *cntx;
    uint8_t stack_pos;
    bool isPointer;
    uint16_t _total_size;
    //string target;
    bool isExternal;

};

class Context
{
public:
    Context(){
        name="";
    };
    list<NodeToken*> functions;
    list<NodeToken*> variables;
    varTypeEnum current_varType;
    Context *parent = NULL;
    list<Context> children;
    string name;
    Context *_global = NULL;

    Context *addChild(Context cntx)
    {

        cntx.parent = this;
        cntx.name = name + "_" + cntx.name;

        cntx._global = _global;
        children.push_back(cntx);
        list<Context>::iterator it=children.end();
        it--;
        return &*it;
    }

    void display(string id)
    {
        //printf("%scontext:%s\n", id.c_str(), name.c_str());
        for (int i = 0; i < variables.size(); i++)
        {
            //printf("%s\t%s %s %d\n", id.c_str(), variables[i]._token.text.c_str(), nodeTypeNames[variables[i]._nodetype].c_str(), variables[i]._token._vartype.external_var);
        }
        for (int i = 0; i < children.size(); i++)
        {
            display(id + "   ");
        }
    }
    void addVariable(NodeToken *nd)
    {
        nd->_token->text = name + "_" + nd->_token->text;
        variables.push_back(nd);
    }
    void addFunction(NodeToken *nd)
    {
         nd->_token->text = name + "_" + nd->_token->text;
        functions.push_back(nd);
    }
    NodeToken *findVariable(token *t)
    {

        printf("lokking for variable |%s|\n", (name + "_" + t->text).c_str());
        for( list<NodeToken*>::iterator it=variables.begin();it!=variables.end();++it)
        {
            //printf("is equalt to |%s|\n", variables[i]._token.text.c_str());
             
            if ((*it)->_token->text.compare(name + "_" + t->text) == 0)
            {
                return *it;
            }
        }
        if (parent != NULL)
        {
            return parent->findVariable(t);
        }
        printf("no variebale found\n");
        return NULL;
    }
        NodeToken *findFunction(token *t)
    {

        //printf("lokking for function |%s|\n", (name + "_" + t.text).c_str());
        for( list<NodeToken*>::iterator it=functions.begin();it!=functions.end();++it)
        {
            //printf("is equalt to |%s|\n", variables[i]._token.text.c_str());
             
            if ((*it)->_token->text.compare(name + "_" + t->text) == 0)
            {
                return *it;
            }
        }
        if (parent != NULL)
        {
            return parent->findFunction(t);
        }
        //printf("no function found\n");
        return NULL;
    }
};


typedef struct
{
    f_error_struct error;
    NodeToken _nd;
} resParse;


class NodeDefFunction : public NodeToken
{
public:
    NodeDefFunction()
    {
        _nodetype = defFunctionNode;
        //visitNode = visitNodeFunctionNode;
    }
    NodeDefFunction(token *t)
    {
        _token=t;
         _nodetype = defFunctionNode;
    }
    NodeDefFunction(NodeToken sts, token *t)
    {
        _token = t;
        _nodetype = defFunctionNode;
        children.push_back(sts);
       // visitNode = visitNodeFunctionNode;
    }
};

class NodeProgram : public NodeToken
{
public:
    NodeProgram()
    {
        _nodetype = programNode;
        //visitNode = visitNodeProgramNode;
    }
};


Context *current_cntx;
