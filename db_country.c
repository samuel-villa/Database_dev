/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients V0
 *
 *       Countries table functions
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"

#define BUF_LEN 200

/****************************************************************************************
* Loading file db_country.csv in the db
****************************************************************************************/
void import_CSV_country(dbc *db) {

    int i=0;
    char line[BUF_LEN];
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
    ccty cty;
    FILE *fpi, *fp_db, *fp_lg;

    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    fp_lg = fopen("data_db_clients/db_clients.log", "a");

    fpi = fopen("data_import/db_country.csv", "r");
    if (fpi == NULL) { printf("Error\n"); return; }

    printf("\n\tImporting countries... ");

    fgets(line, 200, fpi);

    fseek(fp_db, db->hdr.off_cty, SEEK_SET);

    while (fgets(line, 200, fpi) != NULL) {

        memset(&cty, 0, sizeof(ccty));
        strcpy(cty.tp_rec, "CTY");

        ptr1 = strtok(line,";");
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);                // use of strncpy cause allows to handle size
        cty.id_cty = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cty.nm_cty, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cty.nm_zon, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        strncpy(cty.cd_iso, ptr1, strlen(ptr1)-1);

        fwrite(&cty, 1, sizeof(ccty), fp_db);      // write into file 'fp_db'

        i++;
    }

    fprintf(fp_lg, "%s Countries imported: %d\n", timestamp(), i);

    fseek(fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, fp_db);        // load buffer (db) with 'fp_db'

    db->hdr.nr_cty = i;

    fseek(fp_db, 0, SEEK_SET);
    fwrite(&db->hdr, sizeof(hder), 1, fp_db);       // Update Header

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpi);

    printf("DONE => Countries imported: %d", db->hdr.nr_cty);

    return ;
}

/****************************************************************************************
* Export table Country to csv file
****************************************************************************************/
void export_CSV_country(dbc *db) {

    int i;
    ccty cty;
    FILE *fpo, *fp_db, *fp_lg;

    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    fp_lg = fopen("data_db_clients/db_clients.log", "a");

    printf("\n\tExporting countries... ");
    fpo = fopen("data_export/exp_country.csv", "w");

    fprintf(fpo,"Id;Nm_Cty;Nm_Zon;Cd_Iso\n");

    fseek(fp_db, db->hdr.off_cty, SEEK_SET);

    for (i=0; i<db->hdr.nr_cty; i++) {

        memset(&cty, 0, sizeof(ccty));
        fread(&cty, 1, sizeof(ccty), fp_db);

        fprintf(fpo,"%d;%s;%s;%s\n",
                cty.id_cty,
                cty.nm_cty,
                cty.nm_zon,
                cty.cd_iso);
    }
    fprintf(fp_lg, "%s Countries exported: %d\n", timestamp(), db->hdr.nr_cty);

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpo);

    printf("DONE => Countries exported: %d", db->hdr.nr_cty);

    return;
}

/****************************************************************************************
* Load Country table into the buffer
****************************************************************************************/
void load_country(dbc *db) {

    int i;
    ccty cty;
    FILE *fp_db, *fp_lg;

    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    fp_lg = fopen("data_db_clients/db_clients.log", "a");

    fseek(fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, fp_db);        // Read header

    printf("\nLoading countries into buffer... ");

    fseek(fp_db, db->hdr.off_cty, SEEK_SET);

    for (i=1; i<=db->hdr.nr_cty; i++) {

        memset(&cty, 0, sizeof(ccty));
        fread(&cty, 1, sizeof(ccty), fp_db);

        db->cty[i] = cty;
    }

    fprintf(fp_lg, "%s Countries loaded into buffer: %d\n", timestamp(), db->hdr.nr_cty);

    fclose(fp_db);
    fclose(fp_lg);

    printf("DONE => Countries loaded: %d\n\n", db->hdr.nr_cty);

    return;
}

/****************************************************************************************
* List and display Country table from the buffer
****************************************************************************************/
void print_country(dbc *db) {

    for (int i=1; i<=db->hdr.nr_cty; i++) {
        rec_country(db, i);
    }
    puts("");
}

/****************************************************************************************
* Display one Country record from the buffer
****************************************************************************************/
void rec_country(dbc *db, int id_cty) {

    printf("%4d %20s %18s %4s\n",
           db->cty[id_cty].id_cty,
           db->cty[id_cty].nm_zon,
           db->cty[id_cty].nm_cty,
           db->cty[id_cty].cd_iso);
}
