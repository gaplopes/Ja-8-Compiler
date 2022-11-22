%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdarg.h>
    #include "header.h"

    int yylex(void);
    void yyerror (const char *s);

    node *root = NULL;
    node *auxNode = NULL;
    node *auxNode2 = NULL;
    node *auxNode3 = NULL;
%}

%union{
    struct token* value;
    struct node* node;
};

%token <value> CLASS OBRACE SEMI CBRACE STATIC BOOL BOOLLIT DO DOTLENGTH DOUBLE ELSE IF INT
PARSEINT PRINT PUBLIC RETURN STRING VOID WHILE OCURV CCURV AND OR LT GT ID EQ NEQ LEQ GEQ 
PLUS MINUS STAR DIV MOD NOT ASSIGN COMMA RESERVED OSQUARE CSQUARE DECLIT REALLIT STRLIT 

%nonassoc   IFX
%nonassoc   ELSE

%left   COMMA
%right  ASSIGN
%left   OR
%left   AND
%left   EQ NEQ
%left   LT GT LEQ GEQ
%left   PLUS MINUS
%left   STAR DIV MOD
%right  NOT

%type <node> Program auxProgram FieldDecl auxFieldDecl MethodDecl MethodHeader MethodBody auxMethodBody 
FormalParams auxFormalParams VarDecl auxVarDecl Type Statement auxStatement Assignment MethodInvocation 
auxMethodInvocation ParseArgs Expr Expr2

%%

Program: CLASS ID OBRACE auxProgram CBRACE                  {root = createNode("Program", NULL, 0, 0);
                                                            auxNode = createNode("Id", $2->value, $2->line, $2->colunm);
                                                            insertChildren(root, auxNode);
                                                            insertBrother(auxNode, $4);

                                                            freeToken($2);}

auxProgram: %empty                                          {$$ = createNode("NULL", NULL, 0, 0);}
    |   auxProgram FieldDecl                                {$$ = $1; insertBrother($1, $2);}
    |   auxProgram MethodDecl                               {$$ = $1; insertBrother($1, $2);}
    |   auxProgram SEMI                                     {$$ = $1;}
    ;

FieldDecl: PUBLIC STATIC Type ID auxFieldDecl SEMI          {$$ = createNode("FieldDecl", NULL, 0, 0);
                                                            insertChildren($$, $3);
                                                            auxNode = createNode("Id", $4->value, $4->line, $4->colunm);
                                                            insertBrother($3, auxNode);
                                                            atributeType($3, $5);
                                                            insertBrother($$, $5);

                                                            freeToken($4);}

    |   error SEMI                                          {$$ = createNode("Error", NULL, 0, 0);}
    ;

auxFieldDecl: %empty                                        {$$ = createNode("NULL", NULL, 0, 0);}
    |   auxFieldDecl COMMA ID                               {$$ = createNode("FieldDecl", NULL, 0, 0);
                                                            insertBrother($1, $$);
                                                            auxNode = createNode("Id", $3->value, $3->line, $3->colunm);
                                                            insertChildren($$, auxNode);
                                                            $$ = $1; 

                                                            freeToken($3);}
    ;

MethodDecl: PUBLIC STATIC MethodHeader MethodBody           {$$ = createNode("MethodDecl", NULL, 0, 0);
                                                            insertChildren($$, $3);
                                                            insertBrother($3, $4);}

MethodHeader: Type ID OCURV CCURV                           {$$ = createNode("MethodHeader", NULL, 0, 0);
                                                            insertChildren($$, $1);
                                                            auxNode = createNode("Id", $2->value, $2->line, $2->colunm);
                                                            insertBrother($1, auxNode);
                                                            insertBrother(auxNode, createNode("MethodParams", NULL, 0, 0));
                                                            
                                                            freeToken($2);}
    
    |   Type ID OCURV FormalParams CCURV                    {$$ = createNode("MethodHeader", NULL, 0, 0);
                                                            insertChildren($$, $1);
                                                            auxNode = createNode("Id", $2->value, $2->line, $2->colunm);
                                                            insertBrother($1, auxNode);
                                                            insertBrother(auxNode, $4);

                                                            freeToken($2);}

    |   VOID ID OCURV CCURV                                 {$$ = createNode("MethodHeader", NULL, 0, 0);
                                                            auxNode = createNode("Void", NULL, $1->line, $1->colunm);
                                                            insertChildren($$, auxNode);
                                                            auxNode2 = createNode("Id", $2->value, $2->line, $2->colunm);
                                                            insertBrother(auxNode, auxNode2);
                                                            insertBrother(auxNode2, createNode("MethodParams", NULL, 0, 0));
                                                            
                                                            freeToken($1);
                                                            freeToken($2);}

    |   VOID ID OCURV FormalParams CCURV                    {$$ = createNode("MethodHeader", NULL, 0, 0);
                                                            auxNode = createNode("Void", NULL, $1->line, $1->colunm);
                                                            insertChildren($$, auxNode);
                                                            auxNode2 = createNode("Id", $2->value, $2->line, $2->colunm);
                                                            insertBrother(auxNode, auxNode2);
                                                            insertBrother(auxNode2, $4);

                                                            freeToken($1);
                                                            freeToken($2);}
    ;

