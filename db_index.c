/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients
 *
 *       Indexes table functions, finding results by foreign key
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"



/******************************************************************************************
* Create index blocs into the DB and couple Person and Company tables by their foreign key
******************************************************************************************/
void create_index(dbc *db) {

    printf("\n\tCreating indexes... ");

    alloc_sort_table(db, db->hdr.nr_per);
    create_index_per_cpy(db);
    free_sort_table(db);

    alloc_sort_table(db, db->hdr.nr_per);
    create_index_per_name(db);
    free_sort_table(db);

    printf("\n\tDONE => Indexes created: %d", db->hdr.nr_ipc + db->hdr.nr_ipl);
}


/****************************************************************************************
 * Set up the index table with Person foreign key 'id_company' by:
 *      - reading data from the DB and storing it into RAM
 *      - sort the read data
 *      - and write it back into DB
 * This table will contain the offset of the person and its id_company
****************************************************************************************/
void create_index_per_cpy(dbc *db) {

    int i;
    uint pt_per;
    cper per;
    tipc ipc;

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

    printf("DONE => Indexes created: %d", db->hdr.nr_ipc);
}



/****************************************************************************************
 * Set up the index table for Person 'lastname' by collecting lastnames,
 * storing it into RAM, sort it alphabetically and write it back into DB
 * This table will contain the lastname of the person, its offset and
 * another offset pointing to the next element (person) of the list
****************************************************************************************/
void create_index_per_name(dbc *db) {

    int i;
    uint pt_per;
    cper per;
    tipl ipl;

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
        memset(&ipl, 0, sizeof(tipl));                          // init tipl element to be written
        strcpy(ipl.tp_rec, "IPL");                              // set element data
        strcpy(ipl.nm_lst, db->sort[i].ln);                     // fill element data from sorted table
        ipl.per_offset = db->sort[i].off_sort_obj;              // fill element data from sorted table

        fwrite(&ipl, sizeof(tipl), 1, db->fp_db);        // write element into db
    }

    db->hdr.nr_ipl = i;

    fprintf(db->fp_lg, "%s Index persons by lastname created\n", timestamp());

    db->hdr.ipl_root = find_ipl_tree_root(db, (db->hdr.nr_per-1)/2, db->hdr.nr_per);

    // update the db header
    fseek(db->fp_db, 0, SEEK_SET);
    fwrite(&db->hdr, sizeof(hder), 1, db->fp_db);

    printf("DONE => Indexes created: %d", db->hdr.nr_ipl);
}



