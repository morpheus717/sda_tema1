#include "used_mem.h"

int compare_function_memblck(void* a, void* b)
{
    mem_block *a_memblck = ((mem_block*)a);
    mem_block *b_memblck = ((mem_block*)b);
    int *a_int = &a_memblck->start_addr;
    int *b_int = &b_memblck->start_addr;
    return *a_int - *b_int;
}

void memblck_assign(mem_block* blck, int data_size, int address, int size)
{
    blck->data = malloc(data_size * size);
    blck->start_addr = address;
    blck->size = size;
}

mem_block* memblck_alloc(int data_size, int address, int size)
{
    mem_block* new_block = malloc(sizeof(mem_block));
    memblck_assign(new_block, data_size, address, size);
    return new_block;
}

void my_alloc(dll_list* all_list, int nr_bytes, segregated_free_list* sfl, int *cnt_malloc)
{
    int addr = find_addr(sfl, nr_bytes);
    if(addr == -1)
    {
        printf("Out of memory\n");
        return;
    }
    *cnt_malloc += 1;
    mem_block new_block;
    memblck_assign(&new_block, sizeof(char), addr, nr_bytes);
    dll_insert_sorted(all_list, &new_block);
}

void free_memblck_content(void** blockk)
{
    mem_block** block = (mem_block**)blockk;
    free((*block)->data);
    (*block)->data = NULL;
    free(*block);
    *block = NULL;
}

void my_free(dll_list* all_list, int freed_addr, segregated_free_list* sfl, int *cnt_free)
{
    if(!freed_addr)
    {
        *cnt_free += 1;
        return;
    }
    int found = 0;
    dll_node *current = all_list->head;
    for(int i = 0; i < all_list->size; i++)
    {
        if(((mem_block*)current->data)->start_addr == freed_addr)
            {
                dll_remove_nth(all_list, i);
                found++;
                break;
            }
        current = current->next;
    }
    if(found == 0)
    {
        printf("Invalid free\n");
        return;
    }
    *cnt_free += 1;
    int nr_bytes = ((mem_block*)current->data)->size;
    free_memblck_content(&(current->data));
    dll_free_node(&current);
    if(sfl->rebuild_type == 0)
    {
        dll_insert_sorted(sfl->vector[nr_bytes], &freed_addr);
        return;
    }
}

int count_allc_memory(dll_list* all_list)
{
    int memory = 0;
    dll_node* current = all_list->head;
    for(int i = 0; i < all_list->size; i++)
    {
        memory += ((mem_block*)current->data)->size;
        current = current->next;
    }
    return memory;
}

int count_free_memory(segregated_free_list* sfl)
{
    int memory = 0;
    for(int i = 1; i < sfl->nr_lists_big; i++)
        memory += i * sfl->vector[i]->size;
    return memory;
}

int count_free_blocks(segregated_free_list *sfl)
{
    int blocks = 0;
    for(int i = 1; i < sfl->nr_lists_big; i++)
        blocks += sfl->vector[i]->size;
    return blocks;
}

void print_allocated_blocks(dll_list* all_list)
{
    printf("Allocated blocks :");
    dll_node* current = all_list->head;
    while(current != NULL)
    {
        printf(" (0x%x - %d)", ((mem_block*)current->data)->start_addr,
               ((mem_block*)current->data)->size);
        current = current->next;
    }
    printf("\n");
}

