#include <stdio.h>
#include "./db.c"
#include "./include/mysql.h"

void test() {
    QueryResult* res =
        query("select user_number, user_group_name from qq_group_msg limit 10");
    for (size_t i = 0; i < res->length; i++) {
        for (size_t j = 0; j < res->fieldsLength; j++) {
            printf("%s: %s\t", res->fieldsNames[j], res->rows[i].fields[j]);
        }
        printf("\n");
    }
    freeQueryResult(res);
}
int main() {
    initDB();

    test();
    return EXIT_SUCCESS;
}