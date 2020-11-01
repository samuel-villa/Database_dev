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
void main_menu(dbc *db) {

    int menu_sel = 0;

    printf("\tDATABASE CLIENTS V0\n\n");

    while (menu_sel != EXIT) {
        printf("\t%d - Create Empty Database\n", CREATE_DB);
        printf("\t%d - Open Database\n", OPEN_DB);
        printf("\t%d - System Info\n", SYSTEM_INFO);
        printf("\t%d - EXIT\n\n", EXIT);
        printf("\t--> SELECT AN OPTION: "); scanf("%d", &menu_sel); fflush(stdin);

        switch (menu_sel) {
            case CREATE_DB:
                create_db(db);
                break;
            case OPEN_DB:
                puts("Opening db");
                break;
            case SYSTEM_INFO:
                puts("Displaying System Info");
                break;
            case EXIT:
                exit(0);
            default:
                puts("Wrong Selection");
                break;
        }
    }
}