void dump_mem(dll_list* all_list, segregated_free_list* sfl, int cnt_malloc, int cnt_free)
{
    int memory_allocated = count_allc_memory(all_list);
    int blocks_allocated = all_list->size;
    int memory_free = count_free_memory(sfl);
    int blocks_free = count_free_blocks(sfl);
    printf("+++++DUMP+++++\n");
    printf("Total memory: %d bytes\n", memory_allocated + memory_free);
    printf("Total allocated memory: %d bytes\n", memory_allocated);
    printf("Total free memory: %d bytes\n", memory_free);
    printf("Free blocks: %d\n", blocks_free);
    printf("Number of allocated blocks: %d\n", blocks_allocated);
    printf("Number of malloc calls: %d\n", cnt_malloc);
    printf("Number of fragmentations: %d\n", sfl->nr_fragmentations);
    printf("Number of free calls: %d\n", cnt_free);
    print_sfl_blocks(sfl);
    print_allocated_blocks(all_list);
    printf("-----DUMP-----\n");
}

void destroy_allocated(dll_list** all_list)
{
    dll_node *current = (*all_list)->head;
    while(current != NULL)
    {
        dll_node *eliminated = current;
        current = current->next;
        free_memblck_content(&eliminated->data);
        dll_free_node(&eliminated);
    }
    free(*all_list);
}

dll_node* search_memory(dll_list* all_list, int addr)
{
    dll_node *current = all_list->head;
    for(int i = 0; i < all_list->size; i++)
    {
        if(((mem_block*)current->data)->start_addr == addr)
            return current;
        current = current->next;
    }
    return NULL;
}

int is_enough_mem(int addr, dll_list* all_list, int nr_bytes)
{
    while(nr_bytes > 0)
    {
        dll_node *found = search_memory(all_list, addr);
        if(found == NULL)
            return 0;
        nr_bytes -= ((mem_block*)found->data)->size;
        addr += ((mem_block*)found->data)->size;
    }
    return 1;
}

void write(dll_list* all_list, segregated_free_list* sfl,
           int cnt_malloc, int cnt_free)
{
    int addr, nr_bytes;
    scanf("%x", &addr);
    char buffer[600];
    fgets(buffer, BUFFSIZ, stdin);
    char date[600];
    char *p = strtok(buffer, "\"");
    p = strtok(NULL, "\"");
    strcpy(date, p);
    p = strtok(NULL, " ");
    nr_bytes = atoi(p);
    if(nr_bytes > strlen(date))
        nr_bytes = strlen(date);
    if(is_enough_mem(addr, all_list, nr_bytes) == 0)
    {
        printf("Segmentation fault (core dumped)\n");
        dump_mem(all_list, sfl, cnt_malloc, cnt_free);
        destroy_allocated(&all_list);
        destroy_sfl(&sfl);
        exit(0);
    }
    dll_node* zone = search_memory(all_list, addr);
    int start = 0;
    while(nr_bytes > 0)
    {
        mem_block *blck = ((mem_block*)zone->data);
        int nr_cpy = (nr_bytes < blck->size) ? nr_bytes : blck->size;
        strncpy(blck->data, date + start, nr_cpy);
        nr_bytes -= blck->size;
        addr += blck->size;
        start += nr_cpy;
        zone = search_memory(all_list, addr);
    } 
}

void read(dll_list* all_list, segregated_free_list* sfl,
              int cnt_malloc, int cnt_free)
{
    int addr, nr_bytes;
    scanf("%x %d", &addr, &nr_bytes);
    if(is_enough_mem(addr, all_list, nr_bytes) == 0)
    {
        printf("Segmentation fault (core dumped)\n");
        dump_mem(all_list, sfl, cnt_malloc, cnt_free);
        destroy_allocated(&all_list);
        destroy_sfl(&sfl);
        exit(0);
    }
    dll_node *zone = search_memory(all_list, addr);
    while(nr_bytes > 0)
    {
        mem_block blck = *((mem_block*)zone->data);
        if(nr_bytes >= blck.size)
            for(int i = 0; i < blck.size; i++)
                printf("%c", ((char*)blck.data)[i]);
        else
            for(int i = 0; i < nr_bytes; i++)
                printf("%c", ((char*)blck.data)[i]);
        nr_bytes -= blck.size;
        addr += blck.size;
        zone = search_memory(all_list, addr);
    }
    printf("\n");
}