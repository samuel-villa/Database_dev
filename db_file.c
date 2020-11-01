/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients V0
 *
 *       DB file creation
 *
 *       TODO add timestamp for log records
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"

/****************************************************************************************
* DB creation based on SZ_* constants
****************************************************************************************/
void create_db(dbc *db) {

    int i;
    ccty cty;
    cjob job;
    cind ind;
    cgrp grp;
    ccpy cpy;
    cper per;
    FILE *fp_db, *fp_lg;

    memset(&db->hdr, 0, sizeof(hder));
    memset(&cty, 0, sizeof(ccty));

    fp_db = fopen("data_db_clients/db_clients.dat", "wb");
    fp_lg = fopen("data_db_clients/db_clients.log", "a");

    // header creation ----------------------------

    strcpy(db->hdr.db_name, "db_clients");

    db->hdr.sz_cty = SZ_CTY;
    db->hdr.sz_job = SZ_JOB;
    db->hdr.sz_ind = SZ_IND;
    db->hdr.sz_grp = SZ_GRP;
    db->hdr.sz_cpy = SZ_CPY;
    db->hdr.sz_per = SZ_PER;

    db->hdr.off_cty = sizeof(hder);
    db->hdr.off_job = db->hdr.off_cty + SZ_CTY * sizeof(ccty);
    db->hdr.off_ind = db->hdr.off_job + SZ_JOB * sizeof(cjob);
    db->hdr.off_grp = db->hdr.off_ind + SZ_IND * sizeof(cind);
    db->hdr.off_cpy = db->hdr.off_grp + SZ_GRP * sizeof(cgrp);
    db->hdr.off_per = db->hdr.off_cpy + SZ_CPY * sizeof(ccpy);
    db->hdr.db_size = db->hdr.off_per + SZ_PER * sizeof(cper);

    db->hdr.nr_cty = 0;
    db->hdr.nr_job = 0;
    db->hdr.nr_ind = 0;
    db->hdr.nr_grp = 0;
    db->hdr.nr_cpy = 0;
    db->hdr.nr_per = 0;

    fwrite(&db->hdr, 1, sizeof(db->hdr), fp_db);

    // Creation of country table ----------------------------

    memset(&cty, 0, sizeof(ccty));
    strcpy(cty.tp_rec, "CTY");

    for (i=0; i<SZ_CTY; i++)
        fwrite(&cty, 1, sizeof(ccty), fp_db);

    // Creation of job table ----------------------------

    memset(&job, 0, sizeof(cjob));
    strcpy(job.tp_rec, "JOB");

    for (i=0; i<SZ_JOB; i++)
        fwrite(&job, 1, sizeof(cjob), fp_db);

    // Creation of industry table ----------------------------

    memset(&ind, 0, sizeof(cind));
    strcpy(ind.tp_rec, "IND");

    for (i=0; i<SZ_IND; i++)
        fwrite(&ind, 1, sizeof(cind), fp_db);

    // Creation of group table ----------------------------

    memset(&grp, 0, sizeof(cgrp));
    strcpy(grp.tp_rec, "GRP");

    for (i=0; i<SZ_GRP; i++)
        fwrite(&grp, 1, sizeof(cgrp), fp_db);

    // Creation of company table ----------------------------

    memset(&cpy, 0, sizeof(ccpy));
    strcpy(cpy.tp_rec, "CPY");

    for (i=0; i<SZ_CPY; i++)
        fwrite(&cpy, 1, sizeof(ccpy), fp_db);

    // Creation of person table ----------------------------

    memset(&per, 0, sizeof(cper));
    strcpy(per.tp_rec, "PER");

    for (i=0; i<SZ_PER; i++)
        fwrite(&per, 1, sizeof(cper), fp_db);

    fprintf(fp_lg, "Database %s created\n", db->hdr.db_name);

    fclose(fp_db);
    fclose(fp_lg);

    printf("Database %s created\n", db->hdr.db_name);
}



/****************************************************************************************
* Load header if DB file exists
****************************************************************************************/
void load_header(dbc *db) {

    FILE *fp_db;

    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");

    fseek(fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, fp_db);

    fclose(fp_db);
}


/****************************************************************************************
* Check if DB file is created
****************************************************************************************/
void set_db_status(dbc *db) {

    FILE *fp_db;

    if ((fp_db = fopen("data_db_clients/db_clients.dat", "rb")) == NULL) {
        db->status = DB_NOT_CREATED;
    } else {
        load_header(db);
        db->status = DB_CLOSED;
    }
    fclose(fp_db);
}


/****************************************************************************************
* Display System Info: stocked in Header
****************************************************************************************/
void display_system_info(dbc *db) {

    printf("\n====================================\n\n");

    printf("\t* DB name: %s\n", db->hdr.db_name);
    printf("\t* DB size: %d\n", db->hdr.db_size);
    printf("\t* Countries bloc size: %d\n", db->hdr.sz_cty);
    printf("\t* Jobs bloc size: %d\n", db->hdr.sz_job);
    printf("\t* Industries bloc size: %d\n", db->hdr.sz_ind);
    printf("\t* Groups bloc size: %d\n", db->hdr.sz_grp);
    printf("\t* Companies bloc size: %d\n", db->hdr.sz_cpy);
    printf("\t* Persons bloc size: %d\n", db->hdr.sz_per);
    puts("");
    /// debug purpose
    printf("\t* Countries bloc position: %08X\n", db->hdr.off_cty);
    printf("\t* Jobs bloc position: %08X\n", db->hdr.off_job);
    printf("\t* Industries bloc position: %08X\n", db->hdr.off_ind);
    printf("\t* Groups bloc position: %08X\n", db->hdr.off_grp);
    printf("\t* Companies bloc position: %08X\n", db->hdr.off_cpy);
    printf("\t* Persons bloc position: %08X\n", db->hdr.off_per);
    puts("");
    printf("\t* Nb of Countries: %d\n", db->hdr.off_cty);
    printf("\t* Nb of Jobs: %d\n", db->hdr.off_job);
    printf("\t* Nb of Industries: %d\n", db->hdr.off_ind);
    printf("\t* Nb of Groups: %d\n", db->hdr.off_grp);
    printf("\t* Nb of Companies: %d\n", db->hdr.off_cpy);
    printf("\t* Nb of Persons: %d\n", db->hdr.off_per);

    printf("\n\n====================================\n");
}