#include "s21_calc.h"

void get_input(char *line) {
  char str[NMAX_INPUT];
  int max = NMAX_INPUT;
  char c = 'i';

  for (int i = 0; i < max; i++) {
    scanf("%c", &c);
    if (c == '\n') {
      str[i] = '\0';
      break;
    } else if (c == ' ') {
      i--;
      max--;
      continue;
    } else {
      str[i] = c;
    }
  }

  memset(line, '\0', NMAX_INPUT);
  strcpy(line, str);
}

int process_input(char *line, node_t **head_nums, node_t **head_ops, char *x,
                  int graph_flag) {
  int errcode = OK;
  char prev = '\0';

  while (*line != '\0' && errcode == OK) {
    int order_flag = 0;

    if (*line >= '0' && *line <= '9') {
      order_flag = 1;
      int mv = check_numbers(line);
      if (mv > 0) {
        process_numbers(line, mv, head_nums, head_ops);
        line = line + (mv - 1);
      } else {
        errcode = INPUT_ERROR_LINE;
      }
    } else if (*line == 'x') {
      errcode = process_x(head_nums, prev, x, graph_flag);
    } else if (strchr(OPS, *line)) {
      errcode = process_operators(line, prev, head_ops);
    } else if (strchr(PARS, *line)) {
      errcode = process_parentheses(line, head_ops, head_nums);
    } else {
      int mv = check_functions(line);
      if (mv > 0) {
        process_functions(line, mv + 1, head_ops);
        line = line + mv;
      } else {
        errcode = INPUT_ERROR_LINE;
      }
    }

    if (errcode == OK) {
      errcode = resolve_priorities(head_nums, head_ops, order_flag);
    }
    prev = *line;
    line++;
  }

  if (errcode == OK) {
    errcode = check_line_integrity(*head_nums, *head_ops);

    if (errcode == OK) {
      delete_eq_sign(head_ops);
      errcode = resolve_remaining(head_nums, head_ops);
      errcode = check_stack_integrity(*head_nums, *head_ops);
    }
  }

  return errcode;
}

int check_numbers(char *line) {
  int mv = 0;
  int dotflag = 1;

  while ((*line >= '0' && *line <= '9') || *line == '.') {
    if (*line == '.' && dotflag) {
      dotflag = 0;
    } else if ((*line == '.' && !dotflag) || (mv == 0 && *line == '.')) {
      mv = 0;
      break;
    }
    mv++;
    line++;
  }

  return mv;
}

void process_numbers(char *line, int len, node_t **head_nums,
                     node_t **head_ops) {
  char num[NMAX_INPUT] = {"\0"};

  for (int i = 0; i < len; i++) {
    if (*head_ops) {
      if (i == 0) {
        int flag = 0;

        if (strcmp(s_peek_value(*head_ops), "~") == 0) {
          flag = 1;
          num[i] = '-';
          i++;
          len++;
        } else if (strcmp(s_peek_value(*head_ops), "#") == 0) {
          flag = 1;
        }

        if (flag) {
          node_t *unary = s_pop(head_ops);
          s_destroy_node(&unary);
        }
      }
    }

    num[i] = *line;
    line++;
  }
  s_push(head_nums, num, 0);
}

int check_functions(char *line) {
  int mv = 0;
  static char functs[][10] = FUNCTS;

  for (int i = 0; i < NFUNCTS; i++) {
    mv = check_functions_helper(line, functs[i]);
    if (mv > 0) {
      break;
    }
  }
  return mv;
}

int check_functions_helper(char *line, char *funct) {
  int count = 0;
  int flen = strlen(funct);

  for (int i = 0; i < flen; i++) {
    if (!line[i] || line[i] != funct[i]) {
      count = 0;
      break;
    }
    count = i;
  }
  return count;
}

void process_functions(char *line, int len, node_t **head_ops) {
  char *funct = malloc(sizeof(char) * (len + 1));
  memset(funct, '\0', len + 1);

  if (funct) {
    for (int i = 0; i < len; i++) {
      funct[i] = *line;
      line++;
    }
    s_push(head_ops, funct, assign_priority('\0', funct));
    free(funct);
  }
}

int process_x(node_t **head_nums, char prev, char *x, int graph_flag) {
  int errcode = OK;

  if (x && graph_flag && (strchr(OPS, prev) || prev == '(')) {
    s_push(head_nums, x, 0);
  } else if (!x && graph_flag) {
    errcode = MEM_ERROR;
  } else if (graph_flag == FALSE) {
    errcode = INPUT_ERROR_LINE;
  } else if (!strchr(OPS, prev) || prev != '(') {
    errcode = INPUT_ERROR_LINE;
  }
  return errcode;
}

int check_operators(char *line, char prev) {
  int errcode = OK;

  if ((prev == '\0' && (*line != '+' && *line != '-' && *line != '(')) ||
      (prev != '\0' && strchr(OPS, prev) && *line != '(' && *line != ')')) {
    errcode = INPUT_ERROR_LINE;
  }
  return errcode;
}

