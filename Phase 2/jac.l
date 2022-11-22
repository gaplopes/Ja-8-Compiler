%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "y.tab.h"
	#include "ast.h"

	int yylex_destroy();

	int line = 1;
	int colunm = 1;
	int flagLex = 0;
	int flagParse = 0;
	int plica[2];
	extern node* root;
	int nErrors = 0;
%}

NUMBER				[0-9]
BOOL				"boolean"
BOOLLIT				"true"|"false"
CLASS				"class"
DO 					"do"
DOTLENGTH			".length"
DOUBLE				"double"
INT					"int"
PARSEINT			"Integer.parseInt"
PRINT 				"System.out.println"
PUBLIC				"public"
RETURN				"return"
IF 					"if"
ELSE 				"else"
STATIC				"static"
STRING 				"String"
VOID				"void"
OCURV				"("
CCURV				")"
OBRACE				"{"
CBRACE				"}"
OSQUARE				"["
CSQUARE				"]"
AND					"&&"
OR  				"||"
LT 					"<"
GT					">"
EQ					"=="
NEQ					"!="
LEQ					"<="
GEQ					">="
PLUS				"+"
MINUS				"-"
STAR				"*"
DIV					"/"
MOD					"%"
NOT					"!"
ASSIGN				"="
SEMI				";"
COMMA				","
WHILE				"while"
RESERVED 			"abstract"|"continue"|"for"|"switch"|"assert"|"default"|"goto"|"package"|"synchronized"|"new"|"private"|"this"|"break"|"double"|"implements"|"protected"|"throw"|"byte"|"import"|"throws"|"case"|"enum"|"instanceof"|"transient"|"catch"|"extends"|"short"|"try"|"char"|"final"|"interface"|"finally"|"long"|"strictfp"|"volatile"|"const"|"float"|"native"|"super"|"null"|"++"|"--"|"Integer"|"System"
DECLIT				(0|[1-9]|([1-9]({NUMBER}|"_")*{NUMBER}))
LETTER 				[a-z]|[A-Z]|"_"|"$"
ID 					{LETTER}({LETTER}|{NUMBER})*
NUMBERAUX			{NUMBER}("_"*{NUMBER})*
REALLIT				({NUMBERAUX}(e|E)("+"|"-")?{NUMBERAUX})|({NUMBERAUX}+"."{NUMBERAUX}?)|(({NUMBERAUX}?"."{NUMBERAUX}+))|({NUMBERAUX}?"."{NUMBERAUX}?(e|E)("+"|"-")?{NUMBERAUX})
SEQESCAPE			\\\\|\\n|\\f|\\r|\\t|\\\"
STRLIT 				\"([^\n\r\"\\]|(\\f|\\n|\\r|\\t|\\\\|\\\"))*\"
IGNORE				" "|"\t"
NEWLINE				"\n"|"\r"|"\r\n"

%X MULTI_LINE_COMMENT
%X SINGLE_LINE_COMMENT
%X STRLIT_L

%%

"/*"							{BEGIN MULTI_LINE_COMMENT;
								plica[0]=line;
								plica[1]=colunm;
								colunm+=yyleng;}

<MULTI_LINE_COMMENT><<EOF>>		{printf("Line %d, col %d: unterminated comment\n", plica[0], plica[1]);
								colunm+=yyleng;
								return 0;}

<MULTI_LINE_COMMENT>"*/"  		{colunm+=yyleng;
								BEGIN 0;}

<MULTI_LINE_COMMENT>{NEWLINE}	{line++;colunm=1;}

<MULTI_LINE_COMMENT>.			{colunm+=yyleng;}


"//"							{BEGIN SINGLE_LINE_COMMENT;
								colunm+=yyleng;}

<SINGLE_LINE_COMMENT>{NEWLINE}	{line++;colunm=1;BEGIN 0;}

<SINGLE_LINE_COMMENT>.			{colunm+=yyleng;}

{STRLIT}						{if(flagLex == 1){printf("STRLIT(%s)\n", yytext);} colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return STRLIT;}}

"\""							{BEGIN STRLIT_L; 
								plica[0]=line;
								plica[1]=colunm;
								colunm+=yyleng;}

<STRLIT_L>{SEQESCAPE}			{colunm+=yyleng;}

<STRLIT_L>((\\.)|(\\))			{printf("Line %d, col %d: invalid escape sequence (%s)\n", line, colunm, yytext);
								colunm+=yyleng;}

<STRLIT_L><<EOF>>				{printf("Line %d, col %d: unterminated string literal\n", plica[0], plica[1]);
								colunm+=yyleng;
								BEGIN 0;}

<STRLIT_L>"\""  				{colunm+=yyleng;
								BEGIN 0;}	

<STRLIT_L>{NEWLINE}				{line++;colunm=1;
								printf("Line %d, col %d: unterminated string literal\n", plica[0], plica[1]);
								BEGIN 0;}
								
<STRLIT_L>.						{colunm+=yyleng;}


