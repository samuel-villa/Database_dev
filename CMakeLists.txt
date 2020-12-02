cmake_minimum_required(VERSION 3.17)
project(DB_Clients_Data C)

set(CMAKE_C_STANDARD 99)

add_executable(DB_Clients_Data db_main.c db_main.h db_file.c db_country.c db_job.c db_industry.c db_group.c db_company.c db_person.c db_menus.c system.c db_index.c db_doubly_linked_list.c db_reports.c)