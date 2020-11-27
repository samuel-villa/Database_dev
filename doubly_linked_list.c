/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients
 *
 *       Functions related to the doubly linked list created to sort and display elements
 *       of the person table and/or the company table using their foreign key field
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"

/*****************************************************************************************************
 * Create doubly linked list, at this point the list is empty so prev and next both point to the root
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
 *      ls: linked list to delete
*****************************************************************************************************/
void link_ls_delete(node **ls) {

    link_ls_empty(*ls);
    free(*ls);
    *ls = NULL;
}


/*****************************************************************************************************
 * Search the bigger element (alphabetically) matching with the element name
 *      ls : linked list element
 *      cpy: company searched
*****************************************************************************************************/
node *link_ls_search_cpy_z(node *ls, ccpy cpy) {

    node  *cur;

    for (cur = ls->next; cur != ls && strcmp(cur->cpy.nm_cpy, cpy.nm_cpy) < 0; cur = cur->next );

    return cur;
}


/*****************************************************************************************************
 * Add element before the given element within the linked list
 *      elem: element of the linked list
 *      cpy : company element
*****************************************************************************************************/
void link_ls_add_before(node *elem, ccpy cpy, cper per) {

    node *new_elem = malloc(sizeof(*new_elem));
    if (new_elem != NULL) {

        new_elem->cpy = cpy;
        new_elem->prev = elem->prev;
        new_elem->next = elem;

        elem->prev->next = new_elem;
        elem->prev = new_elem;
    }

}


/*****************************************************************************************************
 *
*****************************************************************************************************/
void link_ls_list(dbc *db) {

    ccpy cpy;
    cper per;
    char cpy_name[BUF_LEN];
    int found, nr, len;
    node *root, *it, *cur;

    printf("\n\tEnter partial company name: "); scanf("%s", cpy_name); fflush(stdin);

    len = strlen(cpy_name);

    fseek(db->fp_db, db->hdr.off_cpy, SEEK_SET);
    root = link_ls_create();

    do {
        memset(&cpy, 0, sizeof(cpy));
        fread(&cpy, sizeof(cpy), 1, db->fp_db);

        if (!cpy.id_cpy) {
            break;
        }
        found = 1;

        for(int i=0; i<len; i++) {
            if (toupper(cpy_name[i]) != toupper(cpy.nm_cpy[i])) {
                found = 0;
            }
        }

        if (found) {
            cur = link_ls_search_cpy_z(root, cpy);
            link_ls_add_before(cur, cpy, per);
        }
    } while (cpy.id_cpy);

    nr = 0;

    //for (it = root->next; it != root; it = it->next) {            // A-Z
    for (it = root->prev; it != root; it = it->prev) {              // Z-A

        printf("%d %s %s\n", it->cpy.id_cpy, it->cpy.nm_cpy, it->cpy.dt_cre);
        nr++;
    }

    link_ls_delete(&root);
}