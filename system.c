/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients
 *
 *       System Administration functions, this part handles the different OS used
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
 *
 *      os: user OS
****************************************************************************************/
void pause(int os) {

    if (os == UNIX) system("read -p \"\tPress [Enter] key to continue...\"");
    else system("pause");
}


/****************************************************************************************
 * Clear Screen function used in menus
 *
 *      os: user OS
****************************************************************************************/
void clear(int os) {

    if (os == UNIX) system("clear");
    else system("cls");
}


/****************************************************************************************
 * Get current timestamp and convert it into string
 *
 *      return: current time converted into string
****************************************************************************************/
const char *timestamp() {

    int hours, minutes, seconds, day, month, year;
    char *timestamp;

    timestamp = (char*)malloc(sizeof(char));

    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    hours = local->tm_hour;
    minutes = local->tm_min;
    seconds = local->tm_sec;
    day = local->tm_mday;
    month = local->tm_mon + 1;
    year = local->tm_year + 1900;

    sprintf(timestamp, "%02d/%02d/%d %02d:%02d:%02d -", day, month, year, hours, minutes, seconds);

    return timestamp;
}