MethodBody: OBRACE auxMethodBody CBRACE                     {$$ = createNode("MethodBody", NULL, 0, 0);
                                                            insertChildren($$, $2);}

auxMethodBody: %empty                                       {$$ = createNode("NULL", NULL, 0, 0);}
    |   auxMethodBody VarDecl                               {$$ = $1; insertBrother($1, $2);}
    |   auxMethodBody Statement                             {$$ = $1; insertBrother($1, $2);}
    ;

FormalParams: Type ID auxFormalParams                       {$$ = createNode("MethodParams", NULL, 0, 0);
                                                            auxNode = createNode("ParamDecl", NULL, 0, 0);
                                                            insertChildren($$, auxNode);
                                                            auxNode2 = createNode("Id", $2->value, $2->line, $2->colunm);
                                                            insertBrother($1, auxNode2);
                                                            insertChildren(auxNode, $1);
                                                            insertBrother(auxNode, $3);

                                                            freeToken($2);}

    |   STRING OSQUARE CSQUARE ID                           {$$ = createNode("MethodParams", NULL, 0, 0);
                                                            auxNode = createNode("ParamDecl", NULL, 0, 0);
                                                            insertChildren($$, auxNode);
                                                            auxNode2 = createNode("StringArray", NULL, $1->line, $1->colunm);
                                                            insertChildren(auxNode, auxNode2);
                                                            auxNode3 = createNode("Id", $4->value, $4->line, $4->colunm);
                                                            insertBrother(auxNode2, auxNode3);

                                                            freeToken($1);
                                                            freeToken($4);}
    ;

auxFormalParams: %empty                                     {$$ = createNode("NULL", NULL, 0, 0);}
    |   auxFormalParams COMMA Type ID                       {$$ = createNode("ParamDecl", NULL, 0, 0);
                                                            auxNode = createNode("Id", $4->value, $4->line, $4->colunm);
                                                            insertChildren($$, $3);
                                                            insertBrother($3, auxNode);
                                                            insertBrother($1, $$);
                                                            $$ = $1;

                                                            freeToken($4);}
    ;

VarDecl: Type ID auxVarDecl SEMI                            {$$ = createNode("VarDecl", NULL, 0, 0);
                                                            insertChildren($$, $1);
                                                            auxNode = createNode("Id", $2->value, $2->line, $2->colunm);
                                                            insertBrother($1, auxNode);
                                                            atributeType($1, $3);
                                                            insertBrother($$, $3);

                                                            freeToken($2);}

auxVarDecl: %empty                                          {$$ = createNode("NULL", NULL, 0, 0);}
    |   auxVarDecl COMMA ID                                 {$$ = createNode("VarDecl", NULL, 0, 0);
                                                            auxNode = createNode("Id", $3->value, $3->line, $3->colunm);
                                                            insertChildren($$, auxNode);
                                                            insertBrother($1, $$);
                                                            $$ = $1;

                                                            freeToken($3);}
    ;

Type: BOOL                                                  {$$ = createNode("Bool", NULL, 0, 0);}
    |   INT                                                 {$$ = createNode("Int", NULL, 0, 0);}
    |   DOUBLE                                              {$$ = createNode("Double", NULL, 0, 0);}
    ;

