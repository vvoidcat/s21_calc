#include "s21_calc.h"

graph_t init_graph() {
  graph_t graph;
  graph.domain_size = 0;
  graph.domain = NULL;
  graph.codomain = NULL;
  graph.domain_min = NMIN_GRAPH;
  graph.domain_max = NMAX_GRAPH;
  graph.codomain_min = NMIN_GRAPH;
  graph.codomain_max = NMAX_GRAPH;
  graph.step = N_STEP;
  return graph;
}

int update_graph(graph_t *graph, double domain_min, double domain_max,
                 double codomain_min, double codomain_max, double step) {
  int errcode = OK;

  if (graph) {
    graph->domain_min = domain_min;
    graph->domain_max = domain_max;
    graph->codomain_min = codomain_min;
    graph->codomain_max = codomain_max;
    graph->step = step;
    graph->domain_size = get_domain_size(graph);

    errcode = allocate_graph(graph);
  } else {
    errcode = MEM_ERROR;
  }
  return errcode;
}

size_t get_domain_size(graph_t *graph) {
  size_t result = 0;

  for (double i = graph->domain_min; i <= graph->domain_max;
       i = i + graph->step) {
    result++;
  }
  return result;
}

int allocate_graph(graph_t *graph) {
  int errcode = MEM_ERROR;

  if (graph) {
    graph->domain = alloc_arr_double(graph->domain_size);
    graph->codomain = alloc_arr_double(graph->domain_size);

    if (graph->domain && graph->codomain) {
      errcode = OK;
    } else {
      free_graph(graph);
    }
  }

  return errcode;
}

void free_graph(graph_t *graph) {
  if (graph) {
    free_arr_double(graph->domain);
    free_arr_double(graph->codomain);
  }
}

int calculate_graph(char *line, node_t **head_nums, node_t **head_ops,
                    graph_t *graph) {
  int errcode = OK;
  int count = 0;
  double y = 0;

  if (line && graph) {
    for (double x = graph->domain_min;
         x <= graph->domain_max && count < graph->domain_size && errcode == OK;
         x = x + graph->step) {
      char *x_char = NULL;
      errcode = convert_double_to_str(x, &x_char);

      if (errcode == OK) {
        errcode = process_input(line, head_nums, head_ops, x_char, TRUE);

        if (errcode == OK &&
            convert_str_to_double(s_peek_value(*head_nums), &y)) {
          graph->domain[count] = x;
          graph->codomain[count] = y;
          count++;
        }

        free_arr_char(x_char);
        s_destroy_all(head_nums);
        s_destroy_all(head_ops);
      }
    }
  } else {
    errcode = MEM_ERROR;
  }
  return errcode;
}
