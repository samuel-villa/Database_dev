/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients V0
 *
 *       System Administration
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"

enum OS {
    UNIX,
    WINDOWS
};

/****************************************************************************************
* Request user OS
****************************************************************************************/
int user_os() {

    int os = -1;

    while (os < 0 || os > 1) {
        printf("\n\tSelect your OS:\n\n");
        printf("\t%d - Linux / UNIX / macOS\n", UNIX);
        printf("\t%d - Windows\n\n", WINDOWS);
        printf("\t--> SELECT AN OPTION: "); scanf("%d", &os); fflush(stdin);

        switch (os) {
            case UNIX:
                os = UNIX;
                break;
            case WINDOWS:
                os = WINDOWS;
                break;
            default:
                printf("\n\tWrong Selection, select 0 or 1.\n");
                break;
        }
    }
    return os;
}


/****************************************************************************************
 * System Pause: used in menus
 * os: user OS
****************************************************************************************/
void pause(int os) {
    if (os == UNIX) system("read -p \"Press [Enter] key to continue...\"");
    else system("pause");
}


/****************************************************************************************
 * Clear Screen: used in menus
 * os: user OS
****************************************************************************************/
void clear(int os) {
    if (os == UNIX) system("clear");
    else system("cls");
}


/****************************************************************************************
 * Return current time converted into string
****************************************************************************************/
char *timestamp() {

    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    return asctime(timeinfo);
}