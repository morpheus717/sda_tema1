#ifndef __DLL_H__
#define __DLL_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)

typedef struct dll_node dll_node;
struct dll_node
{
    void *data;
    dll_node *prev, *next;
};

typedef struct dll_list dll_list;
struct dll_list
{
    dll_node *head;
    int data_size, size;
    int (*compare_function)(void*, void*);
};
int compare_function_ints(void* a, void* b);

dll_node *node_create(void *new_data, int data_size);

dll_list* dll_create(int data_size, int (*compare_function)(void*, void*));

dll_node* dll_get_nth_node(dll_list *list, int n);

void dll_add_nth_node(dll_list *list, int n, void* new_data);

dll_node *dll_remove_nth(dll_list *list, int n);

void dll_free_node(dll_node **node);

void dll_free_list(dll_list **list);

void dll_print_ints(dll_list *list);

void dll_insert_sorted(dll_list *list, void* data);
#endif