/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients V0
 *
 *       Main program: Test of all functions
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"

/****************************************************************************************
* Main program
****************************************************************************************/
int main() {

    dbc db;

    create_db(&db);

    import_CSV_country(&db);
    export_CSV_country(&db);
    load_country(&db);
    print_country(&db);

    import_CSV_industry(&db);
    export_CSV_industry(&db);
    load_industry(&db);
    print_industry(&db);

    import_CSV_job(&db);
    export_CSV_job(&db);
    load_job(&db);
    print_job(&db);

    import_CSV_group(&db);
    export_CSV_group(&db);
    load_group(&db);
    print_group(&db);

    return 0;
}