Statement: OBRACE auxStatement CBRACE                       {if(addBlock($2)>2){$$ = createNode("Block", NULL, 0, 0); insertChildren($$, $2);}
                                                            else if(addBlock($2)==0){$$ = NULL; clearAST($2);}
                                                            else{$$ = $2;}}

    |   IF OCURV Expr CCURV Statement %prec IFX             {$$ = createNode("If",NULL, $1->line, $1->colunm); 
                                                            if($5 == NULL){auxNode=createNode("Block",NULL, 0, 0); insertBrother($3, auxNode);}
                                                            else{insertBrother($3,$5);} insertBrother($3, createNode("Block",NULL, 0, 0)); insertChildren($$,$3);
                                                            freeToken($1);}

    |   IF OCURV Expr CCURV Statement ELSE Statement        {$$ = createNode("If", NULL, $1->line, $1->colunm); 
                                                            if($5 == NULL){auxNode=createNode("Block", NULL, 0, 0); insertBrother($3, auxNode);}
                                                            else{insertBrother($3, $5);}
                                                            if($7 == NULL){auxNode = createNode("Block", NULL, 0, 0); insertBrother($3, auxNode);}
                                                            else{insertBrother($3, $7);} insertChildren($$,$3);
                                                            freeToken($1);}

    |   WHILE OCURV Expr CCURV Statement                    {$$ = createNode("While",NULL, $1->line, $1->colunm);
                                                            if($5 == NULL){auxNode=createNode("Block",NULL, 0, 0); insertBrother($3, auxNode);}
                                                            else{insertBrother($3,$5);}
                                                            insertChildren($$,$3);
                                                            freeToken($1);}
                                                            
    |   DO Statement WHILE OCURV Expr CCURV SEMI            {$$ = createNode("DoWhile", NULL, $3->line, $3->colunm);
                                                            if($2 == NULL){auxNode = createNode("Block", NULL, 0, 0); insertBrother(auxNode, $5); insertChildren($$, auxNode);}
                                                            else{insertBrother($2, $5);}  insertChildren($$, $2);
                                                            freeToken($3);}

    |   PRINT OCURV Expr CCURV SEMI                         {$$ = createNode("Print", NULL, $1->line, $1->colunm);
                                                            insertChildren($$, $3);
                                                            freeToken($1);}

    |   PRINT OCURV STRLIT CCURV SEMI                       {$$ = createNode("Print", NULL, $1->line, $1->colunm);
                                                            auxNode = createNode("StrLit", $3->value, $3->line, $3->colunm);
                                                            insertChildren($$, auxNode);

                                                            freeToken($1);
                                                            freeToken($3);}

    |   SEMI                                                {$$ = NULL;}
    |   Assignment SEMI                                     {$$ = $1;}
    |   MethodInvocation SEMI                               {$$ = $1;}
    |   ParseArgs SEMI                                      {$$ = $1;}
    |   RETURN Expr SEMI                                    {$$ = createNode("Return", NULL, $1->line, $1->colunm);
                                                            insertChildren($$, $2);
                                                            freeToken($1);}

    |   RETURN SEMI                                         {$$ = createNode("Return", NULL, $1->line, $1->colunm);
                                                            freeToken($1);}

    |   error SEMI                                          {$$ = createNode("Error", NULL, 0, 0);}
    ;

auxStatement: %empty                                        {$$ = createNode("NULL", NULL, 0, 0);}
    |   auxStatement Statement                              {insertBrother($1, $2); $$ = $1;}
    ;

Assignment: ID ASSIGN Expr                                  {$$ = createNode("Assign", NULL, $2->line, $2->colunm);
                                                            auxNode = createNode("Id", $1->value, $1->line, $1->colunm);
                                                            insertChildren($$, auxNode);
                                                            insertBrother(auxNode, $3);

                                                            freeToken($2);
                                                            freeToken($1);}

MethodInvocation: ID OCURV CCURV                            {$$ = createNode("Call", NULL, $1->line, $1->colunm);
                                                            auxNode = createNode("Id", $1->value, $1->line, $1->colunm);
                                                            insertChildren($$, auxNode);

                                                            freeToken($1);}

    |   ID OCURV Expr auxMethodInvocation CCURV             {$$ = createNode("Call", NULL, $1->line, $1->colunm);
                                                            auxNode = createNode("Id", $1->value, $1->line, $1->colunm);
                                                            insertChildren($$, auxNode);
                                                            insertBrother(auxNode, $3);
                                                            insertBrother(auxNode, $4);

                                                            freeToken($1);}

    |   ID OCURV error CCURV                                {$$ = createNode("Error", NULL, 0, 0); 
                                                            freeToken($1);}
    ;

auxMethodInvocation: %empty                                 {$$ = createNode("NULL", NULL, 0, 0);}
    |   auxMethodInvocation COMMA Expr                      {$$ = $1; insertBrother($1, $3);}
    ;

ParseArgs: PARSEINT OCURV ID OSQUARE Expr CSQUARE CCURV     {$$ = createNode("ParseArgs", NULL, $1->line, $1->colunm);
                                                            auxNode = createNode("Id", $3->value, $3->line, $3->colunm);
                                                            insertChildren($$, auxNode);
                                                            insertBrother(auxNode, $5);

                                                            freeToken($1);
                                                            freeToken($3);}

    |   PARSEINT OCURV error CCURV                          {$$ = createNode("Error", NULL, 0, 0);}
    ;

