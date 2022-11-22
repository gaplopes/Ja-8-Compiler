#include "header.h"

/* ------------------ SEMANTICS ---------------------- */

/* TABLES */

void clearParams(param_list *param) {
  if (param == NULL) {
    return;
  }

  if (param->type != NULL) {
    free(param->type);
    param->type = NULL;
  }
  if (param->id != NULL) {
    free(param->id);
    param->id = NULL;
  }

  clearParams(param->next);

  free(param);
  param = NULL;
}

void clearVars(var_list *var) {
  if (var == NULL) {
    return;
  }

  if (var->type != NULL) {
    free(var->type);
    var->type = NULL;
  }
  if (var->name != NULL) {
    free(var->name);
    var->name = NULL;
  }

  clearParams(var->paramTypes);
  clearVars(var->next);

  free(var);
  var = NULL;
}

void clearLocalTables(sym_table *local) {
  if (local == NULL) {
    return;
  }

  if (local->tableName != NULL) {
    free(local->tableName);
    local->tableName = NULL;
  }
  if (local->tableType != NULL) {
    free(local->tableType);
    local->tableType = NULL;
  }

  if (local->params != NULL) {
    clearParams(local->params->next);
    if (local->params->type != NULL) {
      free(local->params->type);
      local->params->type = NULL;
    }
    if (local->params->id != NULL) {
      free(local->params->id);
      local->params->id = NULL;
    }
    free(local->params);
    local->params = NULL;
  }
  if (local->vars != NULL) {
    clearVars(local->vars->next);
    if (local->vars->type != NULL) {
      free(local->vars->type);
      local->vars->type = NULL;
    }
    if (local->vars->name != NULL) {
      free(local->vars->name);
      local->vars->name = NULL;
    }
    free(local->vars);
    local->vars = NULL;
  }

  clearLocalTables(local->next);

  free(local);
  local = NULL;
}

void clearTables(sym_table *global, sym_table *local) {
  if (global != NULL) {
    if (global->tableName != NULL) {
      free(global->tableName);
      global->tableName = NULL;
    }
    if (global->tableType != NULL) {
      free(global->tableType);
      global->tableType = NULL;
    }

    clearVars(global->vars);

    free(global);
    global = NULL;
  }

  if (local != NULL) {
    clearLocalTables(local);
  }
}

void print_table_global(sym_table *atual) {
  var_list *auxV = atual->vars;
  param_list *auxP;

  printf("===== %s %s Symbol Table =====\n", atual->tableType, atual->tableName);
  while (auxV != NULL) {
    if (auxV->function == 1) {
      auxP = auxV->paramTypes;
      printf("%s\t(", auxV->name);
      while (auxP != NULL) {
        printf("%s", auxP->type);
        if (auxP->next != NULL) {
          printf(",");
        }
        auxP = auxP->next;
      }
      printf(")\t%s", auxV->type);
    } else {
      printf("%s\t\t%s", auxV->name, auxV->type);
    }

    printf("\n");

    auxV = auxV->next;
  }
  printf("\n");
}

void print_tables_local(sym_table *atual) {
  sym_table *aux = atual;
  var_list *auxV;
  param_list *auxP;

  while (aux != NULL) {
    auxV = aux->vars;
    auxP = aux->params;
    if (auxP != NULL) {
      printf("===== %s %s(", aux->tableType, aux->tableName);
      while (auxP != NULL) {
        printf("%s", auxP->type);
        if (auxP->next != NULL) {
          printf(",");
        }
        auxP = auxP->next;
      }
      printf(") Symbol Table =====\n");
    } else {
      printf("===== %s %s() Symbol Table =====\n", aux->tableType, aux->tableName);
    }
    while (auxV != NULL) {
      if (auxV->flag == 2) {
        // dont print, its repeated
      } else if (auxV->flag == 1) {
        printf("%s\t\t%s\t%s\n", auxV->name, auxV->type, "param");
      } else {
        printf("%s\t\t%s\n", auxV->name, auxV->type);
      }

      auxV = auxV->next;
    }

    printf("\n");

    aux = aux->next;
  }
}

void add_to_local_table(sym_table *table, sym_table *element) {
  sym_table *aux = table;

  if (aux == NULL) {
    table = element;
    return;
  }

  while (aux->next != NULL) {
    aux = aux->next;
  }

  aux->next = element;
}

