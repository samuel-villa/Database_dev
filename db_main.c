/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients
 *
 *       Main program: Test of all functions
 *       Program interactive main menu (level 0) is launched here.
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"



/****************************************************************************************
* Main program
****************************************************************************************/
int main() {

    int os;
    dbc db;

    os = user_os();
    set_db_status(&db);
    main_menu(&db, os);

    return 0;
}
