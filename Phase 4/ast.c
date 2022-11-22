#include "header.h"

/* ------------------ TOKEN --------------- */

token *createToken(char *value, int line, int colunm) {
  token *new = (token *)malloc(sizeof(token));
  if (value != NULL) {
    new->value = (char *)strdup(value);
  } else {
    new->value = NULL;
  }
  new->line = line;
  new->colunm = colunm;
  return new;
}

void freeToken(token *aux) {
  if (aux->value != NULL) {
    free(aux->value);
    aux->value = NULL;
  }
  free(aux);
  aux = NULL;
}

/* ------------------- AST --------------- */

int itsTerminal(char *nodeType) {
  if (strcmp(nodeType, "Bool") == 0 || strcmp(nodeType, "BoolLit") == 0 || strcmp(nodeType, "Double") == 0 || strcmp(nodeType, "DecLit") == 0 || strcmp(nodeType, "Id") == 0 || strcmp(nodeType, "Int") == 0 || strcmp(nodeType, "RealLit") == 0 || strcmp(nodeType, "StrLit") == 0 || strcmp(nodeType, "Void") == 0) {
    return 1;
  }
  return 0;
}

node *createNode(char *nodeType, char *value, int line, int colunm) {
  node *aux = (node *)malloc(sizeof(node));
  aux->nodeType = (char *)strdup(nodeType);
  if (value != NULL) {
    aux->value = (char *)strdup(value);
  } else {
    aux->value = NULL;
  }

  aux->line = line;
  aux->colunm = colunm;
  aux->anoted = NULL;
  aux->params = NULL;
  aux->n_params = -1;
  aux->to_anote = 1;
  aux->children = NULL;
  aux->brother = NULL;
  return aux;
}

void insertBrother(node *bOld, node *bNew) {
  if (bOld == NULL || bNew == NULL) {
    return;
  }

  node *aux = bOld;
  while (aux->brother != NULL) {
    aux = aux->brother;
  }

  aux->brother = bNew;
}

void insertChildren(node *f, node *s) {
  if (f == NULL || s == NULL) {
    return;
  }

  f->children = s;
}

void atributeType(node *type, node *varDecl) {
  node *aux = varDecl;
  node *new = NULL;

  while (aux != NULL) {
    new = createNode(type->nodeType, NULL, 0, 0);
    new->brother = aux->children;
    aux->children = new;
    aux = aux->brother;
  }
}

int countChildrens(node *n) {
  int count = 0;
  node *aux;

  aux = n;
  while (aux->brother != NULL) {
    count++;
    aux = aux->brother;
  }

  return count;
}

int addBlock(node *n) {
  int count = 0;
  if (n == NULL) {
    return count;
  }

  if (n->brother != NULL) {
    count = 1;
  }

  while (n->brother != NULL) {
    if (strcmp((n->brother)->nodeType, "NULL") != 0) {
      count++;
    }
    n = n->brother;
  }

  return count;
}

void printNode(node *current) {
  if (strcmp(current->nodeType, "Id") == 0 || strcmp(current->nodeType, "DecLit") == 0 || strcmp(current->nodeType, "RealLit") == 0 || strcmp(current->nodeType, "BoolLit") == 0) {
    printf("%s(%s)\n", current->nodeType, current->value);
  } else {
    printf("%s\n", current->nodeType);
  }
}

void printAST(node *current, int n) {
  int i;

  if (current == NULL) {
    return;
  }

  if (strcmp(current->nodeType, "NULL") == 0) {
    printAST(current->brother, n);
    return;
  }

  if (strcmp(current->nodeType, "NULL") != 0) {
    for (i = 0; i < n; i++) {
      printf("..");
    }

    if (current->value != NULL) {
      printf("%s(%s)\n", current->nodeType, current->value);
    } else {
      printf("%s\n", current->nodeType);
    }
  }

  printAST(current->children, n + 1);
  printAST(current->brother, n);
}

void clearAST(node *atual) {
  if (atual == NULL) {
    return;
  }

  if (atual->nodeType != NULL) {
    free(atual->nodeType);
    atual->nodeType = NULL;
  }
  if (atual->value != NULL) {
    free(atual->value);
    atual->value = NULL;
  }

  clearAST(atual->children);
  atual->children = NULL;
  clearAST(atual->brother);
  atual->brother = NULL;

  free(atual);
  atual = NULL;
}