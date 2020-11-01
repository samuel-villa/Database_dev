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
    FILE *fp_db;
    db->status = DB_OPEN;

    while (menu_sel != CLOSE_DB) {
        printf("\n\tDATABASE CLIENTS V0: '%s' OPEN\n\n", db->hdr.db_name);
        printf("\t%d - Import File into DB\n", IMPORT);
        printf("\t%d - Export DB to File\n", EXPORT);
        printf("\t%d - Search\n", SEARCH);
        printf("\t%d - Generate Report\n", REPORT);
        printf("\t%d - System Info\n", SYSTEM_INFO_2);
        printf("\t%d - Close DB\n\n", CLOSE_DB);
        printf("\t--> SELECT AN OPTION: "); scanf("%d", &menu_sel); fflush(stdin);

        switch (menu_sel) {
            case IMPORT:
                import_CSV_country(db);
                import_CSV_job(db);
                import_CSV_industry(db);
                import_CSV_group(db);
                import_CSV_company(db);
                import_CSV_person(db);
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
                printf("Search Menu...\n");
                pause(os);
                clear(os);
                break;
            case REPORT:
                printf("Generate Report...\n");
                pause(os);
                clear(os);
                break;
            case SYSTEM_INFO_2:
                display_system_info(db);
                pause(os);
                clear(os);
                break;
            case CLOSE_DB:
                //fclose(fp_db);
                printf("DB closed\n");
                break;
            default:
                puts("Wrong Selection");
                break;
        }
    }
}