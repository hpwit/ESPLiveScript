#include <regex.h>
#include <stdio.h>
#include <list>
#include <string>
#include <memory>

#pragma once
using namespace std;
// #include "asm_parser.h"
#include "asm_struct_enum.h"

#define EOF_TEXT 0

#ifndef ARDUINO
/*
template <typename... Args>
std::string string_format(const std::string &format, Args... args)

{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    if (size_s <= 0)
    {
        throw std::runtime_error("Error during formatting.");
    }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
*/
#endif

enum varTypeEnum
{
    __none__,
    __uint8_t__,
    __uint16_t__,
    __uint32_t__,
    __int__,
    __float__,
    __void__,
    __CRGB__,
    __CRGBW__,
    __char__,
    __userDefined__

};

struct varType
{
    varTypeEnum _varType;
    string varName;
    uint8_t _varSize;
    asmInstruction load[20];
    asmInstruction store[20];
    string membersNames[10];
    uint8_t starts[20];
    
    // asmInstruction add;
    // asmInstruction mul;
    // asmInstruction div;
   // string reg_name;
    uint8_t sizes[20];
    uint8_t memberSize[20];
    //string vnames[20];
varTypeEnum types[20];
    uint8_t size;
    uint8_t total_size;
};
varType usded;

vector<varType> _userDefinedTypes;
varType __v;
string varTypeEnumNames[] =
    {
#ifdef __TEST_DEBUG
        "__none__",
        "__unit8_t__",
        "__unit16_t__",
        "__unit32_t__",
        "__int__",
        "__float__",
        "__void__",
        "__CRGB__",
        "__CRGBW__",
        "__char__",
         "__userDefined__",
#endif

};

vector<string> userDefinedVarTypeNames;

varType _varTypes[] =
    {
        {
            ._varType = __none__,
        },
        {._varType = __uint8_t__,
         ._varSize = 1,
         .load={l8ui},
         .store = {s8i},
        // .reg_name = "a",
         .sizes = {1},
         .size = 1,
         .total_size = 1
         },
        {
            ._varType = __uint16_t__,
            ._varSize = 2,
            .load={l16ui},
            .store = {s16i},
           // .reg_name = "a",
            .sizes = {2},
            .size = 1,
            .total_size = 2,
        },
        {
            ._varType = __uint32_t__,
            ._varSize = 4,
            .load={l32i},
            .store = {s32i},
           // .reg_name = "a",
            .sizes = {4},
            .size = 1,
            .total_size = 4,
        },
        {
            ._varType = __int__,
            ._varSize = 2,
            .load={l16si},
            .store = {s16i},
           // .reg_name = "a",
            .sizes = {2},
            .size = 1,
            .total_size = 2,
        },
        {
            ._varType = __float__,
            ._varSize = 4,
            .load = {lsi},
            .store = {ssi},
           // .reg_name = "f",
            .sizes = {4},
            .size = 1,
            .total_size = 4,
        },
        {
            ._varType = __void__,
            ._varSize = 0,
            .load = {},
            .store = {},
           // .reg_name = "a",
            .sizes = {0},
            .size = 0,
            .total_size = 0,
        },
        {
            ._varType = __CRGB__,
            ._varSize = 3,
            .load = {l8ui, l8ui, l8ui},
            .store = {s8i, s8i, s8i},
           // .reg_name = "a",
            .sizes = {1, 1, 1},
            .size = 3,
            .total_size = 3,
        },
        {
            ._varType = __CRGBW__,
            ._varSize = 4,
            .load = {l8ui, l8ui, l8ui, l8ui},
            .store = {s8i, s8i, s8i, s8i},
           // .reg_name = "a",
            .sizes = {1, 1, 1, 1},
            .size = 4,
            .total_size = 4,
        },
        {
            ._varType = __char__,
            ._varSize = 1,
            .load = {l8ui},
            .store = {s8i},
           // .reg_name = "a",
            .sizes = {1},
            .size = 1,
            .total_size = 1,
        }

};

string keywordTypeNames[] =
    {
#ifdef __TEST_DEBUG
        "KeywordVarType",
        "KeywordVarType",
        "KeywordVarType",
        "KeywordVarType",
        "KeywordVarType",
        "KeywordVarType",
        "KeywordVarType",
        "KeywordVarType",
        "KeywordVarType",
        "KeywordExternalVar",
        "KeywordFor",
        "KeywordIf",
        "KeywordThen",
        "KeywordElse",
        "KeywordWhile",
        "KeywordReturn",
        "KeywordImport",
        "KeywordFrom",
        "KeywordASM",
        "KeywordDefine",
        "KeywordSafeMode",
        "KeywordHeader",
        "KeywordContent",
#endif

};

