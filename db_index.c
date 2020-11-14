/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients V0
 *
 *       Indexes table functions, finding results by foreign key
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"

/****************************************************************************************
* Create index blocs into db
****************************************************************************************/
void create_index(dbc *db) {

    printf("\n\tCreating indexes... ");

    // alloc
    create_index_per_cpy(db);
    // free

    // alloc
    create_index_per_lastname(db);
    // free

    write_indexes_hdr(db);

    printf("DONE => Indexes created: %d", db->hdr.nr_ipc + db->hdr.nr_ipl);
}


/****************************************************************************************
* Create index bloc Person/Company into db
****************************************************************************************/
void create_index_per_cpy(dbc *db) {


}


/****************************************************************************************
* Create index bloc Person/Lastname into db
****************************************************************************************/
void create_index_per_lastname(dbc *db) {


}

/****************************************************************************************
* Give the list of employees per Company
****************************************************************************************/
void search_company_employees(dbc *db) {

    printf("*** search company employees ***\n");
    printf("enter beginning of company name: ...\n");
    printf("OR\n");
    printf("enter company ID: ...\n");

    /// print option to generate report ***
}


/****************************************************************************************
* Provide company details (nb of employees, foreign key details, calculations)
****************************************************************************************/
void company_details(dbc *db) {

    printf("*** company details ***\n");
    printf("enter beginning of company name: ...\n");
    printf("OR\n");
    printf("enter company ID: ...\n");

    /// print option to generate report ***
}


/****************************************************************************************
* Give the list of companies per Group
****************************************************************************************/
void search_group_companies(dbc *db) {

    printf("*** search group companies ***\n");
    printf("enter group name: ...\n");
    printf("OR\n");
    printf("enter group ID: ...\n");

    /// print option to generate report ***
}


/****************************************************************************************
* Sorting algorithm used for index creation
****************************************************************************************/
void sort_index(dbc *db) {

}