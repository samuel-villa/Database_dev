/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients V0
 *
 *       Requirements:
 *          • La création de la base de données                                                                   OK
 *          • L’importation des 6 tables fournies sous forme de fichier .csv                                      OK
 *          • L’exportation de ces 6 tables sous forme de fichier .csv                                            OK
 *          • La création des index nécessaires aux requêtes précisées                                            OK
 *          • Des écrans permettant de lister l’information par recherche séquentielle pour les tables de codes   OK
 *          • Des écrans permettant d’afficher par recherche dichotomique                                         OK
 *              • Une personne donnée par sa clé primaire                                                         OK
 *              • Une compagnie donnée par sa clé primaire                                                        OK
 *          • Au moins un écran permettant d’afficher via une liste chainée en mémoire                            OK
 *              • Les personnes travaillant pour une compagnie donnée par sa clé primaire                         OK
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
 *          - Display Countries, Industries, Groups and Jobs
 *          - Search companies by ID
 *          - Search persons by ID and/or by lastname
 *          - Display company employees given its ID (linked list in RAM)
 *
 *       Not yet implemented:
 *          - Search person by partial lastname
 *          - Reports
 *          - Sort linked lists A-Z and Z-A
 *          - Extras
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/***************************************************************************************
* Definitions
****************************************************************************************/

#define SZ_CTY 100
#define SZ_JOB 200
#define SZ_IND 100
#define SZ_GRP 3000
#define SZ_CPY 100000
#define SZ_PER 600000
#define SZ_IPC 600000
#define SZ_IPL 600000


enum Main_Menu {
    CREATE_DB,
    OPEN_DB,
    SYSTEM_INFO,
    EXIT = 9
};

enum Open_DB_Menu {
    IMPORT,
    EXPORT,
    SEARCH,
    REPORT,
    SYSTEM_INFO_2,
    BACK_TO_MAIN = 9
};

enum Search_Menu {
    S_COUNTRY,
    S_INDUSTRY,
    S_GROUP,
    S_JOB,
    S_COMPANY,
    S_PERSON,
    S_BACK = 9
};

enum Report_Menu {
    R_PERS_COMP,
    R_COMP_GROUP,
    R_PERS_NAME,
    R_BACK = 9
};

enum File_Status {
    DB_NOT_CREATED,
    DB_CLOSED,
    DB_OPEN
};

enum Sort_Type {
    SORT_PERS_COMP,
    SORT_COMP_GROUP,
    SORT_PERS_NAME
};

enum Binary_Search_Type {
    PERS_ID,
    COMP_ID,
};

enum Error_Code {
    REC_OUT_RANGE = -1,         // search_binary()
    REC_NOT_FOUND = -2          // search_binary()
};

typedef unsigned int uint;


/***************************************************************************************
* DB Header - Metadata
****************************************************************************************/
typedef struct Header {

    uint db_size;       // db size
    char db_name[28];   // db name                                  // 32 bytes

    uint sz_cty;        // country bloc size
    uint sz_job;        // job bloc size
    uint sz_ind;        // industry bloc size
    uint sz_grp;        // group bloc size
    uint sz_cpy;        // company bloc size
    uint sz_per;        // person bloc size
    uint sz_ipc;        // pers/comp index size
    uint sz_ipl;        // pers/lastname index size                 // 32 bytes

    uint off_cty;       // country bloc position
    uint off_job;       // job bloc position
    uint off_ind;       // industry bloc position
    uint off_grp;       // group bloc position
    uint off_cpy;       // company bloc position
    uint off_per;       // person bloc position
    uint off_ipc;       // pers/comp index bloc position
    uint off_ipl;       // pers/lastname index bloc position        // 32 bytes

    uint nr_cty;        // nr of countries
    uint nr_job;        // nr of jobs
    uint nr_ind;        // nr of industries
    uint nr_grp;        // nr of groups
    uint nr_cpy;        // nr of companies
    uint nr_per;        // nr of persons
    uint nr_ipc;        // nr of indexes pers/comp
    uint nr_ipl;        // nr of indexes pers/lastname              // 32 bytes
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
    char filler1[20];
    char nm_grp[60];
    int  id_cty;
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
    char  nm_cpy[96];   // company name
    char  nm_adr[81];   // company address
    char  cd_pos[15];   // post code
    char  nm_cit[32];   // city
    char  nr_tel[23];   // tel
    char  nm_www[50];   // website
    char  dt_cre[11];   // date of creation in the db
    int   nr_emp;       // nr of employees
    float am_val;       // value of single stock
    char  filler[12];
} ccpy;


