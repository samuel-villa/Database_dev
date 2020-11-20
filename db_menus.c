/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients V0
 *
 *       Navigation through the program with various menus
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"


/****************************************************************************************
* Program main menu
****************************************************************************************/
void main_menu(dbc *db, int os) {

    int menu_sel = 0;

    while (menu_sel != EXIT) {
        printf("\n\tDATABASE CLIENTS V0\n\n");
        printf("\t%d - Create Empty DB\n", CREATE_DB);
        printf("\t%d - Open DB\n", OPEN_DB);
        printf("\t%d - System Info\n", SYSTEM_INFO);
        printf("\t%d - EXIT\n\n", EXIT);
        printf("\t--> SELECT AN OPTION: "); scanf("%d", &menu_sel); fflush(stdin);

        switch (menu_sel) {
            case CREATE_DB:
                if (db->status == DB_NOT_CREATED) {
                    create_db(db);
                    printf("\n\tThe program must restart...\n\n");
                    exit(0);
                } else {
                    printf("\n\t'%s' is already created. Select 'Open DB'.\n\n", db->hdr.db_name);
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
            case SYSTEM_INFO:
                if (db->status != DB_NOT_CREATED) {
                    display_system_info(db);
                } else {
                    printf("\n\tFirst you must create the DB. Select 'Create Empty DB'.\n\n");
                }
                pause(os);
                clear(os);
                break;
            case EXIT:
                fclose(db->fp_db);          // just in case we forget any fclose()
                fclose(db->fp_lg);
                exit(0);
            default:
                puts("Wrong Selection");
                break;
        }
    }
}



/****************************************************************************************
* Open Database Menu
****************************************************************************************/
void open_db_menu(dbc *db, int os) {

    int menu_sel = 0;
    db->status = DB_OPEN;

    while (menu_sel != BACK_TO_MAIN) {
        printf("\n\tDATABASE CLIENTS V0: '%s' OPEN\n\n", db->hdr.db_name);
        printf("\t%d - Import File into DB\n", IMPORT);
        printf("\t%d - Export DB to File\n", EXPORT);
        printf("\t%d - Search\n", SEARCH);
        printf("\t%d - Generate Report\n", REPORT);
        printf("\t%d - System Info\n", SYSTEM_INFO_2);
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
                printf("\n\n");
                pause(os);
                clear(os);
                break;
            case EXPORT:
                export_CSV_country(db);
                export_CSV_job(db);
                export_CSV_industry(db);
                export_CSV_group(db);
                export_CSV_company(db);
                export_CSV_person(db);
                printf("\n\n");
                pause(os);
                clear(os);
                break;
            case SEARCH:
                search_menu(db, os);
                clear(os);
                break;
            case REPORT:
                report_menu(db, os);
                pause(os);
                clear(os);
                break;
            case SYSTEM_INFO_2:
                display_system_info(db);
                pause(os);
                clear(os);
                break;
            case BACK_TO_MAIN:
                break;
            default:
                puts("Wrong Selection");
                break;
        }
    }
}



/****************************************************************************************
* Search in Database Menu
****************************************************************************************/
void search_menu(dbc *db, int os) {

    int menu_sel = 0;

    while (menu_sel != S_BACK) {
        printf("\n\tDATABASE CLIENTS V0: '%s' OPEN\n\n", db->hdr.db_name);
        printf("\t%d - List Countries\n", S_COUNTRY);
        printf("\t%d - List Industries\n", S_INDUSTRY);
        printf("\t%d - List Groups\n", S_GROUP);
        printf("\t%d - List Jobs\n", S_JOB);
        printf("\t%d - Search Companies\n", S_COMPANY);
        printf("\t%d - Search Persons\n", S_PERSON);
        printf("\t%d - Search Persons/Company\n", S_PERS_COMP);
        printf("\t%d - Get Company details\n", S_COMP_DETAIL);
        printf("\t%d - GO BACK\n\n", S_BACK);
        printf("\t--> SELECT AN OPTION: "); scanf("%d", &menu_sel); fflush(stdin);

        switch (menu_sel) {
            case S_COUNTRY:
                load_country(db);
                print_country(db);
                pause(os);
                clear(os);
                break;
            case S_INDUSTRY:
                load_industry(db);
                print_industry(db);
                pause(os);
                clear(os);
                break;
            case S_GROUP:
                load_group(db);
                print_group(db);
                pause(os);
                clear(os);
                break;
            case S_JOB:
                load_job(db);
                print_job(db);
                pause(os);
                clear(os);
                break;
            case S_PERSON:
                search_person_by_id(db);        // OR
                //search_person_by_name(db);
                pause(os);
                clear(os);
                break;
            case S_COMPANY:
                search_company_by_id(db);       // OR
                //search_company_by_name(db);
                pause(os);
                clear(os);
                break;
            case S_PERS_COMP:
                search_company_employees(db);
                pause(os);
                clear(os);
                break;
            case S_COMP_DETAIL:
                company_details(db);
                pause(os);
                clear(os);
                break;
            case S_BACK:
                break;
            default:
                puts("Wrong Selection");
                break;
        }
    }
}


/****************************************************************************************
* Search in Database Menu
****************************************************************************************/
void report_menu(dbc *db, int os) {

    int menu_sel = 0;

    while (menu_sel != R_BACK) {
        printf("\n\tDATABASE CLIENTS V0: '%s' OPEN\n\n", db->hdr.db_name);
        printf("\t%d - Employees per Company\n", R_PERS_COMP);
        printf("\t%d - Companies per Group\n", R_COMP_GROUP);
        printf("\t%d - Employees per Partial Lastname\n", R_PERS_NAME);
        printf("\t%d - GO BACK\n\n", R_BACK);
        printf("\t--> SELECT AN OPTION: "); scanf("%d", &menu_sel); fflush(stdin);

        switch (menu_sel) {
            case R_PERS_COMP:
                printf("create report with search_company_employees('print option')");
                break;
            case R_COMP_GROUP:
                printf("create report with search_group_companies('print option')");
                break;
            case R_PERS_NAME:
                printf("create report with search_person_by_name('print option')");
                break;
            case R_BACK:
                break;
            default:
                puts("Wrong Selection");
                break;
        }
    }
}