{BOOLLIT}						{if(flagLex == 1){printf("BOOLLIT(%s)\n",yytext);}colunm+=yyleng;if(flagParse == 1){yylval.value = (char*) strdup(yytext); return BOOLLIT;}}
{BOOL}							{if(flagLex == 1){printf("BOOL\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return BOOL;}}
{CLASS}							{if(flagLex == 1){printf("CLASS\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return CLASS;}}
{DO}							{if(flagLex == 1){printf("DO\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return DO;}}
{DOTLENGTH}						{if(flagLex == 1){printf("DOTLENGTH\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return DOTLENGTH;}}
{DOUBLE}						{if(flagLex == 1){printf("DOUBLE\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return DOUBLE;}}
{RESERVED}						{if(flagLex == 1){printf("RESERVED(%s)\n",yytext);}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return RESERVED;}}
{INT}							{if(flagLex == 1){printf("INT\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return INT;}}
{PARSEINT}						{if(flagLex == 1){printf("PARSEINT\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return PARSEINT;}}
{PRINT}							{if(flagLex == 1){printf("PRINT\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return PRINT;}}
{PUBLIC}						{if(flagLex == 1){printf("PUBLIC\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return PUBLIC;}}
{RETURN}						{if(flagLex == 1){printf("RETURN\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return RETURN;}}
{IF}							{if(flagLex == 1){printf("IF\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return IF;}}
{ELSE}							{if(flagLex == 1){printf("ELSE\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return ELSE;}}
{STATIC}						{if(flagLex == 1){printf("STATIC\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return STATIC;}}
{STRING}						{if(flagLex == 1){printf("STRING\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return STRING;}}
{VOID}							{if(flagLex == 1){printf("VOID\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return VOID;}}
{SEMI}							{if(flagLex == 1){printf("SEMI\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return SEMI;}}
{OCURV}							{if(flagLex == 1){printf("OCURV\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return OCURV;}}
{CCURV}							{if(flagLex == 1){printf("CCURV\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return CCURV;}}
{OBRACE}						{if(flagLex == 1){printf("OBRACE\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return OBRACE;}}
{CBRACE}						{if(flagLex == 1){printf("CBRACE\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return CBRACE;}}
{OSQUARE}						{if(flagLex == 1){printf("OSQUARE\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return OSQUARE;}}
{CSQUARE}						{if(flagLex == 1){printf("CSQUARE\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return CSQUARE;}}
{AND}							{if(flagLex == 1){printf("AND\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return AND;}}
{OR}							{if(flagLex == 1){printf("OR\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return OR;}}
{LT}							{if(flagLex == 1){printf("LT\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return LT;}}
{GT}							{if(flagLex == 1){printf("GT\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return GT;}}
{EQ}							{if(flagLex == 1){printf("EQ\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return EQ;}}
{NEQ}							{if(flagLex == 1){printf("NEQ\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return NEQ;}}
{LEQ}							{if(flagLex == 1){printf("LEQ\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return LEQ;}}
{GEQ}							{if(flagLex == 1){printf("GEQ\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return GEQ;}}
{PLUS}							{if(flagLex == 1){printf("PLUS\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return PLUS;}}
{MINUS}							{if(flagLex == 1){printf("MINUS\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return MINUS;}}
{STAR}							{if(flagLex == 1){printf("STAR\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return STAR;}}
{DIV}							{if(flagLex == 1){printf("DIV\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return DIV;}}
{MOD}							{if(flagLex == 1){printf("MOD\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return MOD;}}
{NOT}							{if(flagLex == 1){printf("NOT\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return NOT;}}
{ASSIGN}						{if(flagLex == 1){printf("ASSIGN\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return ASSIGN;}}
{COMMA}							{if(flagLex == 1){printf("COMMA\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return COMMA;}}
{WHILE}							{if(flagLex == 1){printf("WHILE\n");}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return WHILE;}}
{DECLIT}						{if(flagLex == 1){printf("DECLIT(%s)\n", yytext);}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return DECLIT;}}
{ID}							{if(flagLex == 1){printf("ID(%s)\n",yytext);}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return ID;}}
{REALLIT}						{if(flagLex == 1){printf("REALLIT(%s)\n",yytext);}colunm+=yyleng; if(flagParse == 1){yylval.value = (char*) strdup(yytext); return REALLIT;}}
{NEWLINE}						{line++;colunm=1;}
" "|\f|\t						{colunm+=1;}
<<eof>>							{colunm+=1; return 0;}
.								{printf("Line %d, col %d: illegal character (%s)\n", line, colunm, yytext); colunm+=yyleng;}

%%

int main(int argc, char ** argv)
{
	if(argc>1){
		if(strcmp(argv[1],"-l")==0){
			flagLex = 1;
			yylex();
		}
		if(strcmp(argv[1],"-1")==0){
			flagLex = 0;
			yylex();
		}
		if(strcmp(argv[1],"-t")==0){
			flagParse = 1;
			yyparse();
			if(nErrors == 0){
				printAST(root, 0);
			}
		}
	}
	else{
		flagParse = 1;
		yyparse();
	}

	clearAST(root);

	yylex_destroy();

	return 0;
}

void yyerror (const char *s) {
	nErrors = 1;
	printf ("Line %d, col %d: %s: %s\n", line, (int)(colunm-(int)yyleng), s, yytext);
}

int yywrap()
{
	return 1;
}