/***************************************************************************************
* Person table
****************************************************************************************/
typedef struct Person {

    char tp_rec[8];     // rec type: PER
    int  id_per;        // primary key
    int  id_cpy;        // foreign key ref to Company
    int  id_job;        // foreign key ref to Job
    char nm_civ[16];    // title (Mr. M.)
    char nm_fst[44];    // firstname
    char nm_lst[56];    // lastname
    char cd_sex[4];     // sex
    char dt_cre[16];    // date of creation in the db
    char nr_tel[16];    // tel
    char nr_gsm[16];    // mobile
    char nm_mail[64];   // email address
    int  nr_val;        // nr of stocks owned
} cper;


/***************************************************************************************
* Index table: Person by Company ID
****************************************************************************************/
typedef struct I_Person_Company {

    char tp_rec[8];     // rec type: IPC
    uint per_offset;    // person offset
    int  id_cpy;        // id company related to this person
} tipc;


/***************************************************************************************
* Index table: Person by Lastname
****************************************************************************************/
typedef struct I_Person_Lastname {

    char tp_rec[8];     // rec type: IPL
    uint per_offset;    // person offset
    uint per_offset_l;  // person offset
    uint per_offset_r;  // person offset
    char fill[12];
    char nm_lst[64];    // lastname of this person
} tipl;


/***************************************************************************************
* Sorting space used to sort list of I_Person_Company type
****************************************************************************************/
typedef struct Sorting {

    int  id;            // object id to be sorted
    char ln[64];        // used when searching by lastname
    uint off_sort_obj;  // object offset
} t_sort;


/***************************************************************************************
* Sorting space used to sort list of I_Person_Lastname type
****************************************************************************************/
typedef struct Linked_Sorting {

    int  id;            // object id to be sorted
    char ln[64];        // used when searching by lastname
    uint off_sort_obj;  // object offset
    uint off_next;      // next object offset
} t_lsort;


/***************************************************************************************
* DB structure
****************************************************************************************/
typedef struct db_client {

    hder    hdr;         // header
    FILE    *fp_db;      // db file pointer
    FILE    *fp_lg;      // log file pointer
    ccty    cty[SZ_CTY]; // buffer country
    cjob    job[SZ_JOB]; // buffer job
    cind    ind[SZ_IND]; // buffer industry
    cgrp    grp[SZ_GRP]; // buffer group
    int     status;      // db nonexistent, closed or open
    t_sort  *sort;       // points to list of elements to be sorted
    t_lsort *lsort;      // points to linked list of elements to be sorted
} dbc;


/****************************************************************************************
* Prototypes
****************************************************************************************/

/// DB File ///
void create_db(dbc *db);                                    // create empty DB
FILE * open_db_file(dbc *db);                               // handles DB file pointer globally
void close_db_file(dbc *db);                                // closes DB file globally
void load_header(dbc *db);                                  // read file header and load RAM
void set_db_status(dbc *db);                                // file can be Nonexistent, closed or open
void display_system_info(dbc *db);                          // display header Info from RAM
void update_hdr(dbc *db);                                   // write into DB header

/// Country ///
void import_CSV_country(dbc *db);                           // import data to db file
void export_CSV_country(dbc *db);                           // export data from db file to csv
void load_country(dbc *db);                                 // load data to RAM
void print_country(dbc *db);                                // display data loaded on RAM
void rec_country(dbc *db, int id_cty);                      // display one single records

