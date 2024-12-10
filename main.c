#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dll.h"
#include "sfl.h"
#include "used_mem.h"

#define COM_SIZ 100

int main()
{
    char command[COM_SIZ];
    int cnt_malloc = 0, cnt_free = 0;
    segregated_free_list *sfl;
    dll_list *all_list; //all = allocated
    all_list = dll_create(sizeof(mem_block), compare_function_memblck);
    while (1)
    {
        scanf("%s", command);
        if(strncmp(command, "INIT_HEAP", 9) == 0)
        {
            int heap_addr, nr_lists, bytes_per_list, rebuild_type;
            scanf("%x %d %d %d", &heap_addr, &nr_lists, &bytes_per_list, &rebuild_type);
            sfl = sfl_create(heap_addr, nr_lists, bytes_per_list,
            rebuild_type, sizeof(int), compare_function_ints);
        }
        else if(strncmp(command, "MALLOC", 6) == 0)
        {
            int nr_bytes;
            scanf("%d", &nr_bytes);
            my_alloc(all_list, nr_bytes, sfl, &cnt_malloc);
        }
        else if(strncmp(command, "FREE", 4) == 0)
        {
            int freed_addr;
            scanf("%x", &freed_addr);
            my_free(all_list, freed_addr, sfl, &cnt_free);
        }
        else if(strncmp(command, "DUMP_MEMORY", 11) == 0)
            dump_mem(all_list, sfl, cnt_malloc, cnt_free);
        else if(strncmp(command, "WRITE", 5) == 0)
            write(all_list, sfl, cnt_malloc, cnt_free);
        else if(strncmp(command, "READ", 4) == 0)
            read(all_list, sfl, cnt_malloc, cnt_free);
        else if(strncmp(command, "DESTROY_HEAP", 12) == 0)
        {
            destroy_allocated(&all_list);
            destroy_sfl(&sfl);
            return 0;
        }
    }
    
}