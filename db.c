#pragma once
#include <stdio.h>
#include <string.h>
#include "./include/mysql.h"
#include "./typedef.c"

// MYSQL 连接配置
typedef struct MysqlConfig {
  string host;
  string user;
  string pass;
  string database;
  short port;
} MysqlConfig;

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

// 全局 connection, 事实上的单例
MYSQL globalConn;
// 全局公用的 sql string buffer
char* globalSqlBuffer = NULL;

// 获取 MYSQL 连接配置
struct MysqlConfig getMysqlConfig() {
  struct MysqlConfig config;
  config.user = "root";
  config.database = "ms";
  config.host = "localhost";
  config.pass = "";
  config.port = 3306;

  return config;
}

MYSQL* initDB() {
  struct MysqlConfig config = getMysqlConfig();
  globalSqlBuffer = (char*)malloc(sizeof(char) * 1024);
  mysql_init(&globalConn);

  if (!mysql_real_connect(&globalConn, config.host, config.user, config.pass,
                          config.database, config.port, NULL, 0)) {
    fprintf(stderr, "%s", mysql_error(&globalConn));
  }

  // utf-8
  mysql_set_character_set(&globalConn, "utf8");
  return &globalConn;
}

// mysql 查询的封装
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

// 释放查询结果
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

// 所有查询
// 病人信息
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
          "update patient set password = '%s' where username = '%s'", password,
          username);
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

// 员工信息
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
  sprintf(globalSqlBuffer, "delete from staff where username = '%s'", username);
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
  sprintf(globalSqlBuffer,
          "insert into staff(username, password, type) values('%s', '%s', %d)",
          username, password, type);
  mysql_query(&globalConn, globalSqlBuffer);
  return mysql_affected_rows(&globalConn);
}

// 医院信息
QueryResult* queryInfo() {  // testing covered
  sprintf(globalSqlBuffer, "select * from info limit 1");
  return query(globalSqlBuffer);
}

// 签到签退
uint64_t insertSignIn(string username, int type) {  // testing covered
  sprintf(globalSqlBuffer,
          "insert into signin(staffUsername, type) values('%s', %d)", username,
          type);
  mysql_query(&globalConn, globalSqlBuffer);
  return mysql_affected_rows(&globalConn);
}

// 紧急呼救
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

// 营业额 天/月/季度/年
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

// 收银
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
  sprintf(globalSqlBuffer, "select * from vip where patient = '%s'", username);
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

// 患者预约
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

// 就诊记录
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

// 用药记录
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

// 护士备忘
uint64_t insertNurseMemo(string nurseUsername, string message) {
  sprintf(globalSqlBuffer,
          "insert into nursememo(nurseUsername, message) "
          "values('%s', '%s')",
          nurseUsername, message);
  mysql_query(&globalConn, globalSqlBuffer);
  return mysql_affected_rows(&globalConn);
}

QueryResult* queryNurseMemoByUsername(string nurseUsername) {
  sprintf(globalSqlBuffer, "select * from nursememo where nurseUsername = '%s'",
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

// 采购记录
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

// 反馈到医生
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
  sprintf(globalSqlBuffer, "select * from feedbacktodoctor where doctor = '%s'",
          doctor);
  return query(globalSqlBuffer);
}

// 反馈到患者
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