int process_operators(char *line, char prev, node_t **head_ops) {
  int errcode = OK;

  errcode = check_operators(line, prev);

  if (errcode == OK) {
    char *op = malloc(sizeof(char) * 2);

    if (op) {
      if ((prev == '\0' || prev == '(') && *line == '-') {
        op[0] = '~';
      } else if ((prev == '\0' || prev == '(') && *line == '+') {
        op[0] = '#';
      } else {
        op[0] = *line;
      }
      op[1] = '\0';

      s_push(head_ops, op, assign_priority(op[0], NULL));
      free(op);
    } else {
      errcode = MEM_ERROR;
    }
  }
  return errcode;
}

int process_parentheses(char *line, node_t **head_ops, node_t **head_nums) {
  int errcode = OK;

  if (*line == '(') {
    s_push(head_ops, "(", assign_priority(*line, NULL));
  } else if (*line == ')') {
    errcode = resolve_parentheses(head_nums, head_ops);
  }
  return errcode;
}

int assign_priority(char op, char *funct) {
  int priority = 0;

  if (funct == NULL) {
    if (op == '=') {
      priority = 0;
    } else if (op == '+' || op == '-') {
      priority = 1;
    } else if (op == '*' || op == '/' || op == '%') {
      priority = 2;
    } else if (op == '^') {
      priority = 3;
    } else if (op == '~' || op == '#') {
      priority = 4;
    } else if (op == '(') {
      priority = 0;
    }
  } else {
    priority = 5;
  }

  return priority;
}

int check_order(node_t **head_nums, node_t **head_ops, node_t *last_op,
                int order_flag) {
  int result = 0;

  if (s_get_size(*head_nums) > 2) {
    if ((*head_ops)->priority == last_op->priority) {
      if (!(*head_ops)->next ||
          ((*head_ops)->next &&
           (((*head_ops)->next->priority == (*head_ops)->priority) ||
            ((*head_ops)->next->priority < last_op->priority && order_flag)))) {
        result = 1;
      }
    }
  }
  return result;
}

int resolve_priorities(node_t **head_nums, node_t **head_ops, int order_flag) {
  int errcode = OK;
  int flag = 0;

  if (head_ops && *head_ops && head_nums && *head_nums) {
    if (strcmp(s_peek_value(*head_ops), "(") != 0) {
      node_t *last_op = s_pop(head_ops);

      if (*head_ops && (*head_ops)->priority >= last_op->priority) {
        node_t *last_num = NULL;

        if (check_order(head_nums, head_ops, last_op, order_flag)) {
          last_num = s_pop(head_nums);
          flag++;
        }

        errcode = calculate_head_values(head_nums, head_ops);

        if (flag) {
          s_push(head_nums, last_num->value, 0);
          s_destroy_node(&last_num);
        }
      }

      s_push(head_ops, last_op->value, last_op->priority);
      s_destroy_node(&last_op);
    }
  }
  return errcode;
}

int resolve_parentheses(node_t **head_nums, node_t **head_ops) {
  int errcode = OK;
  int flag = 0;

  while (*head_ops) {
    if (s_peek_priority(*head_ops) != 0) {
      errcode = calculate_head_values(head_nums, head_ops);
    } else {
      node_t *del = s_pop(head_ops);
      s_destroy_node(&del);
      flag = 1;
      break;
    }
  }

  if (!flag) {
    errcode = INPUT_ERROR_LINE;
  }
  return errcode;
}

int resolve_remaining(node_t **head_nums, node_t **head_ops) {
  int errcode = OK;

  if (head_nums && head_ops && *head_ops && *head_nums) {
    for (size_t i = 0; i < s_get_size(*head_ops) + 1; i++) {
      errcode = calculate_head_values(head_nums, head_ops);
      if (errcode != OK) {
        break;
      }
    }
  }
  return errcode;
}

void delete_eq_sign(node_t **head_ops) {
  if (head_ops && *head_ops) {
    if (strcmp(s_peek_value(*head_ops), "=") == 0) {
      node_t *eq = s_pop(head_ops);
      s_destroy_node(&eq);
    }
  }
}

int check_line_integrity(node_t *head_nums, node_t *head_ops) {
  int errcode = OK;

  if (!head_nums && !head_ops) {
    errcode = INPUT_ERROR_EMPTY;
  } else if (s_count_matching_elements(head_ops, "(") != 0 ||
             (s_count_matching_elements(head_ops, "=") != 1 &&
              s_count_matching_elements(head_ops, "=") != 0)) {
    errcode = INPUT_ERROR_STACK;
  }
  return errcode;
}

int check_stack_integrity(node_t *head_nums, node_t *head_ops) {
  int errcode = OK;

  if (s_get_size(head_ops) != 0 && s_get_size(head_nums) != 1) {
    errcode = CALC_ERROR;
  }
  return errcode;
}
