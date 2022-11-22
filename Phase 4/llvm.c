#include "header.h"

/* ------------------ PROGRAMS (Meta 4) ---------------------- */

// Variáveis globais têm o nome de: global.var.n (sendo n o número da var)
// Declarações de funções: function.declaration.n (sendo n o nome da function)
// Variavéis da função: function.var.n (sendo n o número da var)
// StrLit's globais: global.strlit.n (sendo o n o número da str)

int count_strlit = 1;
int count_global_vars = 1;
int count_temp_vars = 1;
int count_compares_label = 1;
int count_convertions = 1;
int count_label_if = 1;
int count_label_while = 1;
int count_label_dowhile = 1;
int count_label_and_or = 1;
int founded_return = 0;

strlit_list *list_global_strlit;
strlit_list *found_vars;

strlit_list *create_strlit(char *value, char *type) {
  strlit_list *new = (strlit_list *)malloc(sizeof(strlit_list));
  new->value = (char *)strdup(value);
  new->type = (char *)strdup(type);
  new->next = NULL;
  return new;
}

void clearList(strlit_list *var) {
  if (var == NULL) {
    return;
  }

  if (var->value != NULL) {
    free(var->value);
    var->value = NULL;
  }
  if (var->type != NULL) {
    free(var->type);
    var->type = NULL;
  }

  clearList(var->next);

  free(var);
  var = NULL;
}

void add_strlit(strlit_list *strList, strlit_list *new) {
  strlit_list *aux = strList;

  if (aux == NULL) {
    strList = new;
    return;
  }

  while (aux->next != NULL) {
    aux = aux->next;
  }

  aux->next = new;
}

void print_strlit(strlit_list *strList) {
  strlit_list *aux = strList;
  printf("\n");
  while (aux != NULL) {
    printf("%s", aux->value);
    aux = aux->next;
  }
}

void create_llvm(node *root) {
  print_global_declarations(root);
  generate_llvm(root);
  print_strlit(list_global_strlit);
  clearList(list_global_strlit);
}

void print_global_declarations(node *root) {
  // Print first the global variables and functions declarations
  var_list *aux_vars = global_table->vars;

  while (aux_vars != NULL) {
    if (aux_vars->function == 0) {
      if (strcmp(aux_vars->type, "int") == 0) {
        printf("@global.var.%s = common global i32 0, align 4\n", aux_vars->name);
      }
      if (strcmp(aux_vars->type, "double") == 0) {
        printf("@global.var.%s = common global double 0.000000e+00, align 4\n", aux_vars->name);
      }
      if (strcmp(aux_vars->type, "boolean") == 0) {
        printf("@global.var.%s = common global i1 0, align 4\n", aux_vars->name);
      }

      count_global_vars++;
    }

    aux_vars = aux_vars->next;
  }

  printf("declare i32 @atoi(i8*)\n");
  printf("declare i32 @printf(i8*, ...)\n\n");
  printf("define i32 @main(i32 %%argc, i8** %%argv) {\n");
  printf("\tcall void @function.declaration.v.main.s.(i32 %%argc, i8** %%argv)\n");
  printf("\tret i32 0\n");
  printf("}\n\n");
}

int aux_length_strlit;
char *aux_change_strlit;
char *aux_change_declit;
char *aux_change_reallit;
char *global_function_type;

void change_strlit(char *value) {
  value++;                       // removes the first "
  value[strlen(value) - 1] = 0;  // removes the last "

  int i = 0, j = 0;

  aux_length_strlit = 0;

  while (value[i] != '\0') {
    if (value[i] == '%') {
      aux_length_strlit++;
      aux_change_strlit[j] = value[i];
      j++;
      aux_length_strlit++;
      aux_change_strlit[j] = value[i];
      j++;
    } else if (value[i] == '\\') {
      aux_length_strlit++;
      i++;
      if (value[i] == 'n') {
        aux_change_strlit[j] = '\\';
        j++;
        aux_change_strlit[j] = '0';
        j++;
        aux_change_strlit[j] = 'A';
        j++;
      } else if (value[i] == 'f') {
        aux_change_strlit[j] = '\\';
        j++;
        aux_change_strlit[j] = '0';
        j++;
        aux_change_strlit[j] = 'C';
        j++;
      } else if (value[i] == 'r') {
        aux_change_strlit[j] = '\\';
        j++;
        aux_change_strlit[j] = '0';
        j++;
        aux_change_strlit[j] = 'D';
        j++;
      } else if (value[i] == '"') {
        aux_change_strlit[j] = '\\';
        j++;
        aux_change_strlit[j] = '2';
        j++;
        aux_change_strlit[j] = '2';
        j++;
      } else if (value[i] == 't') {
        aux_change_strlit[j] = '\\';
        j++;
        aux_change_strlit[j] = '0';
        j++;
        aux_change_strlit[j] = '9';
        j++;
      } else if (value[i] == '\\') {
        aux_change_strlit[j] = '\\';
        j++;
        aux_change_strlit[j] = '5';
        j++;
        aux_change_strlit[j] = 'C';
        j++;
      }
    } else {
      aux_length_strlit++;
      aux_change_strlit[j] = value[i];
      j++;
    }
    i++;
  }

  aux_length_strlit++;
  aux_change_strlit[j] = '\\';
  aux_change_strlit[j + 1] = '0';
  aux_change_strlit[j + 2] = 'A';

  aux_length_strlit++;
  aux_change_strlit[j + 3] = '\\';
  aux_change_strlit[j + 4] = '0';
  aux_change_strlit[j + 5] = '0';

  aux_change_strlit[j + 6] = '\0';
}

void generate_llvm(node *atual) {
  node *aux1;

  if (atual == NULL || strcmp(atual->nodeType, "NULL") == 0) {
    return;
  }

  if (strcmp(atual->nodeType, "Program") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      generate_llvm(aux1);
      aux1 = aux1->brother;
    }
  } else if (strcmp(atual->nodeType, "MethodDecl") == 0) {
    aux_change_strlit = (char *)malloc(sizeof(char) * 1024);
    aux_change_declit = (char *)malloc(sizeof(char) * 1024);
    aux_change_reallit = (char *)malloc(sizeof(char) * 1024);

    count_temp_vars = 1;
    count_compares_label = 1;
    count_convertions = 1;
    count_label_if = 1;
    count_label_while = 1;
    count_label_dowhile = 1;
    count_label_and_or = 1;
    global_function_type = NULL;
    founded_return = 0;
    found_vars = NULL;

    aux1 = atual->children;
    while (aux1 != NULL) {
      generate_llvm(aux1);
      aux1 = aux1->brother;
    }

    clearList(found_vars);
    found_vars = NULL;
    free(aux_change_strlit);
    aux_change_strlit = NULL;
    free(aux_change_declit);
    aux_change_declit = NULL;
    free(aux_change_reallit);
    aux_change_reallit = NULL;
  } else if (strcmp(atual->nodeType, "MethodHeader") == 0) {
    create_header(atual);
  } else if (strcmp(atual->nodeType, "MethodBody") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      code_llvm(aux1);
      aux1 = aux1->brother;
    }

    if (global_function_type != NULL && strcmp(global_function_type, "Void") == 0) {
      printf("\tret void\n");
    } else if (global_function_type != NULL) {
      if (strcmp(global_function_type, "Int") == 0) {
        printf("\tret i32 0\n");
      } else if (strcmp(global_function_type, "Double") == 0) {
        printf("\tret double 0.000000e+00\n");
      } else {
        printf("\tret i1 1\n");
      }
    }

    printf("}\n");
  }
}