param_list *create_param_type(char *type, char *id) {
  param_list *new = (param_list *)malloc(sizeof(param_list));
  new->type = (char *)strdup(type);
  new->id = (char *)strdup(id);
  new->next = NULL;
  return new;
}

void add_param(param_list *paramList, param_list *new) {
  param_list *aux = paramList;

  if (aux == NULL) {
    paramList = new;
    return;
  }

  while (aux->next != NULL) {
    aux = aux->next;
  }

  aux->next = new;
}

var_list *create_var(char *name, char *type) {
  var_list *new = (var_list *)malloc(sizeof(var_list));
  new->name = (char *)strdup(name);
  new->type = (char *)strdup(type);
  new->function = 0;
  new->flag = 0;
  new->n_params = 0;
  new->paramTypes = NULL;
  new->next = NULL;
  return new;
}

void add_var(var_list *varList, var_list *new) {
  var_list *aux = varList;

  if (aux == NULL) {
    varList = new;
    return;
  }

  while (aux->next != NULL) {
    aux = aux->next;
  }

  aux->next = new;
}

sym_table *create_table(char *name, char *type) {
  sym_table *aux = (sym_table *)malloc(sizeof(sym_table));
  aux->tableName = (char *)strdup(name);
  aux->tableType = (char *)strdup(type);

  aux->vars = NULL;
  aux->params = NULL;
  aux->next = NULL;
  return aux;
}

char *changeType(char *nodeType) {
  if (strcmp(nodeType, "Int") == 0) {
    nodeType = "int";
    return nodeType;
  }

  if (strcmp(nodeType, "Double") == 0) {
    nodeType = "double";
    return nodeType;
  }

  if (strcmp(nodeType, "Bool") == 0) {
    nodeType = "boolean";
    return nodeType;
  }

  if (strcmp(nodeType, "Void") == 0) {
    nodeType = "void";
    return nodeType;
  }

  if (strcmp(nodeType, "StringArray") == 0) {
    nodeType = "String[]";
    return nodeType;
  }

  return nodeType;
}

