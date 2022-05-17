%option noyywrap

%{
#include "symbols.h"
%}

d [0-9]
num ({d}+)
punc [()\-+*/]
ws [\t\r\a ]

%%
{ws}+   {}
{num}   { return NUM; }
{punc}  { return yytext[0]; }
.       { return ERR; }
%%
