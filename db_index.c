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

    alloc_sort_table(db, db->hdr.nr_per);
    create_index_per_name(db);
    free_sort_table(db);

    //write_indexes_hdr(db);

    printf("\n\tDONE => Indexes created: %d", db->hdr.nr_ipc + db->hdr.nr_ipl);
}


/****************************************************************************************
* Create index bloc Person/Company into db
****************************************************************************************/
void create_index_per_cpy(dbc *db) {

    int i;
    uint pt_per;
    cper per;
    tipc ipc;

    db->fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    db->fp_lg = fopen("data_db_clients/db_clients.log", "a");

    printf("\n\t\t* Creating indexes for persons per company... ");

    // read data from person table and set db->sort table
    for (i=0; i<db->hdr.nr_per; i++) {
        memset(&per, 0, sizeof(cper));                          // init element
        pt_per = db->hdr.off_per + i * sizeof(cper);            // getting element offset
        fseek(db->fp_db, pt_per, SEEK_SET);                     // place cursor at element offset
        fread(&per, sizeof(cper), 1, db->fp_db);         // read element
        db->sort[i].id = per.id_cpy;                            // set read data into db->sort[i]
        db->sort[i].off_sort_obj = pt_per;                      // set read data into db->sort[i]
    }

    //sort_bubble_index(db, db->hdr.nr_per, SORT_PERS_COMP);    // inefficient: more than 15 min of waiting
    quicksort(db, 0, db->hdr.nr_per-1, SORT_PERS_COMP);

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
void create_index_per_name(dbc *db) {

    int i;
    uint pt_per;
    cper per;
    tipl ipl;

    db->fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    db->fp_lg = fopen("data_db_clients/db_clients.log", "a");

    printf("\n\t\t* Creating indexes for persons by lastname... ");

    // read data from person table and set db->sort table
    for (i=0; i<db->hdr.nr_per; i++) {
        memset(&per, 0, sizeof(cper));                          // init element
        pt_per = db->hdr.off_per + i * sizeof(cper);            // getting element offset
        fseek(db->fp_db, pt_per, SEEK_SET);                     // place cursor at element offset
        fread(&per, sizeof(cper), 1, db->fp_db);         // read element
        strcpy(db->sort[i].ln, per.nm_lst);                     // set read data into db->sort[i]
        db->sort[i].off_sort_obj = pt_per;                      // set read data into db->sort[i]
    }

    quicksort(db, 0, db->hdr.nr_per-1, SORT_PERS_NAME);

    fseek(db->fp_db, db->hdr.off_ipl, SEEK_SET);                // getting first element offset

    // write sorted data into db
    for (i=0; i<db->hdr.nr_per; i++) {
        memset(&ipl, 0, sizeof(tipl));                          // init tipc element to be written
        strcpy(ipl.tp_rec, "IPL");                              // set element data
        strcpy(ipl.nm_lst, db->sort[i].ln);                     // fill element data from sorted table
        ipl.per_offset = db->sort[i].off_sort_obj;              // fill element data from sorted table
        fwrite(&ipl, sizeof(tipl), 1, db->fp_db);        // write element into db
    }

    db->hdr.nr_ipl = i;

    fprintf(db->fp_lg, "%s Index persons by lastname created\n", timestamp());

    fclose(db->fp_db);
    fclose(db->fp_lg);

    printf("DONE => Indexes created: %d", db->hdr.nr_ipc);
}



/****************************************************************************************
* Binary Search per ID. Split by search type
* return element position within the db company block
****************************************************************************************/
int search_binary(dbc *db, int id, int type) {

    int size, mid, left=0, right;

    if (type == COMP_ID) {

        ccpy elm;
        size = sizeof(ccpy);
        right = db->hdr.nr_cpy - 1;
        memset(&elm, 0, size);

        elm = read_single_company(db, 0);           // check db first element
        if (id < elm.id_cpy) {                            // if ID we search < 108
            return REC_OUT_RANGE;
        }

        if (id == elm.id_cpy) {                           // if match (if ID we search is the first db element)
            return 0;
        }

        elm = read_single_company(db, right);             // check db last element
        if (id > elm.id_cpy) {                            // if ID we search > 686255
            return REC_OUT_RANGE;
        }

        while (right - left > 1) {

            mid = (right + left) / 2;
            elm = read_single_company(db, mid);

            if (id <= elm.id_cpy) {
                right = mid;
            } else {
                left = mid;
            }
        }
        elm = read_single_company(db, right);

        if (id == elm.id_cpy) {
            return right;
        } else {
            return REC_NOT_FOUND;
        }

    } else if (type == PERS_ID) {

        cper elm;
        size = sizeof(cper);
        right = db->hdr.nr_per - 1;
        memset(&elm, 0, size);

        elm = read_single_person(db, 0);           // check db first element
        if (id < elm.id_per) {                           // if ID we search < 108
            return REC_OUT_RANGE;
        }

        if (id == elm.id_per) {                          // if match (if ID we search is the first db element)
            return 0;
        }

        elm = read_single_person(db, right);             // check db last element
        if (id > elm.id_per) {                           // if ID we search > 686255
            return REC_OUT_RANGE;
        }

        while (right - left > 1) {

            mid = (right + left) / 2;
            elm = read_single_person(db, mid);

            if (id <= elm.id_per) {
                right = mid;
            } else {
                left = mid;
            }
        }
        elm = read_single_person(db, right);

        if(id == elm.id_per) {
            return right;
        } else {
            return REC_NOT_FOUND;
        }
    }
    return 0;
}



/****************************************************************************************
* Binary Search per string.
* return element position within the DB Person block
****************************************************************************************/
int search_binary_string(dbc *db, char *name) {

    // FIXME improve it in order to get ALL persons responding to a given string (name)
    // FIXME set it as NOT case sensitive (upper || lower)

    int size, mid, left=0, right;

    tipl elm;
    size = sizeof(tipl);
    right = db->hdr.nr_per - 1;
    memset(&elm, 0, size);

    elm = read_single_tipl_rec(db, 0);           // check db first element
    if (strcmp(name, elm.nm_lst) < 0) {                // if ID we search
        return REC_OUT_RANGE;
    }

    if (strcmp(name, elm.nm_lst) == 0) {               // if match (if ID we search is the first db element)
        return 0;
    }

    elm = read_single_tipl_rec(db, right);             // check db last element
    if (strcmp(name, elm.nm_lst) > 0) {
        return REC_OUT_RANGE;
    }

    while (right - left > 1) {

        mid = (right + left) / 2;
        elm = read_single_tipl_rec(db, mid);

        if (strcmp(name, elm.nm_lst) <= 0) {
            right = mid;
        } else {
            left = mid;
        }
    }
    elm = read_single_tipl_rec(db, right);

    if(strcmp(name, elm.nm_lst) == 0) {
        return right;
    } else {
        return REC_NOT_FOUND;
    }
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
void sort_bubble_index(dbc *db, int nr, int type) {

    if (type == SORT_PERS_COMP) {

        int j, k=0, t=0;
        int unordered=1, bigger=0;
        t_sort tmp;

        while (unordered) {
            unordered = 0;
            k++;
            for (j=0; j<nr-k; j++) {

                bigger = db->sort[j].id > db->sort[j+1].id;

                if (bigger) {
                    tmp = db->sort[j+1];
                    db->sort[j+1] = db->sort[j];
                    db->sort[j] = tmp;
                    unordered = 1;
                }
            }
        }
    }
}


/****************************************************************************************
* Quick Sort algorithm used for index creation
****************************************************************************************/
void quicksort(dbc *db, int first, int last, int type) {

    if (type == SORT_PERS_COMP) {                       // sorting persons by ID company

        int i, j, pivot, temp;

        if (first < last) {
            pivot = first;
            i = first;
            j = last;

            while (i < j) {
                while (db->sort[i].id <= db->sort[pivot].id && i < last)
                    i++;
                while (db->sort[j].id > db->sort[pivot].id)
                    j--;
                if (i < j) {
                    temp = db->sort[i].id;
                    db->sort[i].id = db->sort[j].id;
                    db->sort[j].id = temp;
                }
            }

            temp = db->sort[pivot].id;
            db->sort[pivot].id = db->sort[j].id;
            db->sort[j].id = temp;

            quicksort(db, first, j - 1, SORT_PERS_COMP);
            quicksort(db, j + 1, last, SORT_PERS_COMP);
        }

    } else if (type == SORT_PERS_NAME) {                // sorting persons by lastname

        int i, j, pivot;
        t_sort temp;

        if (first < last) {
            pivot = first;
            i = first;
            j = last;

            while (i < j) {
                while (strcmp(db->sort[i].ln, db->sort[pivot].ln) <= 0 && i < last)
                    i++;
                while (strcmp(db->sort[j].ln, db->sort[pivot].ln) > 0)
                    j--;
                if (i < j) {
                    temp = db->sort[i];
                    db->sort[i] = db->sort[j];
                    db->sort[j] = temp;
                }
            }
            temp = db->sort[pivot];
            db->sort[pivot] = db->sort[j];
            db->sort[j] = temp;

            quicksort(db, first, j - 1, SORT_PERS_NAME);
            quicksort(db, j + 1, last, SORT_PERS_NAME);
        }
    }
}


/****************************************************************************************
* Allocate memory to the sorting table in RAM
****************************************************************************************/
void alloc_sort_table(dbc *db, uint size) {

    db->sort = (t_sort*)malloc(size * sizeof(t_sort));
    memset(db->sort, 0, size * sizeof(t_sort));
}


/****************************************************************************************
* Free memory used by the sorting table in RAM
****************************************************************************************/
void free_sort_table(dbc *db) {

    if (db->sort)
        free(db->sort);
}



/****************************************************************************************
* Read Index Person/lastname record given its position within DB Index tipl block.
****************************************************************************************/
tipl read_single_tipl_rec(dbc *db, int index) {

    tipl ipl;
    FILE *fp_db;

    fp_db = open_db_file(db);

    fseek(fp_db, db->hdr.off_ipl + index * sizeof(tipl), SEEK_SET);
    fread(&ipl, sizeof(tipl), 1, fp_db);

    fclose(fp_db);

    return ipl;
}