/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients V0
 *
 *       DB file creation
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
    FILE *fp_db, *fp_lg;

    memset(&db->hdr, 0, sizeof(hder));
    memset(&cty, 0, sizeof(ccty));

    fp_db = fopen("data_db_comp/db_comp.dat", "wb");
    fp_lg = fopen("data_db_comp/db_comp.log", "a");

    // header creation ----------------------------

    strcpy(db->hdr.db_name, "db_comp");

    db->hdr.sz_cty = SZ_CTY;
    db->hdr.sz_job = SZ_JOB;
    db->hdr.sz_ind = SZ_IND;
    db->hdr.sz_grp = SZ_GRP;

    db->hdr.off_cty = sizeof(hder);
    db->hdr.off_job = db->hdr.off_cty + SZ_CTY * sizeof(ccty);
    db->hdr.off_ind = db->hdr.off_job + SZ_JOB * sizeof(cjob);
    db->hdr.off_grp = db->hdr.off_ind + SZ_IND * sizeof(cind);
    db->hdr.db_size = db->hdr.off_grp + SZ_GRP * sizeof(cgrp);

    db->hdr.nr_cty = 0;
    db->hdr.nr_job = 0;
    db->hdr.nr_ind = 0;
    db->hdr.nr_grp = 0;

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

    fprintf(fp_lg, "Database %s created\n", db->hdr.db_name);

    fclose(fp_db);
    fclose(fp_lg);

    printf("Databse %s created\n", db->hdr.db_name);
}