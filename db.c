#pragma once
#include <stdio.h>
#include <string.h>
#include "./include/mysql.h"
#include "./typedef.c"

// MYSQL connection config
typedef struct MysqlConfig {
    string host;
    string user;
    string pass;
    string database;
    short port;
} MysqlConfig;

// part of the custom MYSQL Result
typedef struct QueryRow {
    size_t length;
    char** fields;
    char** fieldsNames;
} QueryRow;

// custom MYSQL Result
typedef struct QueryResult {
    size_t length;
    size_t fieldsLength;
    QueryRow* rows;
    char** fieldsNames;
} QueryResult;

// output the result of a QueryResult object for debug
void showQueryResult(QueryResult* res) {
    if (res == NULL) {
        printf("NULL\n");
        return;
    }

    printf("length: %zu\n", res->length);
    printf("fieldsLength: %zu\n", res->fieldsLength);

    for (size_t rowidx = 0; rowidx < res->length; rowidx++) {
        for (size_t fieldidx = 0; fieldidx < res->fieldsLength; fieldidx++) {
            printf("%s: %s\t", res->fieldsNames[fieldidx],
                   res->rows[rowidx].fields[fieldidx]);
        }
        printf("\n");
    }
}

// get the value of field with a given name(fieldName) and row index from a
// QueryResult
string getFieldValue(QueryResult* res, string fieldName, size_t rowidx) {
    if (res == NULL) {
        return NULL;
    }
    if (res->length <= rowidx) {
        return NULL;
    }

    for (size_t fieldidx = 0; fieldidx < res->fieldsLength; fieldidx++) {
        if (strcmp(res->fieldsNames[fieldidx], fieldName) == 0) {
            return res->rows[rowidx].fields[fieldidx];
        }
    }

    return NULL;
}

// single-instance MYSQL connection
MYSQL globalConn;
// sql string buffer
char* globalSqlBuffer = NULL;

// config the MYSQL connection
struct MysqlConfig getMysqlConfig() {
    struct MysqlConfig config;
    config.user = "root";
    config.database = "mysql";
    config.host = "localhost";
    config.pass = "root";
    config.port = 3306;

    return config;
}

