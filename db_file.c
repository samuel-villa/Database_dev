/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients
 *
 *       handle DB file (.dat and .log)
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"



/****************************************************************************************
 * DB creation based on SZ_* constants
 * set up the empty db for all tables and the header
****************************************************************************************/
void create_db(dbc *db) {

    int i;
    ccty cty;
    cjob job;
    cind ind;
    cgrp grp;
    ccpy cpy;
    cper per;
    tipc pc;
    tipl pl;
    FILE *fp_db, *fp_lg;

    memset(&db->hdr, 0, sizeof(hder));

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
    db->hdr.sz_ipc = SZ_IPC;
    db->hdr.sz_ipl = SZ_IPL;

    db->hdr.off_cty = sizeof(hder);
    db->hdr.off_job = db->hdr.off_cty + SZ_CTY * sizeof(ccty);
    db->hdr.off_ind = db->hdr.off_job + SZ_JOB * sizeof(cjob);
    db->hdr.off_grp = db->hdr.off_ind + SZ_IND * sizeof(cind);
    db->hdr.off_cpy = db->hdr.off_grp + SZ_GRP * sizeof(cgrp);
    db->hdr.off_per = db->hdr.off_cpy + SZ_CPY * sizeof(ccpy);
    db->hdr.off_ipc = db->hdr.off_per + SZ_PER * sizeof(cper);
    db->hdr.off_ipl = db->hdr.off_ipc + SZ_IPC * sizeof(tipc);
    db->hdr.db_size = db->hdr.off_ipl + SZ_IPL * sizeof(tipl);

    db->hdr.nr_cty = 0;
    db->hdr.nr_job = 0;
    db->hdr.nr_ind = 0;
    db->hdr.nr_grp = 0;
    db->hdr.nr_cpy = 0;
    db->hdr.nr_per = 0;
    db->hdr.nr_ipc = 0;
    db->hdr.nr_ipl = 0;

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

    // Creation of person/company index table ----------------------------

    memset(&pc, 0, sizeof(tipc));
    strcpy(pc.tp_rec, "IPC");

    for (i=0; i<SZ_IPC; i++)
        fwrite(&pc, 1, sizeof(tipc), fp_db);

    // Creation of person lastname index table ----------------------------

    memset(&pl, 0, sizeof(tipl));
    strcpy(pl.tp_rec, "IPL");

    for (i=0; i<SZ_IPL; i++)
        fwrite(&pl, 1, sizeof(tipl), fp_db);

    fprintf(fp_lg, "%s Database %s created\n", timestamp(), db->hdr.db_name);

    fclose(fp_db);
    fclose(fp_lg);

    printf("\n\tDatabase '%s' created\n", db->hdr.db_name);
}


/****************************************************************************************
* Opens DB .dat and .log files globally
****************************************************************************************/
void open_db_files(dbc *db) {

    FILE *fp_db;

    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");

    // with rb+ opening mode the file must be already created
    // so I open it in wb mode (file is created)
    // then close it and open it back again in rb+ mode
    if (fp_db == NULL) {

        fp_db = fopen("data_db_clients/db_clients.dat", "wb");
        fclose(fp_db);
        fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    }

    if (fp_db) {

        fread(&db->hdr, 1, sizeof(hder), fp_db);        // load header

        db->fp_db = fp_db;
        db->fp_lg = fopen("data_db_clients/db_clients.log", "a");
        strcpy(db->hdr.db_name, "db_clients");
        fprintf(db->fp_lg, "%s Database '%s' open\n", timestamp(), db->hdr.db_name);

    } else {
        printf("Error while opening database\n");
    }

    if (db->hdr.nr_cty == 0) {                               // check if data has been already imported
        db->status = DB_OPEN_EMPTY;
    } else {
        db->status = DB_OPEN_LOADED;
    }
}


/****************************************************************************************
* Closes DB files globally
****************************************************************************************/
void close_db_files(dbc *db) {

    fclose(db->fp_db);              // close db file
    fclose(db->fp_lg);              // close log file
    fclose(db->fp_rp);              // close report file
}


/****************************************************************************************
* Load DB header metadata in RAM (if DB file exists)
****************************************************************************************/
void load_header(dbc *db) {

    fseek(db->fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, db->fp_db);

    fprintf(db->fp_lg, "%s Database header updated\n", timestamp());
}


/****************************************************************************************
* Check if DB file is created or not and set appropriate status
****************************************************************************************/
void set_db_status(dbc *db) {

    FILE *fp_db;

    // check if file exists with 'rb' mode (if file doesn't exist returns NULL)
    if ((fp_db = fopen("data_db_clients/db_clients.dat", "rb")) == NULL) {
        db->status = DB_NOT_CREATED;
    } else {
        fclose(fp_db);
        db->status = DB_CLOSED;
    }
}


/****************************************************************************************
* Display System Info stocked in the DB Header
****************************************************************************************/
void display_system_info(dbc *db) {

    printf("\n\t===============================================\n\n");

    printf("\t* DB name                        : %s\n", db->hdr.db_name);
    printf("\t* DB size (bytes)                : %-8d\n", db->hdr.db_size);
    printf("\t* Countries bloc size            : %-8d\n", db->hdr.sz_cty);
    printf("\t* Jobs bloc size                 : %-8d\n", db->hdr.sz_job);
    printf("\t* Industries bloc size           : %-8d\n", db->hdr.sz_ind);
    printf("\t* Groups bloc size               : %-8d\n", db->hdr.sz_grp);
    printf("\t* Companies bloc size            : %-8d\n", db->hdr.sz_cpy);
    printf("\t* Persons bloc size              : %-8d\n", db->hdr.sz_per);
    printf("\t* Persons/Companies bloc size    : %-8d\n", db->hdr.sz_ipc);
    printf("\t* Persons lastname bloc size     : %-8d\n", db->hdr.sz_ipl);
    puts("");
    printf("\t* Nb of Countries                : %-8d\n", db->hdr.nr_cty);
    printf("\t* Nb of Jobs                     : %-8d\n", db->hdr.nr_job);
    printf("\t* Nb of Industries               : %-8d\n", db->hdr.nr_ind);
    printf("\t* Nb of Groups                   : %-8d\n", db->hdr.nr_grp);
    printf("\t* Nb of Companies                : %-8d\n", db->hdr.nr_cpy);
    printf("\t* Nb of Persons                  : %-8d\n", db->hdr.nr_per);

    printf("\n\n\t===============================================\n\n");
}