/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients V0
 *
 *       Groups table functions
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"

#define BUF_LEN 200

/****************************************************************************************
* Loading file db_group.csv in the db
****************************************************************************************/
void import_CSV_group(dbc *db) {

    int i=0;
    char line[BUF_LEN];
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
    cgrp grp;
    FILE *fpi, *fp_db, *fp_lg;

    fp_db = fopen("data_db_comp/db_comp.dat", "rb+");
    fp_lg = fopen("data_db_comp/db_comp.log", "a");

    fpi = fopen("data_import/db_group.csv", "r");
    if (fpi == NULL) { printf("Error\n"); return; }

    printf("\nImporting groups...\n");

    fgets(line, 200, fpi);

    fseek(fp_db, db->hdr.off_grp, SEEK_SET);

    //printf("%08X\n",db->hdr.off_grp);

    while (fgets(line, 200, fpi) != NULL) {

        memset(&grp, 0, sizeof(cgrp));
        strcpy(grp.tp_rec, "GRP");

        ptr1 = strtok(line,";");
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        grp.id_grp = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(grp.nm_grp, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, strlen(ptr1)-1);
        grp.id_cty = atoi(fld);

        fwrite(&grp, 1, sizeof(cgrp), fp_db);      // write into file 'fp_db'

        i++;
    }

    fprintf(fp_lg, "Groups imported: %d\n", i);

    fseek(fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, fp_db);        // load buffer (db) with 'fp_db'

    db->hdr.nr_grp = i;

    fseek(fp_db, 0, SEEK_SET);
    fwrite(&db->hdr, sizeof(hder), 1, fp_db);       // Update Header

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpi);

    printf("\nGroups imported: %d\n\n", db->hdr.nr_grp);

    return ;
}

/****************************************************************************************
* Export table Group to csv file
****************************************************************************************/
void export_CSV_group(dbc *db) {

    int i;
    cgrp grp;
    FILE *fpo, *fp_db, *fp_lg;

    fp_db = fopen("data_db_comp/db_comp.dat", "rb+");
    fp_lg = fopen("data_db_comp/db_comp.log", "a");

    printf("\nExporting groups...\n");
    fpo = fopen("data_export/exp_group.csv", "w");

    fprintf(fpo,"Id;Nm_Grp;Id_Cty\n");

    fseek(fp_db, db->hdr.off_grp, SEEK_SET);

    for (i=0; i<db->hdr.nr_grp; i++) {

        memset(&grp, 0, sizeof(cgrp));
        fread(&grp, 1, sizeof(cgrp), fp_db);

        fprintf(fpo,"%d;%s;%d\n",
                grp.id_grp,
                grp.nm_grp,
                grp.id_cty);
    }

    fprintf(fp_lg, "Groups exported: %d\n", db->hdr.nr_grp);

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpo);

    printf("\nGroups exported: %d\n\n", db->hdr.nr_grp);

    return;
}

/****************************************************************************************
* Load Group table into the buffer
****************************************************************************************/
void load_group(dbc *db) {

    int i;
    cgrp grp;
    FILE *fp_db, *fp_lg;

    fp_db = fopen("data_db_comp/db_comp.dat", "rb+");
    fp_lg = fopen("data_db_comp/db_comp.log", "a");

    fseek(fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, fp_db);        // Read header

    printf("\nLoading groups... %d\n", db->hdr.nr_grp);

    fseek(fp_db, db->hdr.off_grp, SEEK_SET);

    for (i=1; i<=db->hdr.nr_grp; i++) {

        memset(&grp, 0, sizeof(cgrp));
        fread(&grp, 1, sizeof(cgrp), fp_db);

        db->grp[i] = grp;
    }

    fprintf(fp_lg, "Groups loaded into buffer: %d\n", db->hdr.nr_grp);

    fclose(fp_db);
    fclose(fp_lg);

    printf("\nGroups loaded into buffer: %d\n\n", db->hdr.nr_grp);

    return;
}

/****************************************************************************************
* List and display Group table from the buffer
****************************************************************************************/
void print_group(dbc *db) {

    for (int i=1; i<=db->hdr.nr_grp; i++)
        rec_group(db, i);
}

/****************************************************************************************
* Display one Group record from the buffer
****************************************************************************************/
void rec_group(dbc *db, int id_grp) {

    printf("%4d %52s %4d\n",
           db->grp[id_grp].id_grp,
           db->grp[id_grp].nm_grp,
           db->grp[id_grp].id_cty);
}