/// Industry ///
void import_CSV_industry(dbc *db);                          // import data to db file
void export_CSV_industry(dbc *db);                          // export data from db file to csv
void load_industry(dbc *db);                                // load data to RAM
void print_industry(dbc *db);                               // display data loaded on RAM
void rec_industry(dbc *db, int id_ind);                     // display one single records

/// Job ///
void import_CSV_job(dbc *db);                               // import data to db file
void export_CSV_job(dbc *db);                               // export data from db file to csv
void load_job(dbc *db);                                     // load data to RAM
void print_job(dbc *db);                                    // display data loaded on RAM
void rec_job(dbc *db, int id_job);                          // display one single records

/// Group ///
void import_CSV_group(dbc *db);                             // import data to db file
void export_CSV_group(dbc *db);                             // export data from db file to csv
void load_group(dbc *db);                                   // load data to RAM
void print_group(dbc *db);                                  // display data loaded on RAM
void rec_group(dbc *db, int id_grp);                        // display one single records

/// Company ///
void import_CSV_company(dbc *db);                           // import data to db file
void export_CSV_company(dbc *db);                           // export data from db file to csv
void display_single_company(dbc *db, ccpy cpy);             // display company struct attributes
void search_company_by_id(dbc *db);                         // generic search function for cpy ID
void search_company_by_name(dbc *db);                       // TODO BinaryTree:
                                                            // TODO make it not case sensitive && partial name function
ccpy read_single_company(dbc *db, int index);               // read cpy record given its index

/// Person ///
void import_CSV_person(dbc *db);                            // import data to db file
void export_CSV_person(dbc *db);                            // export data from db file to csv
void display_single_person(dbc *db, cper per);              // display company struct attributes
void search_person_by_id(dbc *db);                          // generic search function for cpy ID
void search_person_by_name(dbc *db);                        // TODO BinaryTree:
                                                            // TODO make it not case sensitive && partial name function
cper read_single_person(dbc *db, int index);                // read per record given its index

/// Generic ///
int search_binary(dbc *db, int id, int type);               // multiplexed for pers/id and company/id
int search_binary_string(dbc *db, char *name);              // binary search of person lastname

/// Index ///
void sort_bubble_index(dbc *db, int nr, int type);          // bubble sort (inefficient in this project, very slow)
void quicksort(dbc *db, int first, int last, int type);     // best sort solution for this project
void create_index_per_cpy(dbc *db);                         // creates db bloc of tipc elements
void create_index_per_name(dbc *db);                        // creates db bloc of tipl elements
void create_index(dbc *db);                                 // calls the two previous functions
void search_group_companies(dbc *db);                       // TODO extra
void alloc_sort_table(dbc *db, uint size);                  // RAM allocation for sort type
void free_sort_table(dbc *db);                              // free RAM for sort type
tipl read_single_tipl_rec(dbc *db, int index);              // read ipl record given its index
void alloc_link_sort_table(dbc *db, uint size);             // RAM allocation for linked sort type
void free_link_sort_table(dbc *db);                         // free RAM for linked sort type
void load_ipl_in_ram(dbc *db);                              // load linked sorted list from ipl table
void get_comp_employees(dbc *db);                           // request company ID and gives the list of employees
void list_comp_employees(dbc *db, int comp_id);             // display company and list of employees given its ID

/// Menus ///
void main_menu(dbc *db, int os);                            // First menu when running the program
void open_db_menu(dbc *db, int os);                         // main         -> sub-menu
void search_menu(dbc *db, int os);                          // open_db      -> sub-menu
void report_menu(dbc *db, int os);                          // open_db      -> sub-menu
void search_person_menu(dbc *db, int os);                   // search_menu  -> sub-menu
void search_company_menu(dbc *db, int os);                  // search_menu  -> sub-menu

/// System Administration ///
int user_os();                                              // request OS to user in order to adapt some basic command
void pause(int os);                                         // pause screen, a key must be pressed to continue
void clear(int os);                                         // clear screen
const char *timestamp();                                    // get current time