#define nb_keywords 33
#define nb_typeVariables 10
string keyword_array[nb_keywords] = {"none", "uint8_t", "uint16_t", "uint32_t", "int", "float", "void", "CRGB", "CRGBW", "char", "external", "for", "if", "then", "else", "while", "return", "import", "from", "__ASM__", "define", "safe_mode", "_header_", "_content_", "and", "or", "continue", "break", "fabs", "abs", "save_reg", "save_reg_abs","struct"};
bool __isBlockComment = false;
enum tokenType
{
    TokenNumber,
    TokenAddition,
    TokenStar,
    TokenSubstraction,
    TokenOpenParenthesis,
    TokenCloseParenthesis,
    TokenOpenBracket,
    TokenCloseBracket,
    TokenOpenCurlyBracket,
    TokenCloseCurlyBracket,
    TokenEqual,
    TokenDoubleEqual,
    TokenIdentifier,
    TokenSemicolon,
    TokenUnknown,
    TokenSpace,
    TokenNewline,
    TokenEndOfFile,
    TokenSlash,
    TokenKeyword,
    TokenString,
    TokenExternal,
    TokenComma,
    TokenLessThan,
    TokenPlusPlus,
    TokenMinusMinus,
    TokenModulo,
    TokenLessOrEqualThan,
    TokenMoreThan,
    TokenMoreOrEqualThan,
    TokenNotEqual,
    TokenNot,
    TokenFunction,
    TokenUppersand,
    TokenDiese,
    TokenLineComment,
    TokenStartBlockComment,
    TokenEndBlockComment,
    TokenNegation,
    TokenShiftLeft,
    TokenShiftRight,
    TokenKeywordVarType,
    TokenKeywordExternalVar,
    TokenKeywordFor,
    TokenKeywordIf,
    TokenKeywordThen,
    TokenKeywordElse,
    TokenKeywordWhile,
    TokenKeywordReturn,
    TokenKeywordImport,
    TokenKeywordFrom,
    TokenKeywordASM,
    TokenKeywordDefine,
    TokenKeywordSafeMode,
    TokenKeywordHeader,
    TokenKeywordContent,
    TokenKeywordAnd,
    TokenKeywordOr,
    TokenPower,
    TokenKeywordContinue,
    TokenKeywordBreak,
    TokenKeywordFabs,
    TokenKeywordAbs,
    TokenKeywordSaveReg,
    TokenKeywordSaveRegAbs,
    TokenKeywordStruct,
    TokenUserDefinedName,
    TokenUserDefinedVariable,
    TokenMember,

};

tokenType __keywordTypes[] =
    {
        TokenKeywordVarType,
        TokenKeywordVarType,
        TokenKeywordVarType,
        TokenKeywordVarType,
        TokenKeywordVarType,
        TokenKeywordVarType,
        TokenKeywordVarType,
        TokenKeywordVarType,
        TokenKeywordVarType,
        TokenKeywordVarType,
        TokenKeywordExternalVar,
        TokenKeywordFor,
        TokenKeywordIf,
        TokenKeywordThen,
        TokenKeywordElse,
        TokenKeywordWhile,
        TokenKeywordReturn,
        TokenKeywordImport,
        TokenKeywordFrom,
        TokenKeywordASM,
        TokenKeywordDefine,
        TokenKeywordSafeMode,
        TokenKeywordHeader,
        TokenKeywordContent,
        TokenKeywordAnd,
        TokenKeywordOr,
        TokenKeywordContinue,
        TokenKeywordBreak,
        TokenKeywordFabs,
        TokenKeywordAbs,
        TokenKeywordSaveReg,
        TokenKeywordSaveRegAbs,
        TokenKeywordStruct,

    };

string tokenNames[] = {
#ifdef __TEST_DEBUG
    "TokenNumber",
    "TokenAddition",
    "TokenStar",
    "TokenSubstraction",
    "TokenOpenParenthesis",
    "TokenCloseParenthesis",
    "TokenOpenBracket",
    "TokenCloseBracket",
    "TokenOpenCurlyBracket",
    "TokenCloseCurlyBracket",
    "TokenEqual",
    "TokenDoubleEqual",
    "TokenIdentifier",
    "TokenSemicolon",
    "TokenUnknown",
    "TokenSpace",
    "TokenNewline",
    "TokenEndOfFile",
    "TokenSlash",
    "TokenKeyword",
    "TokenString",
    "TokenExternal",
    "TokenComma",
    "TokenLessThan",
    "TokenPlusPlus",
    "TokenMinusMinus",
    "TokenModulo",
    "TokenLessOrEqualThan",
    "TokenMoreThan",
    "TokenMoreOrEqualThan",
    "TokenNotEqual",
    "TokenNot",
    "TokenFunction",
    "TokenUppersand",
    "TokenDiese",
    "TokenLineComment",
    "TokenStartBlockComment",
    "TokenEndBlockComment",
    "TokenNegation",
    "TokenShiftLeft",
    "TokenShiftRight",
    "TokenKeywordVarType",
    "TokenKeywordExternalVar",
    "TokenKeywordFor",
    "TokenKeywordIf",
    "TokenKeywordThen",
    "TokenKeywordElse",
    "TokenKeywordWhile",
    "TokenKeywordReturn",
    "TokenKeywordImport",
    "TokenKeywordFrom",
    "TokenKeywordASM",
    "TokenKeywordDefine",
    "TokenKeywordSafeMode",
    "TokenKeywordHeader",
    "TokenKeywordContent",
    "TokenKeywordAnd",
    "TokenKeywordOr",
    "TokenPower",
    "TokenKeywordContinue",
    "TokenKeywordBreak",
    "TokenKeywordFabs",
    "TokenKeywordAbs",
    "TokenKeywordSaveReg",
    "TokenKeywordSaveRegAbs",
    "TokenKeywordStruct",
    "TokenUserDefinedName",
    "TokenUserDefinedVariable",
    "TokenMember",

#endif
};

