#include <regex.h>
#include <stdio.h>
#include <list>
#include <string>
#include "memory.h"
#include <vector>

#pragma once
using namespace std;
// #include "asm_parser.h"
#include "string_function.h"
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
    __s_int__,
    __float__,
    __void__,
    __CRGB__,
    __CRGBW__,
    __char__,
    __Args__,
    __bool__,
    __userDefined__,
    __unknown__
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

    uint8_t memberSize[20];
    // string vnames[20];
    varTypeEnum types[20];
    uint8_t sizes[20];
    uint8_t size;
    uint8_t total_size;
};
varType usded;

vector<varType> _userDefinedTypes;
varType __v;
string varTypeEnumNames[] = {
#ifdef __TEST_DEBUG
    "__none__",
    "__unit8_t__",
    "__unit16_t__",
    "__unit32_t__",
    "__int__",
    "__s_int__",
    "__float__",
    "__void__",
    "__CRGB__",
    "__CRGBW__",
    "__char__",
    "__Args__",
    "__bool__",
    "__userDefined__",
    "__unknown__"
#endif

};

vector<string> userDefinedVarTypeNames;

varType _varTypes[] = {
    {._varType = __none__,
     .varName = "",
     ._varSize = 0,
     .load = {},
     .store = {},
     .membersNames = {},
     .starts = {},
     .memberSize = {},
     .types = {},
     .sizes = {},
     .size = 0,
     .total_size = 0},

    {._varType = __uint8_t__,
     .varName = "",
     ._varSize = 1,
     .load = {l8ui},
     .store = {s8i},
     .membersNames = {},
     .starts = {},
     .memberSize = {},
     .types = {},
     .sizes = {1},
     .size = 1,
     .total_size = 1},
    {
        ._varType = __uint16_t__,
        .varName = "",
        ._varSize = 2,
        .load = {l16ui},
        .store = {s16i},
        .membersNames = {},
        .starts = {},
        .memberSize = {},
        .types = {},
        .sizes = {2},
        .size = 1,
        .total_size = 2,
    },
    {
        ._varType = __uint32_t__,
        .varName = "",
        ._varSize = 4,
        .load = {l32i},
        .store = {s32i},
        .membersNames = {},
        .starts = {},
        .memberSize = {},
        .types = {},
        .sizes = {4},
        .size = 1,
        .total_size = 4,
    },
    {
        ._varType = __int__,
        .varName = "",
        ._varSize = 4,
        .load = {l32i},
        .store = {s32i},
        .membersNames = {},
        .starts = {},
        .memberSize = {},
        .types = {},
        .sizes = {4},
        .size = 1,
        .total_size = 4,
    },
        {
        ._varType = __s_int__,
        .varName = "",
        ._varSize = 2,
        .load = {l16si},
        .store = {s16i},
        .membersNames = {},
        .starts = {},
        .memberSize = {},
        .types = {},
        .sizes = {2},
        .size = 1,
        .total_size = 2,
    },
    {
        ._varType = __float__,
        .varName = "",
        ._varSize = 4,
        .load = {lsi},
        .store = {ssi},
        .membersNames = {},
        .starts = {},
        .memberSize = {},
        .types = {},
        .sizes = {4},
        .size = 1,
        .total_size = 4,
    },
    {
        ._varType = __void__,
        .varName = "",
        ._varSize = 0,
        .load = {},
        .store = {},
        .membersNames = {},
        .starts = {},
        .memberSize = {},
        .types = {},
        .sizes = {0},
        .size = 0,
        .total_size = 0,
    },
    {
        ._varType = __CRGB__,
        .varName = "",
        ._varSize = 3,
        .load = {l8ui, l8ui, l8ui},
        .store = {s8i, s8i, s8i},
        .membersNames = {"red", "green", "blue"},
        .starts = {0, 1, 2},
        .memberSize = {1, 1, 1},
        .types = {__uint8_t__, __uint8_t__, __uint8_t__},
        .sizes = {1, 1, 1},
        .size = 3,
        .total_size = 3,
    },
    {
        ._varType = __CRGBW__,
        .varName = "",
        ._varSize = 4,
        .load = {l8ui, l8ui, l8ui, l8ui},
        .store = {s8i, s8i, s8i, s8i},
        .membersNames = {"red", "green", "blue", "white"},
        .starts = {0, 1, 2, 3},
        .memberSize = {1, 1, 1, 1},
        .types = {__uint8_t__, __uint8_t__, __uint8_t__, __uint8_t__},
        .sizes = {1, 1, 1, 1},
        .size = 4,
        .total_size = 4,
    },
    {
        ._varType = __char__,
        .varName = "",
        ._varSize = 1,
        .load = {l8ui},
        .store = {s8i},
        .membersNames = {},
        .starts = {},
        .memberSize = {},
        .types = {},
        .sizes = {1},
        .size = 1,
        .total_size = 1,
    },
    {
        ._varType =__Args__,
        .varName = "",
        ._varSize = 1,
        .load = {},
        .store = {},
        .membersNames = {},
        .starts = {},
        .memberSize = {},
        .types = {},
        .sizes = {1},
        .size = 1,
        .total_size = 1,

    },
     {._varType = __bool__,
     .varName = "",
     ._varSize = 1,
     .load = {l8ui},
     .store = {s8i},
     .membersNames = {},
     .starts = {},
     .memberSize = {},
     .types = {},
     .sizes = {1},
     .size = 1,
     .total_size = 1},

};

