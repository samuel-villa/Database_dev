/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients V0
 *
 *       Company table functions
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"

#define BUF_LEN 300

/****************************************************************************************
* Loading file db_company.csv in the db
****************************************************************************************/
void import_CSV_company(dbc *db) {

    int i=0;
    char line[BUF_LEN];
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
    ccpy cpy;
    FILE *fpi, *fp_db, *fp_lg;

    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    fp_lg = fopen("data_db_clients/db_clients.log", "a");

    fpi = fopen("data_import/db_company.csv", "r");
    if (fpi == NULL) { printf("Error\n"); return; }

    printf("\n\tImporting companies... ");

    fgets(line, 300, fpi);

    fseek(fp_db, db->hdr.off_cpy, SEEK_SET);

    while (fgets(line, 300, fpi) != NULL) {

        memset(&cpy, 0, sizeof(ccpy));
        strcpy(cpy.tp_rec, "CPY");

        ptr1 = strtok(line,";");
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        cpy.id_cpy = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        cpy.id_grp = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        cpy.id_cty = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        cpy.id_ind = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(cpy.nm_cpy, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(cpy.nm_adr, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(cpy.cd_pos, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(cpy.nm_cit, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(cpy.nr_tel, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(cpy.nm_www, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(cpy.dt_cre, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        cpy.nr_emp = atoi(fld);
        ptr1 = ptr2;

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, strlen(ptr1)-1);
        cpy.am_val = atof(fld);

        fwrite(&cpy, 1, sizeof(ccpy), fp_db);

        i++;
    }

    fprintf(fp_lg, "%s Companies imported: %d\n", timestamp(), i);

    fseek(fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, fp_db);

    db->hdr.nr_cpy = i;

    fseek(fp_db, 0, SEEK_SET);
    fwrite(&db->hdr, sizeof(hder), 1, fp_db);

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpi);

    printf("DONE => Companies imported: %d", db->hdr.nr_cpy);

    return ;
}

/****************************************************************************************
* Export table Company to csv file
****************************************************************************************/
void export_CSV_company(dbc *db) {

    int i;
    ccpy cpy;
    FILE *fpo, *fp_db, *fp_lg;

    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    fp_lg = fopen("data_db_clients/db_clients.log", "a");

    printf("\n\tExporting companies... ");
    fpo = fopen("data_export/exp_company.csv", "w");

    fprintf(fpo,"id;id_grp;id_cty;id_ind;nm_cpy;nm_adr;cd_pos;nm_cit;nr_tel;nm_www;dt_cre;nr_emp;am_val\n");

    fseek(fp_db, db->hdr.off_cpy, SEEK_SET);

    for (i=0; i<db->hdr.nr_cpy; i++) {

        memset(&cpy, 0, sizeof(ccpy));
        fread(&cpy, 1, sizeof(ccpy), fp_db);

        fprintf(fpo,"%d;%d;%d;%d;%s;%s;%s;%s;%s;%s;%s;%d;%.2f\n",
                cpy.id_cpy,
                cpy.id_grp,
                cpy.id_cty,
                cpy.id_ind,
                cpy.nm_cpy,
                cpy.nm_adr,
                cpy.cd_pos,
                cpy.nm_cit,
                cpy.nr_tel,
                cpy.nm_www,
                cpy.dt_cre,
                cpy.nr_emp,
                cpy.am_val);
    }

    fprintf(fp_lg, "%s Companies exported: %d\n", timestamp(), db->hdr.nr_cpy);

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpo);

    printf("DONE => Companies exported: %d", db->hdr.nr_cpy);

    return;
}

///****************************************************************************************
//* Load Company table into the buffer
//****************************************************************************************/
//void load_company(dbc *db) {
//
//    int i;
//    ccpy cpy;
//    FILE *fp_db, *fp_lg;
//
//    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
//    fp_lg = fopen("data_db_clients/db_clients.log", "a");
//
//    fseek(fp_db, 0, SEEK_SET);
//    fread(&db->hdr, sizeof(hder), 1, fp_db);        // Read header
//
//    printf("\nLoading companies into buffer... ");
//
//    fseek(fp_db, db->hdr.off_cpy, SEEK_SET);
//
//    for (i=1; i<=db->hdr.nr_cpy; i++) {
//
//        memset(&cpy, 0, sizeof(ccpy));
//        fread(&cpy, 1, sizeof(ccpy), fp_db);
//
//        db->cpy[i] = cpy;
//    }
//
//    fprintf(fp_lg, "%s Companies loaded into buffer: %d\n", timestamp(), db->hdr.nr_cpy);
//
//    fclose(fp_db);
//    fclose(fp_lg);
//
//    printf("DONE => Companies loaded: %d\n\n", db->hdr.nr_cpy);
//}
//
///****************************************************************************************
//* List and display Company table from the buffer
//****************************************************************************************/
//void print_company(dbc *db) {
//
//    for (int i=1; i<=db->hdr.nr_cpy; i++) { rec_company(db, i); }
//    puts("");
//}
//
///****************************************************************************************
//* Display one Company record from the buffer
//****************************************************************************************/
//void rec_company(dbc *db, int id_cpy) {
//
//    printf("%4d %4d %4d %4d %95s %95s %10s %40s %20s %50s %11s %4d %.2f\n",
//           db->cpy[id_cpy].id_cpy,
//           db->cpy[id_cpy].id_grp,
//           db->cpy[id_cpy].id_cty,
//           db->cpy[id_cpy].id_ind,
//           db->cpy[id_cpy].nm_cpy,
//           db->cpy[id_cpy].nm_adr,
//           db->cpy[id_cpy].cd_pos,
//           db->cpy[id_cpy].nm_cit,
//           db->cpy[id_cpy].nr_tel,
//           db->cpy[id_cpy].nm_www,
//           db->cpy[id_cpy].dt_cre,
//           db->cpy[id_cpy].nr_emp,
//           db->cpy[id_cpy].am_val);
//}

/****************************************************************************************
 Search Company by ID
****************************************************************************************/
void search_company_by_name(dbc *db) {

    printf("*** search company by name ***\n");
    printf("enter beginning of company name: ...\n");

}


/****************************************************************************************
 Search Company by ID
****************************************************************************************/
void search_company_by_id(dbc *db) {

    int id, index;
    ccpy cpy;
    FILE *fp_db;

    fp_db = open_db_file(db);       // debug purpose, this must be handled globally: within open_menu()

    printf("\n\t--> Enter Company ID: "); scanf("%d", &id); fflush(stdin);

    index = search_binary(db, id, 0);

    cpy = read_single_company(db, index);

    ///test
    printf("FOUND %d %s\n", cpy.id_cpy, cpy.nm_cpy);

    fclose(fp_db);

}



/****************************************************************************************
* Read Company record given its position nb.
****************************************************************************************/
ccpy read_single_company(dbc *db, int position) {

    ccpy cpy;

    FILE *fp;
    fp = open_db_file(db);

    fseek(fp, db->hdr.off_cpy + position * sizeof(ccpy), SEEK_SET);
    fread(&cpy, sizeof(ccpy), 1, fp);

    fclose(fp);

    return cpy;
}