typedef struct
{
    string name;
    string content;
} _define;

list<_define> define_list;

string getDefine(string name)
{
    for (list<_define>::iterator it = define_list.begin(); it != define_list.end(); ++it)
    {
        if ((*it).name.compare(name) == 0)
        {
            // printf("one rrent %s\n",(*it).content.c_str());
            return (*it).content;
        }
    }
    return "";
}

#ifdef __CONSOLE_ESP32
const char *tokenFormat[] = {
    termColor.Yellow,   // TokenNumber,
    termColor.BWhite,   // TokenAddition,
    termColor.BWhite,   // TokenStar,
    termColor.BWhite,   // TokenSubstraction,
    termColor.BWhite,   // TokenOpenParenthesis,
    termColor.BWhite,   //  TokenCloseParenthesis,
    termColor.BWhite,   // TokenOpenBracket,
    termColor.BWhite,   // TokenCloseBracket,
    termColor.LMagenta, //  TokenOpenCurlyBracket,
    termColor.LMagenta, //  TokenCloseCurlyBracket,
    termColor.BWhite,   //  TokenEqual,
    termColor.BWhite,   //  TokenDoubleEqual,
    termColor.Cyan,     //  TokenIdentifier,
    termColor.BWhite,   //  TokenSemicolon,
    termColor.BWhite,   // TokenUnknown,
    termColor.BWhite,   //  TokenSpace,
    termColor.BWhite,   //  TokenNewline,
    termColor.BWhite,   // TokenEndOfFile,
    termColor.BWhite,   // TokenSlash,
    termColor.Magenta,  //   TokenKeyword,
    termColor.Orange,   // TokenString,
    termColor.LMagenta, //   TokenExternal,
    termColor.BWhite,   //  TokenComma,
    termColor.BWhite,   //   TokenLessThan,
    termColor.BWhite,   // TokenPlusPlus,
    termColor.BWhite,   // TokenMinusMinus,
    termColor.BWhite,   // TokenModulo,
    termColor.BWhite,   // TokenLessOrEqualThan,
    termColor.BWhite,   // TokenMoreThan,
    termColor.BWhite,   // TokenMoreOrEqualThan,
    termColor.BWhite,   // TokenNotEqual,
    termColor.BWhite,   // TokenNot,
    termColor.Yellow,   // TokenFunction
    termColor.BWhite,   // TokenUppersand
    termColor.BWhite,   // TokenDiese
    termColor.Grey,     // TokenLineComment
    termColor.Grey,     //  TokenStartBlockComment
    termColor.Grey,     // TokenEndBlockComment
    termColor.BWhite,   // TokenNegation
    termColor.BWhite,   // TokenShiftLeft
    termColor.BWhite,   // TokenShiftRight
    termColor.Green,    // KeywordVarType,
    termColor.Green,    // KeywordExternalVar,
    termColor.Magenta,  // KeywordFor,
    termColor.Magenta,  // KeywordIf,
    termColor.Magenta,  // KeywordThen,
    termColor.Magenta,  // KeywordElse,
    termColor.Magenta,  // KeywordWhile,
    termColor.Magenta,  // KeyWordReturn
    termColor.LMagenta, // KeyWordImport
    termColor.LMagenta, // KeyWordFrom
    termColor.LMagenta, // KeyWordASM
    termColor.LMagenta, // KeyWordefine
    termColor.BCyan,    // KeywordsafeMmode
    termColor.BCyan,    // KeywordHeader
    termColor.BCyan,    // KeywordContent
    termColor.Magenta,  // Keywordand,
    termColor.Magenta,  // Keywordor,
    termColor.Magenta,  // TokenPower
    termColor.Magenta,  // Keywordcontinue,
    termColor.Magenta,  // Tokenbreak
    termColor.BWhite,   // Tokenfabs
    termColor.BWhite,   // Tokenabs
    termColor.BCyan,    // Keywordsave_reg
    termColor.BCyan,    // Keywordsave_regabs
     termColor.LMagenta,   //TokenKeywordStruct,
     termColor.Cyan, //TokenUserDefinedName,
    termColor.Green ,//TokenUserDefinedVariable,
        termColor.BWhite,   // TokenMember
};
/*
const char *KeywordTypeFormat[] =
    {
        termColor.Green,    // KeywordVarType,
        termColor.Green,    // KeywordExternalVar,
        termColor.Magenta,  // KeywordFor,
        termColor.Magenta,  // KeywordIf,
        termColor.Magenta,  // KeywordThen,
        termColor.Magenta,  // KeywordElse,
        termColor.Magenta,  // KeywordWhile,
        termColor.Magenta,  // KeyWordReturn
        termColor.LMagenta, // KeyWordImport
        termColor.LMagenta, // KeyWordFrom
        termColor.LMagenta, // KeyWordASM
        termColor.LMagenta, // KeyWordefine
        termColor.BCyan,    // KeywordsafeMmode
        termColor.BCyan,    // KeywordHeader
        termColor.BCyan,    // KeywordContent
};
*/
#endif
typedef struct
{
    // switch to uin8_t
    tokenType type;
    varType *_vartype;

    // varTypeEnum _varType;

    // needs to find a solution for this maybe a pointer to list string et on ne garde que les identifiers ...
    string text;

    uint8_t line;

    // switch to uin8_t unn ligne ne fera pas plus de 256 caracteres
    // uint8_t pos;

    // switch to uin8_t
    // KeywordType _keyword;

    // possible sizee  1+4+4+1+1+4=15 au lieu de 44 ...

} token;

