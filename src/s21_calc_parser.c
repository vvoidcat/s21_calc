#include "s21_calc.h"

flags_t init_flags() {
  flags_t flags;
  flags.errcode = OK;
  flags.graph = FALSE;
  flags.help = FALSE;
  return flags;
}

int parse(int argc, char *argv[], flags_t *flags) {
  int errcode = OK;

  if (argc > 1) {
    for (int i = 1; i < argc && errcode == OK; i++) {
      errcode = check_argument(argv[i], flags);
    }
  }
  return errcode;
}

int check_argument(char *arg, flags_t *flags) {
  int errcode = OK;

  if (!arg) {
    errcode = MEM_ERROR;
  } else {
    if (strcmp(arg, "--graph") == 0 || strcmp(arg, "-g") == 0) {
      flags->graph = TRUE;
    } else if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
      flags->help = TRUE;
    } else {
      errcode = PARSER_ERROR;
    }
  }
  return errcode;
}
