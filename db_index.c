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

    alloc_sort_table(db, db->hdr.nr_per);
    create_index_per_cpy(db);
    free_sort_table(db);

    // alloc
    //create_index_per_lastname(db);
    // free

    //write_indexes_hdr(db);

    printf("\nDONE => Indexes created: %d", db->hdr.nr_ipc + db->hdr.nr_ipl);
}


/****************************************************************************************
* Create index bloc Person/Company into db
****************************************************************************************/
void create_index_per_cpy(dbc *db) {

    int i;
    uint pt_cpy;
    cper per;
    tipc ipc;

    db->fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    db->fp_lg = fopen("data_db_clients/db_clients.log", "a");

    printf("\n\tCreating indexes for persons per company... ");

    // read data from person table and set db->sort table
    for (i=0; i<db->hdr.nr_per; i++) {
        memset(&per, 0, sizeof(cper));                          // init element
        pt_cpy = db->hdr.off_per + i * sizeof(cper);            // getting element offset
        fseek(db->fp_db, pt_cpy, SEEK_SET);                     // place cursor at element offset
        fread(&per, sizeof(cper), 1, db->fp_db);         // read element
        db->sort[i].id = per.id_cpy;                            // set read data into db->sort[i]
        db->sort[i].off_sort_obj = pt_cpy;                      // set read data into db->sort[i]
    }

    sort_index(db, SORT_PERS_COMP);

    fseek(db->fp_db, db->hdr.off_ipc, SEEK_SET);                // getting first element offset

    // write sorted data into db
    for (i=0; i<db->hdr.nr_per; i++) {
        memset(&ipc, 0, sizeof(tipc));                          // init tipc element to be written
        strcpy(ipc.tp_rec, "IPC");                              // set element data
        ipc.id_cpy = db->sort[i].id;                            // fill element data from sorted table
        ipc.per_offset = db->sort[i].off_sort_obj;              // fill element data from sorted table
        fwrite(&ipc, sizeof(tipc), 1, db->fp_db);        // write element into db
    }

    db->hdr.nr_ipc = i;

    fprintf(db->fp_lg, "%s Index persons per company created\n", timestamp());

    fclose(db->fp_db);
    fclose(db->fp_lg);

    printf("DONE => Indexes created: %d", db->hdr.nr_ipc);


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
// TODO extra
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
void sort_index(dbc *db, int type) {

    /// test
//    for (int i=0; i<100; i++) {
//        printf("%d %d\n", db->sort[i].id, db->sort[i].off_sort_obj);
//    }
}


/****************************************************************************************
* Allocate memory to the sorting table in RAM
****************************************************************************************/
void alloc_sort_table(dbc *db, uint size) {

    db->sort = malloc(size * sizeof(cper));

}


/****************************************************************************************
* Free memory used by the sorting table in RAM
****************************************************************************************/
void free_sort_table(dbc *db) {

    free(db->sort);

}