#include "dll.h"

int compare_function_ints(void* a, void* b)
{
    int a_int = *((int*)a);
    int b_int = *((int*)b);
    if(a_int == b_int)
        return 0;
    if(a_int > b_int)
        return 1;
    return -1;
}

dll_node *node_create(void *new_data, int data_size)
{
    dll_node *new_node = malloc(sizeof(dll_node));
    new_node->data = malloc(data_size);
    memcpy(new_node->data, new_data, data_size);
    return new_node;
}

dll_list* dll_create(int data_size, int (*compare_function)(void*, void*))
{
    dll_list *list = malloc(sizeof(dll_list));
    list->size = 0;
    list->head = NULL;
    list->data_size = data_size;
    list->compare_function = (*compare_function);
    return list;
}

dll_node* dll_get_nth_node(dll_list *list, int n)
{
    if(list == NULL || list->head == NULL)
        return NULL;
    if(n > list->size)
        n = list->size;
    dll_node *current = list->head;
    for(int i = 0; i < n && current->next != NULL; i++)
        current = current->next;
    return current;
}

void dll_add_nth_node(dll_list *list, int n, void* new_data)
{
    if(list == NULL)
        return;
    dll_node *new_node = node_create(new_data, list->data_size);
    if(list->head == NULL)
    {
        list->head = new_node;
        new_node->next = NULL;
        new_node->prev = NULL;
        list->size++;
        return;
    }
    dll_node *current = dll_get_nth_node(list, n);
    if(n == list->size)
    {
        new_node->prev = current;
        current->next = new_node;
        new_node->next = NULL;
        list->size++;
        return;
    }
    new_node->prev = current->prev;
    new_node->next = current;
    if(current->prev != NULL)
        current->prev->next = new_node;
    else
        list->head = new_node;
    current->prev = new_node;
    list->size++;
}

dll_node *dll_remove_nth(dll_list *list, int n)
{
    if(list == NULL || list->head == NULL)
        return NULL;
    if(n > list->size)
        n = list->size;
    if(list->size == 1)
    {
        dll_node *eliminated = list->head;
        list->head = NULL;
        list->size--;
        return eliminated;
    }
    dll_node *eliminated = dll_get_nth_node(list, n);
    list->size--;
    if(eliminated->prev == NULL)
    {
        list->head = eliminated->next;
        list->head->prev = NULL;
        return eliminated;
    }
    eliminated->prev->next = eliminated->next;
    if(eliminated->next == NULL)
        return eliminated;
    eliminated->next->prev = eliminated->prev;
    return eliminated;
}

void dll_free_node(dll_node **node)
{
    free((*node)->data);
    free(*node);
}

void dll_free_list(dll_list **list)
{
    dll_node *current = (*list)->head;
    for(int i = 0; i < (*list)->size && current != NULL; i++)
    {
        dll_node *previous = current;
        current = current->next;
        dll_free_node(&previous);
    }
    free((*list));
}

void dll_print_ints(dll_list *list)
{
    if(list == NULL || list->head == NULL)
        return;
    dll_node *current = list->head;
    while(current->next != NULL)
    {
        printf("%d ", *((int*)current->data));
        current = current->next;
    }
    printf("%d\n", *((int*)current->data));
}

void dll_insert_sorted(dll_list *list, void* data)
{
    if(list == NULL)
        return;
    if(list->head == NULL)
    {
        dll_add_nth_node(list, 0, data);
        return;
    }
    dll_node *current = list->head;
    for(int i = 0; i < list->size; i++)
    {
        if(list->compare_function(data, current->data) < 0)
            {
                dll_add_nth_node(list, i, data);
                return;
            }
        current = current->next;
    }
    dll_add_nth_node(list, list->size, data);
}