// init the databases
string getTablesSQL() {
    return "create table if not exists Staff(\n"
           "username varchar(255) not null,\n"
           "password varchar(255) not null,\n"
           "type int not null,\n"
           "-- 0 admin 1 front 2 doctor 3 nurse 4 buyer\n"
           "primary key(username)\n"
           ");\n"
           "create table if not exists Patient(\n"
           "username varchar(255) not null,\n"
           "password varchar(255) not null,\n"
           "-- from table VisitRecord -> visited  tinyint(1) not null,\n"
           "situation varchar(1023) not null DEFAULT '',\n"
           "injected tinyint(1) not null DEFAULT 0,\n"
           "-- 0 for not 1 for yes\n"
           "primary key(username)\n"
           ");\n"
           "create table if not exists FeedBackToPatient(\n"
           "id int not null auto_increment,\n"
           "patient varchar(255) not null,\n"
           "doctor varchar(255) not null,\n"
           "advice varchar(1023) not null,\n"
           "primary key(id),\n"
           "foreign key (patient) references Patient(username) on delete \n"
           "cascade,\n"
           "foreign key (doctor) references Staff(username) on delete cascade\n"
           ");\n"
           "create table if not exists FeedBackToDoctor(\n"
           "id int not null auto_increment,\n"
           "patient varchar(255) not null,\n"
           "doctor varchar(255) not null,\n"
           "situation varchar(1023) not null,\n"
           "primary key(id),\n"
           "foreign key (patient) REFERENCES Patient(username) on delete \n"
           "cascade,\n"
           "foreign key (doctor) REFERENCES Staff(username) on delete cascade\n"
           ");\n"
           "create table if not exists SignIn(\n"
           "id int not null auto_increment,\n"
           "staffUsername varchar(255) not null,\n"
           "type int not null,\n"
           "createTime DATETIME not null DEFAULT CURRENT_TIMESTAMP,\n"
           "-- 0 signin 1 signout\n"
           "primary key(id),\n"
           "foreign key (staffUsername) REFERENCES Staff(username) on delete \n"
           "cascade\n"
           ");\n"
           "create table if not exists Info(\n"
           "id int not null auto_increment,\n"
           "name varchar(255) not null,\n"
           "-- 医院名称\n"
           "primary key(id)\n"
           ");\n"
           "create table if not exists Reservation(\n"
           "id int not null auto_increment,\n"
           "patient varchar(255) not null,\n"
           "doctor varchar(255) not null,\n"
           "-- optional -> time varchar(255) not null,\n"
           "primary key(id),\n"
           "foreign key (patient) REFERENCES Patient(username) on delete \n"
           "cascade,\n"
           "foreign key (doctor) REFERENCES Staff(username) on delete cascade\n"
           ");\n"
           "create table if not exists VisitRecord(\n"
           "id int not null auto_increment,\n"
           "patient varchar(255) not null,\n"
           "doctor varchar(255) not null,\n"
           "-- optional -> time varchar(255) not null,\n"
           "primary key(id),\n"
           "foreign key (patient) REFERENCES Patient(username) on delete \n"
           "cascade,\n"
           "foreign key (doctor) REFERENCES Staff(username) on delete cascade\n"
           ");\n"
           "create table if not exists MedicationRecord(\n"
           "id int not null auto_increment,\n"
           "patient varchar(255) not null,\n"
           "doctor varchar(255) not null,\n"
           "dosage varchar(255) not null,\n"
           "primary key(id),\n"
           "foreign key (patient) REFERENCES Patient(username) on delete \n"
           "cascade,\n"
           "foreign key (doctor) REFERENCES Staff(username) on delete cascade\n"
           ");\n"
           "create table if not exists NurseMemo(\n"
           "id int not null auto_increment,\n"
           "message varchar(255) not null,\n"
           "nurseUsername varchar(255) not null,\n"
           "primary key(id),\n"
           "foreign key (nurseUsername) REFERENCES Staff(username) on delete \n"
           "cascade\n"
           ");\n"
           "create table if not exists CashierRecord(\n"
           "id int not null auto_increment,\n"
           "patient varchar(255) not null,\n"
           "amount float not null,\n"
           "createTime datetime not null DEFAULT CURRENT_TIMESTAMP,\n"
           "primary key(id),\n"
           "foreign key (patient) REFERENCES Patient(username) on delete \n"
           "cascade\n"
           ");\n"
           "create table if not exists PurchaseRecord(\n"
           "id int not null auto_increment,\n"
           "message varchar(255) not null,\n"
           "primary key(id)\n"
           ");\n"
           "create table if not exists Emergency(\n"
           "id int not null auto_increment,\n"
           "patient varchar(255) not null,\n"
           "message varchar(255) not null,\n"
           "primary key(id),\n"
           "foreign key (patient) REFERENCES Patient(username) on delete \n"
           "cascade\n"
           ");\n"
           "create table if not exists VIP(\n"
           "id int not null auto_increment,\n"
           "patient varchar(255) not null,\n"
           "points int not null DEFAULT 0,\n"
           "primary key(id),\n"
           "foreign key (patient) REFERENCES Patient(username) on delete \n"
           "cascade,\n"
           "unique(patient)\n"
           ");"
           "insert into Staff(username, password, type) values('admin', "
           "'admin', "
           "0);\n";
}

// init database
MYSQL* initDB() {
    struct MysqlConfig config = getMysqlConfig();

    // init
    globalSqlBuffer = (char*)malloc(sizeof(char) * 1024);

    // connecting
    mysql_init(&globalConn);
    if (!mysql_real_connect(&globalConn, config.host, config.user, config.pass,
                            config.database, config.port, NULL, 0)) {
        fprintf(stderr, "%s", mysql_error(&globalConn));
        exit(1);
    }

    // create database ms
    if (mysql_query(&globalConn, "create database if not exists ms")) {
        fprintf(stderr, "%s", mysql_error(&globalConn));
        exit(1);
    }

    // use ms
    if (mysql_query(&globalConn, "use ms")) {
        fprintf(stderr, "%s", mysql_error(&globalConn));
        exit(1);
    }

    // utf-8
    mysql_set_character_set(&globalConn, "utf8");

    // create tables
    mysql_set_server_option(&globalConn, MYSQL_OPTION_MULTI_STATEMENTS_ON);
    if (mysql_query(&globalConn, getTablesSQL())) {
        //
        fprintf(stderr, "%s", mysql_error(&globalConn));
    }
    mysql_set_server_option(&globalConn, MYSQL_OPTION_MULTI_STATEMENTS_OFF);

    // process all the results to sync the database
    MYSQL_RES* res;
    do {
        res = mysql_store_result(&globalConn);
        if (res) {
            mysql_free_result(res);
        }

    } while (!mysql_next_result(&globalConn));
    return &globalConn;
}