void displaytoken(token t)
{
    // printf("%s\t%d\t%f\t%s%d %d\t%s\n", tokenNames[t.type].c_str(), t.int_value, t.float_value, t.text.c_str(), t.line, //t.pos, keywordTypeNames[t._keyword].c_str());
}
token transNumber(string str)
{
    token t;
    // t.float_value=0;
    // t.int_value=0;
    if (str.find(".") != string::npos)
    {
        // try convert
        /*
        float f;
        f = 0;
        float div = 10;
        bool past_point = false;
        for (int i = 0; i < str.size(); i++)
        {
            if (str[i] == '.')
            {
                past_point = true;
            }
            else
            {
                if (past_point == true)
                {
                    f += (float)(str[i] - 48) / div;
                    div = div * 10;
                }
                else
                {
                    f = 10 * f + (str[i] - 48);
                }
            }
        }*/
        // t.float_value = f;
        t.type = TokenNumber;
        t._vartype = &_varTypes[(int)__float__];
        t.text = str;
        return t;
    }
    // on traite un integer
    /*
    uint16_t res;
    for (int i = 0; i < str.size(); i++)
    {
        res = 10 * res + (str[i] - 48);
    }*/
    // t.uint_value = res;
    t.type = TokenNumber;
    t._vartype = &_varTypes[(int)__uint16_t__];
    t.text = str;

    return t;
}

int isKeyword(string str)
{
    for (int i = 0; i < nb_keywords; i++)
    {
        if (keyword_array[i].compare(str) == 0)
        {
            return i;
        }
    }

    return -1;
}
int isUserDefined(string str)
{
    for (int i = 0; i < userDefinedVarTypeNames.size(); i++)
    {
        if (userDefinedVarTypeNames[i].compare(str) == 0)
        {
            return i;
        }
    }

    return -1;
}

bool isIna_zA_Z_(unsigned char c)
{
    if (c >= 97 && c <= 122)
    {
        return true;
    }
    if (c >= 65 && c <= 90)
    {
        return true;
    }
    if (c == '_')
    {
        return true;
    }
    return false;
}

bool isIna_zA_Z_0_9(unsigned char c)
{
    if (c >= 97 && c <= 122)
    {
        return true;
    }
    if (c >= 65 && c <= 90)
    {
        return true;
    }
    if (c == '_')
    {
        return true;
    }
    if (c >= 48 && c <= 57)
    {
        return true;
    }
    return false;
}

bool isIn0_9(unsigned char c)
{
    if (c >= 48 && c <= 57)
    {
        return true;
    }
    return false;
}
bool isIn0_9_x_b(unsigned char c)
{
    if (c >= 48 && c <= 57)
    {
        return true;
    }
    if (c == 'b')
    {
        return true;
    }
    if (c >= 'A' && c <= 'F')
    {
        return true;
    }
    if (c >= 'a' && c <= 'f')
    {
        return true;
    }
    if (c == 'x')
    {
        return true;
    }
    if (c == '.')
    {
        return true;
    }
    return false;
}

bool _for_display = false;
class Script
{
public:
    Script(string *input)
    {
        script = input;
        position = -1;
    }
    char nextChar()
    {
        // //printf("%d\n",position);
        if ((position + 1) < (*script).size())
        {
            position++;
            return (*script)[position];
        }
        else
        {
            position++;
            return EOF_TEXT;
        }
    }

    char currentChar()
    {
        if (position < (*script).size())
        {
            // position++;
            return (*script)[position];
        }
        else
        {
            return EOF_TEXT;
        }
    }

    char previousChar()
    {
        if ((position - 1) >= 0)
        {
            position--;
            return (*script)[position];
        }
        else
        {
            return EOF_TEXT;
        }
    }
    void insert(string toInsert)
    {
        position = position + 1;
        script->insert(position, toInsert.c_str());
        // printf("%s\n",script->c_str());
    }

private:
    string *script;
    int position;
};

list<token> list_of_token;

class Tokens
{
public:
    Tokens()
    {
        position = 0;
        _tokens = &list_of_token;
    }
    int size()
    {
        return _tokens->size();
    }
    token *getTokenAtPos(int pos)
    {
        int i = 0;
        if (pos >= _tokens->size() || pos < 0)
        {
            return &end_token;
        }
        for (list<token>::iterator it = _tokens->begin(); it != _tokens->end(); ++it)
        {
            if (i == pos)
            {

                return &*it;
            }
            i++;
        }
        return &end_token;
    }
    token *current()
    {
        return getTokenAtPos(position);
    }
    token *next()
    {
        if (position + 1 >= _tokens->size())
        {
            position = _tokens->size() - 1;
        }
        else
        {
            position++;
        }
        return getTokenAtPos(position);
    }
    token *prev()
    {
        if (position > 0)
        {
            position--;
        }
        return getTokenAtPos(position);
    }
    token *peek(int index)
    {
        if (position + index < _tokens->size() && position + index >= 0)
        {
            return getTokenAtPos(position + index);
        }
        else
        {
            return &end_token;
        }
    }
    void init()
    {
        position = 0;
    }

