/**********************************************************************************************************************
 * Programmation procedurale 2020 - Dossier: Database Clients
 *
 *       Person table functions
 *
 * Samuel CIULLA - MacOS 10.13
 *********************************************************************************************************************/

#include "db_main.h"



/****************************************************************************************
 * Import file db_person.csv in the db
 * Read serialized data, deserialize it and populate db with processed data
****************************************************************************************/
void import_CSV_person(dbc *db) {

    int i=0;
    char line[BUF_LEN], fld[BUF_LEN];
    char *ptr1, *ptr2;
    cper per;
    FILE *fpi, *fp_db, *fp_lg;

    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    fp_lg = fopen("data_db_clients/db_clients.log", "a");

    fpi = fopen("data_import/db_person.csv", "r");
    if (fpi == NULL) { printf("Error\n"); return; }

    printf("\n\tImporting persons... ");

    fgets(line, 200, fpi);

    fseek(fp_db, db->hdr.off_per, SEEK_SET);

    while (fgets(line, 200, fpi) != NULL) {

        memset(&per, 0, sizeof(cper));
        strcpy(per.tp_rec, "PER");

        ptr1 = strtok(line,";");
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        per.id_per = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        per.id_cpy = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        per.id_job = atoi(fld);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(per.nm_civ, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(per.nm_fst, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(per.nm_lst, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(per.cd_sex, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(per.dt_cre, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(per.nr_tel, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(per.nr_gsm, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");

        strncpy(per.nm_mail, ptr1, ptr2-ptr1-1);
        ptr1 = ptr2;

        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, strlen(ptr1)-1);
        per.nr_val = atoi(fld);

        fwrite(&per, 1, sizeof(cper), fp_db);

        i++;
    }

    fprintf(fp_lg, "%s Persons imported: %d\n", timestamp(), i);

    fseek(fp_db, 0, SEEK_SET);
    fread(&db->hdr, sizeof(hder), 1, fp_db);

    db->hdr.nr_per = i;

    fseek(fp_db, 0, SEEK_SET);
    fwrite(&db->hdr, sizeof(hder), 1, fp_db);

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpi);

    printf("DONE => Persons imported: %d", db->hdr.nr_per);

    return ;
}



/****************************************************************************************
* Export table Person to csv file
****************************************************************************************/
void export_CSV_person(dbc *db) {

    int i;
    cper per;
    FILE *fpo, *fp_db, *fp_lg;

    fp_db = fopen("data_db_clients/db_clients.dat", "rb+");
    fp_lg = fopen("data_db_clients/db_clients.log", "a");

    printf("\n\tExporting persons... ");
    fpo = fopen("data_export/exp_person.csv", "w");

    fprintf(fpo,"id;id_cpy;id_job;nm_civ;nm_fst;nm_lst;cd_sex;dt_cre;nr_tel;nr_gsm;nm_mail;nr_val\n");

    fseek(fp_db, db->hdr.off_per, SEEK_SET);

    for (i=0; i<db->hdr.nr_per; i++) {

        memset(&per, 0, sizeof(cper));
        fread(&per, 1, sizeof(cper), fp_db);

        fprintf(fpo,"%d;%d;%d;%s;%s;%s;%s;%s;%s;%s;%s;%d\n",
                per.id_per,
                per.id_cpy,
                per.id_job,
                per.nm_civ,
                per.nm_fst,
                per.nm_lst,
                per.cd_sex,
                per.dt_cre,
                per.nr_tel,
                per.nr_gsm,
                per.nm_mail,
                per.nr_val);
    }

    fprintf(fp_lg, "%s Persons exported: %d\n", timestamp(), db->hdr.nr_per);

    fclose(fp_db);
    fclose(fp_lg);
    fclose(fpo);

    printf("DONE => Persons exported: %d", db->hdr.nr_per);
}



/****************************************************************************************
 * Display one Person record
 *
 *      per: person element
****************************************************************************************/
void display_single_person(dbc *db, cper per) {

    ccpy cpy;
    int index, id;

    id = per.id_cpy;
    index = search_binary(db, id, COMP_ID);
    cpy = read_single_company(db, index);

    printf("\n\t================================================================================\n");

    printf("\n\tID person.............. %-80d", per.id_per);
    printf("\n\tTitle.................. %-80s", per.nm_civ);
    printf("\n\tFirstname.............. %-80s", per.nm_fst);
    printf("\n\tLastname............... %-80s", per.nm_lst);
    printf("\n\tCompany................ %s", cpy.nm_cpy);
    printf("\n\tJob.................... %s", db->job[per.id_job].nm_job);
    printf("\n\tSex.................... %-80s", per.cd_sex);
    printf("\n\tMobile................. %-80s", per.nr_gsm);
    printf("\n\tTel.................... %-80s", per.nr_tel);
    printf("\n\tEmail.................. %-80s", per.nm_mail);
    printf("\n\tDate of creation....... %-80s", per.dt_cre);
    printf("\n\tStocks owned........... %-80d", per.nr_val);

    printf("\n\t================================================================================\n\n");
}



/****************************************************************************************
 * Read Person record given its position within DB Person block.
 *
 *      index : person position within the Person block
 *      return: person attributes
****************************************************************************************/
cper read_single_person(dbc *db, int index) {

    cper per;

    fseek(db->fp_db, db->hdr.off_per + index * sizeof(cper), SEEK_SET);
    fread(&per, sizeof(cper), 1, db->fp_db);

    return per;
}



/****************************************************************************************
 Search Person by ID - Binary search by primary key into Person table
****************************************************************************************/
void search_person_by_id(dbc *db) {

    int id, index;
    cper per;

    printf("\n\t--> Enter Person ID: "); scanf("%d", &id); fflush(stdin);
    index = search_binary(db, id, PERS_ID);           // get element index within db file per bloc

    if (index == REC_OUT_RANGE) {
        printf("\n\tPerson ID %d is out of range\n\n", id);
    } else if (index == REC_NOT_FOUND) {
        printf("\n\tNo results with Person ID %d\n\n", id);
    } else {
        per = read_single_person(db, index);               // read per at given index
        display_single_person(db, per);
    }
}


/****************************************************************************************
 * Main function for search person by lastname
****************************************************************************************/
void search_person_by_name(dbc *db) {

    char lastname[50];
    uint per_off;

    printf("\n\t--> Enter Person Lastname: "); scanf("%s", lastname); fflush(stdin);

    per_off = get_person_root(db, lastname);                // if no results -> returns 0

    if (per_off != 0) {
        printf("\n\t%8s\t%-10s %-20s %-20s %-s\n", "id", "civ", "lastname", "firstname", "company");
        printf("\t-------------------------------------------------------------------------------------------------------\n");
        fetch_person(db, per_off, lastname);
        puts("");
    } else {
        printf("\n\tNo results found for '%s'", lastname);
    }
}


/****************************************************************************************
 * Find the binary tree root node matching with the given string
 *
 *      name  : given lastname (partial)
 *      return: root offset of the person name searched
****************************************************************************************/
uint get_person_root(dbc *db, char *name) {

    tipl ipl;
    uint offset;
    char cur[50];

    offset = db->hdr.ipl_root;

    while (1) {

        if (!offset) {
            return 0;
        }

        fseek(db->fp_db, offset, SEEK_SET);
        fread(&ipl, sizeof(tipl), 1, db->fp_db);

        memset(cur, 0, sizeof(cur));

        for (int i=0; i < strlen(name) && i < strlen(ipl.nm_lst); i++) {
            cur[i] = ipl.nm_lst[i];
        }

        if (strcmp(name, cur) < 0) {
            offset = ipl.per_offset_l;
        } else {
            if (strcmp(name, cur) > 0) {
                offset = ipl.per_offset_r;
            } else {
                return offset;
            }
        }
    }
}


/****************************************************************************************
 * Recursive function comparing given string with db and display results
 *
 *      offset  : binary tree root node of the given string
 *      lastname: partial lastname provided
****************************************************************************************/
void fetch_person(dbc *db, uint offset, char *lastname) {

    char buf[50];
    int i, id, index;
    tipl ipl;
    cper per;
    ccpy cpy;

    if (offset) {

        fseek(db->fp_db, offset, SEEK_SET);
        fread(&ipl, sizeof(tipl), 1, db->fp_db);

        if (strlen(ipl.nm_lst) > strlen(lastname)) {
            for (i=0; i < strlen(lastname); i++) {
                buf[i] = ipl.nm_lst[i];
            }
            buf[i] = '\0';
        } else {
            strcpy(buf, ipl.nm_lst);
        }

        fetch_person(db, ipl.per_offset_l, lastname);               // recursion left

        if (strcmp(buf, lastname) == 0) {                           // if match => display

            fseek(db->fp_db, ipl.per_offset, SEEK_SET);
            fread(&per, sizeof(cper), 1, db->fp_db);

            id = per.id_cpy;
            index = search_binary(db, id, COMP_ID);
            cpy = read_single_company(db, index);                   // get person company

            printf("\t%8d\t%-10s %-20s %-20s %-s\n",
                   per.id_per,
                   per.nm_civ,
                   per.nm_lst,
                   per.nm_fst,
                   cpy.nm_cpy);
        }

        fetch_person(db, ipl.per_offset_r, lastname);               // recursion right
    }
}