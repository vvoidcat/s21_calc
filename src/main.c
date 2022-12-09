#include "s21_calc.h"

int main(int argc, char *argv[]) {
  char *line = NULL;
  node_t *head_nums = NULL;
  node_t *head_ops = NULL;
  graph_t graph = init_graph();
  flags_t flags = init_flags();

  flags.errcode = parse(argc, argv, &flags);

  if (flags.errcode == OK && flags.help == FALSE) {
    line = alloc_arr_char(NMAX_INPUT);

    if (line) {
      printf("insert your equation:\n");
      get_input(line);

      if (strchr(line, 'x')) {
        flags.graph = TRUE;
      }

      if (flags.graph == TRUE) {
        flags.errcode = update_graph(&graph, NMIN_GRAPH, NMAX_GRAPH, NMIN_GRAPH,
                                     NMAX_GRAPH, N_STEP);
        flags.errcode = calculate_graph(line, &head_nums, &head_ops, &graph);
      } else {
        flags.errcode = process_input(line, &head_nums, &head_ops, NULL, FALSE);
      }

      if (flags.errcode == OK) {
        print_input(line);
        print_result(head_nums, &graph);
      }
    } else {
      flags.errcode = MEM_ERROR;
    }
  }

  if (flags.help == FALSE) {
    print_error_message(flags.errcode);
  } else {
    print_help();
  }

  free_arr_char(line);
  s_destroy_all(&head_nums);
  s_destroy_all(&head_ops);
  free_graph(&graph);

  return 0;
}
