/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients
 *
 *       Industries table functions
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"



/****************************************************************************************
* Import file db_industry.csv in the db
****************************************************************************************/
void import_CSV_industry(dbc *db) {

    int i=0;
    char line[BUF_LEN], fld[BUF_LEN];
    char *ptr1, *ptr2;
    cind ind;
    FILE *fpi, *fp_db, *fp_lg;

    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    fp_lg = fopen("data_db_clients/db_clients.log", "a");

    fpi = fopen("data_import/db_industry.csv", "r");
    if (fpi == NULL) { printf("Error\n"); return; }

    printf("\n\tImporting industries... ");

    fgets(line, 200, fpi);

    fseek(fp_db, db->hdr.off_ind, SEEK_SET);

    while (fgets(line, 200, fpi) != NULL) {

        memset(&ind, 0, sizeof(cind));
        strcpy(ind.tp_rec, "IND");

        ptr1 = strtok(line,";");
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        ind.id_ind = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(ind.nm_sec, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        strncpy(ind.nm_ind, ptr1, strlen(ptr1)-1);

        fwrite(&ind, 1, sizeof(cind), fp_db);

        i++;
    }

    fprintf(fp_lg, "%s Industries imported: %d\n", timestamp(), i);

    fseek(fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, fp_db);        // load buffer (db) with 'fp_db'

    db->hdr.nr_ind = i;

    fseek(fp_db, 0, SEEK_SET);
    fwrite(&db->hdr, sizeof(hder), 1, fp_db);       // Update Header

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpi);

    printf("DONE => Industries imported: %d", db->hdr.nr_ind);

    return ;
}

/****************************************************************************************
 * Export table Industry to csv file
****************************************************************************************/
void export_CSV_industry(dbc *db) {

    int i;
    cind ind;
    FILE *fpo, *fp_db, *fp_lg;

    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    fp_lg = fopen("data_db_clients/db_clients.log", "a");

    printf("\n\tExporting industries... ");
    fpo = fopen("data_export/exp_industry.csv", "w");

    fprintf(fpo,"Id;Nm_Sec;Nm_Ind\n");

    fseek(fp_db, db->hdr.off_ind, SEEK_SET);

    for (i=0; i<db->hdr.nr_ind; i++) {

        memset(&ind, 0, sizeof(cind));
        fread(&ind, 1, sizeof(cind), fp_db);

        fprintf(fpo,"%d;%s;%s\n",
                ind.id_ind,
                ind.nm_sec,
                ind.nm_ind);
    }

    fprintf(fp_lg, "%s Industries exported: %d\n", timestamp(), db->hdr.nr_ind);

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpo);

    printf("DONE => Industries exported: %d", db->hdr.nr_ind);

    return;
}

/****************************************************************************************
* Load Industry table into the buffer
****************************************************************************************/
void load_industry(dbc *db) {

    int i;
    cind ind;

    fseek(db->fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, db->fp_db);

    printf("\n\tLoading industries into buffer... ");

    fseek(db->fp_db, db->hdr.off_ind, SEEK_SET);

    for (i=1; i<=db->hdr.nr_ind; i++) {

        memset(&ind, 0, sizeof(cind));
        fread(&ind, 1, sizeof(cind), db->fp_db);

        db->ind[i] = ind;
    }

    fprintf(db->fp_lg, "%s Industries loaded into buffer: %d\n", timestamp(), db->hdr.nr_ind);

    printf("DONE => Industries loaded: %d", db->hdr.nr_ind);
}

/****************************************************************************************
* List and display Industry table from the buffer
****************************************************************************************/
void print_industry(dbc *db) {

    for (int i=1; i<=db->hdr.nr_ind; i++) {
        rec_industry(db, i);
    }
    puts("");
}

/****************************************************************************************
 * Display one Industry record from the buffer
 *
 *      id_ind: industry id we want to display
****************************************************************************************/
void rec_industry(dbc *db, int id_ind) {

    printf("%4d %20s %32s\n",
           db->ind[id_ind].id_ind,
           db->ind[id_ind].nm_sec,
           db->ind[id_ind].nm_ind);
}
