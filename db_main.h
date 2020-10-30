/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients V0
 *
 *       Header file: definitions, structures and prototypes
 *
 *       Features:
 *          -
 *
 *       Remarques:
 *          -
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define SZ_CTY 100
#define SZ_JOB 200
#define SZ_IND 100
#define SZ_GRP 3000

typedef unsigned int uint;


/***************************************************************************************
* DB Header
****************************************************************************************/
typedef struct Header {

    uint db_size;       // db size
    char db_name[28];   // db name
    uint sz_cty;        // country bloc size
    uint sz_job;        // job bloc size
    uint sz_ind;        // industry bloc size
    uint sz_grp;        // group bloc size
    uint off_cty;       // country bloc position
    uint off_job;       // job bloc position
    uint off_ind;       // industry bloc position
    uint off_grp;       // group bloc position
    uint nr_cty;        // Nr of countries
    uint nr_job;        // Nr of jobs
    uint nr_ind;        // Nr of industries
    uint nr_grp;        // Nr of groups
    char filler[16];
} hder;


/***************************************************************************************
* Country table
****************************************************************************************/
typedef struct Country {

    char tp_rec[8];     // primary key CTY
    int  id_cty;        // primary key
    char nm_zon[20];    // geographic zone name
    char nm_cty[28];    // country name
    char cd_iso[4];     // country ISO code
} ccty;


/***************************************************************************************
* Job table
****************************************************************************************/
typedef struct Job {

    char tp_rec[8];     // primary key JOB
    int  id_job;        // primary key
    char nm_lev[32];
    char nm_dep[32];
    char nm_job[52];
} cjob;


/***************************************************************************************
* Industry table
****************************************************************************************/
typedef struct Industry {

    char tp_rec[8];     // rec type: IND
    int  id_ind;        // primary key
    char nm_sec[20];
    char nm_ind[32];
} cind;


/***************************************************************************************
* Group table
****************************************************************************************/
typedef struct Group {

    char tp_rec[8];     // rec type: GRP
    int  id_grp;        // primary key
    char nm_grp[52];
    int id_cty;
} cgrp;


/***************************************************************************************
* DB structure
****************************************************************************************/
typedef struct db_client {

    hder hdr;           // header
    ccty cty[SZ_CTY];   // buffer country
    cjob job[SZ_JOB];   // buffer job
    cind ind[SZ_IND];   // buffer industry
    cgrp grp[SZ_GRP];   // buffer group
} dbc;



/****************************************************************************************
* Prototypes
****************************************************************************************/
void create_db(dbc *db);

/// Country ///
void import_CSV_country(dbc *db);
void export_CSV_country(dbc *db);
void load_country(dbc *db);
void print_country(dbc *db);
void rec_country(dbc *db, int id_cty);

/// Industry ///
void import_CSV_industry(dbc *db);
void export_CSV_industry(dbc *db);
void load_industry(dbc *db);
void print_industry(dbc *db);
void rec_industry(dbc *db, int id_ind);

/// Job ///
void import_CSV_job(dbc *db);
void export_CSV_job(dbc *db);
void load_job(dbc *db);
void print_job(dbc *db);
void rec_job(dbc *db, int id_job);

/// Group ///
void import_CSV_group(dbc *db);
void export_CSV_group(dbc *db);
void load_group(dbc *db);
void print_group(dbc *db);
void rec_group(dbc *db, int id_grp);