void create_semantic_table(node *atual) {
  char *aux;
  int error;
  sym_table *auxTable;
  var_list *auxVar, *auxVarLocal;
  param_list *auxParam, *auxParamPrint;
  node *aux1, *aux2, *aux3, *aux4, *aux5, *aux6, *auxProgram;
  int count_params;

  nErrorsSemantic = 0;

  if (atual == NULL) {
    return;
  }

  if (strcmp(atual->nodeType, "Program") == 0) {  // Fazemos primeiro a tabela global toda
    aux1 = atual->children;                       // ID

    global_table = create_table(aux1->value, "Class");

    auxProgram = aux1->brother;  // FieldDecl ou MethodDecl
    atual = aux1->brother;       // FieldDecl ou MethodDecl

    while (atual != NULL) {  // Fazemos primeiro a tabela global toda
      if (strcmp(atual->nodeType, "FieldDecl") == 0) {
        aux1 = atual->children;  // TYPE
        aux2 = aux1->brother;    // ID
        auxVar = create_var(aux2->value, changeType(aux1->nodeType));
        if (search_type_var(global_table, NULL, aux2->value) != NULL) {
          nErrorsSemantic = 1;
          printf("Line %d, col %d: Symbol %s already defined\n", aux2->line, aux2->colunm, aux2->value);
          clearVars(auxVar);
        } else {
          if (global_table->vars == NULL) {
            global_table->vars = auxVar;
          } else {
            add_var(global_table->vars, auxVar);
          }
        }
      }
      if (strcmp(atual->nodeType, "MethodDecl") == 0) {
        aux1 = atual->children;  // MethodHeader
        count_params = 0;
        while (aux1 != NULL) {
          if (strcmp(aux1->nodeType, "MethodHeader") == 0) {
            aux2 = aux1->children;  // TYPE
            aux3 = aux2->brother;   // ID

            auxVar = create_var(aux3->value, changeType(aux2->nodeType));  // ADD TO GLOBAL

            aux4 = aux3->brother;   // MethodParams
            aux5 = aux4->children;  // ParamDecl ou nada
            while (aux5 != NULL) {  // WHILE EXISTS ParamDecl's
              if (strcmp(aux5->nodeType, "NULL") != 0) {
                count_params++;
                // add to auxVar to global_table
                auxParam = create_param_type(changeType(aux5->children->nodeType), (aux5->children)->brother->value);

                aux = search_var_exists(auxVar->paramTypes, (aux5->children)->brother->value);
                if (aux != NULL) {
                  nErrorsSemantic = 1;
                  printf("Line %d, col %d: Symbol %s already defined\n", (aux5->children)->brother->line, (aux5->children)->brother->colunm, (aux5->children)->brother->value);
                }

                if (auxVar->paramTypes == NULL) {
                  auxVar->paramTypes = auxParam;
                } else {
                  add_param(auxVar->paramTypes, auxParam);
                }
              }
              aux5 = aux5->brother;
            }

            auxVar->function = 1;  // ITS FUNCTION
            auxVar->n_params = count_params;
            if (search_function_exists(global_table, auxVar->paramTypes, count_params, aux3->value) == 1) {
              nErrorsSemantic = 1;
              printf("Line %d, col %d: Symbol %s(", aux3->line, aux3->colunm, aux3->value);
              auxParamPrint = auxVar->paramTypes;
              while (auxParamPrint != NULL) {
                printf("%s", auxParamPrint->type);
                if (auxParamPrint->next != NULL) {
                  printf(",");
                }
                auxParamPrint = auxParamPrint->next;
              }
              printf(") already defined\n");
              aux3->to_anote = 0;
              clearVars(auxVar);
            } else {
              if (global_table->vars == NULL) {
                global_table->vars = auxVar;
              } else {
                add_var(global_table->vars, auxVar);
              }
            }
          }

          aux1 = aux1->brother;  // MethodHeader ou MethodBody
        }
      }

      atual = atual->brother;
    }

    atual = auxProgram;

    while (atual != NULL) {  // Depois fazemos as tabelas locais
      if (strcmp(atual->nodeType, "MethodDecl") == 0) {
        error = 0;
        aux1 = atual->children;  // MethodHeader
        while (aux1 != NULL) {
          if (strcmp(aux1->nodeType, "MethodHeader") == 0) {
            aux2 = aux1->children;  // TYPE
            aux3 = aux2->brother;   // ID

            if (aux3->to_anote == 0) {
              error = 1;
              break;
            }

            auxTable = create_table(aux3->value, "Method");

            auxVar = create_var("return", changeType(aux2->nodeType));  // CREATE VAR TYPE
            auxVar->flag = 0;
            auxTable->vars = auxVar;  // TO LOCAL TABLE

            aux4 = aux3->brother;   // MethodParams
            aux5 = aux4->children;  // ParamDecl ou nada
            while (aux5 != NULL) {  // WHILE EXISTS ParamDecl's
              if (strcmp(aux5->nodeType, "NULL") != 0) {
                // add to auxVar to global_table
                auxParam = create_param_type(changeType(aux5->children->nodeType), (aux5->children)->brother->value);

                aux = search_var_exists(auxVar->paramTypes, (aux5->children)->brother->value);

                if (auxVar->paramTypes == NULL) {
                  auxVar->paramTypes = auxParam;
                } else {
                  add_param(auxVar->paramTypes, auxParam);
                }

                // add to local_table
                aux6 = (aux5->children)->brother;  // ID OF ParamDecl
                auxVarLocal = create_var(aux6->value, changeType(aux5->children->nodeType));
                if (aux != NULL) {
                  auxVarLocal->flag = 2;  // DONT PRINT, ITS REPEATED
                } else {
                  auxVarLocal->flag = 1;  // ITS VAR
                }
                auxTable->params = auxVar->paramTypes;
                add_var(auxTable->vars, auxVarLocal);
              }
              aux5 = aux5->brother;
            }
          }
          if (strcmp(aux1->nodeType, "MethodBody") == 0) {
            aux2 = aux1->children;
            while (aux2 != NULL) {
              if (strcmp(aux2->nodeType, "VarDecl") == 0) {
                aux3 = aux2->children;
                aux4 = aux3->brother;

                if (search_type_var(NULL, auxTable, aux4->value) != NULL) {
                  nErrorsSemantic = 1;
                  printf("Line %d, col %d: Symbol %s already defined\n", aux4->line, aux4->colunm, aux4->value);
                } else {
                  auxVar = create_var(aux4->value, changeType(aux3->nodeType));
                  auxVar->flag = 0;
                  add_var(auxTable->vars, auxVar);  // TO LOCAL TABLE
                }
              } else {
                if (!(strcmp(aux2->nodeType, "NULL") == 0)) {
                  anote_ast(global_table, auxTable, aux2);
                }
              }

              aux2 = aux2->brother;
            }
          }

          aux1 = aux1->brother;  // MethodHeader ou MethodBody
        }

        if (error != 1) {
          if (local_table == NULL) {
            local_table = auxTable;
          } else {
            add_to_local_table(local_table, auxTable);  // ADD TO LOCAL TABLE
          }
        }
      }

      atual = atual->brother;
    }
  }
}