void create_header(node *atual) {
  char *aux_type, *aux_name;
  int i, found_string = 0;
  node *aux1, *aux2, *aux3, *aux_node_params;

  aux1 = atual->children;  // FUNCTION TYPE
  aux_type = aux1->nodeType;
  global_function_type = aux1->nodeType;

  aux2 = aux1->brother;  // FUNCTION NAME
  aux_name = aux2->value;
  aux3 = aux2->brother;  // METHOD PARAMS

  char *aux_name_f = (char *)malloc(1024 * sizeof(char));
  char *aux_params_f = (char *)malloc(1024 * sizeof(char));
  char *aux_main = (char *)strdup("i32 %argc, i8** %argv");
  int aux_count_f, aux_count_params_f = 0;

  aux_node_params = aux3->children;

  if (strcmp(aux_type, "Void") == 0) {
    sprintf(aux_name_f, "function.declaration.%s.%s.", "v", aux_name);
  } else if (strcmp(aux_type, "Int") == 0) {
    sprintf(aux_name_f, "function.declaration.%s.%s.", "i", aux_name);
  } else if (strcmp(aux_type, "Double") == 0) {
    sprintf(aux_name_f, "function.declaration.%s.%s.", "d", aux_name);
  } else {
    sprintf(aux_name_f, "function.declaration.%s.%s.", "b", aux_name);
  }

  aux_count_f = strlen(aux_name_f);

  while (aux_node_params != NULL) {
    node *brother = aux_node_params->children;

    if (strcmp(aux_node_params->nodeType, "ParamDecl") == 0) {
      if (found_vars == NULL) {
        found_vars = create_strlit(brother->brother->value, brother->nodeType);
      } else {
        add_strlit(found_vars, create_strlit(brother->brother->value, brother->nodeType));
      }
    }

    aux_node_params = aux_node_params->brother;
  }

  strlit_list *aux_param_decl = found_vars;

  while (aux_param_decl != NULL) {
    if (strcmp(aux_param_decl->type, "StringArray") == 0) {
      char *aux = (char *)strdup("s.");
      i = 0;
      while (aux[i] != '\0') {
        aux_name_f[aux_count_f] = aux[i];
        aux_count_f++;
        i++;
      }

      found_string = 1;

      free(aux);
      aux = NULL;

      break;
    }
    if (strcmp(aux_param_decl->type, "Int") == 0) {
      char *aux;
      aux = (char *)strdup("i.");
      i = 0;
      while (aux[i] != '\0') {
        aux_name_f[aux_count_f] = aux[i];
        aux_count_f++;
        i++;
      }

      free(aux);
      aux = NULL;

      aux = (char *)malloc(1024 * sizeof(char));
      sprintf(aux, "i32 %%%s_aux", aux_param_decl->value);
      i = 0;
      while (aux[i] != '\0') {
        aux_params_f[aux_count_params_f] = aux[i];
        aux_count_params_f++;
        i++;
      }

      free(aux);
      aux = NULL;
    }
    if (strcmp(aux_param_decl->type, "Bool") == 0) {
      char *aux;
      aux = (char *)strdup("b.");
      i = 0;
      while (aux[i] != '\0') {
        aux_name_f[aux_count_f] = aux[i];
        aux_count_f++;
        i++;
      }

      free(aux);
      aux = NULL;

      aux = (char *)malloc(1024 * sizeof(char));
      sprintf(aux, "i1 %%%s_aux", aux_param_decl->value);
      i = 0;
      while (aux[i] != '\0') {
        aux_params_f[aux_count_params_f] = aux[i];
        aux_count_params_f++;
        i++;
      }

      free(aux);
      aux = NULL;
    }
    if (strcmp(aux_param_decl->type, "Double") == 0) {
      char *aux;
      aux = (char *)strdup("d.");
      i = 0;
      while (aux[i] != '\0') {
        aux_name_f[aux_count_f] = aux[i];
        aux_count_f++;
        i++;
      }

      free(aux);
      aux = NULL;

      aux = (char *)malloc(1024 * sizeof(char));
      sprintf(aux, "double %%%s_aux", aux_param_decl->value);
      i = 0;
      while (aux[i] != '\0') {
        aux_params_f[aux_count_params_f] = aux[i];
        aux_count_params_f++;
        i++;
      }

      free(aux);
      aux = NULL;
    }

    if (aux_param_decl->next != NULL) {
      aux_params_f[aux_count_params_f] = ',';
      aux_count_params_f++;
      aux_params_f[aux_count_params_f] = ' ';
      aux_count_params_f++;
    }

    aux_param_decl = aux_param_decl->next;
  }

  aux_name_f[aux_count_f] = '\0';
  aux_params_f[aux_count_params_f] = '\0';

  if (strcmp(aux_type, "Void") == 0) {
    if (found_string == 0) {
      printf("define void @%s(%s) {\n", aux_name_f, aux_params_f);
    } else {
      printf("define void @%s(%s) {\n", aux_name_f, aux_main);
    }
  } else if (strcmp(aux_type, "Int") == 0) {
    if (found_string == 0) {
      printf("define i32 @%s(%s) {\n", aux_name_f, aux_params_f);
    } else {
      printf("define i32 @%s(%s) {\n", aux_name_f, aux_main);
    }
  } else if (strcmp(aux_type, "Double") == 0) {
    if (found_string == 0) {
      printf("define double @%s(%s) {\n", aux_name_f, aux_params_f);
    } else {
      printf("define double @%s(%s) {\n", aux_name_f, aux_main);
    }
  } else if (strcmp(aux_type, "Bool") == 0) {
    if (found_string == 0) {
      printf("define i1 @%s(%s) {\n", aux_name_f, aux_params_f);
    } else {
      printf("define i1 @%s(%s) {\n", aux_name_f, aux_main);
    }
  }

  if (found_string == 1) {
    printf("\t%%argc_aux = alloca i32, align 4\n");
    printf("\t%%argv_aux = alloca i8**, align 8\n");
    printf("\tstore i32 %%argc, i32* %%argc_aux, align 4\n");
    printf("\tstore i8** %%argv, i8*** %%argv_aux, align 8\n");
    clearList(found_vars);
    found_vars = NULL;
  } else {
    strlit_list *atual = found_vars;
    while (atual != NULL) {
      if (strcmp(atual->type, "Int") == 0) {
        printf("\t%%%s = alloca i32, align 4\n", atual->value);
        printf("\tstore i32 %%%s_aux, i32* %%%s, align 4\n", atual->value, atual->value);
      } else if (strcmp(atual->type, "Double") == 0) {
        printf("\t%%%s = alloca double, align 8\n", atual->value);
        printf("\tstore double %%%s_aux, double* %%%s, align 8\n", atual->value, atual->value);
      } else if (strcmp(atual->type, "Bool") == 0) {
        printf("\t%%%s = alloca i1\n", atual->value);
        printf("\tstore i1 %%%s_aux, i1* %%%s\n", atual->value, atual->value);
      }

      atual = atual->next;
    }
  }

  free(aux_name_f);
  aux_name_f = NULL;
  free(aux_params_f);
  aux_params_f = NULL;
  free(aux_main);
  aux_main = NULL;
}

