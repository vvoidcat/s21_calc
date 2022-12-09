#include "s21_stack.h"

int s_push(node_t **head, char *value, int priority) {
  int errcode = S_MEM_ERROR;

  if (value) {
    node_t *newnode = malloc(sizeof(node_t));
    char *newvalue = malloc(sizeof(char) * (strlen(value) + 1));

    if (newnode && newvalue) {
      memset(newvalue, '\0', strlen(value) + 1);
      strcpy(newvalue, value);

      newnode->next = *head;
      newnode->value = newvalue;
      newnode->priority = priority;
      *head = newnode;
      errcode = S_OK;
    }
  }
  return errcode;
}

node_t *s_pop(node_t **head) {
  node_t *outnode = NULL;

  if (head && *head && (*head)->value) {
    outnode = *head;
    *head = (*head)->next;
  }
  return outnode;
}

char *s_peek_value(node_t *head) {
  char *result = NULL;

  if (head) {
    if (head->value) {
      result = head->value;
    }
  }
  return result;
}

int s_peek_priority(node_t *head) {
  int result = S_MEM_ERROR;

  if (head) {
    result = head->priority;
  }
  return result;
}

void s_print_head(node_t *head) {
  int errflag = 1;
  if (head != NULL) {
    if (head->value) {
      printf("%s[%d]", head->value, head->priority);
      errflag = 0;
    }
  }
  if (errflag) {
    printf("(null)");
  }
}

void s_print_all(node_t *head) {
  if (head) {
    while (head) {
      s_print_head(head);
      printf(" ");
      head = head->next;
    }
    printf("\n");
  } else {
    printf("(null)\n");
  }
}

void s_destroy_node(node_t **head) {
  if (head && *head) {
    if ((*head)->value) {
      free((*head)->value);
      (*head)->value = NULL;
    }
    free((*head));
    (*head) = NULL;
  }
}

void s_destroy_all(node_t **head) {
  size_t size = s_get_size((*head));
  for (size_t i = 0; i < size; i++) {
    node_t *pop = s_pop(head);
    s_destroy_node(&pop);
  }
}

size_t s_get_size(node_t *head) {
  size_t result = 0;

  while (head) {
    result++;
    head = head->next;
  }
  return result;
}

int s_count_matching_elements(node_t *head, char *match) {
  int result = 0;

  while (head && head->value && match) {
    if (strlen(head->value) == strlen(match)) {
      int count = 0;
      for (size_t i = 0; i < strlen(match); i++) {
        if (head->value[i] != match[i]) {
          count = 0;
          break;
        }
        count++;
      }
      if (count > 0) {
        result++;
      }
    }
    head = head->next;
  }
  return result;
}