/* ANOTED AST */

char *search_var_exists(param_list *params, char *var_name) {
  if (params == NULL) {
    return NULL;
  }

  param_list *auxP = params;

  while (auxP != NULL) {
    if (strcmp(auxP->id, var_name) == 0) {
      return auxP->type;
    }
    auxP = auxP->next;
  }

  return NULL;
}

int search_function_exists(sym_table *table_global, param_list *aux_paramTypes, int count_params, char *name) {
  var_list *aux_vars;
  param_list *aux_params1, *aux_params2;
  int count_equals = 0;

  aux_vars = table_global->vars;
  while (aux_vars != NULL) {
    if (aux_vars->function == 1 && aux_vars->n_params == count_params && strcmp(aux_vars->name, name) == 0) {
      count_equals = 0;
      aux_params1 = aux_vars->paramTypes;
      aux_params2 = aux_paramTypes;
      while (aux_params1 != NULL) {
        if (strcmp(aux_params1->type, aux_params2->type) == 0) {
          count_equals++;
        }
        aux_params1 = aux_params1->next;
        aux_params2 = aux_params2->next;
      }
      if (count_equals == count_params) {
        return 1;
      }
    }
    aux_vars = aux_vars->next;
  }

  return 0;
}

char *search_type_var_in_table(sym_table *table, char *var_name) {
  if (table == NULL) {
    return NULL;
  }

  var_list *auxV = table->vars;

  while (auxV != NULL) {
    if (strcmp(auxV->name, var_name) == 0 && auxV->function != 1) {
      return auxV->type;
    }
    auxV = auxV->next;
  }

  return NULL;
}

char *search_type_var(sym_table *table_global, sym_table *table_local, char *var_name) {
  char *aux = search_type_var_in_table(table_local, var_name);
  if (aux != NULL) {
    return aux;
  }
  aux = search_type_var_in_table(table_global, var_name);
  if (aux != NULL) {
    return aux;
  }
  return NULL;
}