char *verify_its_global(char *name) {
  char *aux = NULL;
  strlit_list *atual = found_vars;
  while (atual != NULL) {
    if (strcmp(atual->value, name) == 0) {
      return name;
    }
    atual = atual->next;
  }

  return aux;
}

void function_varDecl(char *type, char *name) {
  if (strcmp(type, "Int") == 0) {
    printf("\t%%%s = alloca i32, align 4\n", name);
  }
  if (strcmp(type, "Double") == 0) {
    printf("\t%%%s = alloca double, align 8\n", name);
  }
  if (strcmp(type, "Bool") == 0) {
    printf("\t%%%s = alloca i1\n", name);
  }

  if (found_vars == NULL) {
    found_vars = create_strlit(name, type);
  } else {
    add_strlit(found_vars, create_strlit(name, type));
  }
}

void function_realLit(node *atual) {
  /*printf("\t%%%d = alloca double, align 8\n", count_temp_vars);
  change_reallit(atual->value);
  printf("\tstore double %.16E, double* %%%d, align 8\n", atof(aux_change_reallit), count_temp_vars);
  count_temp_vars++;*/
  change_reallit(atual->value);
  printf("\t%%%d = fadd double 0.000000e+00, %.16E\n", count_temp_vars, atof(aux_change_reallit));
  count_temp_vars++;
}

void function_decLit(node *atual) {
  /*printf("\t%%%d = alloca i32, align 4\n", count_temp_vars);
  change_declit(atual->value);
  printf("\tstore i32 %d, i32* %%%d, align 4\n", atoi(aux_change_declit), count_temp_vars);
  count_temp_vars++;*/
  change_declit(atual->value);
  printf("\t%%%d = add i32 0, %s\n", count_temp_vars, aux_change_declit);
  count_temp_vars++;
}

void function_boolLit(node *atual) {
  /*printf("\t%%%d = alloca i1\n", count_temp_vars);
  printf("\tstore i1 %s, i1* %%%d\n", atual->value, count_temp_vars);
  count_temp_vars++;
  printf("\t%%%d = load i1, i1* %%%d\n", count_temp_vars, count_temp_vars-1);*/
  if (strcmp(atual->value, "true") == 0) {
    printf("\t%%%d = add i1 0, 1\n", count_temp_vars);
  } else {
    printf("\t%%%d = add i1 0, 0\n", count_temp_vars);
  }
  count_temp_vars++;
}

void function_length(node *atual) {
  printf("\t%%%d = load i32, i32* %%argc_aux, align 4\n", count_temp_vars);
  count_temp_vars++;
  printf("\t%%%d = sub i32 %%%d, 1\n", count_temp_vars, count_temp_vars - 1);
  count_temp_vars++;
}

void function_print(node *atual) {
  strlit_list *aux_str;
  char *aux = (char *)malloc(1024 * sizeof(char));

  if (strcmp(atual->children->anoted, "String") == 0) {
    change_strlit(atual->children->value);
    sprintf(aux, "@.global.strlit.%d = private unnamed_addr constant [%d x i8] c\"%s\", align 1\n", count_strlit, aux_length_strlit, aux_change_strlit);
    printf("\t%%%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([%d x i8], [%d x i8]* @.global.strlit.%d, i32 0, i32 0))\n", count_temp_vars, aux_length_strlit, aux_length_strlit, count_strlit);
    count_temp_vars++;
  } else if (strcmp(atual->children->anoted, "int") == 0) {
    sprintf(aux, "@.global.strlit.%d = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\", align 1\n", count_strlit);
    printf("\t%%%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.global.strlit.%d, i32 0, i32 0), i32 %%%d)\n", count_temp_vars, count_strlit, count_temp_vars - 1);
    count_temp_vars++;
  } else if (strcmp(atual->children->anoted, "double") == 0) {
    sprintf(aux, "@.global.strlit.%d = private unnamed_addr constant [7 x i8] c\"%%.16E\\0A\\00\", align 1\n", count_strlit);
    printf("\t%%%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.global.strlit.%d, i32 0, i32 0), double %%%d)\n", count_temp_vars, count_strlit, count_temp_vars - 1);
    count_temp_vars++;
  } else if (strcmp(atual->children->anoted, "boolean") == 0) {
    printf("\t%%compare.%d = icmp eq i1 %%%d, 1\n", count_compares_label, count_temp_vars - 1);
    printf("\tbr i1 %%compare.%d, label %%compare.if.%d, label %%compare.else.%d\n", count_compares_label, count_compares_label, count_compares_label);
    printf("\tcompare.if.%d:\n", count_compares_label);
    sprintf(aux, "@.global.strlit.%d = private unnamed_addr constant [6 x i8] c\"true\\0A\\00\", align 1\n", count_strlit);
    printf("\t\t%%%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.global.strlit.%d, i32 0, i32 0))\n", count_temp_vars, count_strlit);
    printf("\t\tbr label %%compare.end.%d\n", count_compares_label);
    count_temp_vars++;
    count_strlit++;

    // ADD TO LIST
    if (list_global_strlit == NULL) {
      list_global_strlit = create_strlit(aux, atual->children->anoted);
    } else {
      aux_str = create_strlit(aux, atual->children->anoted);
      add_strlit(list_global_strlit, aux_str);
    }

    printf("\tcompare.else.%d:\n", count_compares_label);
    printf("\t\t%%%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.global.strlit.%d, i32 0, i32 0))\n", count_temp_vars, count_strlit);
    printf("\t\tbr label %%compare.end.%d\n", count_compares_label);
    sprintf(aux, "@.global.strlit.%d = private unnamed_addr constant [7 x i8] c\"false\\0A\\00\", align 1\n", count_strlit);
    printf("\tcompare.end.%d:\n", count_compares_label);
    count_compares_label++;
    count_temp_vars++;
  }

  if (list_global_strlit == NULL) {
    list_global_strlit = create_strlit(aux, atual->children->anoted);
  } else {
    aux_str = create_strlit(aux, atual->children->anoted);
    add_strlit(list_global_strlit, aux_str);
  }

  count_strlit++;

  free(aux);
  aux = NULL;
}

