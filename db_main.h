/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients V0
 *
 *       Requirements:
 *          • La création de la base de données
 *          • L’importation des 6 tables fournies sous forme de fichier .csv
 *          • L’exportation de ces 6 tables sous forme de fichier .csv
 *          • La création des index nécessaires aux requêtes précisées
 *          • Des écrans permettant de lister l’information par recherche séquentielle pour les tables de codes
 *          • Des écrans permettant d’afficher par recherche dichotomique
 *              • Une personne donnée par sa clé primaire
 *              • Une compagnie donnée par sa clé primaire
 *          • Au moins un écran permettant d’afficher via une liste chainée en mémoire
 *              • Les personnes travaillant pour une compagnie donnée par sa clé primaire
 *              • Les compagnies appartenant à un groupe donné
 *          • Au moins un écran utilisant un index binaire sur disque dans la recherche
 *              • Les personnes travaillant pour une compagnie donnée par sa clé primaire
 *              • Les personnes dont le nom commence par une chaine donnée
 *          • Les listes chainées pourront être triées ascendant ou descendant
 *          • Deux rapports répondants aux requêtes précisées ci-dessous
 *
 *       Header file:
 *          - definitions
 *          - structures
 *          - prototypes
 *
 *       Features:
 *          - Database creation
 *          - Import data from .csv file
 *          - Export data to .csv file
 *
 *       Notes:
 *          -
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SZ_CTY 100
#define SZ_JOB 200
#define SZ_IND 100
#define SZ_GRP 3000
//#define SZ_CPY 100000
//#define SZ_PER 500000
#define SZ_CPY 200
#define SZ_PER 200

enum Main_Menu {
    CREATE_DB,
    OPEN_DB,
    SYSTEM_INFO,
    EXIT = 9
};

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
    uint sz_cpy;        // company bloc size
    uint sz_per;        // person bloc size
    uint off_cty;       // country bloc position
    uint off_job;       // job bloc position
    uint off_ind;       // industry bloc position
    uint off_grp;       // group bloc position
    uint off_cpy;       // company bloc position
    uint off_per;       // person bloc position
    uint nr_cty;        // nr of countries
    uint nr_job;        // nr of jobs
    uint nr_ind;        // nr of industries
    uint nr_grp;        // nr of groups
    uint nr_cpy;        // nr of companies
    uint nr_per;        // nr of persons
    char filler[16];    /// rearrange
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
* Company table
****************************************************************************************/
typedef struct Company {

    char  tp_rec[8];    // rec type: CPY
    int   id_cpy;       // primary key
    int   id_grp;       // foreign key ref to Group
    int   id_cty;       // foreign key ref to Country
    int   id_ind;       // foreign key ref to Industry
    char  nm_cpy[95];   // company name
    char  nm_adr[95];   // company address
    char  cd_pos[10];   // post code
    char  nm_cit[40];   // city
    char  nr_tel[20];   // tel
    char  nm_www[50];   // website
                        // TODO convert dt_cre to date (check on Macbook)
    char  dt_cre[11];   // date of creation in the db
    int   nr_emp;       // nr of employees
    float am_val;       // value of single stock
} ccpy;


/***************************************************************************************
* Person table
****************************************************************************************/
typedef struct Person {

    char tp_rec[8];     // rec type: PER
    int  id_per;        // primary key
    int  id_cpy;        // foreign key ref to Company
    int  id_job;        // foreign key ref to Job
    char nm_civ[20];    // title (Mr. M.)
    char nm_fst[52];    // firstname
    char nm_lst[52];    // lastname
    char cd_sex[2];        // sex
                        // TODO convert dt_cre to date (check on Macbook)
    char dt_cre[11];    // date of creation in the db
    char nr_tel[16];    // tel
    char nr_gsm[16];    // mobile
    char nm_mail[65];   // email address
    int  nr_val;        // nr of stocks owned
} cper;


/***************************************************************************************
* DB structure
****************************************************************************************/
typedef struct db_client {

    hder hdr;           // header
    ccty cty[SZ_CTY];   // buffer country
    cjob job[SZ_JOB];   // buffer job
    cind ind[SZ_IND];   // buffer industry
    cgrp grp[SZ_GRP];   // buffer group
    ccpy cpy[SZ_CPY];   // buffer company
    cper per[SZ_PER];   // buffer person
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

/// Company ///
void import_CSV_company(dbc *db);
void export_CSV_company(dbc *db);
void load_company(dbc *db);
void print_company(dbc *db);
void rec_company(dbc *db, int id_grp);

/// Person ///
void import_CSV_person(dbc *db);
void export_CSV_person(dbc *db);
void load_person(dbc *db);
void print_person(dbc *db);
void rec_person(dbc *db, int id_grp);

/// Menus ///
void main_menu(dbc *db);