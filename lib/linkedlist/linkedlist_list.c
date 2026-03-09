/*
** EPITECH PROJECT, 2026
** linkedlist
** File description:
** A simple linked list library.
*/

#include "./linkedlist.h"

linkedlist_t *linkedlist_ini()
{
    linkedlist_t *new_list = malloc(sizeof(linkedlist_t));

    if (!new_list)
        return NULL;
    new_list->head = NULL;
    new_list->node_count = 0;
    return new_list;
}

size_t linkedlist_destroy(linkedlist_t *linkedlist, bool delete_data)
{
    node_t *temp = NULL;

    for (node_t *node = linkedlist->head; node;) {
        temp = node;
        node = node->next;
        if (delete_data)
            free(temp->data);
        free(temp);
    }
    free(linkedlist);
    return (size_t)LINKED_SUCC;
}

node_t *linkedlist_newnode(void *data)
{
    node_t *new_node = malloc(sizeof(node_t));

    if (!new_node)
        return NULL;
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}
