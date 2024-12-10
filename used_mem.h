#ifndef __USEDMEM_H__
#define __USEDMEM_H__

#define BUFFSIZ 600

#include "dll.h"
#include "sfl.h"
#include <string.h>

typedef struct mem_block mem_block;
struct mem_block
{
    void *data;
    int start_addr;
	int size;
};

int compare_function_memblck(void* a, void* b);

mem_block* memblck_alloc(int data_size, int address, int size);

void my_alloc(dll_list* all_list, int nr_bytes, segregated_free_list* sfl, int *cnt_malloc);

void free_memblck_content(void** block);

void my_free(dll_list* all_list, int freed_addr, segregated_free_list* sfl, int *cnt_free);

int count_allc_memory(dll_list* all_list);

int count_free_memory(segregated_free_list* sfl);

int count_free_blocks(segregated_free_list *sfl);

void print_allocated_blocks(dll_list* all_list);

void dump_mem(dll_list* all_list, segregated_free_list* sfl, int cnt_malloc, int cnt_free);

void destroy_allocated(dll_list** all_list);

dll_node* search_memory(dll_list* all_list, int addr);

int is_enough_mem(int addr, dll_list* all_list, int nr_bytes);

void write(dll_list* all_list, segregated_free_list* sfl,
           int cnt_malloc, int cnt_free);

void read(dll_list* all_list, segregated_free_list* sfl,
              int cnt_malloc, int cnt_free);

#endif