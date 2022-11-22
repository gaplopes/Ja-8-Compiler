%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdarg.h>
    #include "ast.h"

    int yylex(void);
    void yyerror (const char *s);

    node *root = NULL;
    node *auxNode = NULL;
    node *auxNode2 = NULL;
    node *auxNode3 = NULL;
%}

%union{
    char* value;
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

Program: CLASS ID OBRACE auxProgram CBRACE                  {root = createNode("Program", NULL); 
                                                            auxNode = createNode("Id", $2);
                                                            insertChildren(root, auxNode);
                                                            insertBrother(auxNode, $4);}

auxProgram: %empty                                          {$$ = createNode("NULL", NULL);}
    |   auxProgram FieldDecl                                {$$ = $1; insertBrother($1, $2);}
    |   auxProgram MethodDecl                               {$$ = $1; insertBrother($1, $2);}
    |   auxProgram SEMI                                     {$$ = $1;}
    ;

FieldDecl: PUBLIC STATIC Type ID auxFieldDecl SEMI          {$$ = createNode("FieldDecl", NULL);
                                                            insertChildren($$, $3);
                                                            auxNode = createNode("Id", $4);
                                                            insertBrother($3, auxNode);
                                                            atributeType($3, $5);
                                                            insertBrother($$, $5);}

    |   error SEMI                                          {$$ = createNode("Error", NULL);}
    ;

auxFieldDecl: %empty                                        {$$ = createNode("NULL", NULL);}
    |   auxFieldDecl COMMA ID                               {$$ = createNode("FieldDecl", NULL);
                                                            insertBrother($1, $$);
                                                            auxNode = createNode("Id", $3);
                                                            insertChildren($$, auxNode);
                                                            $$ = $1;}
    ;

MethodDecl: PUBLIC STATIC MethodHeader MethodBody           {$$ = createNode("MethodDecl", NULL);
                                                            insertChildren($$, $3);
                                                            insertBrother($3, $4);}

MethodHeader: Type ID OCURV CCURV                           {$$ = createNode("MethodHeader", NULL);
                                                            insertChildren($$, $1);
                                                            auxNode = createNode("Id", $2);
                                                            insertBrother($1, auxNode);
                                                            insertBrother(auxNode, createNode("MethodParams", NULL));}
    
    |   Type ID OCURV FormalParams CCURV                    {$$ = createNode("MethodHeader", NULL);
                                                            insertChildren($$, $1);
                                                            auxNode = createNode("Id", $2);
                                                            insertBrother($1, auxNode);
                                                            insertBrother(auxNode, $4);}

    |   VOID ID OCURV CCURV                                 {$$ = createNode("MethodHeader", NULL);
                                                            auxNode = createNode("Void", NULL);
                                                            insertChildren($$, auxNode);
                                                            auxNode2 = createNode("Id", $2);
                                                            insertBrother(auxNode, auxNode2);
                                                            insertBrother(auxNode2, createNode("MethodParams", NULL));}

    |   VOID ID OCURV FormalParams CCURV                    {$$ = createNode("MethodHeader", NULL);
                                                            auxNode = createNode("Void", NULL);
                                                            insertChildren($$, auxNode);
                                                            auxNode2 = createNode("Id", $2);
                                                            insertBrother(auxNode, auxNode2);
                                                            insertBrother(auxNode2, $4);}
    ;

MethodBody: OBRACE auxMethodBody CBRACE                     {$$ = createNode("MethodBody", NULL);
                                                            insertChildren($$, $2);}

auxMethodBody: %empty                                       {$$ = createNode("NULL", NULL);}
    |   auxMethodBody VarDecl                               {$$ = $1; insertBrother($1, $2);}
    |   auxMethodBody Statement                             {$$ = $1; insertBrother($1, $2);}
    ;

FormalParams: Type ID auxFormalParams                       {$$ = createNode("MethodParams", NULL);
                                                            auxNode = createNode("ParamDecl", NULL);
                                                            insertChildren($$, auxNode);
                                                            insertBrother($1, createNode("Id", $2));
                                                            insertChildren(auxNode, $1);
                                                            insertBrother(auxNode, $3);}

    |   STRING OSQUARE CSQUARE ID                           {$$ = createNode("MethodParams", NULL);
                                                            auxNode = createNode("ParamDecl", NULL);
                                                            insertChildren($$, auxNode);
                                                            auxNode2 = createNode("StringArray", NULL);
                                                            insertChildren(auxNode, auxNode2);
                                                            auxNode3 = createNode("Id", $4);
                                                            insertBrother(auxNode2, auxNode3);}
    ;

auxFormalParams: %empty                                     {$$ = createNode("NULL", NULL);}
    |   auxFormalParams COMMA Type ID                       {$$ = createNode("ParamDecl", NULL);
                                                            auxNode = createNode("Id", $4);
                                                            insertChildren($$, $3);
                                                            insertBrother($3, auxNode);
                                                            insertBrother($1, $$);
                                                            $$ = $1;}
    ;

VarDecl: Type ID auxVarDecl SEMI                            {$$ = createNode("VarDecl", NULL);
                                                            insertChildren($$, $1);
                                                            insertBrother($1, createNode("Id", $2));
                                                            atributeType($1, $3);
                                                            insertBrother($$, $3);}

auxVarDecl: %empty                                          {$$ = createNode("NULL", NULL);}
    |   auxVarDecl COMMA ID                                 {$$ = createNode("VarDecl", NULL);
                                                            insertChildren($$, createNode("Id", $3));
                                                            insertBrother($1, $$);
                                                            $$ = $1;}
    ;

Type: BOOL                                                  {$$ = createNode("Bool", NULL);}
    |   INT                                                 {$$ = createNode("Int", NULL);}
    |   DOUBLE                                              {$$ = createNode("Double", NULL);}
    ;

Statement: OBRACE auxStatement CBRACE                       {if(addBlock($2)>2){$$ = createNode("Block", NULL); insertChildren($$, $2);}
                                                            else if(addBlock($2)==0){$$ = NULL;}
                                                            else{$$ = $2;}}

    |   IF OCURV Expr CCURV Statement %prec IFX             {$$ = createNode("If",NULL); 
                                                            if($5 == NULL){auxNode=createNode("Block",NULL); insertBrother($3, auxNode);}
                                                            else{insertBrother($3,$5);} insertBrother($3, createNode("Block",NULL)); insertChildren($$,$3);}

    |   IF OCURV Expr CCURV Statement ELSE Statement        {$$ = createNode("If", NULL); 
                                                            if($5 == NULL){auxNode=createNode("Block", NULL); insertBrother($3, auxNode);}
                                                            else{insertBrother($3, $5);}
                                                            if($7 == NULL){auxNode = createNode("Block", NULL); insertBrother($3, auxNode);}
                                                            else{insertBrother($3, $7);} insertChildren($$,$3); }

    |   WHILE OCURV Expr CCURV Statement                    {$$ = createNode("While",NULL);
                                                            if($5 == NULL){auxNode=createNode("Block",NULL); insertBrother($3, auxNode);}
                                                            else{insertBrother($3,$5);}
                                                            insertChildren($$,$3);}
                                                            
    |   DO Statement WHILE OCURV Expr CCURV SEMI            {$$ = createNode("DoWhile", NULL);
                                                            if($2 == NULL){auxNode = createNode("Block", NULL); insertBrother(auxNode, $5); insertChildren($$, auxNode);}
                                                            else{insertBrother($2, $5);}  insertChildren($$, $2);}

    |   PRINT OCURV Expr CCURV SEMI                         {$$ = createNode("Print", NULL); insertChildren($$, $3);}
    |   PRINT OCURV STRLIT CCURV SEMI                       {$$ = createNode("Print", NULL);
                                                            auxNode = createNode("StrLit", $3);
                                                            insertChildren($$, auxNode);}

    |   SEMI                                                {$$ = NULL;}
    |   Assignment SEMI                                     {$$ = $1;}
    |   MethodInvocation SEMI                               {$$ = $1;}
    |   ParseArgs SEMI                                      {$$ = $1;}
    |   RETURN Expr SEMI                                    {$$ = createNode("Return", NULL); insertChildren($$, $2);}
    |   RETURN SEMI                                         {$$ = createNode("Return", NULL);}
    |   error SEMI                                          {$$ = createNode("Error", NULL);}
    ;

auxStatement: %empty                                        {$$ = createNode("NULL", NULL);}
    |   auxStatement Statement                              {insertBrother($1, $2); $$ = $1;}
    ;

Assignment: ID ASSIGN Expr                                  {$$ = createNode("Assign", NULL);
                                                            auxNode = createNode("Id", $1);
                                                            insertChildren($$, auxNode);
                                                            insertBrother(auxNode, $3);}

MethodInvocation: ID OCURV CCURV                            {$$ = createNode("Call", NULL);
                                                            auxNode = createNode("Id", $1);
                                                            insertChildren($$, auxNode);}

    |   ID OCURV Expr auxMethodInvocation CCURV             {$$ = createNode("Call", NULL);
                                                            auxNode = createNode("Id", $1);
                                                            insertChildren($$, auxNode);
                                                            insertBrother(auxNode, $3);
                                                            insertBrother(auxNode, $4);}

    |   ID OCURV error CCURV                                {$$ = createNode("Error", NULL);}
    ;

auxMethodInvocation: %empty                                 {$$ = createNode("NULL", NULL);}
    |   auxMethodInvocation COMMA Expr                      {$$ = $1; insertBrother($1, $3);}
    ;

ParseArgs: PARSEINT OCURV ID OSQUARE Expr CSQUARE CCURV     {$$ = createNode("ParseArgs", NULL);
                                                            auxNode = createNode("Id", $3);
                                                            insertChildren($$, auxNode);
                                                            insertBrother(auxNode, $5);}

    |   PARSEINT OCURV error CCURV                          {$$ = createNode("Error", NULL);}
    ;

Expr: Assignment                                            {$$ = $1;}
    |   Expr2                                               {$$ = $1;}
    ;

Expr2: MethodInvocation                                     {$$ = $1;}
    |   ParseArgs                                           {$$ = $1;}
    |   Expr2 AND Expr2                                     {$$ = createNode("And", NULL);
                                                            insertChildren($$,$1); insertBrother($1,$3);}

    |   Expr2 OR Expr2                                      {$$ = createNode("Or", NULL);
                                                            insertChildren($$,$1); insertBrother($1,$3);}

    |   Expr2 EQ Expr2                                      {$$ = createNode("Eq", NULL);
                                                            insertChildren($$,$1); insertBrother($1,$3);}

    |   Expr2 GEQ Expr2                                     {$$ = createNode("Geq", NULL);
                                                            insertChildren($$,$1); insertBrother($1,$3);}

    |   Expr2 GT Expr2                                      {$$ = createNode("Gt", NULL);
                                                            insertChildren($$,$1); insertBrother($1,$3);}

    |   Expr2 LEQ Expr2                                     {$$ = createNode("Leq", NULL);
                                                            insertChildren($$,$1); insertBrother($1,$3);}

    |   Expr2 LT Expr2                                      {$$ = createNode("Lt", NULL);
                                                            insertChildren($$,$1); insertBrother($1,$3);}

    |   Expr2 NEQ Expr2                                     {$$ = createNode("Neq", NULL);
                                                            insertChildren($$,$1); insertBrother($1,$3);}

    |   Expr2 PLUS Expr2                                    {$$ = createNode("Add", NULL);
                                                            insertChildren($$,$1); insertBrother($1,$3);}

    |   Expr2 MINUS Expr2                                   {$$ = createNode("Sub", NULL);
                                                            insertChildren($$,$1); insertBrother($1,$3);}

    |   Expr2 STAR Expr2                                    {$$ = createNode("Mul", NULL);
                                                            insertChildren($$,$1); insertBrother($1,$3);}

    |   Expr2 DIV Expr2                                     {$$ = createNode("Div", NULL);
                                                            insertChildren($$,$1); insertBrother($1,$3);}

    |   Expr2 MOD Expr2                                     {$$ = createNode("Mod", NULL);
                                                            insertChildren($$,$1); insertBrother($1,$3);}

    |   PLUS Expr2 %prec NOT                                {$$ = createNode("Plus", NULL);
                                                            insertChildren($$,$2);}

    |   MINUS Expr2 %prec NOT                               {$$ = createNode("Minus", NULL);
                                                            insertChildren($$,$2);}

    |   NOT Expr2                                           {$$ = createNode("Not", NULL);
                                                            insertChildren($$,$2);}

    |   ID DOTLENGTH                                        {$$ = createNode("Length", NULL);
                                                            insertChildren($$, createNode("Id", $1));}

    |   ID                                                  {$$ = createNode("Id", $1);}
    |   OCURV Expr CCURV                                    {$$ = $2;}
    |   BOOLLIT                                             {$$ = createNode("BoolLit", $1);}
    |   DECLIT                                              {$$ = createNode("DecLit", $1);}
    |   REALLIT                                             {$$ = createNode("RealLit", $1);}
    |   OCURV error CCURV                                   {$$ = createNode("Error", NULL);}
    ;

%%
