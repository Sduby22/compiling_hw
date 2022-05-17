/******************************************************************************
* File:             lex.l
*
* Author:           Li Zihao
* Created:          10/01/21 
* Description:      提供词法扫描函数lexhelper(), 全局变量yywc, yylc, yybc
*****************************************************************************/

%x com IN_STR EXCEED_STR
%option noyywrap

%{
#include "symbol.h"
%}

macro ("#"[^\n]+)
ws [\t\r\a ]
l [_a-zA-Z]
letter [a-zA-Z]
d [0-9]
hexd [0-9a-f]
octd [0-7]
bind [01]
id {l}({l}|{d})*
illegal_id {d}{l}+
punc [(){}[\]<>"';:.,#+\-*/|^&!%=?~]
dsuffix (l|u|ll|ul|lu|llu|ull)
fsuffix (f|l)
hex (0x{hexd}+{dsuffix}?)
bin (0b{bind}+{dsuffix}?)
oct (0{octd}+{dsuffix}?)
num ({d}+{dsuffix}?)
float (\.{d}+|{d}+\.|{d}+\.{d}+){fsuffix}?
char ([^\\\n]|(\\.))
string_nowrap ([^\"\n]|\\\")
string_wrap (\\\n)
char_literal ('{char}')
single_comment ("//"[^\n]*)

%%
"\n"                        { yywc.lc++;yywc.bc+=strlen(yytext); }
{ws}+                       { yywc.bc+=strlen(yytext); }

<INITIAL>"/*"               { BEGIN com;yywc.bc+=strlen(yytext); }
<com>"*/"                   { BEGIN 0; yywc.comment++;yywc.bc+=strlen(yytext); }
<com>\n                     { yywc.lc++; yywc.bc+=strlen(yytext); }
<com>.                      { yywc.bc+=strlen(yytext); }

{single_comment}            { yywc.comment++;yywc.bc+=strlen(yytext); }
{macro}                     { printf("macro\n");yywc.bc+=strlen(yytext); }

"if"                        { return T_IF; }
"else"                      { return T_ELSE; }
"while"                     { return T_WHILE; }
"for"                       { return T_FOR; }
"switch"                    { return T_SWITCH; }
"case"                      { return T_CASE; }
"goto"                      { return T_GOTO; }
"break"                     { return T_BREAK; }
"continue"                  { return T_CONTINUE; }
"do"                        { return T_DO; }
"return"                    { return T_RETURN; }
"default"                   { return T_DEFAULT; }
"typedef"                   { return T_TYPEDEF; }
"int"                       { return T_INT; }
"long"                      { return T_LONG; }
"short"                     { return T_SHORT; }
"float"                     { return T_FLOAT; }
"double"                    { return T_DOUBLE; }
"char"                      { return T_CHAR; }
"unsigned"                  { return T_UNSIGNED; }
"signed"                    { return T_SIGNED; }
"const"                     { return T_CONST; }
"void"                      { return T_VOID; }
"volatile"                  { return T_VOLATILE; }
"enum"                      { return T_ENUM; }
"struct"                    { return T_STRUCT; }
"union"                     { return T_UNION; }
"register"                  { return T_REGISTER; }
"extern"                    { return T_EXTERN; }
"static"                    { return T_STATIC; }
"sizeof"                    { return T_SIZEOF; }

">="                        { return T_RELOP_GE; }
"<="                        { return T_RELOP_LE; }
"=="                        { return T_RELOP_EQ; }
"!="                        { return T_RELOP_NEQ; }
"->"                        { return T_ARROW; }
"||"                        { return T_BOOLOP_OR; }
"&&"                        { return T_BOOLOP_AND; }
">>"                        { return T_CALOP_RSHIFT; }
"<<"                        { return T_CALOP_LSHIFT; }
"++"                        { return T_CALOP_INC; }
"--"                        { return T_CALOP_DEC; }
"+="                        { return T_PLUSEQUAL; }
"-="                        { return T_MINUSEQUAL; }
"*="                        { return T_MULEQUAL; }
"/="                        { return T_DIVEQUAL; }
"%="                        { return T_MODEQUAL; }
"&="                        { return T_ANDEQUAL; }
"|="                        { return T_OREQUAL; }
"^="                        { return T_XOREQUAL; }
">>="                       { return T_RSEQUAL; }
"<<="                       { return T_LSEQUAL; }

{float}                     { return T_NUM_FLOAT; }
{hex}                       { return T_NUM_HEX; }
{bin}                       { return T_NUM_BIN; }
{oct}                       { return T_NUM_OCT; }
{num}                       { return T_NUM; }
{illegal_id}                { return 0;  }
 /*{string_literal}        {  yywc.literal++; return T_LITERAL; }*/

<INITIAL>\"                 { BEGIN IN_STR; yystrinit(); yyappend(yytext); }
<IN_STR>{string_wrap}       { yywc.lc++; if(yyappend(yytext) != 0) BEGIN EXCEED_STR; }
<IN_STR>\n                  { yywc.lc++; yywc.bc++; BEGIN 0; return T_NULL; }
<IN_STR>{string_nowrap}     { if(yyappend(yytext) != 0) BEGIN EXCEED_STR; }
<IN_STR>\"                  { BEGIN 0; yyappend(yytext); return T_LITERAL; }

<EXCEED_STR>{string_wrap}   { yywc.bc+=strlen(yytext); yywc.lc++; }
<EXCEED_STR>{string_nowrap} { yywc.bc+=strlen(yytext); }
<EXCEED_STR>\n              { yywc.lc++; BEGIN 0; return T_NULL; }
<EXCEED_STR>\"              { BEGIN 0; yywc.bc+=strlen(yytext); return T_NULL; }

{char_literal}              { return T_NUM; }
{id}                        { return T_ID; }

{punc}                      { return yytext[0]; }
.                           { return 0; }
%%