    bool Match(tokenType tt)
    {
        token *g = current();
        if (g->type == tt)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool Match(tokenType tt, int index)
    {
        token *g = peek(index);
        if (g->type == tt)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    /*
    bool MatchKeyword(KeywordType t)
    {
        if (current()->_keyword == t)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool MatchKeyword(KeywordType t, int offset)
    {
        if (peek(offset)->_keyword == t)
        {
            return true;
        }
        else
        {
            return false;
        }
    }*/
    void clear()
    {
        _tokens->clear();
    }
    int position;

private:
    list<token> *_tokens;
    token end_token = {.type = TokenEndOfFile};
};

Tokens _tks;

int _token_line;
list<token>::iterator _index_token;
void tokenizer(Script *script, bool update, bool increae_line)
{

    // list<token> list_of_token;
    // int line = 1;
   
   
    int pos = 0;
    if (update)
    {
         userDefinedVarTypeNames.clear();
        list_of_token.clear();
        _token_line = 1;
        _index_token = list_of_token.end();
        define_list.clear();
        __isBlockComment = false;
    }

    while (script->nextChar() != EOF_TEXT)
    {

        pos++;
        char c = script->currentChar();
        // printf("line : %d pos:%d char:%c token size:%d %d\n",line,pos,c,list_of_token.size(),heap_caps_get_free_size(MALLOC_CAP_8BIT));
        if (c == '=')
        {
            char c2 = script->nextChar();
            if (c2 == '=')
            {
                token t;
                t._vartype = NULL;
                t.type = TokenDoubleEqual;
                if (_for_display)
                    t.text = "==";
                t.line = _token_line;
                // t.pos = pos;
                list_of_token.insert(_index_token, t);

                continue;
            }
            else
            {
                script->previousChar();
                token t;
                t._vartype = NULL;
                t.type = TokenEqual;
                t.line = _token_line;
                // t.pos = pos;
                if (_for_display)
                    t.text = "=";
                list_of_token.insert(_index_token, t);
                continue;
            }
        }
        if (c == '<')
        {
            char c2 = script->nextChar();
            if (c2 == '=')
            {
                token t;
                t._vartype = NULL;
                t.type = TokenLessOrEqualThan;
                if (_for_display)
                    t.text = "<=";
                t.line = _token_line;
                // t.pos = pos;
                list_of_token.insert(_index_token, t);
                continue;
            }
            else if (c2 == '<')
            {
                token t;
                t._vartype = NULL;
                t.type = TokenShiftLeft;
                if (_for_display)
                    t.text = "<<";
                t.line = _token_line;
                // t.pos = pos;
                list_of_token.insert(_index_token, t);
                continue;
            }
            else
            {
                script->previousChar();
                token t;
                t._vartype = NULL;
                t.type = TokenLessThan;
                t.line = _token_line;
                // t.pos = pos;
                if (_for_display)
                    t.text = "<";
                list_of_token.insert(_index_token, t);
                continue;
            }
        }
        if (c == '>')
        {
            char c2 = script->nextChar();
            if (c2 == '=')
            {
                token t;
                t._vartype = NULL;
                t.type = TokenMoreOrEqualThan;
                if (_for_display)
                    t.text = ">=";
                t.line = _token_line;
                // t.pos = pos;
                list_of_token.insert(_index_token, t);
                continue;
            }
            else if (c2 == '>')
            {
                token t;
                t._vartype = NULL;
                t.type = TokenShiftRight;
                if (_for_display)
                    t.text = ">>";
                t.line = _token_line;
                // t.pos = pos;
                list_of_token.insert(_index_token, t);
                continue;
            }
            else
            {
                script->previousChar();
                token t;
                t._vartype = NULL;
                t.type = TokenMoreThan;
                t.line = _token_line;
                // t.pos = pos;
                if (_for_display)
                    t.text = ">";
                list_of_token.insert(_index_token, t);
                continue;
            }
        }
        if (c == '!')
        {
            char c2 = script->nextChar();
            if (c2 == '=')
            {
                token t;
                t._vartype = NULL;
                t.type = TokenNotEqual;
                if (_for_display)
                    t.text = "!=";
                t.line = _token_line;
                // t.pos = pos;
                list_of_token.insert(_index_token, t);
                continue;
            }
            else
            {
                script->previousChar();
                token t;
                t._vartype = NULL;
                t.type = TokenNot;
                t.line = _token_line;
                // t.pos = pos;
                if (_for_display)
                    t.text = "!";
                list_of_token.insert(_index_token, t);
                continue;
            }
        }
        if (c == '+')
        {
            char c2 = script->nextChar();
            if (c2 == '+')
            {
                token t;
                t._vartype = NULL;
                t.type = TokenPlusPlus;
                if (_for_display)
                    t.text = "++";
                t.line = _token_line;
                // t.pos = pos;
                list_of_token.insert(_index_token, t);
                continue;
            }
            else
            {
                script->previousChar();
                token t;
                t._vartype = NULL;
                t.type = TokenAddition;
                if (_for_display)
                    t.text = "+";
                t.line = _token_line;
                // t.pos = pos;
                list_of_token.insert(_index_token, t);
                continue;
            }
        }

        if (isIna_zA_Z_(c))
        {
            string v;
            int newpos = pos;
            while (isIna_zA_Z_0_9(c))
            {
                v += c;
                newpos++;
                c = script->nextChar();
            }
            script->previousChar(); // on revient un caractere en arriere
            token t;
            // t._vartype=NULL;
            t.line = _token_line;
            // t.pos = pos;
            if (isKeyword(v) > -1)
            {
                // printf("keyword;%s\n",v.c_str());
               // t.type = TokenKeyword;
                t.type = __keywordTypes[isKeyword(v)];
                if (isKeyword(v) < nb_typeVariables)
                    t._vartype = &_varTypes[isKeyword(v)];
                if (t.type == TokenKeywordExternalVar)
                {
                    t.type = TokenExternal;
                }
            }
            else if(isUserDefined(v)>-1)
            {
                t.type=TokenUserDefinedVariable;
                t._vartype=&_varTypes[(int)__userDefined__];
            }
            else
            {
                t.type = TokenIdentifier;
                if (list_of_token.size() > 0)
                {
                    token prev = list_of_token.back();
                    if (prev.type == TokenKeywordImport && !_for_display)
                    {
                        if (findLibFunction(v) > -1)
                        {
                            list_of_token.pop_back();
                            add_on.push_back(findLibFunction(v));
                            continue;
                        }
                    }
                    else if (prev.type == TokenKeywordDefine && !_for_display)
                    {
                        list_of_token.pop_back();
                        _define newdef;
                        newdef.name = v;
                        newdef.content = "";
                        char c2;
                        c2 = script->nextChar();
                        // c2 = script->nextChar();
                        while (c2 != '\n' and c2 != EOF_TEXT)
                        {
                            newdef.content = newdef.content + c2;
                            c2 = script->nextChar();
                        }
                        // printf("ona joute %s\n",newdef.content.c_str());
                        define_list.push_back(newdef);
                        if (increae_line)
                            _token_line++;
                        continue;
                    }
                    else if(prev.type == TokenKeywordStruct && !_for_display)
                    {
                        userDefinedVarTypeNames.push_back(v);
                        t.type=TokenUserDefinedName;
                        //continue;
                    }
                }
                if (!_for_display) // on ne remplace pas lorsque l'on display
                {
                    if (getDefine(v) != "")
                    {
                        script->insert(getDefine(v));
                        continue;
                    }
                }

            }
            pos = newpos - 1;
                t.text = v;
                list_of_token.insert(_index_token, t);
            continue;
        }

        if (isIn0_9(c))
        {
            // //printf("on a %c\n",c);
            string value = "";
            int newpos = pos;
            while (isIn0_9_x_b(c))
            {
                value += c;
                c = script->nextChar();
                newpos++;
            }
            script->previousChar(); // on revient un caractere en arriere
            token t = transNumber(value);
            //  t._vartype=NULL;
            t.line = _token_line;
            // t.pos = pos;
            list_of_token.insert(_index_token, t);
            pos = newpos - 1;
            continue;
        }
        if (c == ';')
        {
            token t;
            t.type = TokenSemicolon;
            t._vartype = NULL;
            if (_for_display)
                t.text = ";";
            t.line = _token_line;
            // t.pos = pos;
            list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == '\t')
        {
            token t;
            t.type = TokenSpace;
            t._vartype = NULL;
            if (_for_display)
                t.text = "\t";
            t.line = _token_line;
            // t.pos = pos;
            if (_for_display)
                list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == '&')
        {
            token t;
            t.type = TokenUppersand;
            t._vartype = NULL;
            if (_for_display)
                t.text = "&";
            t.line = _token_line;
            // t.pos = pos;
            list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == '#')
        {
            token t;
            t.type = TokenDiese;
            t._vartype = NULL;
            if (_for_display)
                t.text = "#";
            t.line = _token_line;
            // t.pos = pos;
            list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == '(')
        {
            token t;
            t.type = TokenOpenParenthesis;
            t._vartype = NULL;
            if (_for_display)
                t.text = "(";
            t.line = _token_line;
            // t.pos = pos;
            list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == '%')
        {
            token t;
            t.type = TokenModulo;
            t._vartype = NULL;
            if (_for_display)
                t.text = "%";
            t.line = _token_line;
            // t.pos = pos;
            list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == ')')
        {
            token t;
            t.type = TokenCloseParenthesis;
            t._vartype = NULL;
            if (_for_display)
                t.text = ")";
            t.line = _token_line;
            // t.pos = pos;
            list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == '{')
        {
            token t;
            t.type = TokenOpenCurlyBracket;
            t._vartype = NULL;
            if (_for_display)
                t.text = "{";
            t.line = _token_line;
            // t.pos = pos;
            list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == '}')
        {
            token t;
            t.type = TokenCloseCurlyBracket;
            t._vartype = NULL;
            if (_for_display)
                t.text = "}";
            t.line = _token_line;
            // t.pos = pos;
            list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == '[')
        {
            token t;
            t.type = TokenOpenBracket;
            t._vartype = NULL;
            if (_for_display)
                t.text = "[";
            t.line = _token_line;
            // t.pos = pos;
            list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == ']')
        {
            token t;
            t.type = TokenCloseBracket;
            t._vartype = NULL;
            if (_for_display)
                t.text = "]";
            t.line = _token_line;
            // t.pos = pos;
            list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == '/')
        {
            char c2 = script->nextChar();
            if (c2 == '/')
            {
                token t;
                t._vartype = NULL;
                t.type = TokenLineComment;
                if (_for_display)
                    t.text = "//";
                c2 = script->nextChar();
                while (c2 != '\n' and c2 != EOF_TEXT)
                {
                    t.text = t.text + c2;
                    c2 = script->nextChar();
                }
                t.line = _token_line;
                // t.pos = pos;
                if (increae_line)
                    _token_line++;
                if (_for_display)
                    list_of_token.insert(_index_token, t);
                continue;
            }
            else if (c2 == '*')
            {
                token t;
                t._vartype = NULL;
                t.type = TokenLineComment;
                if (_for_display)
                    t.text = "/*";
                c = script->nextChar();
                c2 = script->nextChar();
                while ((c != '*' or c2 != '/') and c2 != EOF_TEXT and c != EOF_TEXT) // stop when (c=* and c2=/) or c=0 or c2=0
                {
                    if (_for_display)
                        t.text = t.text + c;
                    c = c2;
                    c2 = script->nextChar();
                }
                t.line = _token_line;
                // t.pos = pos;
                if (_for_display)
                    list_of_token.insert(_index_token, t);
                continue;
            }
            else
            {
                script->previousChar();
                token t;
                t.type = TokenSlash;
                t._vartype = NULL;
                if (_for_display)
                    t.text = "/";
                t.line = _token_line;
                // t.pos = pos;
                list_of_token.insert(_index_token, t);
                continue;
            }
        }
        if (c == '-')
        {
            token t;
            t.type = TokenSubstraction;
            t._vartype = NULL;
            if (_for_display)
                t.text = "-";
            t.line = _token_line;
            // t.pos = pos;
            list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == ' ')
        {
            token t;
            t.line = _token_line;
            // t.pos = pos;
            if (_for_display)
                t.text = "";
            while (c == ' ')
            {
                c = script->nextChar();
                pos++;
                t.text = t.text + " ";
            }
            script->previousChar(); // on revient un caractere en arriere
            pos--;
            t.type = TokenSpace;
            // t.text = " ";
            if (_for_display)
                list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == '"')
        {
            string v = "";
            token t;
            t._vartype = NULL;
            t.line = _token_line;
            // t.pos = pos;
            v += c;
            c = script->nextChar();
            pos++;
            while (c != '"' && c != EOF_TEXT)
            {
                v += c;
                c = script->nextChar();
                pos++;
            }
            // script->previousChar(); //on revient un caractere en arriere
            // pos--;
            v += c;
            t.type = TokenString;
            t.text = v;
            list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == '\n')
        {
            token t;
            t.type = TokenNewline;
            if (_for_display)
                t.text = "\r\n";
            t.line = _token_line;
            // t.pos = pos;
            if (increae_line)
                _token_line++;
            pos = 0;
            if (_for_display)
                list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == '?')
        {
            token t;
            t.type = TokenUnknown;
            if (_for_display)
                t.text = "?";
            t.line = _token_line;
            // t.pos = pos;
            //_token_line++;
            //  pos = 0;
            if (_for_display)
                list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == '.')
        {
            token t;
            t.type = TokenMember;
            if (_for_display)
                t.text = ".";
            t.line = _token_line;
            // t.pos = pos;
            //_token_line++;
            //  pos = 0;
            // if (_for_display)
            list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == '^')
        {
            token t;
            t.type = TokenPower;
            if (_for_display)
                t.text = "^";
            t.line = _token_line;
            // t.pos = pos;
            //_token_line++;
            //  pos = 0;
            // if (_for_display)
            list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == '\'')
        {
            token t;
            t.type = TokenUnknown;
            if (_for_display)
                t.text = "\'";
            t.line = _token_line;
            // t.pos = pos;
            //  _token_line++;
            //  pos = 0;
            if (_for_display)
                list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == ':')
        {
            token t;
            t.type = TokenUnknown;
            if (_for_display)
                t.text = ":";
            t.line = _token_line;
            // t.pos = pos;
            //_token_line++;
            //  pos = 0;
            if (_for_display)
                list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == '*')
        {
            token t;
            t._vartype = NULL;
            t.type = TokenStar;
            if (_for_display)
                t.text = "*";
            t.line = _token_line;
            // t.pos = pos;
            list_of_token.insert(_index_token, t);
            continue;
        }
        if (c == ',')
        {
            token t;
            t._vartype = NULL;
            t.type = TokenComma;
            if (_for_display)
                t.text = ",";
            t.line = _token_line;
            // t.pos = pos;
            list_of_token.insert(_index_token, t);
            continue;
        }
        // printf("Error invalid character |%c|\n", c);
    }
    token t;
    t.type = TokenEndOfFile;
    t.text = "";
    t.line = _token_line;
    // t.pos = pos + 1;
    if (update)
    {
        list_of_token.insert(_index_token, t);
    }
    // return list_of_token;
}
void tokenizer(Script *script)
{
    tokenizer(script, true, false);
}
#ifdef __CONSOLE_ESP32
list<const char *> _parenthesis;
list<const char *> _curlybracket;
list<const char *> _bracket;
int _prevparenthesis;
int _prevcurlybracket;
int _prevbracket;
#define _NB_COLORS 3
const char *_colors[_NB_COLORS] =
    {

        termColor.Magenta,
        termColor.LBlue,
        termColor.Yellow,

};

void formatInit()
{
    _parenthesis.clear();
    _curlybracket.clear();
    _bracket.clear();
    _prevparenthesis = 0;
    _prevcurlybracket = 0;
    _prevbracket = 0;
}

void formatNewLine()
{
    _prevparenthesis = 0;
    _prevcurlybracket = 0;
    _prevbracket = 0;
}

string formatLine(string str)
{
    // Serial.printf("streing:%s\r\n",str.c_str());
    // _parent.clear();
    _for_display = true;
    Script s(&str);
    tokenizer(&s);
    _tks.init();
    string res = "";
    for (int i = 0; i < _prevparenthesis; i++)
    {
        _parenthesis.pop_back();
    }
    for (int i = 0; i < _prevbracket; i++)
    {
        _bracket.pop_back();
    }
    for (int i = 0; i < _prevcurlybracket; i++)
    {
        _curlybracket.pop_back();
    }
    _prevparenthesis = 0;
    _prevcurlybracket = 0;
    _prevbracket = 0;

    for (int i = 0; i < _tks.size(); i++)
    {
        token tk = *_tks.current();
        //    Serial.printf("token %s\r\n",tk.text.c_str());
        /* if (tk.type == TokenOpenCurlyBracket)
         {

             // char *color= (char *)_colors[_curlybracket.size()%_NB_COLORS];
             res = res + string_format("%s%s", _colors[_curlybracket.size() % _NB_COLORS], tk.text.c_str());
             _curlybracket.push_back(_colors[_curlybracket.size() % _NB_COLORS]);
             _prevcurlybracket++;
         }
         else if (tk.type == TokenCloseCurlyBracket)
         {

             if (_curlybracket.size() == 0)
             {
                 res = res + string_format("%s%s", "\u001b[38;5;196m", tk.text.c_str());
             }
             else
             {
                 // char * color=_curlybracket.back();
                 _prevcurlybracket--;
                 res = res + string_format("%s%s", _curlybracket.back(), tk.text.c_str());
                 _curlybracket.pop_back();
             }
         }*/
        if (tk.type == TokenOpenParenthesis)
        {
            _prevparenthesis++;
            res = res + string_format("%s%s", _colors[(_parenthesis.size() + 2) % _NB_COLORS], tk.text.c_str());
            _parenthesis.push_back(_colors[(_parenthesis.size() + 2) % _NB_COLORS]);
        }
        else if (tk.type == TokenCloseParenthesis)
        {

            if (_parenthesis.size() == 0)
            {
                res = res + string_format("%s%s", "\u001b[38;5;196m", tk.text.c_str());
            }
            else
            {
                _prevparenthesis--;
                res = res + string_format("%s%s", _parenthesis.back(), tk.text.c_str());
                _parenthesis.pop_back();
            }
        }
        else if (tk.type == TokenOpenBracket)
        {
            _prevbracket++;
            res = res + string_format("%s%s", _colors[_bracket.size() % _NB_COLORS], tk.text.c_str());
            _bracket.push_back(_colors[_bracket.size() % _NB_COLORS]);
        }
        else if (tk.type == TokenCloseBracket)
        {

            if (_bracket.size() == 0)
            {
                res = res + string_format("%s%s", "\u001b[38;5;196m", tk.text.c_str());
            }
            else
            {
                _prevbracket--;
                res = res + string_format("%s%s", _bracket.back(), tk.text.c_str());
                _bracket.pop_back();
            }
        }
        /*
        else if (tk.type == TokenKeyword)
        {
            res = res + string_format("%s%s", KeywordTypeFormat[tk._keyword], tk.text.c_str());
        }*/
        else
        {
            token tkn = *_tks.next();
            if (tk.type == TokenIdentifier && tkn.type == TokenOpenParenthesis)
            {
                res = res + string_format("%s%s", tokenFormat[TokenFunction], tk.text.c_str());
            }
            else
            {

                res = res + string_format("%s%s", tokenFormat[tk.type], tk.text.c_str());
            }
            _tks.prev();
        }
        _tks.next();
    }

    _tks.clear();
    //  _parent.clear();

    _for_display = false;
    return res;
}
class __INIT_TOKEN
{
public:
    __INIT_TOKEN()
    {
        LedOS.addHightLightinf("sc", formatLine, formatInit, formatNewLine);
    }
};
__INIT_TOKEN _init_token;
#endif