Expr: Assignment                                            {$$ = $1;}
    |   Expr2                                               {$$ = $1;}
    ;

Expr2: MethodInvocation                                     {$$ = $1;}
    |   ParseArgs                                           {$$ = $1;}
    |   Expr2 AND Expr2                                     {$$ = createNode("And", NULL, $2->line, $2->colunm);
                                                            insertChildren($$,$1); insertBrother($1,$3); freeToken($2);}

    |   Expr2 OR Expr2                                      {$$ = createNode("Or", NULL, $2->line, $2->colunm);
                                                            insertChildren($$,$1); insertBrother($1,$3); freeToken($2);}

    |   Expr2 EQ Expr2                                      {$$ = createNode("Eq", NULL, $2->line, $2->colunm);
                                                            insertChildren($$,$1); insertBrother($1,$3); freeToken($2);}

    |   Expr2 GEQ Expr2                                     {$$ = createNode("Geq", NULL, $2->line, $2->colunm);
                                                            insertChildren($$,$1); insertBrother($1,$3); freeToken($2);}

    |   Expr2 GT Expr2                                      {$$ = createNode("Gt", NULL, $2->line, $2->colunm);
                                                            insertChildren($$,$1); insertBrother($1,$3); freeToken($2);}

    |   Expr2 LEQ Expr2                                     {$$ = createNode("Leq", NULL, $2->line, $2->colunm);
                                                            insertChildren($$,$1); insertBrother($1,$3); freeToken($2);}

    |   Expr2 LT Expr2                                      {$$ = createNode("Lt", NULL, $2->line, $2->colunm);
                                                            insertChildren($$,$1); insertBrother($1,$3); freeToken($2);}

    |   Expr2 NEQ Expr2                                     {$$ = createNode("Neq", NULL, $2->line, $2->colunm);
                                                            insertChildren($$,$1); insertBrother($1,$3); freeToken($2);}

    |   Expr2 PLUS Expr2                                    {$$ = createNode("Add", NULL, $2->line, $2->colunm);
                                                            insertChildren($$,$1); insertBrother($1,$3); freeToken($2);}

    |   Expr2 MINUS Expr2                                   {$$ = createNode("Sub", NULL, $2->line, $2->colunm);
                                                            insertChildren($$,$1); insertBrother($1,$3); freeToken($2);}

    |   Expr2 STAR Expr2                                    {$$ = createNode("Mul", NULL, $2->line, $2->colunm);
                                                            insertChildren($$,$1); insertBrother($1,$3); freeToken($2);}

    |   Expr2 DIV Expr2                                     {$$ = createNode("Div", NULL, $2->line, $2->colunm);
                                                            insertChildren($$,$1); insertBrother($1,$3); freeToken($2);}

    |   Expr2 MOD Expr2                                     {$$ = createNode("Mod", NULL, $2->line, $2->colunm);
                                                            insertChildren($$,$1); insertBrother($1,$3); freeToken($2);}

    |   PLUS Expr2 %prec NOT                                {$$ = createNode("Plus", NULL, $1->line, $1->colunm);
                                                            insertChildren($$,$2); freeToken($1);}

    |   MINUS Expr2 %prec NOT                               {$$ = createNode("Minus", NULL, $1->line, $1->colunm);
                                                            insertChildren($$,$2); freeToken($1);}

    |   NOT Expr2                                           {$$ = createNode("Not", NULL, $1->line, $1->colunm);
                                                            insertChildren($$,$2); freeToken($1);}

    |   ID DOTLENGTH                                        {$$ = createNode("Length", NULL, $2->line, $2->colunm);
                                                            auxNode = createNode("Id", $1->value, $1->line, $1->colunm);
                                                            insertChildren($$, auxNode);
                                                            
                                                            freeToken($2); freeToken($1);}

    |   ID                                                  {$$ = createNode("Id", $1->value, $1->line, $1->colunm);
                                                            freeToken($1);}

    |   OCURV Expr CCURV                                    {$$ = $2;}

    |   BOOLLIT                                             {$$ = createNode("BoolLit", $1->value, $1->line, $1->colunm); 
                                                            freeToken($1);}

    |   DECLIT                                              {$$ = createNode("DecLit", $1->value, $1->line, $1->colunm);
                                                            freeToken($1);}

    |   REALLIT                                             {$$ = createNode("RealLit", $1->value, $1->line, $1->colunm);
                                                            freeToken($1);}

    |   OCURV error CCURV                                   {$$ = createNode("Error", NULL, 0, 0);}
    ;

%%
