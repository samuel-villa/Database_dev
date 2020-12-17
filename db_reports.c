/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients
 *
 *       Reports: data searching, sorting and report generation
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"



/****************************************************************************************
* Generate .txt report file with header (user name and date)
****************************************************************************************/
void report_template(dbc *db) {

    char file_path[50], report[80];

    strcpy(file_path, "data_db_clients/report_");
    sprintf(report, "%s%s.txt", file_path, timestamp_filename());

    db->fp_rp = fopen(report, "w");

    if(db->fp_rp == NULL) {
        printf("\nUnable to create file.\n");
        return;
    }

    // document header
    fprintf(db->fp_rp, "%s REPORT %50s\n", timestamp(), "Samuel CIULLA");
    fprintf(db->fp_rp, "--------------------------------------------------------------------------------\n");
}



/****************************************************************************************
 * Search companies matching with given group ID and
 * generate report listing the results grouped by country
****************************************************************************************/
void report_group_companies(dbc *db) {

    ccpy cpy;
    int group_id, found, count=0;
    node *root, *it, *cur;

    printf("\n\tEnter group ID: "); scanf("%d", &group_id); fflush(stdin);

    fseek(db->fp_db, db->hdr.off_cpy, SEEK_SET);
    root = link_ls_create();                                        // create an empty doubly linked list

    do {                                                            // search for matching elements
        memset(&cpy, 0, sizeof(cpy));
        fread(&cpy, sizeof(cpy), 1, db->fp_db);

        if (!cpy.id_cpy) {
            break;
        }
        found = 1;

        if (group_id != cpy.id_grp) {
            found = 0;
        }

        if (found) {                                                // if match add element to linked list
            cur = search_bigger_cpy(root, cpy);
            add_cpy_before(cur, cpy);
        }
    } while (cpy.id_cpy);

    report_template(db);                                            // generate report

    fprintf(db->fp_rp, "\n\n\nList of all companies belonging to the group '%s'\n\n", db->grp[group_id].nm_grp);

    for (int c=1; c<=db->hdr.nr_cty; c++) {
        fprintf(db->fp_rp, "\n\t\t\t\t\t%d - %s\n\n", db->cty[c].id_cty, db->cty[c].nm_cty);
        fprintf(db->fp_rp, "%8s | %-40s | %-32s\n", "ID", "COMPANY NAME", "CITY");

        for (it = root->next; it != root; it = it->next) {

            if (db->cty[c].id_cty == it->cpy.id_cty) {
                fprintf(db->fp_rp, "--------------------------------------------------------------------------------\n");
                fprintf(db->fp_rp, "%8d | %-40s | %-32s\n", it->cpy.id_cpy, it->cpy.nm_cpy, it->cpy.nm_cit);
                count++;
            }
        }
    }

    fprintf(db->fp_rp, "\n\n================================================================================\n\n");
    fprintf(db->fp_rp, "\t-> Total Number of Companies: %d\n\n", count);
    fclose(db->fp_rp);

    fprintf(db->fp_lg, "%s Report generated: companies per group\n", timestamp());

    link_ls_delete(&root);
}


/****************************************************************************************
 * Count persons working for a given group and generate report
****************************************************************************************/
void report_group_persons(dbc *db) {

    ccpy cpy;
    tipc ipc;
    int group_id, found, index, total_nr_emp=0, total_db_per=0;
    node *root, *it, *cur;

    printf("\n\tEnter group ID: "); scanf("%d", &group_id); fflush(stdin);

    fseek(db->fp_db, db->hdr.off_cpy, SEEK_SET);
    root = link_ls_create();                                        // create an empty doubly linked list

    do {                                                            // search for matching elements
        memset(&cpy, 0, sizeof(cpy));
        fread(&cpy, sizeof(cpy), 1, db->fp_db);

        if (!cpy.id_cpy) {
            break;
        }
        found = 1;

        if (group_id != cpy.id_grp) {
            found = 0;
        }

        if (found) {                                                // if match add element to linked list
            cur = search_bigger_cpy(root, cpy);
            add_cpy_before(cur, cpy);
        }
    } while (cpy.id_cpy);

    for (it = root->next; it != root; it = it->next) {              // calculate total nr_emp of given group
        total_nr_emp += it->cpy.nr_emp;

        index = search_binary_ipc(db, it->cpy.id_cpy);              // calculate total employees present into db

        while (1) {
            ipc = read_single_tipc_rec(db, index);

            if (ipc.id_cpy != it->cpy.id_cpy) {
                break;
            }
            total_db_per++;
            index++;
        }
    }

    report_template(db);                                           // generate report

    fprintf(db->fp_rp, "\n\n\nGroup '%s'", db->grp[group_id].nm_grp);
    fprintf(db->fp_rp, "\n--------------------------------------------------------------------------------\n");
    fprintf(db->fp_rp, "\n\t- %-43s%d", "Total number of employees: ", total_nr_emp);
    fprintf(db->fp_rp, "\n\t- Number of employees known in the database: %d\n\n", total_db_per);
    fclose(db->fp_rp);

    fprintf(db->fp_lg, "%s Report generated: number of employees per group\n", timestamp());

    link_ls_delete(&root);
}
