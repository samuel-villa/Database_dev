/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients V0
 *
 *       Jobs table functions
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"

#define BUF_LEN 200

/****************************************************************************************
* Loading file db_job.csv in the db
****************************************************************************************/
void import_CSV_job(dbc *db) {

    int i=0;
    char line[BUF_LEN];
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
    cjob job;
    FILE *fpi, *fp_db, *fp_lg;

    fp_db = fopen("data_db_comp/db_comp.dat", "rb+");
    fp_lg = fopen("data_db_comp/db_comp.log", "a");

    fpi = fopen("data_import/db_job.csv", "r");
    if (fpi == NULL) { printf("Error\n"); return; }

    printf("\nImporting jobs ...\n");

    fgets(line, 200, fpi);

    fseek(fp_db, db->hdr.off_job, SEEK_SET);

    //printf("%08X\n",db->hdr.off_job);

    while (fgets(line, 200, fpi) != NULL) {

        memset(&job, 0, sizeof(cjob));
        strcpy(job.tp_rec, "JOB");

        ptr1 = strtok(line,";");
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);                // use of strncpy cause allows to handle size
        job.id_job = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(job.nm_lev, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(job.nm_dep, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        strncpy(job.nm_job, ptr1, strlen(ptr1)-1);

        fwrite(&job, 1, sizeof(cjob), fp_db);      // write into file 'fp_db'

        i++;
    }

    fprintf(fp_lg, "Jobs imported: %d\n", i);

    fseek(fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, fp_db);        // load buffer (db) with 'fp_db'

    db->hdr.nr_job = i;

    fseek(fp_db, 0, SEEK_SET);
    fwrite(&db->hdr, sizeof(hder), 1, fp_db);       // Update Header

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpi);

    printf("\nJobs imported: %d\n\n", db->hdr.nr_job);

    return ;
}

/****************************************************************************************
* Export table Job to csv file
****************************************************************************************/
void export_CSV_job(dbc *db) {

    int i;
    cjob job;
    FILE *fpo, *fp_db, *fp_lg;

    fp_db = fopen("data_db_comp/db_comp.dat", "rb+");
    fp_lg = fopen("data_db_comp/db_comp.log", "a");

    printf("\nExporting jobs...\n");
    fpo = fopen("data_export/exp_job.csv", "w");

    fprintf(fpo,"Id;Nm_Lev;Nm_Dep;Nm_Job\n");

    fseek(fp_db, db->hdr.off_job, SEEK_SET);

    for (i=0; i<db->hdr.nr_job; i++) {

        memset(&job, 0, sizeof(cjob));
        fread(&job, 1, sizeof(cjob), fp_db);

        fprintf(fpo,"%d;%s;%s;%s\n",
                job.id_job,
                job.nm_lev,
                job.nm_dep,
                job.nm_job);
    }

    fprintf(fp_lg, "Jobs exported: %d\n", db->hdr.nr_job);

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpo);

    printf("\nJobs exported: %d\n\n", db->hdr.nr_job);

    return;
}

/****************************************************************************************
* Load Job table into the buffer
****************************************************************************************/
void load_job(dbc *db) {

    int i;
    cjob job;
    FILE *fp_db, *fp_lg;

    fp_db = fopen("data_db_comp/db_comp.dat", "rb+");
    fp_lg = fopen("data_db_comp/db_comp.log", "a");

    fseek(fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, fp_db);        // Read header

    printf("\nLoading jobs... %d\n", db->hdr.nr_job);

    fseek(fp_db, db->hdr.off_job, SEEK_SET);

    for (i=1; i<=db->hdr.nr_job; i++) {

        memset(&job, 0, sizeof(cjob));
        fread(&job, 1, sizeof(cjob), fp_db);

        db->job[i] = job;
    }

    fprintf(fp_lg, "Jobs loaded into buffer: %d\n", db->hdr.nr_job);

    fclose(fp_db);
    fclose(fp_lg);

    printf("\nJobs loaded into buffer: %d\n\n", db->hdr.nr_job);

    return;
}

/****************************************************************************************
* List and display Job table from the buffer
****************************************************************************************/
void print_job(dbc *db) {

    for (int i=1; i<=db->hdr.nr_job; i++)
        rec_job(db, i);
}

/****************************************************************************************
* Display one Job record from the buffer
****************************************************************************************/
void rec_job(dbc *db, int id_job) {

    printf("%4d %32s %32s %52s\n",
           db->job[id_job].id_job,
           db->job[id_job].nm_lev,
           db->job[id_job].nm_dep,
           db->job[id_job].nm_job);
}