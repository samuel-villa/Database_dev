/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients V0
 *
 *       Person table functions
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"

#define BUF_LEN 200

/****************************************************************************************
* Loading file db_person.csv in the db
****************************************************************************************/
void import_CSV_person(dbc *db) {

    int i=0;
    char line[BUF_LEN];
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
    cper per;
    FILE *fpi, *fp_db, *fp_lg;

    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    fp_lg = fopen("data_db_clients/db_clients.log", "a");

    fpi = fopen("data_import/db_person.csv", "r");
    if (fpi == NULL) { printf("Error\n"); return; }

    printf("\n\tImporting persons... ");

    fgets(line, 200, fpi);

    fseek(fp_db, db->hdr.off_per, SEEK_SET);

    while (fgets(line, 200, fpi) != NULL) {

        memset(&per, 0, sizeof(cper));
        strcpy(per.tp_rec, "PER");

        ptr1 = strtok(line,";");
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        per.id_per = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        per.id_cpy = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        per.id_job = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(per.nm_civ, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(per.nm_fst, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(per.nm_lst, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(per.cd_sex, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(per.dt_cre, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(per.nr_tel, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(per.nr_gsm, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(per.nm_mail, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, strlen(ptr1)-1);
        per.nr_val = atoi(fld);

        fwrite(&per, 1, sizeof(cper), fp_db);

        i++;
    }

    fprintf(fp_lg, "%s Persons imported: %d\n", timestamp(), i);

    fseek(fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, fp_db);

    db->hdr.nr_per = i;

    fseek(fp_db, 0, SEEK_SET);
    fwrite(&db->hdr, sizeof(hder), 1, fp_db);

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpi);

    printf("DONE => Persons imported: %d", db->hdr.nr_per);

    return ;
}

/****************************************************************************************
* Export table Person to csv file
****************************************************************************************/
void export_CSV_person(dbc *db) {

    int i;
    cper per;
    FILE *fpo, *fp_db, *fp_lg;

    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    fp_lg = fopen("data_db_clients/db_clients.log", "a");

    printf("\n\tExporting persons... ");
    fpo = fopen("data_export/exp_person.csv", "w");

    fprintf(fpo,"id;id_cpy;id_job;nm_civ;nm_fst;nm_lst;cd_sex;dt_cre;nr_tel;nr_gsm;nm_mail;nr_val\n");

    fseek(fp_db, db->hdr.off_per, SEEK_SET);

    for (i=0; i<db->hdr.nr_per; i++) {

        memset(&per, 0, sizeof(cper));
        fread(&per, 1, sizeof(cper), fp_db);

        fprintf(fpo,"%d;%d;%d;%s;%s;%s;%s;%s;%s;%s;%s;%d\n",
                per.id_per,
                per.id_cpy,
                per.id_job,
                per.nm_civ,
                per.nm_fst,
                per.nm_lst,
                per.cd_sex,
                per.dt_cre,
                per.nr_tel,
                per.nr_gsm,
                per.nm_mail,
                per.nr_val);
    }

    fprintf(fp_lg, "%s Persons exported: %d\n", timestamp(), db->hdr.nr_per);

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpo);

    printf("DONE => Persons exported: %d", db->hdr.nr_per);

    return;
}

///****************************************************************************************
//* Load Person table into the buffer
//****************************************************************************************/
//void load_person(dbc *db) {
//
//    int i;
//    cper per;
//    FILE *fp_db, *fp_lg;
//
//    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
//    fp_lg = fopen("data_db_clients/db_clients.log", "a");
//
//    fseek(fp_db, 0, SEEK_SET);
//    fread(&db->hdr, sizeof(hder), 1, fp_db);        // Read header
//
//    printf("\nLoading persons into buffer... ");
//
//    fseek(fp_db, db->hdr.off_per, SEEK_SET);
//
//    for (i=1; i<=db->hdr.nr_per; i++) {
//
//        memset(&per, 0, sizeof(cper));
//        fread(&per, 1, sizeof(cper), fp_db);
//
//        db->per[i] = per;
//    }
//
//    fprintf(fp_lg, "%s Persons loaded into buffer: %d\n", timestamp(), db->hdr.nr_per);
//
//    fclose(fp_db);
//    fclose(fp_lg);
//
//    printf("DONE => Persons loaded: %d\n\n", db->hdr.nr_per);
//}
//
///****************************************************************************************
//* List and display Person table from the buffer
//****************************************************************************************/
//void print_person(dbc *db) {
//
//    for (int i=1; i<=db->hdr.nr_per; i++) { rec_person(db, i); }
//    puts("");
//}
//
///****************************************************************************************
//* Display one Person record from the buffer
//****************************************************************************************/
//void rec_person(dbc *db, int id_per) {
//
//    printf("%4d %4d %4d %20s %52s %52s %2s %11s %16s %16s %65s %4d\n",
//           db->per[id_per].id_per,
//           db->per[id_per].id_cpy,
//           db->per[id_per].id_job,
//           db->per[id_per].nm_civ,
//           db->per[id_per].nm_fst,
//           db->per[id_per].nm_lst,
//           db->per[id_per].cd_sex,
//           db->per[id_per].dt_cre,
//           db->per[id_per].nr_tel,
//           db->per[id_per].nr_gsm,
//           db->per[id_per].nm_mail,
//           db->per[id_per].nr_val);
//}


/****************************************************************************************
* Read Person record given its position within DB Person block.
****************************************************************************************/
cper read_single_person(dbc *db, int offset) {

    cper per;

    fseek(db->fp_db, db->hdr.off_per + offset * sizeof(cper), SEEK_SET);
    fread(&per, sizeof(ccpy), 1, db->fp_db);

    return per;
}



/****************************************************************************************
 Search Person by Lastname
****************************************************************************************/
void search_person_by_name(dbc *db) {

    printf("*** search peron by name ***\n");
    printf("enter beginning of lastname: ...\n");

    /// print option to generate report ***

}


/****************************************************************************************
 Search Person by Lastname
****************************************************************************************/
void search_person_by_id(dbc *db) {

    printf("*** search peron by id ***\n");
    printf("enter person ID: ...\n");

    /// print option to generate report ***

}