/*****************************************************************************************
 * Binary Search per ID. Multiplexed for Company ID and Person ID depending on the 'type'
 *
 *      id    : element ID we want to search
 *      type  : Company ID or Person ID
 *      return: the element index/position within the db company block (per_id or per_ln)
******************************************************************************************/
int search_binary(dbc *db, int id, int type) {

    int size, mid, left=0, right;

    if (type == COMP_ID) {

        ccpy elm;
        size = sizeof(ccpy);
        right = db->hdr.nr_cpy - 1;
        memset(&elm, 0, size);

        elm = read_single_company(db, 0);           // check db first element
        if (id < elm.id_cpy) {                            // if ID we search < first cpy ID
            return REC_OUT_RANGE;
        }

        if (id == elm.id_cpy) {                           // if match (if ID we search IS the first db element)
            return 0;
        }

        elm = read_single_company(db, right);             // check db last element
        if (id > elm.id_cpy) {                            // if ID we search > last cpy ID
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
        if (id < elm.id_per) {                           // if ID we search < first person ID
            return REC_OUT_RANGE;
        }

        if (id == elm.id_per) {                          // if match (if ID we search IS the first db element)
            return 0;
        }

        elm = read_single_person(db, right);             // check db last element
        if (id > elm.id_per) {                           // if ID we search > last person ID
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


/*****************************************************************************************
 * Binary Search per company ID on ipc table.
 *
 *      id    : person table foreign key company ID
 *      ipcc  : ipc element
 *      return: the element index/position within the db ipc block
******************************************************************************************/
int search_binary_ipc(dbc *db, int id) {

    int mid, left=0, right;
    tipc ipc;

    right = db->hdr.nr_per - 1;
    memset(&ipc, 0, sizeof(tipc));

    ipc = read_single_tipc_rec(db, 0);           // check db first element
    if (id < ipc.id_cpy) {                             // if ID we search < first cpy ID
        return REC_OUT_RANGE;
    }

    if (id == ipc.id_cpy) {                            // if ID we search IS the first db element
        return 0;
    }

    ipc = read_single_tipc_rec(db, right);             // check db last element
    if (id > ipc.id_cpy) {                             // if ID we search > last cpy ID
        return REC_OUT_RANGE;
    }

    while (right - left > 1) {                         // binary search algorithm

        mid = (right + left) / 2;
        ipc = read_single_tipc_rec(db, mid);

        if (id <= ipc.id_cpy) {
            right = mid;
        } else {
            left = mid;
        }
    }
    ipc = read_single_tipc_rec(db, right);

    if (id == ipc.id_cpy) {
        return right;
    } else {
        return REC_NOT_FOUND;
    }
}




/****************************************************************************************
 * Binary Search per person lastname.
 *
 *      name  : person name we want to search
 *      return: the element index/position within the db person block
****************************************************************************************/
int search_binary_string(dbc *db, char *name) {

    // TODO delete it. It has been replaced

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
 * Bubble sort algorithm used for index creation (INEFFICIENT for this project: very slow)
 *
 *      nr  : nr of elements in the list we want to sort
 *      type: persons by company ID || persons by lastname
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
 * Quick sort algorithm used for index tables creation
 *
 *      nr  : nr of elements in the list we want to sort
 *      type: persons by company ID || persons by lastname
****************************************************************************************/
void quicksort(dbc *db, int first, int last, int type) {

    if (type == SORT_PERS_COMP) {                       // sorting persons by company ID

        int i, j, pivot;
        t_sort temp;

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
                    temp = db->sort[i];
                    db->sort[i] = db->sort[j];
                    db->sort[j] = temp;
                }
            }

            temp = db->sort[pivot];
            db->sort[pivot] = db->sort[j];
            db->sort[j] = temp;

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

/// test
void quicksort_test(dbc *db, int first, int last) {

    int i, j, pivot;
    t_lsort temp;


    if (first < last) {
        pivot = first;
        i = first;
        j = last;

        while (i < j) {
            while (db->lsort[i].id <= db->lsort[pivot].id && i < last)
                i++;
            while (db->lsort[j].id > db->lsort[pivot].id)
                j--;
            if (i < j) {
                temp = db->lsort[i];
                db->lsort[i] = db->lsort[j];
                db->lsort[j] = temp;
            }
        }

        temp = db->lsort[pivot];
        db->lsort[pivot] = db->lsort[j];
        db->lsort[j] = temp;

        quicksort_test(db, first, j - 1);
        quicksort_test(db, j + 1, last);
    }
}


/****************************************************************************************
 * Memory allocation in RAM for the list sorting area (person by ID)
 *
 *      size: nr of elements in the list we want to sort
****************************************************************************************/
void alloc_sort_table(dbc *db, uint size) {

    db->sort = (t_sort*)malloc(size * sizeof(t_sort));
    memset(db->sort, 0, size * sizeof(t_sort));
}


/****************************************************************************************
 * Memory allocation in RAM for the list sorting area (person by Lastname)
 *
 *      size: nr of elements in the list we want to sort
****************************************************************************************/
void alloc_link_sort_table(dbc *db, uint size) {

    db->lsort = (t_lsort*)malloc(size * sizeof(t_lsort));
    memset(db->lsort, 0, size * sizeof(t_lsort));
}


/****************************************************************************************
* Free memory allocated with alloc_sort_table()
****************************************************************************************/
void free_sort_table(dbc *db) {

    if (db->sort)
        free(db->sort);
}


/****************************************************************************************
* Free memory allocated with alloc_link_sort_table()
****************************************************************************************/
void free_link_sort_table(dbc *db) {

    if (db->lsort)
        free(db->lsort);
}


/****************************************************************************************
 * Read Index Person/lastname record given its position within DB Index tipl block.
 *
 *      index : element position within the 'person by lastname' db block
 *      return: tipl element attributes
****************************************************************************************/
tipl read_single_tipl_rec(dbc *db, int index) {

    tipl ipl;

    fseek(db->fp_db, db->hdr.off_ipl + index * sizeof(tipl), SEEK_SET);
    fread(&ipl, sizeof(tipl), 1, db->fp_db);

    return ipl;
}


/****************************************************************************************
 * Read Index Person/companyID record given its position within DB Index tipc block.
 *
 *      index : element position within the 'person by company ID' db block
 *      return: tipc element attributes
****************************************************************************************/
tipc read_single_tipc_rec(dbc *db, int index) {

    tipc ipc;

    fseek(db->fp_db, db->hdr.off_ipc + index * sizeof(tipc), SEEK_SET);
    fread(&ipc, sizeof(tipc), 1, db->fp_db);

    return ipc;
}


/****************************************************************************************
* Read data from person/companyID table and store it in RAM
****************************************************************************************/
//void load_ipl_in_ram(dbc *db) {
//
//    uint pt_next;
//    uint pt_ipc;
//    tipc ipc;
//    int i;
//
//    // read data from ipc table and set db->lsort table
//    for (i=0; i<db->hdr.nr_per; i++) {
//
//        memset(&ipc, 0, sizeof(tipc));
//        pt_ipc = db->hdr.off_ipc + i * sizeof(tipc);               // getting element offset
//        fseek(db->fp_db, pt_ipc, SEEK_SET);                        // place cursor at element offset
//        fread(&ipc, sizeof(tipc), 1, db->fp_db);            // read element
//
//        db->lsort[i].id = ipc.id_cpy;                              // set read data into db->lsort[i]
//        db->lsort[i].off_sort_obj = ipc.per_offset;                // set read data into db->lsort[i]
//
//        pt_next = ipc.per_offset + 1*sizeof(cper);                 // getting next element offset
//        if (pt_next >= db->hdr.db_size) {                          // prevent out range
//            pt_next = db->hdr.db_size;
//        }
//        db->lsort[i].off_next = pt_next;                           // set read data into db->lsort[i]
//    }
//}


/****************************************************************************************
* Give the list of employees per Company given its ID (main function)
****************************************************************************************/
void get_comp_employees(dbc *db) {

    int comp_id;

    printf("\n\t--> Enter Company ID: "); scanf("%d", &comp_id); fflush(stdin);
    list_comp_employees(db, comp_id);
}


/****************************************************************************************
 * Display the list of employees of one Company given its ID:
 *
 *      comp_id: ID company we want to search
****************************************************************************************/
void list_comp_employees(dbc *db, int comp_id) {

    cper per;
    ccpy cpy;
    tipc ipc;
    int index, count=0;

    index = search_binary(db, comp_id, COMP_ID);                  // get cpy index within db file cpy bloc

    if (index == REC_OUT_RANGE) {
        printf("\n\tCompany ID %d is out of range\n\n", comp_id);
    } else if (index == REC_NOT_FOUND) {
        printf("\n\tNo results with Company ID %d\n\n", comp_id);
    } else {
        cpy = read_single_company(db, index);                          // read cpy at given index and display it

        printf("\n\t********************************************************************************\n");
        printf("\n\tCompany name......... %s", cpy.nm_cpy);
        printf("\n\tID company........... %-d", cpy.id_cpy);
        printf("\n\tCountry.............. %s", db->cty[cpy.id_cty].nm_cty);
        printf("\n\n\t********************************************************************************\n");
        printf("\n\t*** EMPLOYEES ***\n");
        printf("\n\t%8s", "ID");
        printf(" %15s", "Lastname");
        printf(" %15s", "Firstname");
        printf("%40s", "Job");
        printf("\n\t--------------------------------------------------------------------------------");
    }

    index = search_binary_ipc(db, comp_id);

    while (1) {
        ipc = read_single_tipc_rec(db, index);

        if (ipc.id_cpy != cpy.id_cpy) {
            break;
        }

        fseek(db->fp_db, ipc.per_offset, SEEK_SET);
        fread(&per, sizeof(cper), 1, db->fp_db);

        printf("\n\t%8d", per.id_per);
            printf(" %15s", per.nm_lst);
            printf(" %15s", per.nm_fst);
            printf(" %40s", db->job[per.id_job].nm_job);
        index++;
        count++;
    }

    printf("\n\t--------------------------------------------------------------------------------");
    printf("\n\tEmployees: %d", count);
    printf("\n\n\t********************************************************************************\n");
}


/****************************************************************************************
 * Provide the binary tree root of person/lastname index table and complete
 * person/lastname index table by writing the offset left and offset right
 * for every element of the table.
 *
 *      offset: root offset of the ipl table
 *      size  : total number of elements in ipl table
 *      return: tree root of ipl table
****************************************************************************************/
uint find_ipl_tree_root(dbc *db, uint offset, int size) {

    uint cur, off_l, off_r;
    int diff = size-1;
    int left = diff/2;
    int right = diff - left;
    tipl ipl;

    off_l = offset - left + (left-1)/2;
    off_r = offset + (right-1)/2 + 1;
    cur = db->hdr.off_ipl + offset * sizeof(tipl);

    fseek(db->fp_db, cur, SEEK_SET);
    fread(&ipl, sizeof(tipl), 1, db->fp_db);

    if (left > 0) {
        ipl.per_offset_l = find_ipl_tree_root(db, off_l, left);
    }
    if (right > 0) {
        ipl.per_offset_r = find_ipl_tree_root(db, off_r, right);
    }

    fseek(db->fp_db, cur, SEEK_SET);
    fwrite(&ipl, sizeof(tipl), 1, db->fp_db);

    return cur;
}