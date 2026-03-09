/*
** EPITECH PROJECT, 2026
** linkedlist
** File description:
** A simple linked list library.
*/

#ifndef LINKEDLIST
    #define LINKEDLIST

    #include <stddef.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #define LINKED_SUCC 0
    #define LINKED_FAIL -1

// Typedefs

typedef struct node {
    void *data;
    struct node *next;
    struct node *prev;
} node_t;

typedef struct linkedlist {
    node_t *head;
    size_t node_count;
} linkedlist_t;

// Functions

/*
Create a new linkedlist object.
Returns NULL on fail.
*/
linkedlist_t *linkedlist_ini(void);

/*
Destroys a linkedlist object.
Set "delete_data" to true to free the data in nodes.
*/
size_t linkedlist_destroy(linkedlist_t *linkedlist, bool delete_data);

/*
Creates a new node object and initializes it.
Returns NULL on fail.
*/
node_t *linkedlist_newnode(void *data);

/*
Inserts a node object as the new head of the list.
*/
size_t linkedlist_inserthead(linkedlist_t *linkedlist, node_t *node);

/*
Inserts a node object in the list after the first successful comparison.
Returns LINKED_FAIL if no object was found.
*/
size_t linkedlist_insert(linkedlist_t *linkedlist, node_t *node,
    bool (*comp)(node_t *toinsert, node_t *comp));

/*
Attempts to remove a node from a linked list.
Will attempt to free the node's data with delete_data.
Returns LINKED_FAIL on error.
*/
size_t linkedlist_remove(linkedlist_t *linkedlist,
    node_t *node, bool delete_data);

/*
Removes node objects in a linked list that match the comparison function.
Can delete the data or mass delete all matches based on the given booleans.
Will return LINKED_FAIL on error, or the amount of deletions on success.
*/
size_t linkedlist_removecomp(linkedlist_t *linkedlist,
    bool (*comp)(node_t *node), bool delete_data, bool mass_delete);

/*
Attempts to find a node in a given linked list, given a comparison function.
Will return NULL if no node was found.
*/
node_t *linkedlist_find(linkedlist_t *linkedlist, bool (*comp)(node_t *node));

#endif