void anote_ast(sym_table *table_global, sym_table *table_local, node *atual) {
  char *aux;
  param_list *aux_params, *final_params;
  var_list *aux_vars;
  int count_params, count_equals, count_all_equals, find_function;
  node *aux1, *aux2, *aux3, *ant;

  if (atual == NULL) {
    return;
  }

  if (strcmp(atual->nodeType, "NULL") == 0) {
    return;
  } else if (strcmp(atual->nodeType, "Id") == 0) {
    aux = search_type_var(table_global, table_local, atual->value);
    if (aux != NULL) {
      atual->anoted = aux;
    } else {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Cannot find symbol %s\n", atual->line, atual->colunm, atual->value);
      atual->anoted = "undef";
    }
  } else if (strcmp(atual->nodeType, "If") == 0) {
    aux1 = atual->children;
    anote_ast(table_global, table_local, aux1);
    aux1 = aux1->brother;

    aux2 = atual->children;
    if (strcmp(aux2->anoted, "boolean")) {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Incompatible type %s in if statement\n", aux2->line, aux2->colunm, aux2->anoted);
    }

    while (aux1 != NULL) {
      anote_ast(table_global, table_local, aux1);
      aux1 = aux1->brother;
    }
  } else if (strcmp(atual->nodeType, "Block") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      anote_ast(table_global, table_local, aux1);
      aux1 = aux1->brother;
    }
  } else if (strcmp(atual->nodeType, "While") == 0) {
    // dentro do while() só pode estar um boolit, gt, eq, get, leq, lt, neq
    aux1 = atual->children;
    anote_ast(table_global, table_local, aux1);
    aux1 = aux1->brother;

    aux2 = atual->children;
    if (strcmp(aux2->anoted, "boolean")) {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Incompatible type %s in while statement\n", aux2->line, aux2->colunm, aux2->anoted);
      atual->anoted = "undef";
    } else {
      atual->anoted = "boolean";
    }

    while (aux1 != NULL) {
      anote_ast(table_global, table_local, aux1);
      aux1 = aux1->brother;
    }
  } else if (strcmp(atual->nodeType, "DoWhile") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      anote_ast(table_global, table_local, aux1);
      if (strcmp(aux1->nodeType, "NULL")) {
        ant = aux1;
      }
      aux1 = aux1->brother;
    }

    aux2 = ant;
    if (strcmp(aux2->anoted, "boolean")) {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Incompatible type %s in do statement\n", aux2->line, aux2->colunm, aux2->anoted);
      atual->anoted = "undef";
    } else {
      atual->anoted = "boolean";
    }
  } else if (strcmp(atual->nodeType, "Print") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      anote_ast(table_global, table_local, aux1);
      aux1 = aux1->brother;
    }

    aux2 = atual->children;
    if (strcmp(aux2->anoted, "undef") == 0 || strcmp(aux2->anoted, "String[]") == 0 || strcmp(aux2->anoted, "void") == 0) {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Incompatible type %s in System.out.println statement\n", aux2->line, aux2->colunm, aux2->anoted);
    }
  } else if (strcmp(atual->nodeType, "Return") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      anote_ast(table_global, table_local, aux1);
      aux1 = aux1->brother;
    }

    aux2 = atual->children;
    if (aux2 != NULL) {
      if (strcmp(table_local->vars->type, "void") == 0) {
        nErrorsSemantic = 1;
        printf("Line %d, col %d: Incompatible type %s in return statement\n", aux2->line, aux2->colunm, aux2->anoted);
      } else if (strcmp(table_local->vars->type, aux2->anoted) == 0) {
        return;
      } else if (strcmp(table_local->vars->type, "double") == 0) {
        if ((strcmp(aux2->anoted, "int") && strcmp(aux2->anoted, "double"))) {
          nErrorsSemantic = 1;
          printf("Line %d, col %d: Incompatible type %s in return statement\n", aux2->line, aux2->colunm, aux2->anoted);
        }
      } else {
        nErrorsSemantic = 1;
        printf("Line %d, col %d: Incompatible type %s in return statement\n", aux2->line, aux2->colunm, aux2->anoted);
      }
    } else {
      if (strcmp(table_local->vars->type, "void")) {
        nErrorsSemantic = 1;
        printf("Line %d, col %d: Incompatible type void in return statement\n", atual->line, atual->colunm);
      }
    }
  } else if (strcmp(atual->nodeType, "Assign") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      anote_ast(table_global, table_local, aux1);
      aux1 = aux1->brother;
    }

    // aux2 = atual->children;
    // atual->anoted = aux2->anoted;
    aux2 = atual->children;
    aux3 = aux2->brother;

    atual->anoted = aux2->anoted;

    if (strcmp(aux2->anoted, aux3->anoted) == 0 && strcmp(aux2->anoted, "undef") && strcmp(aux2->anoted, "String[]")) {
      return;
    } else if (strcmp(aux2->anoted, "double") == 0 && strcmp(aux3->anoted, "int") == 0) {
      return;
    } else {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n", atual->line, atual->colunm, aux2->anoted, aux3->anoted);
    }
  } else if (strcmp(atual->nodeType, "Call") == 0) {
    count_params = 0;
    count_equals = 0;
    count_all_equals = 0;
    find_function = 0;
    aux1 = (atual->children)->brother;
    while (aux1 != NULL) {
      if (strcmp(aux1->nodeType, "NULL")) {
        count_params++;
      }
      anote_ast(table_global, table_local, aux1);
      aux1 = aux1->brother;
    }

    aux_vars = table_global->vars;
    while (aux_vars != NULL) {
      count_equals = 0;
      count_all_equals = 0;
      if (aux_vars->function == 1 && aux_vars->n_params == count_params && strcmp(aux_vars->name, atual->children->value) == 0) {
        aux_params = aux_vars->paramTypes;
        aux1 = (atual->children)->brother;
        while (aux1 != NULL) {
          if (strcmp(aux1->nodeType, "NULL")) {
            if (strcmp(aux_params->type, aux1->anoted) == 0) {
              count_all_equals++;
              count_equals++;
            } else if (strcmp(aux1->anoted, "int") == 0 && strcmp(aux_params->type, "double") == 0) {
              count_equals++;
            } else {
              break;
            }
            aux_params = aux_params->next;
          }
          aux1 = aux1->brother;
        }
        if (count_all_equals == count_params) {
          find_function = 1;
          aux = aux_vars->type;
          final_params = aux_vars->paramTypes;
          break;
        } else if (count_equals == count_params) {
          aux = aux_vars->type;
          final_params = aux_vars->paramTypes;
          find_function++;
        }
      }
      aux_vars = aux_vars->next;
    }

    if (find_function == 1) {
      atual->children->n_params = count_params;
      if (final_params == NULL) {
        atual->children->params = NULL;
      } else {
        atual->children->params = final_params;
      }
      atual->anoted = aux;
    } else if (find_function > 1) {
      nErrorsSemantic = 1;

      printf("Line %d, col %d: Reference to method %s(", atual->children->line, atual->children->colunm, atual->children->value);
      aux1 = (atual->children)->brother;
      while (aux1 != NULL) {
        if (strcmp(aux1->nodeType, "NULL")) {
          printf("%s", aux1->anoted);
          if (aux1->brother != NULL) {
            printf(",");
          }
        }
        aux1 = aux1->brother;
      }
      printf(") is ambiguous\n");
      atual->anoted = "undef";
      atual->children->anoted = "undef";
    } else {
      atual->children->anoted = "undef";
      atual->children->params = NULL;
      atual->anoted = "undef";

      nErrorsSemantic = 1;

      printf("Line %d, col %d: Cannot find symbol %s(", atual->children->line, atual->children->colunm, atual->children->value);
      aux1 = (atual->children)->brother;
      while (aux1 != NULL) {
        if (strcmp(aux1->nodeType, "NULL")) {
          printf("%s", aux1->anoted);
          if (count_params != 1) {
            printf(",");
          }
          count_params--;
        }
        aux1 = aux1->brother;
      }
      printf(")\n");
    }
  } else if (strcmp(atual->nodeType, "ParseArgs") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      anote_ast(table_global, table_local, aux1);
      aux1 = aux1->brother;
    }

    aux2 = atual->children;
    aux3 = aux2->brother;
    if (strcmp(aux2->anoted, "String[]")) {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to types %s, %s\n", atual->line, atual->colunm, aux2->anoted, aux3->anoted);
    } else if (strcmp(aux3->anoted, "int")) {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to types %s, %s\n", atual->line, atual->colunm, aux2->anoted, aux3->anoted);
    }

    atual->anoted = "int";
  } else if (strcmp(atual->nodeType, "And") == 0 || strcmp(atual->nodeType, "Or") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      anote_ast(table_global, table_local, aux1);
      aux1 = aux1->brother;
    }

    aux2 = atual->children;
    aux3 = aux2->brother;
    if (strcmp(aux2->anoted, "boolean") && strcmp(atual->nodeType, "And") == 0) {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Operator && cannot be applied to types %s, %s\n", atual->line, atual->colunm, aux2->anoted, aux3->anoted);
    } else if (strcmp(aux2->anoted, "boolean") && strcmp(atual->nodeType, "Or") == 0) {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Operator || cannot be applied to types %s, %s\n", atual->line, atual->colunm, aux2->anoted, aux3->anoted);
    } else if (strcmp(aux3->anoted, "boolean") && strcmp(atual->nodeType, "And") == 0) {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Operator && cannot be applied to types %s, %s\n", atual->line, atual->colunm, aux2->anoted, aux3->anoted);
    } else if (strcmp(aux3->anoted, "boolean") && strcmp(atual->nodeType, "Or") == 0) {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Operator || cannot be applied to types %s, %s\n", atual->line, atual->colunm, aux2->anoted, aux3->anoted);
    }

    atual->anoted = "boolean";
  } else if (strcmp(atual->nodeType, "Eq") == 0 || strcmp(atual->nodeType, "Gt") == 0 || strcmp(atual->nodeType, "Geq") == 0 || strcmp(atual->nodeType, "Leq") == 0 || strcmp(atual->nodeType, "Lt") == 0 || strcmp(atual->nodeType, "Neq") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      anote_ast(table_global, table_local, aux1);
      aux1 = aux1->brother;
    }

    aux2 = atual->children;
    aux3 = aux2->brother;

    atual->anoted = "boolean";

    if (strcmp(atual->nodeType, "Eq") == 0) {
      aux = "==";
    } else if (strcmp(atual->nodeType, "Gt") == 0) {
      aux = ">";
    } else if (strcmp(atual->nodeType, "Geq") == 0) {
      aux = ">=";
    } else if (strcmp(atual->nodeType, "Leq") == 0) {
      aux = "<=";
    } else if (strcmp(atual->nodeType, "Lt") == 0) {
      aux = "<";
    } else if (strcmp(atual->nodeType, "Neq") == 0) {
      aux = "!=";
    }

    if (strcmp(atual->nodeType, "Eq") == 0 || strcmp(atual->nodeType, "Neq") == 0) {
      if (strcmp(aux2->anoted, "boolean") == 0 && strcmp(aux3->anoted, "boolean") == 0) {
        return;
      }
    }

    if (strcmp(aux2->anoted, "int") && strcmp(aux2->anoted, "double")) {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", atual->line, atual->colunm, aux, aux2->anoted, aux3->anoted);
    } else if (strcmp(aux3->anoted, "double") && strcmp(aux3->anoted, "int")) {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", atual->line, atual->colunm, aux, aux2->anoted, aux3->anoted);
    }
  } else if (strcmp(atual->nodeType, "Add") == 0 || strcmp(atual->nodeType, "Sub") == 0 || strcmp(atual->nodeType, "Mul") == 0 || strcmp(atual->nodeType, "Div") == 0 || strcmp(atual->nodeType, "Mod") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      anote_ast(table_global, table_local, aux1);
      aux1 = aux1->brother;
    }

    aux2 = atual->children;
    aux3 = aux2->brother;

    if (strcmp(atual->nodeType, "Add") == 0) {
      aux = "+";
    } else if (strcmp(atual->nodeType, "Sub") == 0) {
      aux = "-";
    } else if (strcmp(atual->nodeType, "Mul") == 0) {
      aux = "*";
    } else if (strcmp(atual->nodeType, "Div") == 0) {
      aux = "/";
    } else {
      aux = "%";
    }

    if (strcmp(aux2->anoted, "int") == 0) {
      if (strcmp(aux3->anoted, "int") == 0) {
        atual->anoted = "int";
      } else if (strcmp(aux3->anoted, "double") == 0) {
        atual->anoted = "double";
      } else {
        nErrorsSemantic = 1;
        printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", atual->line, atual->colunm, aux, aux2->anoted, aux3->anoted);
        atual->anoted = "undef";
      }
    } else if (strcmp(aux2->anoted, "double") == 0) {
      if (strcmp(aux3->anoted, "int") && strcmp(aux3->anoted, "double")) {
        nErrorsSemantic = 1;
        printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", atual->line, atual->colunm, aux, aux2->anoted, aux3->anoted);
        atual->anoted = "undef";
      } else {
        atual->anoted = "double";
      }
    } else {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", atual->line, atual->colunm, aux, aux2->anoted, aux3->anoted);
      atual->anoted = "undef";
    }
  } else if (strcmp(atual->nodeType, "Plus") == 0 || strcmp(atual->nodeType, "Minus") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      anote_ast(table_global, table_local, aux1);
      aux1 = aux1->brother;
    }

    aux2 = atual->children;
    atual->anoted = aux2->anoted;
    if (strcmp(aux2->anoted, "int") == 0 || strcmp(aux2->anoted, "double") == 0) {
      atual->anoted = aux2->anoted;
    } else {
      atual->anoted = "undef";
      if (strcmp(atual->nodeType, "Plus") == 0) {
        nErrorsSemantic = 1;
        printf("Line %d, col %d: Operator + cannot be applied to type %s\n", atual->line, atual->colunm, aux2->anoted);
      } else {
        nErrorsSemantic = 1;
        printf("Line %d, col %d: Operator - cannot be applied to type %s\n", atual->line, atual->colunm, aux2->anoted);
      }
    }
  } else if (strcmp(atual->nodeType, "Not") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      anote_ast(table_global, table_local, aux1);
      aux1 = aux1->brother;
    }

    aux2 = atual->children;
    if (strcmp(aux2->anoted, "boolean")) {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Operator ! cannot be applied to type %s\n", atual->line, atual->colunm, aux2->anoted);
    }

    atual->anoted = "boolean";
  } else if (strcmp(atual->nodeType, "Length") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      anote_ast(table_global, table_local, aux1);
      aux1 = aux1->brother;
    }

    aux2 = atual->children;
    if (strcmp(aux2->anoted, "String[]")) {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Operator .length cannot be applied to type %s\n", atual->line, atual->colunm, aux2->anoted);
    }
    atual->anoted = "int";
  } else if (strcmp(atual->nodeType, "BoolLit") == 0) {
    atual->anoted = "boolean";
  } else if (strcmp(atual->nodeType, "DecLit") == 0) {
    /*char *aux2 = (char*)strdup(atual->value);
    char *pr, *pw;

    pr = atual->value;
    pw = atual->value;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != '_');
    }
    *pw = '\0';*/

    long l = strtol(atual->value, NULL, 10);
    if (l >= 0 && l <= INT_MAX) {
      atual->anoted = "int";
    } else {
      nErrorsSemantic = 1;
      printf("Line %d, col %d: Number %s out of bounds\n", atual->line, atual->colunm, atual->value);
    }

    atual->anoted = "int";

    /*free(atual->value);
    atual->value = NULL;
    atual->value = aux2;*/
  } else if (strcmp(atual->nodeType, "RealLit") == 0) {
    char *value = atual->value;
    char *aux = (char *)malloc(sizeof(char) * 1024);
    int found = 0, zeros = 1, i = 0, j = 0;

    while (value[i] != '\0') {
      if ((value[i] >= '0' && value[i] <= '9') || value[i] == 'e' || value[i] == 'E' || value[i] == '.' || value[i] == '-') {
        if (value[i] == 'e' || value[i] == 'E') {
          found = 1;
        }
        if (value[i] != '.' && value[i] != '0' && !found) {
          zeros = 0;
        }
        aux[j] = value[i];
        j++;
      }
      i++;
    }
    aux[j] = '\0';

    if (!zeros) {
      double d = atof(aux);
      if (isinf(d) || d == 0 || d > DBL_MAX) {
        nErrorsSemantic = 1;
        printf("Line %d, col %d: Number %s out of bounds\n", atual->line, atual->colunm, atual->value);
      }
    }

    atual->anoted = "double";

    free(aux);
    aux = NULL;
  } else if (strcmp(atual->nodeType, "StrLit") == 0) {
    atual->anoted = "String";
  }
}

