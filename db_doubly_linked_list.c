/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients
 *
 *       Functions related to the doubly linked list
 *       creation (memory allocation) and deletion (memory free) in two steps.
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"



/*****************************************************************************************************
 * Create doubly linked list, at this point the list is empty so prev and next both point to the root
 *
 *      return: root pointer
*****************************************************************************************************/
node *link_ls_create() {

    node *root = malloc(sizeof(*root));

    if (root != NULL) {
        root->prev = root;
        root->next = root;
    }
    return root;
}


/*****************************************************************************************************
 * Empty the doubly linked list
 *
 *      ls: linked list to process
*****************************************************************************************************/
void link_ls_empty(node *ls) {

    node *cur, *next = NULL;

    for (cur = ls->next; cur != ls; cur = next) {
        next = cur->next;
        free(cur);
    }
    ls->prev = ls;
    ls->next = ls;
}


/*****************************************************************************************************
 * Delete the doubly linked list
 *
 *      ls: linked list to delete
*****************************************************************************************************/
void link_ls_delete(node **ls) {

    link_ls_empty(*ls);
    free(*ls);
    *ls = NULL;
}
