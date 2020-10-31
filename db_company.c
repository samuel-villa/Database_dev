/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients V0
 *
 *       Company table functions
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"

#define BUF_LEN 200

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

    fp_db = fopen("data_db_comp/db_comp.dat", "rb+");
    fp_lg = fopen("data_db_comp/db_comp.log", "a");

    fpi = fopen("data_import/test_cpy.csv", "r");
    if (fpi == NULL) { printf("Error\n"); return; }

    printf("\nImporting companies...\n");

    fgets(line, 200, fpi);

    fseek(fp_db, db->hdr.off_cpy, SEEK_SET);

    printf("%08X\n",db->hdr.off_cpy);

    while (fgets(line, 200, fpi) != NULL) {

        memset(&cpy, 0, sizeof(ccpy));
        strcpy(cpy.tp_rec, "CPY");

        // TODO rearrange fields

        ptr1 = strtok(line,";");
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        cpy.id_cpy = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(cpy.nm_cpy, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, strlen(ptr1)-1);
        cpy.id_cty = atoi(fld);

        fwrite(&cpy, 1, sizeof(ccpy), fp_db);

        i++;
    }

    fprintf(fp_lg, "Companies imported: %d\n", i);

    fseek(fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, fp_db);

    db->hdr.nr_cpy = i;

    fseek(fp_db, 0, SEEK_SET);
    fwrite(&db->hdr, sizeof(hder), 1, fp_db);

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpi);

    printf("\nCompanies imported: %d\n\n", db->hdr.nr_cpy);

    return ;
}

/****************************************************************************************
* Export table Company to csv file
****************************************************************************************/
void export_CSV_company(dbc *db) {

    int i;
    ccpy cpy;
    FILE *fpo, *fp_db, *fp_lg;

    fp_db = fopen("data_db_comp/db_comp.dat", "rb+");
    fp_lg = fopen("data_db_comp/db_comp.log", "a");

    printf("\nExporting companies...\n");
    fpo = fopen("data_export/exp_company.csv", "w");

    fprintf(fpo,"id;id_grp;id_cty;id_ind;nm_cpy;nm_adr;cd_pos;nm_cit;nr_tel;nm_www;dt_cre;nr_emp;am_val\n");

    fseek(fp_db, db->hdr.off_cpy, SEEK_SET);

    for (i=0; i<db->hdr.nr_cpy; i++) {

        memset(&cpy, 0, sizeof(ccpy));
        fread(&cpy, 1, sizeof(ccpy), fp_db);

        // TODO rearrange fields
        fprintf(fpo,"%d;%s;%d\n",
                cpy.id_cpy,
                cpy.nm_cpy,
                cpy.id_cty);
    }

    fprintf(fp_lg, "Companies exported: %d\n", db->hdr.nr_cpy);

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpo);

    printf("\nCompanies exported: %d\n\n", db->hdr.nr_cpy);

    return;
}

/****************************************************************************************
* Load Company table into the buffer
****************************************************************************************/
void load_company(dbc *db) {

    int i;
    ccpy cpy;
    FILE *fp_db, *fp_lg;

    fp_db = fopen("data_db_comp/db_comp.dat", "rb+");
    fp_lg = fopen("data_db_comp/db_comp.log", "a");

    fseek(fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, fp_db);        // Read header

    printf("\nLoading companies... %d\n", db->hdr.nr_cpy);

    fseek(fp_db, db->hdr.off_cpy, SEEK_SET);

    for (i=1; i<=db->hdr.nr_cpy; i++) {

        memset(&cpy, 0, sizeof(ccpy));
        fread(&cpy, 1, sizeof(ccpy), fp_db);

        db->cpy[i] = cpy;
    }

    fprintf(fp_lg, "Companies loaded into buffer: %d\n", db->hdr.nr_cpy);

    fclose(fp_db);
    fclose(fp_lg);

    printf("\nCompanies loaded into buffer: %d\n\n", db->hdr.nr_cpy);

    return;
}

/****************************************************************************************
* List and display Company table from the buffer
****************************************************************************************/
void print_company(dbc *db) {

    for (int i=1; i<=db->hdr.nr_cpy; i++)
        rec_company(db, i);
}

/****************************************************************************************
* Display one Company record from the buffer
    TODO rearrange fields
****************************************************************************************/
void rec_company(dbc *db, int id_cpy) {

    printf("%4d %52s %4d\n",
           db->cpy[id_cpy].id_cpy,
           db->cpy[id_cpy].nm_cpy,
           db->cpy[id_cpy].id_cty);
}
