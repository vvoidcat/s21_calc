#ifndef S21_STACK_H_
#define S21_STACK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S_OK 1
#define S_MEM_ERROR -301

typedef struct node {
  char *value;
  int priority;
  struct node *next;
} node_t;

int s_push(node_t **head, char *value, int priority);
node_t *s_pop(node_t **head);
char *s_peek_value(node_t *head);
int s_peek_priority(node_t *head);
void s_print_head(node_t *head);
void s_print_all(node_t *head);
void s_destroy_node(node_t **head);
void s_destroy_all(node_t **head);
size_t s_get_size(node_t *head);
int s_count_matching_elements(node_t *head, char *match);

#ifdef __cplusplus
}
#endif

#endif  // S21_STACK_H_
