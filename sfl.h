#ifndef __SFL_H__
#define __SFL_H__

#include "dll.h"

typedef struct segregated_free_list segregated_free_list;
struct segregated_free_list
{
    dll_list **vector;
    int heap_addr, bytes_per_list, rebuild_type;
    int nr_lists_small, nr_lists_big;
    int nr_fragmentations;
};

segregated_free_list* sfl_create(int heap_addr, int nr_lists,
            int bytes_per_list, int rebuild_type, int data_size,
            int (*compare_function)(void*, void*));

void print_sfl_hex(segregated_free_list* sfl);

void print_sfl_blocks(segregated_free_list* sfl);

void print_sfl_hex_all(segregated_free_list* sfl);

int find_addr(segregated_free_list *sfl, int nr_bytes);

void destroy_sfl(segregated_free_list** sfl);

#endif