int itsExpression(char *nodeType) {
  if (strcmp(nodeType, "Assign") == 0 || strcmp(nodeType, "Or") == 0 || strcmp(nodeType, "And") == 0 || strcmp(nodeType, "Eq") == 0 || strcmp(nodeType, "Neq") == 0 || strcmp(nodeType, "Lt") == 0 || strcmp(nodeType, "Geq") == 0 || strcmp(nodeType, "Add") == 0 || strcmp(nodeType, "Sub") == 0 || strcmp(nodeType, "Mul") == 0 || strcmp(nodeType, "Div") == 0 || strcmp(nodeType, "Mod") == 0 || strcmp(nodeType, "Not") == 0 || strcmp(nodeType, "Minus") == 0 || strcmp(nodeType, "Plus") == 0 || strcmp(nodeType, "Length") == 0 || strcmp(nodeType, "Call") == 0 || strcmp(nodeType, "ParseArgs") == 0 || strcmp(nodeType, "BoolLit") == 0 || strcmp(nodeType, "DecLit") == 0 || strcmp(nodeType, "Id") == 0 || strcmp(nodeType, "RealLit") == 0 || strcmp(nodeType, "StrLit") == 0 || strcmp(nodeType, "Gt") == 0 || strcmp(nodeType, "Leq") == 0) {
    return 1;
  } else {
    return 0;
  }
}

