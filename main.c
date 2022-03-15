#include <stdio.h>
#include "./db.c"
#include "./include/mysql.h"

int main() {
    MYSQL mysql;
    struct MysqlConfig config = getMysqlConfig();

    mysql_init(&mysql);

    if (!mysql_real_connect(&mysql, config.host, config.user, config.pass,
                            config.database, config.port, NULL, 0)) {
        fprintf(stderr, "%s", mysql_error(&mysql));
    }

    // utf-8
    mysql_set_character_set(&mysql, "utf8");
    {
        QueryResult* res = query(
            &mysql,
            "select user_number, user_group_name from qq_group_msg limit 10");
        for (size_t i = 0; i < res->length; i++) {
            for (size_t j = 0; j < res->fieldsLength; j++) {
                printf("%s: %s\t", res->fieldsNames[j], res->rows[i].fields[j]);
            }
            printf("\n");
        }
        freeQueryResult(res);
    }

    return EXIT_SUCCESS;
}