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

    fp_db = fopen("data_db_comp/db_comp.dat", "rb+");
    fp_lg = fopen("data_db_comp/db_comp.log", "a");

    fpi = fopen("data_import/test_person.csv", "r");
    if (fpi == NULL) { printf("Error\n"); return; }

    printf("\nImporting persons...\n");

    fgets(line, 200, fpi);

    fseek(fp_db, db->hdr.off_per, SEEK_SET);

    printf("%08X\n",db->hdr.off_per);

    while (fgets(line, 200, fpi) != NULL) {

        memset(&per, 0, sizeof(cper));
        strcpy(per.tp_rec, "per");

        // TODO rearrange fields

        ptr1 = strtok(line,";");
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        per.id_per = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(per.nm_civ, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, strlen(ptr1)-1);
        per.id_cpy = atoi(fld);

        fwrite(&per, 1, sizeof(cper), fp_db);

        i++;
    }

    fprintf(fp_lg, "Persons imported: %d\n", i);

    fseek(fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, fp_db);

    db->hdr.nr_per = i;

    fseek(fp_db, 0, SEEK_SET);
    fwrite(&db->hdr, sizeof(hder), 1, fp_db);

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpi);

    printf("\nPersons imported: %d\n\n", db->hdr.nr_per);

    return ;
}

/****************************************************************************************
* Export table Person to csv file
****************************************************************************************/
void export_CSV_person(dbc *db) {

    int i;
    cper per;
    FILE *fpo, *fp_db, *fp_lg;

    fp_db = fopen("data_db_comp/db_comp.dat", "rb+");
    fp_lg = fopen("data_db_comp/db_comp.log", "a");

    printf("\nExporting persons...\n");
    fpo = fopen("data_export/exp_person.csv", "w");

    fprintf(fpo,"id;id_grp;id_cty;id_ind;nm_per;nm_adr;cd_pos;nm_cit;nr_tel;nm_www;dt_cre;nr_emp;am_val\n");

    fseek(fp_db, db->hdr.off_per, SEEK_SET);

    for (i=0; i<db->hdr.nr_per; i++) {

        memset(&per, 0, sizeof(cper));
        fread(&per, 1, sizeof(cper), fp_db);

        // TODO rearrange fields
        fprintf(fpo,"%d;%s;%d\n",
                per.id_per,
                per.nm_civ,
                per.id_cpy);
    }

    fprintf(fp_lg, "Persons exported: %d\n", db->hdr.nr_per);

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpo);

    printf("\nPersons exported: %d\n\n", db->hdr.nr_per);

    return;
}

/****************************************************************************************
* Load Person table into the buffer
****************************************************************************************/
void load_person(dbc *db) {

    int i;
    cper per;
    FILE *fp_db, *fp_lg;

    fp_db = fopen("data_db_comp/db_comp.dat", "rb+");
    fp_lg = fopen("data_db_comp/db_comp.log", "a");

    fseek(fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, fp_db);        // Read header

    printf("\nLoading persons... %d\n", db->hdr.nr_per);

    fseek(fp_db, db->hdr.off_per, SEEK_SET);

    for (i=1; i<=db->hdr.nr_per; i++) {

        memset(&per, 0, sizeof(cper));
        fread(&per, 1, sizeof(cper), fp_db);

        db->per[i] = per;
    }

    fprintf(fp_lg, "Persons loaded into buffer: %d\n", db->hdr.nr_per);

    fclose(fp_db);
    fclose(fp_lg);

    printf("\nPersons loaded into buffer: %d\n\n", db->hdr.nr_per);

    return;
}

/****************************************************************************************
* List and display Person table from the buffer
****************************************************************************************/
void print_person(dbc *db) {

    for (int i=1; i<=db->hdr.nr_per; i++)
        rec_person(db, i);
}

/****************************************************************************************
* Display one Person record from the buffer
    TODO rearrange fields
****************************************************************************************/
void rec_person(dbc *db, int id_per) {

    printf("%4d %52s %4d\n",
           db->per[id_per].id_per,
           db->per[id_per].nm_civ,
           db->per[id_per].id_cpy);
}
