/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients
 *
 *       Interaction with the program through various selection menus
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"



/****************************************************************************************
 * Program main menu: level 0
 *
 *      os: Operating System used
****************************************************************************************/
void main_menu(dbc *db, int os) {

    int menu_sel = 0;

    while (menu_sel != EXIT) {
        printf("\n\n\tDATABASE CLIENTS V0\n\n");
        printf("\t%d - Create Empty DB\n", CREATE_DB);
        printf("\t%d - Open DB\n", OPEN_DB);
        printf("\t%d - EXIT\n\n", EXIT);
        printf("\t--> SELECT AN OPTION: "); scanf("%d", &menu_sel); fflush(stdin);

        switch (menu_sel) {
            case CREATE_DB:
                if (db->status == DB_NOT_CREATED) {
                    create_db(db);
                    printf("\n\tThe program must restart...\n\n");
                    exit(0);
                } else {
                    printf("\n\tDatabase is already created. Select 'Open DB'.\n\n");
                    pause(os);
                    clear(os);
                }
                break;
            case OPEN_DB:
                if (db->status == DB_CLOSED) {
                    printf("\n\tOpening database...\n");
                    open_db_menu(db, os);
                } else {
                    printf("\n\tFirst you must create the DB. Select 'Create Empty DB'.\n\n");
                    pause(os);
                    clear(os);
                }
                break;
            case EXIT:
                close_db_files(db);
                exit(0);
            default:
                puts("\n\tWrong Selection");
                break;
        }
    }
}



/****************************************************************************************
 * Open Database Menu: level 1
 *
 *      os: Operating System used
****************************************************************************************/
void open_db_menu(dbc *db, int os) {

    int menu_sel = 0;

    open_db_files(db);

    if (db->status == DB_OPEN_LOADED) {
        load_header(db);
        load_country(db);
        load_industry(db);
        load_group(db);
        load_job(db);
    } else {
        printf("\n\tFirst you must import data into the database\n");
    }

    while (menu_sel != BACK_TO_MAIN) {
        printf("\n\n\tDATABASE CLIENTS V0: '%s' OPEN\n\n", db->hdr.db_name);
        printf("\t%d - Import Files into DB\n", IMPORT);
        printf("\t%d - Export DB to Files\n", EXPORT);
        printf("\t%d - Search\n", SEARCH);
        printf("\t%d - Generate Report\n", REPORT);
        printf("\t%d - DB Info\n", DB_INFO_2);
        printf("\t%d - GO BACK\n\n", BACK_TO_MAIN);
        printf("\t--> SELECT AN OPTION: "); scanf("%d", &menu_sel); fflush(stdin);

        switch (menu_sel) {
            case IMPORT:
                import_CSV_country(db);
                import_CSV_job(db);
                import_CSV_industry(db);
                import_CSV_group(db);
                import_CSV_company(db);
                import_CSV_person(db);
                create_index(db);
                load_header(db);
                load_country(db);
                load_industry(db);
                load_group(db);
                load_job(db);
                db->status = DB_OPEN_LOADED;
                printf("\n\n");
                pause(os);
                clear(os);
                break;
            case EXPORT:
                if (db->status == DB_OPEN_LOADED) {
                    export_CSV_country(db);
                    export_CSV_job(db);
                    export_CSV_industry(db);
                    export_CSV_group(db);
                    export_CSV_company(db);
                    export_CSV_person(db);
                    printf("\n\n");
                } else {
                    printf("\n\tFirst you must import data into the database\n\n");
                }
                pause(os);
                clear(os);
                break;
            case SEARCH:
                if (db->status == DB_OPEN_LOADED) {
                    search_menu(db, os);
                } else {
                    printf("\n\tFirst you must import data into the database\n\n");
                    pause(os);
                }
                clear(os);
                break;
            case REPORT:
                if (db->status == DB_OPEN_LOADED) {
                    report_menu(db, os);
                } else {
                    printf("\n\tFirst you must import data into the database\n\n");
                    pause(os);
                }
                clear(os);
                break;
            case DB_INFO_2:
                display_system_info(db);
                pause(os);
                clear(os);
                break;
            case BACK_TO_MAIN:
                break;
            default:
                puts("\n\tWrong Selection");
                break;
        }
    }
}



