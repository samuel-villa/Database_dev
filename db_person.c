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
****************************************************************************************/
void import_CSV_person(dbc *db) {

    int i=0;
    char line[BUF_LEN];
    char fld[BUF_LEN];
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
****************************************************************************************/
void display_single_person(dbc *db, cper per) {

    ccpy cpy;
    int index, id = per.id_cpy;

    index = search_binary(db, id, COMP_ID);
    cpy = read_single_company(db, index);

    printf("\n***************************************************************************\n");

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

    printf("\n\n***************************************************************************\n\n");
}



/****************************************************************************************
 * Read Person record given its position within DB Person block.
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
 Search Person by ID - Binary search by primary key in Person table
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



///****************************************************************************************
// * Search Person by Lastname - Binary search by lastname in I_Person_Lastname table
//****************************************************************************************/
//void search_person_by_name(dbc *db) {
//
//    int index;
//    char lastname[64];
//    cper per;
//    tipl ipl;
//
//    printf("\n\t--> Enter Person Name: "); scanf("%s", lastname); fflush(stdin);
//
//    index = search_binary_string(db, lastname);             // index from sorted list (tipl)
//    ipl = read_single_tipl_rec(db, index);
//
//    if (index == REC_NOT_FOUND) {
//        printf("\n\tPerson not found\n\n");
//    } else {
//        fseek(db->fp_db, ipl.per_offset, SEEK_SET);         // go to person offset
//        fread(&per, sizeof(cper), 1, db->fp_db);     // read person
//        display_single_person(db, per);
//    }
//}


/*****************************************************************************************************
 * Search the bigger person (alphabetically) matching with the element name
 *      ls : linked list element
 *      per: person searched
*****************************************************************************************************/
node *search_bigger_per(node *ls, tipl ipl) {

    node  *cur;

    for (cur = ls->next; cur != ls && strcmp(cur->ipl.nm_lst, ipl.nm_lst) < 0; cur = cur->next);

    return cur;
}


/*****************************************************************************************************
 * Add person element before the given element within the linked list
 *      elem: element of the linked list
 *      per : person element
*****************************************************************************************************/
void add_per_before(node *elem, tipl ipl, ccpy cpy) {

    node *new_elem = malloc(sizeof(*new_elem));

    if (new_elem != NULL) {

        new_elem->ipl = ipl;
        new_elem->prev = elem->prev;
        new_elem->next = elem;

        elem->prev->next = new_elem;
        elem->prev = new_elem;
    }
}


/****************************************************************************************
 * Search Company by ID
 * Binary search (dichotomic) on the primary key in Company table
****************************************************************************************/
void search_person(dbc *db) {

    char lastname[50];
    uint per_off;

    printf("\n\t--> Enter Person Lastname: "); scanf("%s", lastname); fflush(stdin);

    per_off = get_person_root(db, lastname);        // ok => provide correct offset (ipl)

    search_person_by_name(db, per_off, lastname, T_AZ);
}


/****************************************************************************************
 * Search Company by ID
 * Binary search (dichotomic) on the primary key in Company table
****************************************************************************************/
uint get_person_root(dbc *db, char *name) {

    tipl ipl;
    uint offset;
    int i, len;
    char cur[50];

    offset = find_ipl_tree_root(db, (db->hdr.nr_per-1)/2, db->hdr.nr_per);
    len = strlen(name);

    while (1) {
        if (offset==0) {
            return 0;
        }

        fseek(db->fp_db, offset, SEEK_SET);
        fread(&ipl, sizeof(tipl), 1, db->fp_db);

        memset(cur, 0, sizeof(cur));

        for (i=0; i < len && i < strlen(ipl.nm_lst); i++) {
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
 * Search Person by Lastname - Binary search by lastname in I_Person_Lastname table
****************************************************************************************/
void search_person_by_name(dbc *db, uint offset, char *lastname, int type) {

    char buf[50];
    int i;
    tipl ipl;
    cper per;

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

        //FIXME if I delete this line it works fine
        search_person_by_name(db, ipl.per_offset_l, lastname, T_AZ);

        if (strcmp(buf, lastname) == 0) {

            memset(&per, 0, sizeof(cper));
            fseek(db->fp_db, ipl.per_offset, SEEK_SET);
            fread(&per, sizeof(cper), 1, db->fp_db);
            printf("%d %s %s %s\n", per.id_per, per.nm_civ, per.nm_lst, per.nm_fst);
        }
        search_person_by_name(db, ipl.per_offset_r, lastname, T_AZ);
    }
}