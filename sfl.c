#include "sfl.h"

int power_of_2(int exponent)
{
    return (1 << exponent);
}

void allocate_powers(segregated_free_list* sfl)
{
    int addr = sfl->heap_addr;
    for(int i = 3; i <= sfl->nr_lists_small; i++)
    {
        int pow = power_of_2(i);
        for(int j = 0; j < sfl->bytes_per_list / pow; j++)
            {
                dll_add_nth_node(sfl->vector[pow], j, &addr);
                addr += pow;
            }
    }//dimensiunea se retine in index
}

segregated_free_list* sfl_create(int heap_addr, int nr_lists,
            int bytes_per_list, int rebuild_type, int data_size,
            int (*compare_function)(void*, void*))
{
    segregated_free_list *sfl = malloc(sizeof(segregated_free_list));
    sfl->nr_fragmentations = 0;
    sfl->bytes_per_list = bytes_per_list;
    sfl->heap_addr = heap_addr;
    sfl->nr_lists_small = nr_lists + 2;
    sfl->nr_lists_big = power_of_2(sfl->nr_lists_small) + 1;
    sfl->rebuild_type = rebuild_type;
    sfl->vector = malloc(sizeof(dll_list*) * (sfl->nr_lists_big));
    for(int i = 1; i < sfl->nr_lists_big; i++)
        sfl->vector[i] = dll_create(data_size, (*compare_function));
    allocate_powers(sfl);
    return sfl;
}

void print_sfl_hex(segregated_free_list* sfl)
{
    for(int i = 3; i < sfl->nr_lists_small; i++)
    {
        int pow = power_of_2(i);
        dll_node *current = sfl->vector[pow]->head;
        for(int j = 0; j < sfl->vector[pow]->size; j++)
        {
            printf("0x%x ", *((int*)current->data));
            current = current->next;
        }
        printf("\n");
    }
}

void print_sfl_hex_all(segregated_free_list* sfl)
{
    for(int i = 1; i < sfl->nr_lists_big; i++)
    {
        if(sfl->vector[i]->size == 0)
            continue;
        printf("%d: ", i);
        dll_node *current = sfl->vector[i]->head;
        for(int j = 0; j < sfl->vector[i]->size; j++)
        {
            printf("0x%x ", *((int*)current->data));
            current = current->next;
        }
        printf("\n");
    }
}

void print_sfl_blocks(segregated_free_list* sfl)
{
    for(int i = 1; i < sfl->nr_lists_big; i++)
    {
        if(sfl->vector[i]->size > 0)
        {
            printf("Blocks with %d bytes - %d free block(s) :", i,
                   sfl->vector[i]->size);
            dll_node *current = sfl->vector[i]->head;
            for(int j = 0; j < sfl->vector[i]->size; j++)
            {
                printf(" 0x%x", *(int*)current->data);
                current = current->next;
            }
            printf("\n");
        }
    }
}

int find_addr(segregated_free_list *sfl, int nr_bytes)
{
    int address = -1;
    if(nr_bytes >= sfl->nr_lists_big)
        return address;
    if(sfl->vector[nr_bytes]->size > 0)
    {
        dll_node *eliminated = dll_remove_nth(sfl->vector[nr_bytes], 0);
        address = *((int*)eliminated->data);
        dll_free_node(&eliminated);
        return address;
    }
    for(int i = nr_bytes + 1; i < sfl->nr_lists_big; i++)
    {
        if(sfl->vector[i]->size > 0)
        {
            dll_node *eliminated = dll_remove_nth(sfl->vector[i], 0);
            address = *((int*)eliminated->data);
            int remaining = address + nr_bytes;
            dll_insert_sorted(sfl->vector[i - nr_bytes], &remaining);
            dll_free_node(&eliminated);
            sfl->nr_fragmentations++;
            return address;
        }
    }
    return address;
}

void destroy_sfl(segregated_free_list** sfl)
{
    for(int i = 1; i < (*sfl)->nr_lists_big; i++)
        dll_free_list(&((*sfl)->vector[i]));
    free((*sfl)->vector);
    free(*sfl);
}