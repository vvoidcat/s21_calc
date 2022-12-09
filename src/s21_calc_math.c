#include "s21_calc.h"

int calculate_head_values(node_t **head_nums, node_t **head_ops) {
  int errcode = MEM_ERROR;
  int un_flag = 0;
  double result = 0;
  double value_1 = 0;
  double value_2 = 0;

  node_t *s_value_1 = s_pop(head_nums);
  node_t *s_value_2 = s_pop(head_nums);
  node_t *s_op = s_pop(head_ops);

  if (s_value_1) {
    convert_str_to_double(s_value_1->value, &value_1);
  }
  if (s_value_2) {
    convert_str_to_double(s_value_2->value, &value_2);
  }

  errcode = perform_operation(value_1, value_2, &result, s_op, &un_flag);

  if (errcode == OK) {
    char *result_str = NULL;
    if (convert_double_to_str(result, &result_str) && result_str) {
      if (un_flag) {
        if (s_value_2) {
          s_push(head_nums, s_value_2->value, s_value_2->priority);
        }
      }
      s_push(head_nums, result_str, 0);
      free_arr_char(result_str);
    } else {
      errcode = MEM_ERROR;
    }
  }

  s_destroy_node(&s_value_1);
  s_destroy_node(&s_value_2);
  s_destroy_node(&s_op);

  return errcode;
}

int perform_operation(double value_1, double value_2, double *result,
                      node_t *s_op, int *un_flag) {
  int errcode = CALC_ERROR;

  if (s_op && result && un_flag) {
    char *op_value = s_op->value;

    if (op_value && (perform_unary(value_1, op_value, result, un_flag) == OK ||
                     perform_binary(value_1, value_2, op_value, result))) {
      errcode = OK;
    }
  } else {
    errcode = MEM_ERROR;
  }
  return errcode;
}

int perform_binary(double value_1, double value_2, char *op_value,
                   double *result) {
  int errcode = CALC_ERROR;

  if (strcmp(op_value, "+") == 0) {
    errcode = calc_add(value_1, value_2, result);
  } else if (strcmp(op_value, "-") == 0) {
    errcode = calc_sub(value_1, value_2, result);
  } else if (strcmp(op_value, "*") == 0) {
    errcode = calc_mult(value_1, value_2, result);
  } else if (strcmp(op_value, "/") == 0) {
    errcode = calc_div(value_1, value_2, result);
  } else if (strcmp(op_value, "^") == 0) {
    errcode = calc_pow(value_1, value_2, result);
  } else if (strcmp(op_value, "%") == 0) {
    errcode = calc_mod(value_1, value_2, result);
  }
  return errcode;
}

int perform_unary(double value_1, char *op_value, double *result,
                  int *un_flag) {
  int errcode = CALC_ERROR;

  if (strcmp(op_value, "~") == 0) {
    errcode = calc_un_minus(value_1, result);
  } else if (strcmp(op_value, "#") == 0) {
    errcode = calc_un_plus(value_1, result);
  } else if (strcmp(op_value, "cos") == 0) {
    errcode = calc_cos(value_1, result);
  } else if (strcmp(op_value, "sin") == 0) {
    errcode = calc_sin(value_1, result);
  } else if (strcmp(op_value, "tan") == 0) {
    errcode = calc_tan(value_1, result);
  } else if (strcmp(op_value, "acos") == 0) {
    errcode = calc_acos(value_1, result);
  } else if (strcmp(op_value, "asin") == 0) {
    errcode = calc_asin(value_1, result);
  } else if (strcmp(op_value, "atan") == 0) {
    errcode = calc_atan(value_1, result);
  } else if (strcmp(op_value, "sqrt") == 0) {
    errcode = calc_sqrt(value_1, result);
  } else if (strcmp(op_value, "ln") == 0) {
    errcode = calc_ln(value_1, result);
  } else if (strcmp(op_value, "log") == 0) {
    errcode = calc_log(value_1, result);
  }

  if (errcode == OK) {
    *un_flag = 1;
  }
  return errcode;
}

int calc_add(double value_1, double value_2, double *result) {
  *result = value_1 + value_2;
  return OK;
}

int calc_sub(double value_1, double value_2, double *result) {
  *result = value_2 - value_1;
  return OK;
}

int calc_mult(double value_1, double value_2, double *result) {
  *result = value_1 * value_2;

  return OK;
}

int calc_div(double value_1, double value_2, double *result) {
  *result = value_2 / value_1;
  return OK;
}

int calc_pow(double value_1, double value_2, double *result) {
  *result = pow(value_2, value_1);
  return OK;
}

int calc_mod(double value_1, double value_2, double *result) {
  *result = fmod(value_2, value_1);
  return OK;
}

int calc_un_minus(double value_1, double *result) {
  *result = -1 * value_1;
  return OK;
}

int calc_un_plus(double value_1, double *result) {
  *result = value_1;
  return OK;
}

int calc_cos(double value_1, double *result) {
  *result = cos(value_1);
  return OK;
}

int calc_sin(double value_1, double *result) {
  *result = sin(value_1);
  return OK;
}

int calc_tan(double value_1, double *result) {
  *result = tan(value_1);
  return OK;
}

int calc_acos(double value_1, double *result) {
  *result = acos(value_1);
  return OK;
}

int calc_asin(double value_1, double *result) {
  *result = asin(value_1);
  return OK;
}

int calc_atan(double value_1, double *result) {
  *result = atan(value_1);
  return OK;
}

int calc_sqrt(double value_1, double *result) {
  *result = sqrt(value_1);
  return OK;
}

int calc_ln(double value_1, double *result) {
  *result = log(value_1);
  return OK;
}

int calc_log(double value_1, double *result) {
  *result = log10(value_1);
  return OK;
}