/****************************************************************************************
 * Search in Database Menu: level 2
 *
 *      os: Operating System used
****************************************************************************************/
void search_menu(dbc *db, int os) {

    int menu_sel = 0;

    while (menu_sel != S_BACK) {
        printf("\n\n\tDATABASE CLIENTS V0: '%s' OPEN\n\n", db->hdr.db_name);
        printf("\t%d - List Countries\n", S_COUNTRY);
        printf("\t%d - List Industries\n", S_INDUSTRY);
        printf("\t%d - List Groups\n", S_GROUP);
        printf("\t%d - List Jobs\n", S_JOB);
        printf("\t%d - Search Companies\n", S_COMPANY);
        printf("\t%d - Search Persons\n", S_PERSON);
        printf("\t%d - GO BACK\n\n", S_BACK);
        printf("\t--> SELECT AN OPTION: "); scanf("%d", &menu_sel); fflush(stdin);

        switch (menu_sel) {
            case S_COUNTRY:
                print_country(db);
                pause(os);
                clear(os);
                break;
            case S_INDUSTRY:
                print_industry(db);
                pause(os);
                clear(os);
                break;
            case S_GROUP:
                print_group(db);
                pause(os);
                clear(os);
                break;
            case S_JOB:
                print_job(db);
                pause(os);
                clear(os);
                break;
            case S_PERSON:
                search_person_menu(db, os);
                clear(os);
                break;
            case S_COMPANY:
                search_company_menu(db, os);
                clear(os);
                break;
            case S_BACK:
                break;
            default:
                puts("\n\tWrong Selection");
                break;
        }
    }
}



/****************************************************************************************
 * Search Person Menu: level 3
 *
 *      os: Operating System used
****************************************************************************************/
void search_person_menu(dbc *db, int os) {

    int menu_sel = 0;

    while (menu_sel != 9) {
        printf("\n\n\tDATABASE CLIENTS V0: '%s' OPEN\n\n", db->hdr.db_name);
        printf("\t%d - Search Person by ID\n", 0);
        printf("\t%d - Search Persons by lastname\n", 1);
        printf("\t%d - GO BACK\n\n", 9);
        printf("\t--> SELECT AN OPTION: "); scanf("%d", &menu_sel); fflush(stdin);

        switch (menu_sel) {
            case 0:
                search_person_by_id(db);
                pause(os);
                break;
            case 1:
                search_person_by_name(db);
                pause(os);
                break;
            case 9:
                break;
            default:
                puts("\n\tWrong Selection");
                pause(os);
                break;
        }
    }
}



/****************************************************************************************
 * Search Company Menu: level 3
 *
 *      os: Operating System used
****************************************************************************************/
void search_company_menu(dbc *db, int os) {

    int menu_sel = 0;

    while (menu_sel != 9) {
        printf("\n\n\tDATABASE CLIENTS V0: '%s' OPEN\n\n", db->hdr.db_name);
        printf("\t%d - Search Company by ID\n", T_CPYID);
        printf("\t%d - Search Company by name (A-Z)\n", T_CPYNAME_AZ);
        printf("\t%d - Search Company by name (Z-A)\n", T_CPYNAME_ZA);
        printf("\t%d - Search Companies by Group\n", T_CPYGRP);
        printf("\t%d - Get Company Employees\n", T_CPYPER);
        printf("\t%d - GO BACK\n\n", 9);
        printf("\t--> SELECT AN OPTION: "); scanf("%d", &menu_sel); fflush(stdin);

        switch (menu_sel) {
            case T_CPYID:
                search_company_by_id(db);
                pause(os);
                break;
            case T_CPYNAME_AZ:
                search_company_by_name(db, T_AZ);
                pause(os);
                break;
            case T_CPYNAME_ZA:
                search_company_by_name(db, T_ZA);
                pause(os);
                break;
            case T_CPYGRP:
                search_companies_by_group(db);
                pause(os);
                break;
            case T_CPYPER:
                get_comp_employees(db);
                pause(os);
                break;
            case 9:
                break;
            default:
                puts("\n\tWrong Selection");
                break;
        }
    }
}


/****************************************************************************************
 * Report Menu: level 2
 *
 *      os: Operating System used
****************************************************************************************/
void report_menu(dbc *db, int os) {

    int menu_sel = 0;

    while (menu_sel != R_BACK) {
        printf("\n\n\tDATABASE CLIENTS V0: '%s' OPEN\n\n", db->hdr.db_name);
        printf("\t%d - Report: List of Companies per Group (grouped by Country)\n", R_COMP_GROUP);
        printf("\t%d - Report: Total Number of Employees per Group\n", R_PERS_GROUP);
        printf("\t%d - GO BACK\n\n", R_BACK);
        printf("\t--> SELECT AN OPTION: "); scanf("%d", &menu_sel); fflush(stdin);

        switch (menu_sel) {
            case R_COMP_GROUP:
                report_group_companies(db);
                pause(os);
                break;
            case R_PERS_GROUP:
                report_group_persons(db);
                pause(os);
                break;
            case R_BACK:
                break;
            default:
                puts("\n\tWrong Selection");
                pause(os);
                break;
        }
    }
}