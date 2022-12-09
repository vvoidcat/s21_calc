#ifndef S21_CALC_H_
#define S21_CALC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_stack.h"

#define TRUE 1
#define FALSE 0

#define OK 1
#define INPUT_ERROR_LINE -200
#define INPUT_ERROR_STACK -201
#define INPUT_ERROR_EMPTY -202
#define MEM_ERROR -300
#define CALC_ERROR -400
#define PARSER_ERROR -500
#define GRAPH_ERROR -600

#define C_RED "\x1B[31m"
#define C_RESET "\x1B[0m"

#define NMAX_INPUT 256
#define NFUNCTS 9

#define OPS "+-*/^%%="
#define PARS "()"
#define FUNCTS \
  {"cos", "sin", "tan", "acos", "asin", "atan", "sqrt", "ln", "log"};

#define NMAX_GRAPH 10
#define NMIN_GRAPH -10
#define N_STEP 1

typedef struct flags {
  int errcode;
  int graph;
  int help;
} flags_t;

typedef struct graph {
  double *domain;
  double *codomain;
  double domain_size;
  double domain_min;
  double domain_max;
  double codomain_min;
  double codomain_max;
  double step;
} graph_t;

// parser
flags_t init_flags();
int parse(int argc, char *argv[], flags_t *flags);
int check_argument(char *arg, flags_t *flags);

// core
void get_input(char *line);
int process_input(char *line, node_t **head_nums, node_t **head_ops, char *x,
                  int graph_flag);
int check_numbers(char *line);
void process_numbers(char *line, int len, node_t **head_nums,
                     node_t **head_ops);
int check_functions(char *line);
int check_functions_helper(char *line, char *funct);
void process_functions(char *line, int len, node_t **head_ops);
int process_x(node_t **head_nums, char prev, char *x, int graph_flag);
int check_operators(char *line, char prev);
int process_operators(char *line, char prev, node_t **head_ops);
int process_parentheses(char *line, node_t **head_ops, node_t **head_nums);
int assign_priority(char op, char *funct);
int check_order(node_t **head_nums, node_t **head_ops, node_t *last_op,
                int order_flag);
int resolve_priorities(node_t **head_nums, node_t **head_ops, int order_flag);
int resolve_parentheses(node_t **head_nums, node_t **head_ops);
int resolve_remaining(node_t **head_nums, node_t **head_ops);
void delete_eq_sign(node_t **head_ops);
int check_line_integrity(node_t *head_nums, node_t *head_ops);
int check_stack_integrity(node_t *head_nums, node_t *head_ops);

// math
int calculate_head_values(node_t **head_nums, node_t **head_ops);
int perform_operation(double value_1, double value_2, double *result,
                      node_t *s_op, int *un_flag);
int perform_binary(double value_1, double value_2, char *op_value,
                   double *result);
int perform_unary(double value_1, char *op_value, double *result, int *un_flag);
int calc_add(double value_1, double value_2, double *result);
int calc_sub(double value_1, double value_2, double *result);
int calc_mult(double value_1, double value_2, double *result);
int calc_div(double value_1, double value_2, double *result);
int calc_pow(double value_1, double value_2, double *result);
int calc_mod(double value_1, double value_2, double *result);
int calc_un_minus(double value_1, double *result);
int calc_un_plus(double value_1, double *result);
int calc_cos(double value_1, double *result);
int calc_sin(double value_1, double *result);
int calc_tan(double value_1, double *result);
int calc_acos(double value_1, double *result);
int calc_asin(double value_1, double *result);
int calc_atan(double value_1, double *result);
int calc_sqrt(double value_1, double *result);
int calc_ln(double value_1, double *result);
int calc_log(double value_1, double *result);

// graphs
graph_t init_graph();
int update_graph(graph_t *graph, double domain_min, double domain_max,
                 double codomain_min, double codomain_max, double step);
size_t get_domain_size(graph_t *graph);
int allocate_graph(graph_t *graph);
void free_graph(graph_t *graph);
int calculate_graph(char *line, node_t **head_nums, node_t **head_ops,
                    graph_t *graph);

// other
void print_error_message(int errcode);
void print_error(int errcode);
void print_help();
void print_input(char *line);
void print_result(node_t *head_nums, graph_t *graph);
void print_number(node_t *head_nums);
void print_graph(graph_t *graph);
int convert_str_to_double(char *value, double *result);
int convert_double_to_str(double value, char **result);
double *alloc_arr_double(size_t size);
char *alloc_arr_char(size_t size);
void print_arr_double(double *arr, int size);
void free_arr_double(double *arr);
void free_arr_char(char *arr);

#ifdef __cplusplus
}
#endif

#endif  // S21_CALC_H_