// mysql query using the QueryResult struct for its return type
QueryResult* query(string sql) {
    mysql_query(&globalConn, sql);
    MYSQL_RES* res = mysql_store_result(&globalConn);
    if (res == NULL)
        return NULL;
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

// release the memory of given QueryResult ptr
enum State freeQueryResult(QueryResult* result) {
    if (result == NULL) {
        return ERR;
    }
    for (size_t i = 0; i < result->length; i++) {
        free(result->rows[i].fields);
    }
    free(result->rows);
    free(result);
    return OK;
}

const string getError() {
    return mysql_error(&globalConn);
}

// all of query functions we need
// patient
QueryResult* queryPatientByUsername(string username) {  // testing covered
    sprintf(globalSqlBuffer, "select * from patient where username = '%s'",
            username);
    return query(globalSqlBuffer);
}

QueryResult* queryPatient(int limit) {  // testing covered
    if (limit < 0) {
        sprintf(globalSqlBuffer, "select * from patient");
        return query(globalSqlBuffer);
    }

    sprintf(globalSqlBuffer, "select * from patient limit %d", limit);
    return query(globalSqlBuffer);
}

uint64_t deletePatientByUsername(string username) {  // testing covered
    sprintf(globalSqlBuffer, "delete from patient where username = '%s'",
            username);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

uint64_t alterPatientPasswordByUsername(string username,
                                        string password) {  // testing covered
    sprintf(globalSqlBuffer,
            "update patient set password = '%s' where username = '%s'",
            password, username);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

uint64_t alterPatientSituationByUsername(string username,
                                         string situation) {  // testing covered
    sprintf(globalSqlBuffer,
            "update patient set situation = '%s' where username = '%s'",
            situation, username);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

uint64_t alterPatientInjectedByUsername(string username,
                                        int injected) {  // testing covered
    sprintf(globalSqlBuffer,
            "update patient set injected = %d where username = '%s'", injected,
            username);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

uint64_t insertPatient(string username,
                       string password,
                       string situation,
                       int injected) {  // testing covered
    sprintf(globalSqlBuffer,
            "insert into patient(username, password, situation, injected) "
            "values('%s', '%s', '%s', %d)",
            username, password, situation, injected);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

// staff
QueryResult* queryStaffByUsername(string username) {  // testing covered
    sprintf(globalSqlBuffer, "select * from staff where username = '%s'",
            username);
    return query(globalSqlBuffer);
}
QueryResult* queryStaffByType(int type) {  // testing covered
    sprintf(globalSqlBuffer, "select * from staff where type = '%d'", type);
    return query(globalSqlBuffer);
}
QueryResult* queryStaff(int limit) {  // testing covered
    if (limit < 0) {
        sprintf(globalSqlBuffer, "select * from staff");
        return query(globalSqlBuffer);
    }

    sprintf(globalSqlBuffer, "select * from staff limit %d", limit);
    return query(globalSqlBuffer);
}
uint64_t deleteStaffByUsername(string username) {  // testing covered
    sprintf(globalSqlBuffer, "delete from staff where username = '%s'",
            username);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

uint64_t alterStaffPasswordByUsername(string username,
                                      string password) {  // testing covered
    sprintf(globalSqlBuffer,
            "update staff set password = '%s' where username = '%s'", password,
            username);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

uint64_t alterStaffTypeByUsername(string username,
                                  int type) {  // testing covered
    sprintf(globalSqlBuffer, "update staff set type = %d where username = '%s'",
            type, username);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

uint64_t insertStaff(string username,
                     string password,
                     int type) {  // testing covered
    sprintf(
        globalSqlBuffer,
        "insert into staff(username, password, type) values('%s', '%s', %d)",
        username, password, type);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

// info
QueryResult* queryInfo() {  // testing covered
    sprintf(globalSqlBuffer, "select * from info limit 1");
    return query(globalSqlBuffer);
}

// signin
uint64_t insertSignIn(string username, int type) {  // testing covered
    sprintf(globalSqlBuffer,
            "insert into signin(staffUsername, type) values('%s', %d)",
            username, type);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

// emergency
uint64_t insertEmergency(string username, string message) {  // testing covered
    sprintf(globalSqlBuffer,
            "insert into emergency(patient, message) values('%s', '%s')",
            username, message);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

QueryResult* queryEmergencyByUsername(string username,
                                      int limit) {  // testing covered
    if (limit < 0) {
        sprintf(globalSqlBuffer, "select * from emergency where patient = '%s'",
                username);
        return query(globalSqlBuffer);
    }
    sprintf(globalSqlBuffer,
            "select * from emergency where patient = '%s' limit %d", username,
            limit);
    return query(globalSqlBuffer);
}

QueryResult* queryEmergency(int limit) {  // testing covered
    if (limit < 0) {
        sprintf(globalSqlBuffer, "select * from emergency");
        return query(globalSqlBuffer);
    }
    sprintf(globalSqlBuffer, "select * from emergency limit %d", limit);
    return query(globalSqlBuffer);
}

// cashierRecord
double queryDayTurnover(string date) {  // testing covered
    sprintf(globalSqlBuffer,
            "select sum(amount) from cashierrecord "
            "where TO_DAYS(createTime) = TO_DAYS('%s')",
            date);
    QueryResult* result = query(globalSqlBuffer);
    if (result == NULL) {
        return -1;
    }

    if (result->length == 0 || result->rows[0].fields[0] == NULL) {
        return 0;
    }

    // showQueryResult(result);
    double returnVal = atof(result->rows[0].fields[0]);
    // fixed .2
    returnVal = ((int)(returnVal * 100)) / 100.0;
    freeQueryResult(result);
    return returnVal;
}
double queryMonthTurnover(string date) {  // testing covered
    sprintf(globalSqlBuffer,
            "select sum(amount) from cashierrecord "
            "where MONTH(createTime) = MONTH('%s')",
            date);
    QueryResult* result = query(globalSqlBuffer);
    if (result == NULL) {
        return -1;
    }
    // showQueryResult(result);

    if (result->length == 0 || result->rows[0].fields[0] == NULL) {
        return 0;
    }

    double returnVal = atof(result->rows[0].fields[0]);
    // fixed .2
    returnVal = ((int)(returnVal * 100)) / 100.0;
    freeQueryResult(result);
    return returnVal;
}

double queryQuarterTurnover(string date) {  // testing covered
    sprintf(globalSqlBuffer,
            "select sum(amount) from cashierrecord "
            "where QUARTER(createTime) = QUARTER('%s')",
            date);
    QueryResult* result = query(globalSqlBuffer);
    if (result == NULL) {
        return -1;
    }

    if (result->length == 0 || result->rows[0].fields[0] == NULL) {
        return 0;
    }

    // showQueryResult(result);
    double returnVal = atof(result->rows[0].fields[0]);
    // fixed .2
    returnVal = ((int)(returnVal * 100)) / 100.0;
    freeQueryResult(result);
    return returnVal;
}

double queryYearTurnover(string date) {  // testing covered
    sprintf(globalSqlBuffer,
            "select sum(amount) from cashierrecord "
            "where YEAR(createTime) = YEAR('%s')",
            date);
    QueryResult* result = query(globalSqlBuffer);
    if (result == NULL) {
        return -1;
    }

    if (result->length == 0 || result->rows[0].fields[0] == NULL) {
        return 0;
    }

    double returnVal = atof(result->rows[0].fields[0]);
    // fixed .2
    returnVal = ((int)(returnVal * 100)) / 100.0;
    freeQueryResult(result);
    return returnVal;
}

int64_t insertCashierRecord(string patient, double amount) {
    sprintf(globalSqlBuffer,
            "insert into cashierrecord(patient, amount) values('%s', %f)",
            patient, amount);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

// VIP
QueryResult* queryVIP(int limit) {
    if (limit < 0) {
        sprintf(globalSqlBuffer, "select * from vip");
        return query(globalSqlBuffer);
    }

    sprintf(globalSqlBuffer, "select * from vip limit %d", limit);
    return query(globalSqlBuffer);
}

QueryResult* queryVIPByUsername(string username) {
    sprintf(globalSqlBuffer, "select * from vip where patient = '%s'",
            username);
    return query(globalSqlBuffer);
}

uint64_t insertVIP(string patientUsername) {
    sprintf(globalSqlBuffer, "insert into vip(patient) values('%s')",
            patientUsername);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

uint64_t alterVIPPointsByUsername(string username, int points) {
    sprintf(globalSqlBuffer,
            "update vip set points = points + %d where patient = '%s'", points,
            username);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

uint64_t deleteVIPByUsername(string username) {
    sprintf(globalSqlBuffer, "delete from vip where patient = '%s'", username);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

// reservation
QueryResult* queryReservationByDoctorUsername(string username) {
    sprintf(globalSqlBuffer, "select * from reservation where doctor = '%s'",
            username);

    return query(globalSqlBuffer);
}

uint64_t insertReservation(string patientUsername, string doctorUsername) {
    sprintf(globalSqlBuffer,
            "insert into reservation(patient, doctor) "
            "values('%s', '%s')",
            patientUsername, doctorUsername);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}
uint64_t deleteReservationByUsername(string patientUsername,
                                     string doctorUsername) {
    sprintf(globalSqlBuffer,
            "delete from reservation where patient = '%s' and doctor = '%s'",
            patientUsername, doctorUsername);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

// visitRecord
uint64_t insertVisitRecord(string patientUsername, string doctorUsername) {
    sprintf(globalSqlBuffer,
            "insert into visitrecord(patient, doctor) "
            "values('%s', '%s')",
            patientUsername, doctorUsername);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

QueryResult* queryVisitRecordByPatientUsername(string username) {
    sprintf(globalSqlBuffer, "select * from visitrecord where patient = '%s'",
            username);
    return query(globalSqlBuffer);
}

QueryResult* queryVisitRecordByDoctorUsername(string username) {
    sprintf(globalSqlBuffer, "select * from visitrecord where doctor = '%s'",
            username);
    return query(globalSqlBuffer);
}

// MedicineRecord
int64_t insertMedicationRecord(string patient, string doctor, string dosage) {
    sprintf(globalSqlBuffer,
            "insert into medicationrecord(patient, doctor, dosage) "
            "values('%s', '%s', '%s')",
            patient, doctor, dosage);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

QueryResult* queryMedicationRecordByPatientUsername(string patientUsername) {
    sprintf(globalSqlBuffer,
            "select * from medicationrecord where patient = '%s'",
            patientUsername);
    return query(globalSqlBuffer);
}

// nurseMemo
uint64_t insertNurseMemo(string nurseUsername, string message) {
    sprintf(globalSqlBuffer,
            "insert into nursememo(nurseUsername, message) "
            "values('%s', '%s')",
            nurseUsername, message);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

QueryResult* queryNurseMemoByUsername(string nurseUsername) {
    sprintf(globalSqlBuffer,
            "select * from nursememo where nurseUsername = '%s'",
            nurseUsername);
    return query(globalSqlBuffer);
}

QueryResult* queryNurseMemo(int limit) {
    if (limit < 0) {
        sprintf(globalSqlBuffer, "select * from nursememo");
        return query(globalSqlBuffer);
    }

    sprintf(globalSqlBuffer, "select * from nursememo limit %d", limit);
    return query(globalSqlBuffer);
}

// purchaseRecord
int64_t insertPurchaseRecord(string message) {
    sprintf(globalSqlBuffer,
            "insert into purchaserecord(message) "
            "values('%s')",
            message);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

QueryResult* queryPurchaseRecord(int limit) {
    if (limit < 0) {
        sprintf(globalSqlBuffer, "select * from purchaserecord");
        return query(globalSqlBuffer);
    }

    sprintf(globalSqlBuffer, "select * from purchaserecord limit %d", limit);
    return query(globalSqlBuffer);
}

// feedbacktodoctor
int64_t insertFeedbackToDoctor(string patient,
                               string doctor,
                               string situation) {
    sprintf(globalSqlBuffer,
            "insert into feedbacktodoctor(patient, doctor, situation) "
            "values('%s', '%s', '%s')",
            patient, doctor, situation);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

QueryResult* queryFeedbackToDoctorByDoctorUsername(string doctor) {
    sprintf(globalSqlBuffer,
            "select * from feedbacktodoctor where doctor = '%s'", doctor);
    return query(globalSqlBuffer);
}

// feedbacktopatient
int64_t insertFeedbackToPatient(string patient, string doctor, string advice) {
    sprintf(globalSqlBuffer,
            "insert into feedbacktopatient(patient, doctor, advice) "
            "values('%s', '%s', '%s')",
            patient, doctor, advice);
    mysql_query(&globalConn, globalSqlBuffer);
    return mysql_affected_rows(&globalConn);
}

QueryResult* queryFeedbackToPatientByPatientUsername(string patient) {
    sprintf(globalSqlBuffer,
            "select * from feedbacktopatient where patient = '%s'", patient);
    return query(globalSqlBuffer);
}