string keywordTypeNames[] = {
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

#define nb_keywords 36
#define nb_typeVariables 13
string keyword_array[nb_keywords] =
    {"none", "uint8_t", "uint16_t", "uint32_t", "int","s_int" ,"float", "void", "CRGB",
     "CRGBW", "char", "Args","bool","external", "for", "if", "then", "else", "while", "return",
     "import", "from", "__ASM__",
     "define", "safe_mode", "_header_", "_content_", "and", "or", "continue",
     "break", "fabs", "abs", "save_reg",
     "save_reg_abs", "Object"};

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
    TokenUserDefinedVariableMember,
    TokenUserDefinedVariableMemberFunction,
    TokenDoubleUppersand,
    TokenDoubleOr,
    TokenQuestionMark,
    TokenColon,
    TokenPlusEqual,
    TokenMinusEqual,
    TokenStarEqual,
    TokenSlashEqual

};

tokenType __keywordTypes[] = {
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
    "TokenUserDefinedVariableMember",
    "TokenUserDefinedVariableMemberFunction",
    "TokenDoubleUppersand",
    "TokenDoubleOr",
    "TokenQuestionMark",
    "TokenColon",
    "TokenPlusEqual",
    "TokenMinusEqual",
    "TokenStarEqual",
    "TokenSlashEqual"

#endif
};

typedef struct
{
    string name;
    string content;
    string hh;
} _define;

vector<_define> define_list;

const char *getDefine(string name)
{
    for (vector<_define>::iterator it = define_list.begin();
         it != define_list.end(); ++it)
    {
        if ((*it).name.compare(name) == 0)
        {
            // printf("one rrent %s\n",(*it).content.c_str());
            return (*it).content.c_str();
        }
    }
    return NULL;
}

void deleteDefine()
{
    for (vector<_define>::iterator it = define_list.begin();
         it != define_list.end(); ++it)
    {
        (*it).content.clear();
        (*it).content.shrink_to_fit();
        (*it).name.clear();
        (*it).name.shrink_to_fit();
    }
    define_list.clear();
    define_list.shrink_to_fit();
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
    termColor.LMagenta, // TokenKeywordStruct,
    termColor.Cyan,     // TokenUserDefinedName,
    termColor.Green,    // TokenUserDefinedVariable,
    termColor.BWhite,   // TokenMember,
    termColor.BWhite,   // TokenDoubleUppersand,
    termColor.BWhite,   // TokenDoubleOr,
    termColor.BWhite,   // TokenQuestionMark,
    termColor.BWhite,   // TokenCOlon,
    termColor.BWhite,   // TokenDoubleOr,
    termColor.BWhite,   // TokenQuestionMark,
    termColor.BWhite,   // TokenCOlon,
    termColor.BWhite,   //TokenPlusEqual
    termColor.BWhite,  //TokenMinusEqual
    termColor.BWhite,   //TokenStarEqual
    termColor.BWhite,  //TokenSlashEqual
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
     uint16_t line;

    // switch to uin8_t unn ligne ne fera pas plus de 256 caracteres
     uint8_t pos;

    // switch to uin8_t
    // KeywordType _keyword;

    // possible sizee  1+4+4+1+1+4=15 au lieu de 44 ...

} token;

vector<char *> texts;

class Script
{
public:
    Script()
    {

        position = -1;
    }
    void init()
    {
        it = script.begin();
        position = -1;
    }
    void addContent(char *str)
    {
        script.push_back(str);
    }
    void clear()
    {
        script.clear();
        script.shrink_to_fit();
        position = -1;
    }

    char nextChar()
    {

        if ((*it)[position + 1] != 0)
        {
            position++;
            return (*it)[position];
        }
        else
        {

            if (next(it) == script.end())
            {

                position++;
                return EOF_TEXT;
            }
            else
            {
                it = next(it);
                position = 0;
                return (*it)[position];
            }
        }
    }

    char currentChar()
    {

        if ((*it)[position] != 0)
        {

            return (*it)[position];
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
            return (*it)[position];
        }
        else
        {
            if (it != script.begin())
            {

                it--;
                position = 0;
                while ((*it)[position] != 0)
                {
                    position++;
                }
                position--;
                return (*it)[position];
            }
            else
            {
                position = 0;
                return (*it)[0];
            }
        }
    }

    void insert(char *toInsert)
    {

        char *_cur = &(*it)[position];
        char *_next = &(*it)[position + 1];
        it++;
        if (*_next != 0 and *_cur != 0)
            it = script.insert(it, _next);
        it = script.insert(it, toInsert);
        position = -1;
    }
    void insertAtEnd(char *toInsert)
    {
        int i = 0;
        int res = -1;
        for (vector<char *>::iterator _it = script.begin(); _it != script.end(); _it++)
        {
            if (it == _it)
            {
                res = i;
            }
            else
            {
                i++;
            }
        }
        script.insert(script.end(), toInsert);
        it = script.begin();
        while (res > 0)
        {
            it++;
            res--;
        }
    }

private:
    // string * script;
    int position;

    vector<char *> script;
    vector<char *>::iterator it;
};