void printAnotedAST(node *current, int n) {
  int i;
  param_list *aux;

  if (current == NULL) {
    return;
  }

  if (strcmp(current->nodeType, "NULL") == 0) {
    printAnotedAST(current->brother, n);
    return;
  } else {
    for (i = 0; i < n; i++) {
      printf("..");
    }

    if (current->value != NULL) {
      if (current->n_params >= 0 && itsExpression(current->nodeType) == 1) {
        printf("%s(%s) - (", current->nodeType, current->value);
        aux = current->params;
        while (aux != NULL) {
          printf("%s", aux->type);
          if (aux->next != NULL) {
            printf(",");
          }
          aux = aux->next;
        }
        printf(")");
      } else if (current->anoted != NULL && itsExpression(current->nodeType) == 1) {
        printf("%s(%s) - %s", current->nodeType, current->value, current->anoted);
      } else {
        printf("%s(%s)", current->nodeType, current->value);
      }
    } else {
      if (current->anoted != NULL && itsExpression(current->nodeType) == 1) {
        printf("%s - %s", current->nodeType, current->anoted);
      } else {
        printf("%s", current->nodeType);
      }
    }

    // printf(" %d %d", current->line, current->colunm);

    printf("\n");
  }

  printAnotedAST(current->children, n + 1);
  printAnotedAST(current->brother, n);
}