void function_assign(node *atual) {
  node *aux = atual->children;
  if (strcmp(aux->anoted, "int") == 0) {
    if (verify_its_global(aux->value) != NULL) {
      printf("\tstore i32 %%%d, i32* %%%s, align 4\n", count_temp_vars - 1, aux->value);
    } else {
      printf("\tstore i32 %%%d, i32* @global.var.%s, align 4\n", count_temp_vars - 1, aux->value);
    }
  } else if (strcmp(aux->anoted, "double") == 0) {
    if (strcmp(aux->brother->anoted, "int") == 0) {
      printf("\t%%convertion.%d = sitofp i32 %%%d to double\n", count_convertions, count_temp_vars - 1);
      if (verify_its_global(aux->value) != NULL) {
        printf("\tstore double %%convertion.%d, double* %%%s, align 8\n", count_convertions, aux->value);
      } else {
        printf("\tstore double %%convertion.%d, double* @global.var.%s, align 8\n", count_convertions, aux->value);
      }
      count_convertions++;
    } else {
      if (verify_its_global(aux->value) != NULL) {
        printf("\tstore double %%%d, double* %%%s, align 8\n", count_temp_vars - 1, aux->value);
      } else {
        printf("\tstore double %%%d, double* @global.var.%s, align 8\n", count_temp_vars - 1, aux->value);
      }
    }
  } else if (strcmp(aux->anoted, "boolean") == 0) {
    if (verify_its_global(aux->value) != NULL) {
      printf("\tstore i1 %%%d, i1* %%%s\n", count_temp_vars - 1, aux->value);
    } else {
      printf("\tstore i1 %%%d, i1* @global.var.%s\n", count_temp_vars - 1, aux->value);
    }
  }
}

void function_id(node *atual) {
  if (strcmp(atual->anoted, "String[]") == 0) {
    // printf("\t%%%d = load i8**, i8*** %%argv_aux, align 8\n", count_temp_vars);
    // count_temp_vars++;
    return;
  } else if (verify_its_global(atual->value) != NULL) {
    if (strcmp(atual->anoted, "int") == 0) {
      printf("\t%%%d = load i32, i32* %%%s, align 4\n", count_temp_vars, atual->value);
    } else if (strcmp(atual->anoted, "double") == 0) {
      printf("\t%%%d = load double, double* %%%s, align 8\n", count_temp_vars, atual->value);
    } else if (strcmp(atual->anoted, "boolean") == 0) {
      printf("\t%%%d = load i1, i1* %%%s\n", count_temp_vars, atual->value);
    }
    count_temp_vars++;
  } else {
    if (strcmp(atual->anoted, "int") == 0) {
      printf("\t%%%d = load i32, i32* @global.var.%s, align 4\n", count_temp_vars, atual->value);
    } else if (strcmp(atual->anoted, "double") == 0) {
      printf("\t%%%d = load double, double* @global.var.%s, align 8\n", count_temp_vars, atual->value);
    } else if (strcmp(atual->anoted, "boolean") == 0) {
      printf("\t%%%d = load i1, i1* @global.var.%s\n", count_temp_vars, atual->value);
    }
    count_temp_vars++;
  }
}

void function_minus(node *atual) {
  if (strcmp(atual->anoted, "int") == 0) {
    printf("\t%%%d = sub nsw i32 0, %%%d\n", count_temp_vars, count_temp_vars - 1);
    count_temp_vars++;
  } else {
    printf("\t%%%d = fsub double -0.000000e+00, %%%d\n", count_temp_vars, count_temp_vars - 1);
    count_temp_vars++;
  }
}

void function_parseArgs(node *atual) {
  printf("\t%%%d = add i32 %%%d, 1\n", count_temp_vars, count_temp_vars - 1);
  count_temp_vars++;
  printf("\t%%%d = load i8**, i8*** %%argv_aux, align 8\n", count_temp_vars);
  count_temp_vars++;
  printf("\t%%%d = getelementptr inbounds i8*, i8** %%%d, i32 %%%d\n", count_temp_vars, count_temp_vars - 1, count_temp_vars - 2);
  count_temp_vars++;
  printf("\t%%%d = load i8*, i8** %%%d, align 8\n", count_temp_vars, count_temp_vars - 1);
  count_temp_vars++;
  printf("\t%%%d = call i32 @atoi(i8* %%%d)\n", count_temp_vars, count_temp_vars - 1);
  count_temp_vars++;
}

