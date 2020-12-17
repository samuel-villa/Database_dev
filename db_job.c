/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients
 *
 *       Jobs table functions
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"



/****************************************************************************************
* Import file db_job.csv in the db
****************************************************************************************/
void import_CSV_job(dbc *db) {

    int i=0;
    char line[BUF_LEN], fld[BUF_LEN];
    char *ptr1, *ptr2;
    cjob job;
    FILE *fpi, *fp_db, *fp_lg;

    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    fp_lg = fopen("data_db_clients/db_clients.log", "a");

    fpi = fopen("data_import/db_job.csv", "r");
    if (fpi == NULL) { printf("Error\n"); return; }

    printf("\n\tImporting jobs... ");

    fgets(line, 200, fpi);

    fseek(fp_db, db->hdr.off_job, SEEK_SET);

    while (fgets(line, 200, fpi) != NULL) {

        memset(&job, 0, sizeof(cjob));
        strcpy(job.tp_rec, "JOB");

        ptr1 = strtok(line,";");
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);                       // use of strncpy cause allows to handle size
        job.id_job = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(job.nm_lev, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(job.nm_dep, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        strncpy(job.nm_job, ptr1, strlen(ptr1)-1);

        fwrite(&job, 1, sizeof(cjob), fp_db);             // write into file 'fp_db'

        i++;
    }

    fprintf(fp_lg, "%s Jobs imported: %d\n", timestamp(), i);

    fseek(fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, fp_db);            // load buffer (db) with 'fp_db'

    db->hdr.nr_job = i;

    fseek(fp_db, 0, SEEK_SET);
    fwrite(&db->hdr, sizeof(hder), 1, fp_db);           // Update Header

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpi);

    printf("DONE => Jobs imported: %d", db->hdr.nr_job);

    return ;
}

/****************************************************************************************
* Export table Job to csv file
****************************************************************************************/
void export_CSV_job(dbc *db) {

    int i;
    cjob job;
    FILE *fpo, *fp_db, *fp_lg;

    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    fp_lg = fopen("data_db_clients/db_clients.log", "a");

    printf("\n\tExporting jobs... ");
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

    fprintf(fp_lg, "%s Jobs exported: %d\n", timestamp(), db->hdr.nr_job);

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpo);

    printf("DONE => Jobs exported: %d", db->hdr.nr_job);

    return;
}

/****************************************************************************************
* Load Job table into the buffer
****************************************************************************************/
void load_job(dbc *db) {

    int i;
    cjob job;

    fseek(db->fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, db->fp_db);

    printf("\n\tLoading jobs into buffer... ");

    fseek(db->fp_db, db->hdr.off_job, SEEK_SET);

    for (i=1; i<=db->hdr.nr_job; i++) {

        memset(&job, 0, sizeof(cjob));
        fread(&job, 1, sizeof(cjob), db->fp_db);

        db->job[i] = job;
    }

    fprintf(db->fp_lg, "%s Jobs loaded into buffer: %d\n", timestamp(), db->hdr.nr_job);

    printf("DONE => Jobs loaded: %d", db->hdr.nr_job);
}

/****************************************************************************************
* List and display Job table from the buffer
****************************************************************************************/
void print_job(dbc *db) {

    for (int i=1; i<=db->hdr.nr_job; i++) { rec_job(db, i); }
    puts("");
}

/****************************************************************************************
 * Display one Job record from the buffer
 *
 *      id_job: job id we want to display
****************************************************************************************/
void rec_job(dbc *db, int id_job) {

    printf("%4d %24s %24s %40s\n",
           db->job[id_job].id_job,
           db->job[id_job].nm_lev,
           db->job[id_job].nm_dep,
           db->job[id_job].nm_job);
}