class Token
{

public:
    Token()
    {
        type = (int)TokenUnknown;
        _vartype = EOF_VARTYPE;
    }

    Token(tokenType h)
    {
        type = (int)h;
    }
    Token(tokenType _type, int __vartype, int _line)
    {
        type = (int)_type;
        _vartype = __vartype;
        line = _line;
    }
    Token(tokenType _type, int __vartype)
    {
        type = (int)_type;
        _vartype = __vartype;
    }
    tokenType getType()
    {
        return (tokenType)type;
    }
    void setType(tokenType _type)
    {
        type = (int)_type;
    }
    void clean()
    {
        line = 0;
        type = 0;
        _vartype = EOF_VARTYPE;

        textref = EOF_TEXTARRAY;
    }
    void addText(string t)
    {
        textref = all_text.addText(t);
    }
    char *getText()
    {
        return all_text.getText(textref);
    }
    varType *getVarType()
    {
        if (_vartype == EOF_VARTYPE)

            return NULL;

        return &_varTypes[_vartype];
    }
    uint16_t line = 0;
    uint8_t type = (int)TokenUnknown;
    uint8_t _vartype = EOF_VARTYPE;
    uint8_t pos=0;
    uint16_t textref = EOF_TEXTARRAY;
};

#ifdef __FULL_TOKEN
#define __DEPTH 0
#else
#define __DEPTH 5
#endif
int tokenizer(Script *script, bool update, bool increae_line,
              int nbMaxTokenToRead);
class Tokens
{
public:
    void init()
    {
#ifdef __FULL_TOKEN

        position = 0;
#endif
    }
    Tokens()
    {
        // _tokens = &_list_of_tokens;
        clear();
        init();
    }

    void clear()
    {
        _tokens.clear();

        _tokens.shrink_to_fit();

#ifdef __FULL_TOKEN
        position = 0;
#endif
    }
    int size()
    {
        return _tokens.size();
    }
    void tokenize(Script *script, bool update, bool increae_line, int nbToken)
    {
        _script = script;
        clear();
        tokenizer(script, true, increae_line, nbToken);
        // list_of_token.push_back(token());
        // Serial.printf("token read %d\n", tokenizer(script, true, increae_line, nbToken));
    }
    void push(Token t)
    {
        _tokens.push_back(t);
    }
    void pop_back()
    {
        if (_tokens.size() > 0)
        {
            //	Token t = _tokens.back ();

            _tokens.pop_back();

            //	return t;
        }
        //	else
        //	  return end_token;
    }
    Token *getTokenAtPos(int pos)
    {
        if (pos >= 0 and pos < _tokens.size())
        {
            return &(_tokens[pos]);
        }
        else
            return &end_token;
    }
    Token *current()
    {
#ifdef __FULL_TOKEN
        return getTokenAtPos(position);
#else
        return getTokenAtPos(__DEPTH);
#endif
    }
    Token *next()
    {
#ifdef __FULL_TOKEN

        // position++;
        tokenizer(_script, false, true, 1);
        position++;

        return getTokenAtPos(position);
#else
        _tokens.erase(_tokens.begin());
        _tokens.shrink_to_fit();

        tokenizer(_script, false, true, 1);
        return getTokenAtPos(__DEPTH);
#endif
    }
    Token *prev()
    {
#ifdef __FULL_TOKEN
        position--;
        return getTokenAtPos(position);
#else
        _tokens.insert(_tokens.begin(), Token());
        return getTokenAtPos(__DEPTH);
#endif
    }
    Token *peek(int index)
    {

#ifdef __FULL_TOKEN
        if (index + position < _tokens.size() && index + position >= 0)
        {
            return getTokenAtPos(index + position);
        }
        else
        {
            tokenizer(_script, false, true,
                      -_tokens.size() + index + position + 1);

            return getTokenAtPos(index + position);
        }
#else
        if (index + __DEPTH < _tokens.size() && index + __DEPTH >= 0)
        {
            return getTokenAtPos(index + __DEPTH);
        }
        else
        {
            tokenizer(_script, false, true,
                      -_tokens.size() + index + __DEPTH + 1);

            return getTokenAtPos(index + __DEPTH);
        }
#endif
    }
    Token back()
    {
        if (_tokens.size() > 0)
            return _tokens.back();
        else
            return Token();
    }

