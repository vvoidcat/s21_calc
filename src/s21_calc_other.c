#include "s21_calc.h"

void print_error_message(int errcode) {
  if (errcode != OK) {
    printf("./s21_calc_main.c: error: [%d] ", errcode);
  }
  print_error(errcode);
  printf("\n");
}

void print_error(int errcode) {
  if (errcode == INPUT_ERROR_LINE || errcode == INPUT_ERROR_STACK ||
      errcode == INPUT_ERROR_EMPTY) {
    printf("incorrect input");
  } else if (errcode == CALC_ERROR) {
    printf("calculation error");
  } else if (errcode == MEM_ERROR || errcode == S_MEM_ERROR) {
    printf("memory allocation failure");
  } else if (errcode == PARSER_ERROR) {
    printf("unknown option");
  } else if (errcode == GRAPH_ERROR) {
    printf("domain and/or codomain impossible");
  } else if (errcode != OK) {
    printf("unknown error");
  }
}

void print_help() {
  printf("usage: ./s21_calc [ options ]\n\n");
  printf("options:\n");
  printf("\t-h --help\t-  displays help;\n");
  printf("\t-g --graph\t-  enables graph plotting mode.\n");
}

void print_input(char *line) {
  if (line) {
    printf("\ninput: \"%s\"\n", line);
  } else {
    printf("\ninput: (null)\n");
  }
}

void print_result(node_t *head_nums, graph_t *graph) {
  if (graph->domain_size > 0) {
    print_graph(graph);
  } else {
    print_number(head_nums);
  }
}

void print_number(node_t *head_nums) {
  if (head_nums) {
    printf("result: %s\n", s_peek_value(head_nums));
  }
}

void print_graph(graph_t *graph) {
  printf("result:\n\n");

  if (graph->domain && graph->codomain) {
    printf("%24c10\n", ' ');

    for (int i = 10; i >= -10; i--) {
      if (i == 0) {
        printf("-10 ");
      } else {
        printf("    ");
      }

      for (int j = -10; j < 10; j++) {
        int print_flag = 0;

        for (int k = 0; k < 20; k++) {
          double x = round(graph->domain[k]);
          double y = round(graph->codomain[k]);
          if (i == y && j == x) {
            printf(C_RED ". " C_RESET);
            print_flag = 1;
            break;
          }
        }

        if (!print_flag) {
          if (i == 0) {
            printf("--");
          } else if (j == 0) {
            printf("¦");
          } else {
            printf("  ");
          }
        }
      }

      if (i == 0) {
        printf(" 10");
      }
      printf("\n");
    }
    printf("%23c-10\n\n", ' ');
  } else {
    printf("    (null)\n\n");
  }
}

int convert_str_to_double(char *value, double *result) {
  int errcode = MEM_ERROR;

  if (value) {
    sscanf(value, "%lf", result);
    errcode = OK;
  }
  return errcode;
}

int convert_double_to_str(double value, char **result) {
  int errcode = MEM_ERROR;
  *result = malloc(sizeof(char) * (NMAX_INPUT));

  if (*result) {
    memset(*result, '\0', NMAX_INPUT);
    snprintf(*result, NMAX_INPUT, "%lf", value);
    errcode = OK;
  }
  return errcode;
}

double *alloc_arr_double(size_t size) {
  double *arr = NULL;

  if (size > 0) {
    arr = malloc(sizeof(double) * size);
  }
  return arr;
}

char *alloc_arr_char(size_t size) {
  char *arr = NULL;

  if (size > 0) {
    arr = malloc(sizeof(char) * (size));
  }
  return arr;
}

void print_arr_double(double *arr, int size) {
  if (arr) {
    for (int i = 0; i < size; i++) {
      printf("%lf ", arr[i]);
    }
    printf("\n");
  } else {
    printf("(null)\n");
  }
}

void free_arr_double(double *arr) {
  if (arr) {
    free(arr);
  }
}

void free_arr_char(char *arr) {
  if (arr) {
    free(arr);
  }
}
