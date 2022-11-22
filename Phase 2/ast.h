#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
  struct node *children;
  struct node *brother;
  int terminal;  // 1 se for terminal
  char *nodeType;
  char *value;
  int nChildrens;
} node;

int itsTerminal(char *nodeType);
node *createNode(char *nodeType, char *value);
void insertBrother(node *b1, node *b2);
void insertChildren(node *f, node *s);
void insertBrother(node *f, node *b);
void printNode(node *current);
int addBlock(node *n);
void atributeType(node *type, node *varDecl);
void printAST(node *atual, int n);
void clearAST(node *atual);