    bool Match(tokenType tt)
    {
        Token *g = current();
        if (g->getType() == tt)
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
        Token *g = peek(index);
        if (g->getType() == tt)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    vector<Token> _tokens;
    Token end_token = Token(TokenEndOfFile);
    Script *_script;
#ifdef __FULL_TOKEN
    int position;
#endif
};

Tokens _tks = Tokens();
void displaytoken(token t)
{
    // printf("%s\t%d\t%f\t%s%d %d\t%s\n", tokenNames[t.type].c_str(), t.int_value, t.float_value, t.text.c_str(), t.line, //t.pos, keywordTypeNames[t._keyword].c_str());
}

Token transNumber(string str)
{
    // t;
    // t.float_value=0;
    // t.int_value=0;
                if(_tks.size()>1)
            {
                if(_tks.back().type==TokenSubstraction)
                {
                    tokenType subtype= (tokenType)_tks.getTokenAtPos(_tks.size()-2)->type;
                if( subtype==TokenComma ||subtype==TokenEqual ||subtype==TokenDoubleEqual ||  subtype==TokenLessOrEqualThan  ||  subtype==TokenDoubleEqual||  subtype==TokenMoreThan  ||  subtype==TokenMoreOrEqualThan  ||  subtype==TokenNotEqual ||  subtype==TokenStarEqual  ||  subtype==TokenPlusEqual   || subtype==TokenOpenParenthesis)
                {
                        str="-"+str;
                        _tks.pop_back();
                }
                }

            }
    if (str.find(".") != string::npos)
    {

        Token t = Token(TokenNumber, (int)__float__);
        t.addText(str);
        return t;
    }
    else
    {

        Token t = Token(TokenNumber, (int)__int__);
        t.addText(str);
        return t;
    }
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

int _token_line;
int _sav_token_line=0;
int pos=0;
list<token>::iterator _index_token;
int tokenizer(Script *script, bool update, bool increae_line,
              int nbMaxTokenToRead)
{

    // list<token> list_of_token;
    // int line = 1;

    Token t;
    //int pos = 0;
    char c;
    char c2;
    _define newdef;
    string v;
    if (update)
    {
        userDefinedVarTypeNames.clear();
        userDefinedVarTypeNames.shrink_to_fit();
        all_text.clear();
        _tks.clear();
        for (int i = 0; i < __DEPTH; i++)
        {
            _tks.push(Token());
        }
        _token_line = 1;
        pos=0;
        deleteDefine();

        __isBlockComment = false;
        // _for_display= true;
    }
    else
    {
        script->previousChar();
    }
    int nbReadToken = 0;
    while (script->nextChar() != EOF_TEXT and nbReadToken < nbMaxTokenToRead)
    {
        // printf(" nb read :%c:\n",script->currentChar());
        t.clean();
        v.clear();
        pos++;

        c = script->currentChar();
        // printf("line : %d pos:%d char:%c token size:%d %d\n",line,pos,c,list_of_token.size(),heap_caps_get_free_size(MALLOC_CAP_8BIT));
        if (c == '=')
        {
            c2 = script->nextChar();
            if (c2 == '=')
            {
                t = Token(TokenDoubleEqual, EOF_VARTYPE);
                // t._vartype = NULL;
                // t.type = TokenDoubleEqual;
                if (_for_display)
                    t.addText("==");
                t.line = _token_line;
                t.pos = pos;
                _tks.push(t);
               // nbReadToken++;

                continue;
            }
            else
            {
                script->previousChar();
                // token t;
                //  t._vartype = NULL;
                //  t.type = TokenEqual;
                //  t.line = _token_line;
                t = Token(TokenEqual, EOF_VARTYPE, _token_line);
                t.pos = pos;
                if (_for_display)
                    t.addText("=");
                _tks.push(t);
               // nbReadToken++;
                continue;
            }
        }
        if (c == '<')
        {
            c2 = script->nextChar();
            if (c2 == '=')
            {
                // token t;
                // t._vartype = NULL;
                // t.type = TokenLessOrEqualThan;

                t = Token(TokenLessOrEqualThan, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("<=");
                // t.line = _token_line;
                t.pos = pos;
                _tks.push(t);
               // nbReadToken++;
                continue;
            }
            else if (c2 == '<')
            {
                // token t;
                // t._vartype = NULL;
                //  t.type = TokenShiftLeft;
                t = Token(TokenShiftLeft, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("<<");
                // t.line = _token_line;
                //  t.pos = pos;
                //  _tks.push(t);
                _tks.push(t);
                nbReadToken++;
                continue;
            }
            else
            {
                script->previousChar();
                // token t;
                // t._vartype = NULL;
                // t.type = TokenLessThan;
                // t.line = _token_line;//
                // TokenLessThan
                t = Token(TokenLessThan, EOF_VARTYPE, _token_line);
                t.pos = pos;
                if (_for_display)
                    t.addText("<");
                _tks.push(t);
                nbReadToken++;
                continue;
            }
        }
        if (c == '>')
        {
            c2 = script->nextChar();
            if (c2 == '=')
            {
                //  token t;
                //  t._vartype = NULL;
                // t.type = TokenMoreOrEqualThan;
                t = Token(TokenMoreOrEqualThan, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText(">=");
                // t.line = _token_line;
                t.pos = pos;
                //_tks.push(t);
                _tks.push(t);
                //nbReadToken++;
                continue;
            }
            else if (c2 == '>')
            {
                // token t;
                // t._vartype = NULL;
                // t.type = TokenShiftRight;
                t = Token(TokenShiftRight, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText(">>");
                // t.line = _token_line;
                t.pos = pos;
                // _tks.push(t);
                _tks.push(t);
                nbReadToken++;
                continue;
            }
            else
            {
                script->previousChar();
                // token t;
                // t._vartype = NULL;
                // t.type = TokenMoreThan;
                // t.line = _token_line;
                t = Token(TokenMoreThan, EOF_VARTYPE, _token_line);
                t.pos = pos;
                if (_for_display)
                    t.addText(">");
                // _tks.push(t);
                _tks.push(t);
               // nbReadToken++;
                continue;
            }
        }
        if (c == '!')
        {
            c2 = script->nextChar();
            if (c2 == '=')
            {
                // token t;
                // t._vartype = NULL;
                // t.type = TokenNotEqual;
                t = Token(TokenNotEqual, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("!=");
                // t.line = _token_line;
                t.pos = pos;
                //_tks.push(t);
                _tks.push(t);
               /// nbReadToken++;
                continue;
            }
            else
            {
                script->previousChar();
                //  token t;
                //  t._vartype = NULL;
                // t.type = TokenNot;
                // t.line = _token_line;
                t = Token(TokenNot, EOF_VARTYPE, _token_line);
                t.pos = pos;
                if (_for_display)
                    t.addText("!");
                // _tks.push(t);
                _tks.push(t);
                nbReadToken++;
                continue;
            }
        }
        if (c == '+')
        {
            c2 = script->nextChar();
            if (c2 == '+')
            {
                // token t;
                // t._vartype = NULL;
                // t.type = TokenPlusPlus;
                t = Token(TokenPlusPlus, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("++");
                // t.line = _token_line;
                t.pos = pos;
                // _tks.push(t);
                // nbReadToken++;
                _tks.push(t);
                continue;
            }
            else if(c2=='=')
            {
                t = Token(TokenPlusEqual, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("+=");
                // t.line = _token_line;
                t.pos = pos;
                // _tks.push(t);
                // nbReadToken++;
                _tks.push(t);
                continue;
            }
            else
            {
                script->previousChar();
                // token t;
                //  t._vartype = NULL;
                // t.type = TokenAddition;
                t = Token(TokenAddition, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("+");
                // t.line = _token_line;
                t.pos = pos;
                // _tks.push(t);
                _tks.push(t);
                nbReadToken++;
                continue;
            }
        }

        if (isIna_zA_Z_(c))
        {

            int newpos = pos;
            while (isIna_zA_Z_0_9(c))
            {
                v += c;
                newpos++;
                c = script->nextChar();
            }
            script->previousChar(); // on revient un caractere en arriere
            // token t;
            // Token t;
            //  t._vartype=NULL;
            t.line = _token_line;
            t.pos = pos;
            if (isKeyword(v) > -1)
            {
                // printf("keyword;%s\n",v.c_str());
                // t.type = TokenKeyword;
                t.type = (int)__keywordTypes[isKeyword(v)];
                if (isKeyword(v) < nb_typeVariables)
                    t._vartype = isKeyword(v);
                if (t.getType() == TokenKeywordExternalVar)
                {
                    t.type = (int)TokenExternal;
                }
                if ((t.getType() == TokenKeywordImport or t.getType() == TokenKeywordDefine) && !_for_display)
                {

                    nbReadToken--;
                }
            }
            else if (isUserDefined(v) > -1)
            {
                t.type = (int)TokenUserDefinedVariable;
                t._vartype = (int)__userDefined__;
            }
            else
            {
                t.type = (int)TokenIdentifier;

                if (_tks.size() >= __DEPTH)
                {

                    Token prev = _tks.back();
                    if (prev.getType() == TokenKeywordImport && !_for_display)
                    {

                        // script->insert(import);

                        _sav_token_line = _token_line;
                        nbReadToken--;
                        if (findLibFunction(v) > -1)
                        {

                            _tks.pop_back();

                            all_text.pop();

                            // list_of_token.pop_back();
                            //  add_on.push_back(findLibFunction(v));
                            script->insert((char *)((*_stdlib[findLibFunction(v)]).c_str()));

                            // script->previousChar ();

                            continue;
                        }
                    }
                    else if (prev.getType() == TokenDiese && !_for_display)
                    {
                        nbReadToken--;
                        if (findLibFunction(v) > -1)
                        {

                            _tks.pop_back();
                            // printf("token %d\n",_tks.back().type);
                            all_text.pop();

                            // list_of_token.pop_back();
                            //  add_on.push_back(findLibFunction(v));
                            script->insertAtEnd((char *)((*_stdlib[findLibFunction(v)]).c_str()));
                            // printf("ll%d %s\n",findLibFunction(v),(*_stdlib[findLibFunction(v)]).c_str());
                            script->nextChar();
                            // script->previousChar ();
                            continue;
                        }
                    }
                    else if (prev.getType() == TokenKeywordDefine && !_for_display)
                    {
                        _tks.pop_back();
                        all_text.pop();
                        // nbReadToken--;

                        newdef.name = v;
                        newdef.content = "";

                        c2 = script->nextChar();
                        // c2 = script->nextChar();
                        while (c2 != '\n' and c2 != EOF_TEXT)
                        {
                            newdef.content = newdef.content + c2;
                            c2 = script->nextChar();
                        }
                        define_list.push_back(newdef);
                        if (increae_line)
                            _token_line++;
                        // script->previousChar();
                        continue;
                    }
                    else if (prev.getType() == TokenKeywordStruct && !_for_display)
                    {
                        userDefinedVarTypeNames.push_back(v);
                        t.type = (int)TokenUserDefinedName;
                        // continue;
                    }
                }
                if (!_for_display) // on ne remplace pas lorsque l'on display
                {
                    if (getDefine(v) != NULL)
                    {

                        script->insert((char *)(getDefine(v)));
                        script->nextChar();
                        // nbReadToken--;
                        continue;
                    }
                }
            }
            pos = newpos - 1;

            t.addText(v);
            //_tks.push(t);
            _tks.push(t);
            nbReadToken++;
            continue;
        }

        if (isIn0_9(c))
        {
            // //printf("on a %c\n",c);
            v = "";
            int newpos = pos;
            while (isIn0_9_x_b(c))
            {
                v += c;
                c = script->nextChar();
                newpos++;
            }
            script->previousChar(); // on revient un caractere en arriere
            t = transNumber(v);
            //  t._vartype=NULL;
            t.line = _token_line;
            t.pos = pos;
            //_tks.push(t);

            
                _tks.push(t);
            nbReadToken++;
            pos = newpos - 1;
            continue;
        }
        if (c == ';')
        {
            // token t;
            // t.type = TokenSemicolon;
            // t._vartype = NULL;
            t = Token(TokenSemicolon, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText(";");
            // t.line = _token_line;
            t.pos = pos;
            // _tks.push(t);
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == '\t')
        {
            // token t;
            // t.type = TokenSpace;
            // t._vartype = NULL;
            t = Token(TokenSpace, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText("\t");
            // t.line = _token_line;
            //  t.pos = pos;
            if (_for_display)
            {
                //_tks.push(t);
                // nbReadToken++;
                _tks.push(t);
            }
            continue;
        }
        if (c == '&')
        {
            c2 = script->nextChar();
            if (c2 == '&')
            {
                t = Token(TokenDoubleUppersand, EOF_VARTYPE);
                // t._vartype = NULL;
                // t.type = TokenDoubleEqual;
                if (_for_display)
                    t.addText("&&");
                t.line = _token_line;
                t.pos = pos;
                _tks.push(t);
               //nbReadToken++;

                continue;
            }
            else
            {
                script->previousChar();
                // token t;
                //  t._vartype = NULL;
                //  t.type = TokenEqual;
                //  t.line = _token_line;
                t = Token(TokenUppersand, EOF_VARTYPE, _token_line);
                t.pos = pos;
                if (_for_display)
                    t.addText("&");
                _tks.push(t);
                nbReadToken++;
                continue;
            }
        }
        if (c == '#')
        {
            // token t;
            // t.type = TokenDiese;
            // t._vartype = NULL;
            t = Token(TokenDiese, EOF_VARTYPE, _token_line);
            if (_for_display)
            {
                t.addText("#");
                // _tks.push(t);
            }
            // t.line = _token_line;
            //  t.pos = pos;
            _tks.push(t);
            // nbReadToken++;
            continue;
        }
        if (c == '(')
        {
            // token t;
            // t.type = TokenOpenParenthesis;
            // t._vartype = NULL;
            t = Token(TokenOpenParenthesis, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText("(");
            // t.line = _token_line;
            t.pos = pos;
            //_tks.push(t);
            _tks.push(t);
           // nbReadToken++;
            continue;
        }
        if (c == '%')
        {
            // token t;
            //  t.type = TokenModulo;
            // t._vartype = NULL;
            t = Token(TokenModulo, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText("%");
            // t.line = _token_line;
            t.pos = pos;
            //_tks.push(t);
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == ')')
        {
            //  token t;
            // t.type = TokenCloseParenthesis;
            // t._vartype = NULL;
            t = Token(TokenCloseParenthesis, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText(")");
            // t.line = _token_line;
            t.pos = pos;
            //_tks.push(t);
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == '{')
        {
            // token t;
            // t.type = TokenOpenCurlyBracket;
            // t._vartype = NULL;
            t = Token(TokenOpenCurlyBracket, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText("{");
            // t.line = _token_line;
            //  t.pos = pos;
            //_tks.push(t);
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == '}')
        {
            // token t;
            // t.type = TokenCloseCurlyBracket;
            // t._vartype = NULL;
            t = Token(TokenCloseCurlyBracket, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText("}");
            // t.line = _token_line;
            t.pos = pos;
            // _tks.push(t);
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == '[')
        {
            //  token t;
            // t.type = TokenOpenBracket;
            // t._vartype = NULL;
            t = Token(TokenOpenBracket, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText("[");
            // t.line = _token_line;
            t.pos = pos;
            //_tks.push(t);
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == ']')
        {
            // token t;
            // t.type = TokenCloseBracket;
            // t._vartype = NULL;
          c2 = script->nextChar();
            if (c2 == '[')
            {
                t = Token(TokenComma, EOF_VARTYPE);
                // t._vartype = NULL;
                // t.type = TokenDoubleEqual;
                if (_for_display)
                    t.addText("][");
                t.line = _token_line;
                t.pos = pos;
                _tks.push(t);
                nbReadToken++;

                continue;
            }
            else
            {
                script->previousChar();
                // token t;
                //  t._vartype = NULL;
                //  t.type = TokenEqual;
                //  t.line = _token_line;
                t = Token(TokenCloseBracket, EOF_VARTYPE, _token_line);
                t.pos = pos;
                if (_for_display)
                    t.addText("]");
                _tks.push(t);
                nbReadToken++;
                continue;
            }
        }
        if (c == '/')
        {
            char c2 = script->nextChar();
            if (c2 == '/')
            {
                // Token t;
                t._vartype = EOF_VARTYPE;
                t.type = (int)TokenLineComment;
                string str = "//";

                c2 = script->nextChar();
                while (c2 != '\n' and c2 != EOF_TEXT)
                {
                    str = str + c2; // string_format("%s%c", t.getText(), c2);
                    c2 = script->nextChar();
                }
                 str=str+'\0';
               // c2 = script->previousChar();
                if (_for_display)
                {
                    c2 = script->previousChar();
                    t.addText(str);
                }
                t.line = _token_line;
                t.pos = pos;
                if (increae_line)
                    _token_line++;
                if (_for_display)
                {
                    // script->previousChar();
                    _tks.push(t);
                    nbReadToken++;
                }
                continue;
            }
             else if(c2=='=')
            {
                t = Token(TokenSlashEqual, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("/=");
                // t.line = _token_line;
                t.pos = pos;
                // _tks.push(t);
                // nbReadToken++;
                _tks.push(t);
                continue;
            }
            else if (c2 == '*')
            {
                // Token t;
                t._vartype = EOF_VARTYPE;
                t.type = (int)TokenLineComment;

                string str = "/*";
                c = script->nextChar();
                c2 = script->nextChar();
                while ((c != '*' or c2 != '/') and c2 != EOF_TEXT and c != EOF_TEXT) // stop when (c=* and c2=/) or c=0 or c2=0
                {
                    if (_for_display)
                        // str = string_format("%s%c", t.getText(), c);
                        str = str + c2;
                    c = c2;
                    c2 = script->nextChar();
                }
                if (_for_display)
                    t.addText(str);
                t.line = _token_line;
                t.pos = pos;
                if (_for_display)
                {
                    _tks.push(t);
                    nbReadToken++;
                }
                continue;
            }
            else
            {
                script->previousChar();
                // Token t;
                t.type = (int)TokenSlash;
                t._vartype = EOF_VARTYPE;
                if (_for_display)
                    t.addText("/");
                t.line = _token_line;
                t.pos = pos;
                _tks.push(t);
                nbReadToken++;
                continue;
            }
        }
        if (c == '-')
        {
            c2 = script->nextChar();
            if (c2 == '-')
            {
                // token t;
                // t._vartype = NULL;
                // t.type = TokenPlusPlus;
                t = Token(TokenMinusMinus, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("--");
                // t.line = _token_line;
                t.pos = pos;
                // _tks.push(t);
                // nbReadToken++;
                _tks.push(t);
                continue;
            }
            if (c2 == '=')
            {
                // token t;
                // t._vartype = NULL;
                // t.type = TokenPlusPlus;
                t = Token(TokenMinusEqual, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("-=");
                // t.line = _token_line;
                t.pos = pos;
                // _tks.push(t);
                // nbReadToken++;
                _tks.push(t);
                continue;
            }
            else
            {
                script->previousChar();
                // token t;
                //  t._vartype = NULL;
                // t.type = TokenAddition;
                t = Token(TokenSubstraction, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("-");
                // t.line = _token_line;
                t.pos = pos;
                // _tks.push(t);
                _tks.push(t);
                //nbReadToken++;
                continue;
            }
            // Token t;
        }
        if (c == ' ')
        {
            // Token t;
            t.line = _token_line;
            t.pos = pos;
            string str = "";

            while (c == ' ')
            {
                c = script->nextChar();
                pos++;
                str = str + " ";
            }
            if (_for_display)
                t.addText(str);
            script->previousChar(); // on revient un caractere en arriere
            pos--;
            t.type = TokenSpace;
            // t.addText(" ";
            if (_for_display)
            {
                _tks.push(t);
                nbReadToken++;
            }
            continue;
        }
        if (c == '"')
        {
            v = "";
            // Token t;
            t._vartype = EOF_VARTYPE;
            t.line = _token_line;
            t.pos = pos;
            v += c;
            c = script->nextChar();
            pos++;
        while (c != '"' && c != EOF_TEXT)
            {
                      if (! _for_display)
            {
                char c2=script->nextChar();
                if(c=='\\' and c2== 'n')
                {
                    c='\x0d';
                    v+=c;
                      c='\x0a';
                    v+=c;    
                    c = script->nextChar();              
                }
                else
                {
                v += c;
                c=c2;
                }
               
                pos++;
            }
            else
            {
                v+=c;
                 c = script->nextChar();  
                 pos++;
            } 
            }
            // script->previousChar(); //on revient un caractere en arriere
            // pos--;
            v += c;
            t.type = (int)TokenString;
            t.addText(v);
            _tks.push(t);
            nbReadToken++;
            continue;
        }
        if (c == '\n')
        {
            // Token t;
            t.type = (int)TokenNewline;
            if (_for_display)
                t.addText("\r\n");
            t.line = _token_line;
            t.pos = pos;
            if (increae_line)
                _token_line++;
            pos = 0;
            if (_for_display)
                _tks.push(t);
            continue;
        }
        if (c == '?')
        {
            // Token t;
             t = Token(TokenQuestionMark, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText("?");
            t.line = _token_line;
            t.pos = pos;
            //_token_line++;
            //  pos = 0;
            //if (_for_display)
                _tks.push(t);
            continue;
        }
        if (c == '.')
        {
            // Token t;
            t.type = (int)TokenMember;
            if (_for_display)
                t.addText(".");
            t.line = _token_line;
            t.pos = pos;
            //_token_line++;
            //  pos = 0;
            // if (_for_display)
            _tks.push(t);
            // nbReadToken++;
            continue;
        }
        if (c == '^')
        {
            // Token t;
            t.type = (int)TokenPower;
            if (_for_display)
                t.addText("^");
            t.line = _token_line;
            t.pos = pos;
            //_token_line++;
            //  pos = 0;
            // if (_for_display)
            _tks.push(t);
            // nbReadToken++;
            continue;
        }
        if (c == '@')
        {
            // Token t;
            t.type = (int)TokenUnknown;
            if (_for_display)
                t.addText("@");
            _token_line = _sav_token_line;
            if (_for_display)
                _tks.push(t);
        }
        if (c == '\'')
        {
            // Token t;
            t.type = (int)TokenUnknown;
            if (_for_display)
                t.addText("\'");
            t.line = _token_line;
            t.pos = pos;
            //  _token_line++;
            //  pos = 0;
            if (_for_display)
                _tks.push(t);
            continue;
        }
        if (c == ':')
        {
            // Token t;
            t = Token(TokenColon, EOF_VARTYPE, _token_line);
            if (_for_display)
                t.addText(":");
            t.line = _token_line;
            t.pos = pos;
            //_token_line++;
            //  pos = 0;
           // if (_for_display)
                _tks.push(t);
            continue;
        }
        if (c == '*')
        {
             c2 = script->nextChar();
            if (c2 == '=')
            {
                // token t;
                // t._vartype = NULL;
                // t.type = TokenPlusPlus;
                t = Token(TokenStarEqual, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("*=");
                // t.line = _token_line;
                t.pos = pos;
                // _tks.push(t);
                // nbReadToken++;
                _tks.push(t);
                continue;
            }
            else
            {
                script->previousChar();
                // token t;
                //  t._vartype = NULL;
                // t.type = TokenAddition;
                t = Token(TokenStar, EOF_VARTYPE, _token_line);
                if (_for_display)
                    t.addText("*");
                // t.line = _token_line;
                t.pos = pos;
                // _tks.push(t);
                _tks.push(t);
                nbReadToken++;
                continue;
            }
            // Token t;

        }
        if (c == '|')
        {
            c2 = script->nextChar();
            if (c2 == '|')
            {
                t = Token(TokenDoubleOr, EOF_VARTYPE);
                // t._vartype = NULL;
                // t.type = TokenDoubleEqual;
                if (_for_display)
                    t.addText("||");
                t.line = _token_line;
                t.pos = pos;
                _tks.push(t);
                nbReadToken++;

                continue;
            }
            else
            {
                script->previousChar();
                // token t;
                //  t._vartype = NULL;
                //  t.type = TokenEqual;
                //  t.line = _token_line;
                t = Token(TokenKeywordOr, EOF_VARTYPE, _token_line);
                t.pos = pos;
                if (_for_display)
                    t.addText("|");
                _tks.push(t);
                nbReadToken++;
                continue;
            }
        }
        if (c == ',')
        {
            // Token t;
            t._vartype = EOF_VARTYPE;
            t.type = (int)TokenComma;
            if (_for_display)
                t.addText(",");
            t.line = _token_line;
            t.pos = pos;
            _tks.push(t);
           // nbReadToken++;
            continue;
        }
        // printf("Error invalid character |%c|\n", c);
    }
    //  token t;
    //  t.type = TokenEndOfFile;
    //  t.addText("";
    //  t.line = _token_line;
    // t.pos = pos + 1;
    if (script->currentChar() == EOF_TEXT)
    {
        if (_tks.back().getType() != TokenEndOfFile)
        { // printf("oinr termine");
            // token t;
            //  t.type = TokenEndOfFile;
            //  t.addText("";
            //   t.line = _token_line;

            t = Token(TokenEndOfFile, EOF_VARTYPE, _token_line);
            // _tks.push(t);
            _tks.push(t);
        }
    }
    // return list_of_token;
    return nbReadToken - 1;
}

/*
class Tokens
{
public:
    Tokens()
    {
        position = 0;
        _tokens = &list_of_token;
        //_script=script;
    }
    int size()
    {
        return _tokens->size();
    }
    void tokenize(Script *script, bool update, bool increae_line, int nbToken)
    {
        _script = script;
        list_of_token.clear();
        // list_of_token.push_back(token());
        tokenizer(script, true, increae_line, nbToken);
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
        return getTokenAtPos(__DEPTH);
    }
    token *next()
    {

        _tokens->pop_front();
        tokenizer(_script, false, true, 1);
        return getTokenAtPos(__DEPTH);
    }
    token *prev()
    {

       list_of_token.push_front(token());
        return getTokenAtPos(__DEPTH);
    }
    token *peek(int index)
    {
        if (index + __DEPTH < _tokens->size() && position + __DEPTH >= 0)
        {
            return getTokenAtPos(index + __DEPTH);
        }
        else
        {
              tokenizer(_script, false, true,  _tokens->size()-index-__DEPTH+1);

            return getTokenAtPos(index + __DEPTH);
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

    void clear()
    {
        _tokens->clear();
    }
    int position;

private:
    list<token> *_tokens;
    token end_token = {.type = TokenEndOfFile};
    Script *_script;
};

*/

#ifdef __CONSOLE_ESP32

#endif
