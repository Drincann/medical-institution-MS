#include <stdio.h>
#include "./include/mysql.h"
#define string char*

// 函数执行结束的状态
enum State {
    OK,
    ERROR,
};

// MYSQL 连接配置
struct MysqlConfig {
    string host;
    string user;
    string pass;
    string database;
    short port;
};

// 获取 MYSQL 连接配置
struct MysqlConfig getMysqlConfig() {
    struct MysqlConfig config;
    config.user = "root";
    config.database = "qqwork";
    config.host = "localhost";
    config.pass = "";
    config.port = 3306;

    return config;
}

// 封装的 mysql 查询结果的一部分
typedef struct QueryRow {
    size_t length;
    char** fields;
    char** fieldsNames;
} QueryRow;

// 封装的 mysql 查询结果
typedef struct QueryResult {
    size_t length;
    size_t fieldsLength;
    QueryRow* rows;
    char** fieldsNames;
} QueryResult;

// mysql 查询的封装
QueryResult* query(MYSQL* conn, string sql) {
    mysql_query(conn, sql);
    MYSQL_RES* res = mysql_store_result(conn);
    size_t rowsLength = mysql_num_rows(res);
    size_t fieldsLength = mysql_num_fields(res);

    QueryResult* result = (QueryResult*)malloc(sizeof(QueryResult));
    result->fieldsLength = fieldsLength;
    result->length = rowsLength;
    result->rows = (QueryRow*)malloc(sizeof(QueryRow) * rowsLength);

    result->fieldsNames = (char**)malloc(sizeof(char*) * fieldsLength);
    MYSQL_FIELD* fields = mysql_fetch_fields(res);
    for (size_t i = 0; i < fieldsLength; i++) {
        result->fieldsNames[i] = fields[i].name;
    }

    for (size_t i = 0; i < rowsLength; i++) {
        MYSQL_ROW row = mysql_fetch_row(res);
        result->rows[i].length = fieldsLength;
        result->rows[i].fields = (char**)malloc(sizeof(char*) * fieldsLength);
        for (size_t j = 0; j < fieldsLength; j++) {
            result->rows[i].fields[j] = row[j];
        }
    }

    mysql_free_result(res);
    return result;
}

// 释放查询结果
enum State freeQueryResult(QueryResult* result) {
    if (result == NULL) {
        return ERROR;
    }
    for (size_t i = 0; i < result->length; i++) {
        free(result->rows[i].fields);
    }
    free(result->rows);
    free(result);
    return OK;
}