void function_add(node *atual, int var1, int var2) {
  if (strcmp(atual->anoted, "int") == 0) {
    printf("\t%%%d = add i32 %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  } else if (strcmp(atual->anoted, "double") == 0) {
    if (strcmp(atual->children->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var1);
      var1 = count_temp_vars;
      count_temp_vars++;
    }
    if (strcmp(atual->children->brother->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var2);
      var2 = count_temp_vars;
      count_temp_vars++;
    }
    printf("\t%%%d = fadd double %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  }
}

void function_sub(node *atual, int var1, int var2) {
  if (strcmp(atual->anoted, "int") == 0) {
    printf("\t%%%d = sub i32 %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  } else if (strcmp(atual->anoted, "double") == 0) {
    if (strcmp(atual->children->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var1);
      var1 = count_temp_vars;
      count_temp_vars++;
    }
    if (strcmp(atual->children->brother->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var2);
      var2 = count_temp_vars;
      count_temp_vars++;
    }
    printf("\t%%%d = fsub double %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  }
}

void function_mul(node *atual, int var1, int var2) {
  if (strcmp(atual->anoted, "int") == 0) {
    printf("\t%%%d = mul i32 %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  } else if (strcmp(atual->anoted, "double") == 0) {
    if (strcmp(atual->children->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var1);
      var1 = count_temp_vars;
      count_temp_vars++;
    }
    if (strcmp(atual->children->brother->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var2);
      var2 = count_temp_vars;
      count_temp_vars++;
    }
    printf("\t%%%d = fmul double %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  }
}

void function_div(node *atual, int var1, int var2) {
  if (strcmp(atual->anoted, "int") == 0) {
    printf("\t%%%d = sdiv i32 %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  } else if (strcmp(atual->anoted, "double") == 0) {
    if (strcmp(atual->children->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var1);
      var1 = count_temp_vars;
      count_temp_vars++;
    }
    if (strcmp(atual->children->brother->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var2);
      var2 = count_temp_vars;
      count_temp_vars++;
    }
    printf("\t%%%d = fdiv double %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  }
}

void function_mod(node *atual, int var1, int var2) {
  if (strcmp(atual->anoted, "int") == 0) {
    printf("\t%%%d = srem i32 %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  } else if (strcmp(atual->anoted, "double") == 0) {
    if (strcmp(atual->children->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var1);
      var1 = count_temp_vars;
      count_temp_vars++;
    }
    if (strcmp(atual->children->brother->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var2);
      var2 = count_temp_vars;
      count_temp_vars++;
    }
    printf("\t%%%d = frem double %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  }
}

void function_not(node *atual, int var1) {
  printf("\t%%%d = zext i1 %%%d to i32\n", count_temp_vars, var1);
  count_temp_vars++;
  printf("\t%%%d = icmp eq i32 %%%d, 0\n", count_temp_vars, count_temp_vars - 1);
  count_temp_vars++;
}

void function_eq(node *atual, int var1, int var2) {
  if (strcmp(atual->children->anoted, "int") == 0 && strcmp(atual->children->brother->anoted, "int") == 0) {
    printf("\t%%%d = icmp eq i32 %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  } else if (strcmp(atual->children->anoted, "boolean") == 0 && strcmp(atual->children->brother->anoted, "boolean") == 0) {
    printf("\t%%%d = zext i1 %%%d to i32\n", count_temp_vars, var1);
    count_temp_vars++;
    printf("\t%%%d = zext i1 %%%d to i32\n", count_temp_vars, var2);
    count_temp_vars++;
    printf("\t%%%d = icmp eq i32 %%%d, %%%d\n", count_temp_vars, count_temp_vars - 1, count_temp_vars - 2);
    count_temp_vars++;
  } else {
    if (strcmp(atual->children->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var1);
      var1 = count_temp_vars;
      count_temp_vars++;
    }
    if (strcmp(atual->children->brother->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var2);
      var2 = count_temp_vars;
      count_temp_vars++;
    }
    printf("\t%%%d = fcmp oeq double %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  }
}

void function_neq(node *atual, int var1, int var2) {
  if (strcmp(atual->children->anoted, "int") == 0 && strcmp(atual->children->brother->anoted, "int") == 0) {
    printf("\t%%%d = icmp ne i32 %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  } else if (strcmp(atual->children->anoted, "boolean") == 0 && strcmp(atual->children->brother->anoted, "boolean") == 0) {
    printf("\t%%%d = zext i1 %%%d to i32\n", count_temp_vars, var1);
    count_temp_vars++;
    printf("\t%%%d = zext i1 %%%d to i32\n", count_temp_vars, var2);
    count_temp_vars++;
    printf("\t%%%d = icmp ne i32 %%%d, %%%d\n", count_temp_vars, count_temp_vars - 1, count_temp_vars - 2);
    count_temp_vars++;
  } else {  // tem um double
    if (strcmp(atual->children->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var1);
      var1 = count_temp_vars;
      count_temp_vars++;
    }
    if (strcmp(atual->children->brother->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var2);
      var2 = count_temp_vars;
      count_temp_vars++;
    }
    printf("\t%%%d = fcmp une double %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  }
}

void function_lt(node *atual, int var1, int var2) {
  if (strcmp(atual->children->anoted, "int") == 0 && strcmp(atual->children->brother->anoted, "int") == 0) {
    printf("\t%%%d = icmp slt i32 %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  } else {
    if (strcmp(atual->children->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var1);
      var1 = count_temp_vars;
      count_temp_vars++;
    }
    if (strcmp(atual->children->brother->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var2);
      var2 = count_temp_vars;
      count_temp_vars++;
    }
    printf("\t%%%d = fcmp olt double %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  }
}

void function_gt(node *atual, int var1, int var2) {
  if (strcmp(atual->children->anoted, "int") == 0 && strcmp(atual->children->brother->anoted, "int") == 0) {
    printf("\t%%%d = icmp sgt i32 %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  } else {  // tem um double
    if (strcmp(atual->children->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var1);
      var1 = count_temp_vars;
      count_temp_vars++;
    }
    if (strcmp(atual->children->brother->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var2);
      var2 = count_temp_vars;
      count_temp_vars++;
    }
    printf("\t%%%d = fcmp ogt double %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  }
}

void function_leq(node *atual, int var1, int var2) {
  if (strcmp(atual->children->anoted, "int") == 0 && strcmp(atual->children->brother->anoted, "int") == 0) {
    printf("\t%%%d = icmp sle i32 %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  } else {
    if (strcmp(atual->children->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var1);
      var1 = count_temp_vars;
      count_temp_vars++;
    }
    if (strcmp(atual->children->brother->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var2);
      var2 = count_temp_vars;
      count_temp_vars++;
    }
    printf("\t%%%d = fcmp ole double %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  }
}

void function_geq(node *atual, int var1, int var2) {
  if (strcmp(atual->children->anoted, "int") == 0 && strcmp(atual->children->brother->anoted, "int") == 0) {
    printf("\t%%%d = icmp sge i32 %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  } else {
    if (strcmp(atual->children->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var1);
      var1 = count_temp_vars;
      count_temp_vars++;
    }
    if (strcmp(atual->children->brother->anoted, "int") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, var2);
      var2 = count_temp_vars;
      count_temp_vars++;
    }
    printf("\t%%%d = fcmp oge double %%%d, %%%d\n", count_temp_vars, var1, var2);
    count_temp_vars++;
  }
}

void function_and(node *atual) {
  node *first = atual->children->brother;
  while (strcmp(first->nodeType, "NULL") == 0) {
    first = first->brother;
  }

  int aux_count_and_or = count_label_and_or;
  count_label_and_or++;
  printf("\tbr label %%label.entry%d\n", aux_count_and_or);
  printf("label.entry%d:        ;it's an and\n", aux_count_and_or);
  printf("\t%%%d = icmp eq i1 %%%d, 1\n", count_temp_vars, count_temp_vars - 1);
  printf("\tbr i1 %%%d, label %%label.transition%d, label %%label.end%d\n", count_temp_vars, aux_count_and_or, aux_count_and_or);
  printf("label.transition%d:       ;transition of an and\n", aux_count_and_or);
  count_temp_vars++;
  code_llvm(first);
  printf("\t%%%d = icmp eq i1 %%%d, 1\n", count_temp_vars, count_temp_vars - 1);
  printf("\tbr label %%label.end%d\n", aux_count_and_or);
  printf("label.end%d:      ; it's the end of an and\n", aux_count_and_or);
  count_temp_vars++;
  if (aux_count_and_or == count_label_and_or - 1) {  // não existiu mais nenhum and
    printf("\t%%%d = phi i1 [ 0, %%label.entry%d ], [ %%%d, %%label.transition%d ]\n", count_temp_vars, aux_count_and_or, count_temp_vars - 1, aux_count_and_or);
  } else {  // existiu mais um and
    printf("\t%%%d = phi i1 [ 0, %%label.entry%d ], [ %%%d, %%label.end%d ]\n", count_temp_vars, aux_count_and_or, count_temp_vars - 1, aux_count_and_or + 1);
  }
  count_temp_vars++;
}

void function_or(node *atual) {
  node *first = atual->children->brother;
  while (strcmp(first->nodeType, "NULL") == 0) {
    first = first->brother;
  }

  int aux_count_and_or = count_label_and_or;
  count_label_and_or++;
  printf("\tbr label %%label.entry%d\n", aux_count_and_or);
  printf("label.entry%d:        ;it's an or\n", aux_count_and_or);
  printf("\t%%%d = icmp eq i1 %%%d, 0\n", count_temp_vars, count_temp_vars - 1);
  printf("\tbr i1 %%%d, label %%label.transition%d, label %%label.end%d\n", count_temp_vars, aux_count_and_or, aux_count_and_or);
  printf("label.transition%d:       ;transition of an or\n", aux_count_and_or);
  count_temp_vars++;
  code_llvm(first);
  printf("\t%%%d = icmp eq i1 %%%d, 1\n", count_temp_vars, count_temp_vars - 1);
  printf("\tbr label %%label.end%d\n", aux_count_and_or);
  printf("label.end%d:      ; it's the end of an or\n", aux_count_and_or);
  count_temp_vars++;
  if (aux_count_and_or == count_label_and_or - 1) {
    printf("\t%%%d = phi i1 [ 1, %%label.entry%d ], [ %%%d, %%label.transition%d ]\n", count_temp_vars, aux_count_and_or, count_temp_vars - 1, aux_count_and_or);
  } else {
    printf("\t%%%d = phi i1 [ 1, %%label.entry%d ], [ %%%d, %%label.end%d ]\n", count_temp_vars, aux_count_and_or, count_temp_vars - 1, aux_count_and_or + 1);
  }
  count_temp_vars++;
}

void function_if(node *atual) {
  node *first = atual->children->brother;
  while (strcmp(first->nodeType, "NULL") == 0) {
    first = first->brother;
  }
  node *second = first->brother;
  while (strcmp(second->nodeType, "NULL") == 0) {
    second = second->brother;
  }

  int aux_count_label_if = count_label_if;
  count_label_if++;

  printf("\tbr label %%label.entry.if%d\n", aux_count_label_if);
  printf("label.entry.if%d:       ;it's if \n", aux_count_label_if);
  printf("\t%%%d = icmp eq i1 %%%d, 1\n", count_temp_vars, count_temp_vars - 1);
  printf("\tbr i1 %%%d, label %%label.then%d, label %%label.else%d\n", count_temp_vars, aux_count_label_if, aux_count_label_if);
  count_temp_vars++;

  printf("label.then%d:       ;it's if \n", aux_count_label_if);
  code_llvm(first);
  printf("br label %%label.finished.if%d\n", aux_count_label_if);

  printf("label.else%d:       ;it's else \n", aux_count_label_if);
  code_llvm(second);
  printf("br label %%label.finished.if%d\n", aux_count_label_if);

  printf("label.finished.if%d:\n", aux_count_label_if);
}

void function_while(node *atual) {
  node *first = atual->children;
  while (strcmp(first->nodeType, "NULL") == 0) {
    first = first->brother;
  }
  node *second = first->brother;
  while (strcmp(second->nodeType, "NULL") == 0) {
    second = second->brother;
  }

  int aux_count_label_while = count_label_while;
  count_label_while++;

  printf("\tbr label %%label.entry.while%d\n", aux_count_label_while);
  printf("label.entry.while%d:       ;it's while \n", aux_count_label_while);
  code_llvm(first);
  printf("\t%%%d = icmp eq i1 %%%d, 1\n", count_temp_vars, count_temp_vars - 1);
  printf("\tbr i1 %%%d, label %%label.work.while%d, label %%label.finished.while%d\n", count_temp_vars, aux_count_label_while, aux_count_label_while);
  count_temp_vars++;

  printf("label.work.while%d:       ;it's while \n", aux_count_label_while);
  code_llvm(second);
  printf("\tbr label %%label.entry.while%d\n", aux_count_label_while);

  printf("label.finished.while%d:\n", aux_count_label_while);
}

void function_dowhile(node *atual) {
  node *first = atual->children;
  while (strcmp(first->nodeType, "NULL") == 0) {
    first = first->brother;
  }
  node *second = first->brother;
  while (strcmp(second->nodeType, "NULL") == 0) {
    second = second->brother;
  }

  int aux_count_label_dowhile = count_label_dowhile;
  count_label_dowhile++;

  printf("\tbr label %%label.work.dowhile%d\n", aux_count_label_dowhile);
  printf("label.entry.dowhile%d:       ;it's dowhile \n", aux_count_label_dowhile);
  code_llvm(second);
  printf("\t%%%d = icmp eq i1 %%%d, 1\n", count_temp_vars, count_temp_vars - 1);
  printf("\tbr i1 %%%d, label %%label.work.dowhile%d, label %%label.finished.dowhile%d\n", count_temp_vars, aux_count_label_dowhile, aux_count_label_dowhile);
  count_temp_vars++;

  printf("label.work.dowhile%d:       ;it's dowhile\n", aux_count_label_dowhile);
  code_llvm(first);
  printf("\tbr label %%label.entry.dowhile%d\n", aux_count_label_dowhile);

  printf("label.finished.dowhile%d:\n", aux_count_label_dowhile);
}

void function_return(node *atual) {
  if (atual->children == NULL) {
    printf("\tret void\n");
    count_temp_vars++;
    return;
  }
  code_llvm(atual->children);
  if (strcmp(atual->children->anoted, "int") == 0) {
    if (strcmp(global_function_type, "Double") == 0) {
      printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, count_temp_vars - 1);
      count_temp_vars++;
      printf("\tret double %%%d\n", count_temp_vars - 1);
    } else {
      printf("\tret i32 %%%d\n", count_temp_vars - 1);
    }
  } else if (strcmp(atual->children->anoted, "double") == 0) {
    printf("\tret double %%%d\n", count_temp_vars - 1);
  } else {
    printf("\tret i1 %%%d\n", count_temp_vars - 1);
  }
  count_temp_vars++;
}

void function_call(node *atual) {
  int i;
  param_list *aux_params;
  node *aux_node_params;
  char *aux_name_f = (char *)malloc(1024 * sizeof(char));
  char *aux_params_f = (char *)malloc(1024 * sizeof(char));
  int aux_count_f = 0, aux_count_params_f = 0, n_params;

  aux_params = atual->children->params;
  n_params = atual->children->n_params;
  aux_node_params = atual->children->brother;

  if (strcmp(atual->anoted, "boolean") == 0) {
    sprintf(aux_name_f, "function.declaration.%s.%s.", "b", atual->children->value);
  } else if (strcmp(atual->anoted, "int") == 0) {
    sprintf(aux_name_f, "function.declaration.%s.%s.", "i", atual->children->value);
  } else if (strcmp(atual->anoted, "double") == 0) {
    sprintf(aux_name_f, "function.declaration.%s.%s.", "d", atual->children->value);
  } else {
    sprintf(aux_name_f, "function.declaration.%s.%s.", "v", atual->children->value);
  }

  aux_count_f = strlen(aux_name_f);

  while (aux_params != NULL) {
    if (strcmp(aux_node_params->nodeType, "NULL") == 0) {
      aux_node_params = aux_node_params->brother;
    }

    if (strcmp(aux_params->type, "String[]") == 0) {
      code_llvm(aux_node_params);

      char *aux;
      aux = (char *)strdup("s.");
      i = 0;
      while (aux[i] != '\0') {
        aux_name_f[aux_count_f] = aux[i];
        aux_count_f++;
        i++;
      }

      free(aux);
      aux = NULL;

      sprintf(aux_params_f, "i%d %%argc, i8** %%argv", 32);
      aux_count_params_f = strlen(aux_params_f);

      break;
    } else if (strcmp(aux_params->type, "int") == 0) {
      code_llvm(aux_node_params);

      char *aux;
      aux = (char *)strdup("i.");
      i = 0;
      while (aux[i] != '\0') {
        aux_name_f[aux_count_f] = aux[i];
        aux_count_f++;
        i++;
      }

      free(aux);
      aux = NULL;

      aux = (char *)malloc(1024 * sizeof(char));
      sprintf(aux, "i32 %%%d", count_temp_vars - 1);

      i = 0;
      while (aux[i] != '\0') {
        aux_params_f[aux_count_params_f] = aux[i];
        aux_count_params_f++;
        i++;
      }

      free(aux);
      aux = NULL;
    } else if (strcmp(aux_params->type, "boolean") == 0) {
      code_llvm(aux_node_params);

      char *aux;
      aux = (char *)strdup("b.");
      i = 0;
      while (aux[i] != '\0') {
        aux_name_f[aux_count_f] = aux[i];
        aux_count_f++;
        i++;
      }

      free(aux);
      aux = NULL;

      aux = (char *)malloc(1024 * sizeof(char));
      sprintf(aux, "i1 %%%d", count_temp_vars - 1);

      i = 0;
      while (aux[i] != '\0') {
        aux_params_f[aux_count_params_f] = aux[i];
        aux_count_params_f++;
        i++;
      }

      free(aux);
      aux = NULL;
    } else if (strcmp(aux_params->type, "double") == 0) {
      if (strcmp(aux_node_params->anoted, "int") == 0) {
        code_llvm(aux_node_params);
        printf("\t%%%d = sitofp i32 %%%d to double\n", count_temp_vars, count_temp_vars - 1);
        count_temp_vars++;
      } else {
        code_llvm(aux_node_params);
      }

      char *aux;
      aux = (char *)strdup("d.");
      i = 0;
      while (aux[i] != '\0') {
        aux_name_f[aux_count_f] = aux[i];
        aux_count_f++;
        i++;
      }

      free(aux);
      aux = NULL;

      aux = (char *)malloc(1024 * sizeof(char));
      sprintf(aux, "double %%%d", count_temp_vars - 1);

      i = 0;
      while (aux[i] != '\0') {
        aux_params_f[aux_count_params_f] = aux[i];
        aux_count_params_f++;
        i++;
      }

      free(aux);
      aux = NULL;
    }

    if (aux_params->next != NULL) {
      char *aux;
      aux = (char *)strdup(", ");

      i = 0;
      while (aux[i] != '\0') {
        aux_params_f[aux_count_params_f] = aux[i];
        aux_count_params_f++;
        i++;
      }
      free(aux);
      aux = NULL;
    }

    n_params--;
    aux_params = aux_params->next;
    aux_node_params = aux_node_params->brother;
  }

  aux_name_f[aux_count_f] = '\0';
  aux_params_f[aux_count_params_f] = '\0';

  if (strcmp(atual->anoted, "int") == 0) {
    printf("\t%%%d = call i32 @%s(%s)\n", count_temp_vars, aux_name_f, aux_params_f);
    count_temp_vars++;
  } else if (strcmp(atual->anoted, "double") == 0) {
    printf("\t%%%d = call double @%s(%s)\n", count_temp_vars, aux_name_f, aux_params_f);
    count_temp_vars++;
  } else if (strcmp(atual->anoted, "boolean") == 0) {
    printf("\t%%%d = call i1 @%s(%s)\n", count_temp_vars, aux_name_f, aux_params_f);
    count_temp_vars++;
  } else {
    printf("\tcall void @%s(%s)\n", aux_name_f, aux_params_f);
  }

  free(aux_name_f);
  aux_name_f = NULL;
  free(aux_params_f);
  aux_params_f = NULL;
}

void change_declit(char *value) {
  int i = 0, j = 0;

  while (value[i] != '\0') {
    if (value[i] != '_') {
      aux_change_declit[j] = value[i];
      j++;
    }
    i++;
  }

  aux_change_declit[j] = '\0';
}

void change_reallit(char *value) {
  int i = 0, j = 0;

  while (value[i] != '\0') {
    if (value[i] != '_') {
      aux_change_reallit[j] = value[i];
      j++;
    }
    i++;
  }

  aux_change_reallit[j] = '\0';
}

void code_llvm(node *atual) {
  node *aux1;

  if (strcmp(atual->nodeType, "VarDecl") == 0) {
    function_varDecl(atual->children->nodeType, atual->children->brother->value);
  } else if (strcmp(atual->nodeType, "Call") == 0) {
    function_call(atual);
  } else if (strcmp(atual->nodeType, "Print") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      code_llvm(aux1);
      aux1 = aux1->brother;
    }

    function_print(atual);
  } else if (strcmp(atual->nodeType, "RealLit") == 0) {
    function_realLit(atual);
  } else if (strcmp(atual->nodeType, "DecLit") == 0) {
    function_decLit(atual);
  } else if (strcmp(atual->nodeType, "BoolLit") == 0) {
    function_boolLit(atual);
  } else if (strcmp(atual->nodeType, "Assign") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      code_llvm(aux1);
      aux1 = aux1->brother;
    }
    function_assign(atual);
    function_id(atual->children);
  } else if (strcmp(atual->nodeType, "Id") == 0) {
    function_id(atual);
  } else if (strcmp(atual->nodeType, "Plus") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      code_llvm(aux1);
      aux1 = aux1->brother;
    }
  } else if (strcmp(atual->nodeType, "Minus") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      code_llvm(aux1);
      aux1 = aux1->brother;
    }
    function_minus(atual);
  } else if (strcmp(atual->nodeType, "Length") == 0) {
    function_length(atual);
  } else if (strcmp(atual->nodeType, "ParseArgs") == 0) {
    code_llvm(atual->children->brother);
    function_parseArgs(atual);
  } else if (strcmp(atual->nodeType, "Add") == 0) {
    aux1 = atual->children;
    code_llvm(aux1);
    int var1 = count_temp_vars - 1;
    aux1 = atual->children->brother;
    code_llvm(aux1);
    int var2 = count_temp_vars - 1;

    function_add(atual, var1, var2);
  } else if (strcmp(atual->nodeType, "Sub") == 0) {
    aux1 = atual->children;
    code_llvm(aux1);
    int var1 = count_temp_vars - 1;
    aux1 = atual->children->brother;
    code_llvm(aux1);
    int var2 = count_temp_vars - 1;

    function_sub(atual, var1, var2);
  } else if (strcmp(atual->nodeType, "Mul") == 0) {
    aux1 = atual->children;
    code_llvm(aux1);
    int var1 = count_temp_vars - 1;
    aux1 = atual->children->brother;
    code_llvm(aux1);
    int var2 = count_temp_vars - 1;

    function_mul(atual, var1, var2);
  } else if (strcmp(atual->nodeType, "Div") == 0) {
    aux1 = atual->children;
    code_llvm(aux1);
    int var1 = count_temp_vars - 1;
    aux1 = atual->children->brother;
    code_llvm(aux1);
    int var2 = count_temp_vars - 1;

    function_div(atual, var1, var2);
  } else if (strcmp(atual->nodeType, "Mod") == 0) {
    aux1 = atual->children;
    code_llvm(aux1);
    int var1 = count_temp_vars - 1;
    aux1 = atual->children->brother;
    code_llvm(aux1);
    int var2 = count_temp_vars - 1;

    function_mod(atual, var1, var2);
  } else if (strcmp(atual->nodeType, "Not") == 0) {
    aux1 = atual->children;
    code_llvm(aux1);
    int var1 = count_temp_vars - 1;

    function_not(atual, var1);
  } else if (strcmp(atual->nodeType, "And") == 0) {
    code_llvm(atual->children);
    function_and(atual);
  } else if (strcmp(atual->nodeType, "Or") == 0) {
    code_llvm(atual->children);
    function_or(atual);
  } else if (strcmp(atual->nodeType, "Eq") == 0) {
    aux1 = atual->children;
    code_llvm(aux1);
    int var1 = count_temp_vars - 1;
    aux1 = atual->children->brother;
    code_llvm(aux1);
    int var2 = count_temp_vars - 1;

    function_eq(atual, var1, var2);
  } else if (strcmp(atual->nodeType, "Neq") == 0) {
    aux1 = atual->children;
    code_llvm(aux1);
    int var1 = count_temp_vars - 1;
    aux1 = atual->children->brother;
    code_llvm(aux1);
    int var2 = count_temp_vars - 1;

    function_neq(atual, var1, var2);
  } else if (strcmp(atual->nodeType, "Lt") == 0) {
    aux1 = atual->children;
    code_llvm(aux1);
    int var1 = count_temp_vars - 1;
    aux1 = atual->children->brother;
    code_llvm(aux1);
    int var2 = count_temp_vars - 1;

    function_lt(atual, var1, var2);
  } else if (strcmp(atual->nodeType, "Gt") == 0) {
    aux1 = atual->children;
    code_llvm(aux1);
    int var1 = count_temp_vars - 1;
    aux1 = atual->children->brother;
    code_llvm(aux1);
    int var2 = count_temp_vars - 1;

    function_gt(atual, var1, var2);
  } else if (strcmp(atual->nodeType, "Leq") == 0) {
    aux1 = atual->children;
    code_llvm(aux1);
    int var1 = count_temp_vars - 1;
    aux1 = atual->children->brother;
    code_llvm(aux1);
    int var2 = count_temp_vars - 1;

    function_leq(atual, var1, var2);
  } else if (strcmp(atual->nodeType, "Geq") == 0) {
    aux1 = atual->children;
    code_llvm(aux1);
    int var1 = count_temp_vars - 1;
    aux1 = atual->children->brother;
    code_llvm(aux1);
    int var2 = count_temp_vars - 1;

    function_geq(atual, var1, var2);
  } else if (strcmp(atual->nodeType, "Block") == 0) {
    aux1 = atual->children;
    while (aux1 != NULL) {
      code_llvm(aux1);
      aux1 = aux1->brother;
    }
  } else if (strcmp(atual->nodeType, "If") == 0) {
    code_llvm(atual->children);
    function_if(atual);
  } else if (strcmp(atual->nodeType, "While") == 0) {
    function_while(atual);
  } else if (strcmp(atual->nodeType, "DoWhile") == 0) {
    function_dowhile(atual);
  } else if (strcmp(atual->nodeType, "Return") == 0) {
    founded_return = 1;
    function_return(atual);
  }
}