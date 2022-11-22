%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	int line = 1;
	int colunm = 1;
	int validSTRLIT = 1;
	int flag = 0;
	int plica[2];
	char *strlitAux;
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
IGNORE				" "|"\t"
NEWLINE				"\n"|"\r"|"\r\n"

%X MULTI_LINE_COMMENT
%X SINGLE_LINE_COMMENT
%X STRLIT

%%

"/*"							{BEGIN MULTI_LINE_COMMENT;
								plica[0]=line;
								plica[1]=colunm;
								colunm+=yyleng;}

<MULTI_LINE_COMMENT><<EOF>>		{printf("Line %d, col %d: unterminated comment\n", plica[0], plica[1]);
								colunm+=yyleng;
								BEGIN 0;}

<MULTI_LINE_COMMENT>"*/"  		{colunm+=yyleng;
								BEGIN 0;}

<MULTI_LINE_COMMENT>{NEWLINE}	{line++;colunm=1;}

<MULTI_LINE_COMMENT>.			{colunm+=yyleng;}


"//"							{BEGIN SINGLE_LINE_COMMENT;
								colunm+=yyleng;}

<SINGLE_LINE_COMMENT>{NEWLINE}	{line++;colunm=1;BEGIN 0;}

<SINGLE_LINE_COMMENT>.			{colunm+=yyleng;}



"\""							{BEGIN STRLIT;
								plica[0]=line;
								plica[1]=colunm;
								colunm+=yyleng;
								validSTRLIT = 1;
								strlitAux = yytext;}

<STRLIT>{SEQESCAPE}				{colunm+=yyleng;}

<STRLIT>((\\.)|(\\))			{printf("Line %d, col %d: invalid escape sequence (%s)\n", line, colunm, yytext);
								colunm+=yyleng;
								validSTRLIT = 0;}

<STRLIT><<EOF>>					{colunm+=yyleng;
								printf("Line %d, col %d: unterminated string literal\n", plica[0], plica[1]);
								BEGIN 0;}

<STRLIT>"\""  					{colunm+=yyleng;
								if(flag==1&&validSTRLIT==1)printf("STRLIT(%s)\n", strlitAux);
								BEGIN 0;}

<STRLIT>{NEWLINE}				{line++;colunm=1;
								printf("Line %d, col %d: unterminated string literal\n", plica[0], plica[1]);
								BEGIN 0;}
								
<STRLIT>.						{colunm+=yyleng;}


{BOOLLIT}						{if(flag==1)printf("BOOLLIT(%s)\n",yytext);colunm+=yyleng;}
{BOOL}							{if(flag==1)printf("BOOL\n");colunm+=yyleng;}
{CLASS}							{if(flag==1)printf("CLASS\n");colunm+=yyleng;}
{DO}							{if(flag==1)printf("DO\n");colunm+=yyleng;}
{DOTLENGTH}						{if(flag==1)printf("DOTLENGTH\n");colunm+=yyleng;}
{DOUBLE}						{if(flag==1)printf("DOUBLE\n");colunm+=yyleng;}
{RESERVED}						{if(flag==1)printf("RESERVED(%s)\n",yytext);colunm+=yyleng;}
{INT}							{if(flag==1)printf("INT\n");colunm+=yyleng;}
{PARSEINT}						{if(flag==1)printf("PARSEINT\n");colunm+=yyleng;}
{PRINT}							{if(flag==1)printf("PRINT\n");colunm+=yyleng;}
{PUBLIC}						{if(flag==1)printf("PUBLIC\n");colunm+=yyleng;}
{RETURN}						{if(flag==1)printf("RETURN\n");colunm+=yyleng;}
{IF}							{if(flag==1)printf("IF\n");colunm+=yyleng;}
{ELSE}							{if(flag==1)printf("ELSE\n");colunm+=yyleng;}
{STATIC}						{if(flag==1)printf("STATIC\n");colunm+=yyleng;}
{STRING}						{if(flag==1)printf("STRING\n");colunm+=yyleng;}
{VOID}							{if(flag==1)printf("VOID\n");colunm+=yyleng;}
{SEMI}							{if(flag==1)printf("SEMI\n");colunm+=yyleng;}
{OCURV}							{if(flag==1)printf("OCURV\n");colunm+=yyleng;}
{CCURV}							{if(flag==1)printf("CCURV\n");colunm+=yyleng;}
{OBRACE}						{if(flag==1)printf("OBRACE\n");colunm+=yyleng;}
{CBRACE}						{if(flag==1)printf("CBRACE\n");colunm+=yyleng;}
{OSQUARE}						{if(flag==1)printf("OSQUARE\n");colunm+=yyleng;}
{CSQUARE}						{if(flag==1)printf("CSQUARE\n");colunm+=yyleng;}
{AND}							{if(flag==1)printf("AND\n");colunm+=yyleng;}
{OR}							{if(flag==1)printf("OR\n");colunm+=yyleng;}
{LT}							{if(flag==1)printf("LT\n");colunm+=yyleng;}
{GT}							{if(flag==1)printf("GT\n");colunm+=yyleng;}
{EQ}							{if(flag==1)printf("EQ\n");colunm+=yyleng;}
{NEQ}							{if(flag==1)printf("NEQ\n");colunm+=yyleng;}
{LEQ}							{if(flag==1)printf("LEQ\n");colunm+=yyleng;}
{GEQ}							{if(flag==1)printf("GEQ\n");colunm+=yyleng;}
{PLUS}							{if(flag==1)printf("PLUS\n");colunm+=yyleng;}
{MINUS}							{if(flag==1)printf("MINUS\n");colunm+=yyleng;}
{STAR}							{if(flag==1)printf("STAR\n");colunm+=yyleng;}
{DIV}							{if(flag==1)printf("DIV\n");colunm+=yyleng;}
{MOD}							{if(flag==1)printf("MOD\n");colunm+=yyleng;}
{NOT}							{if(flag==1)printf("NOT\n");colunm+=yyleng;}
{ASSIGN}						{if(flag==1)printf("ASSIGN\n");colunm+=yyleng;}
{COMMA}							{if(flag==1)printf("COMMA\n");colunm+=yyleng;}
{WHILE}							{if(flag==1)printf("WHILE\n");colunm+=yyleng;}
{DECLIT}						{if(flag==1)printf("DECLIT(%s)\n", yytext);colunm+=yyleng;}
{ID}							{if(flag==1)printf("ID(%s)\n",yytext);colunm+=yyleng;}
{REALLIT}						{if(flag==1)printf("REALLIT(%s)\n",yytext);colunm+=yyleng;}
{NEWLINE}						{line++;colunm=1;}
" "|\f|\t						{colunm+=yyleng;}
.								{printf("Line %d, col %d: illegal character (%s)\n", line, colunm, yytext); colunm+=yyleng;}

%%

int main(int argc, char ** argv)
{	
	if(argc>1){
		if(strcmp(argv[1],"-1")==0|| strcmp(argv[1],"-l")==0){
			flag = 1;
		}
	}
	yylex();
	return 0;
}

int yywrap()
{
	return 1;
}