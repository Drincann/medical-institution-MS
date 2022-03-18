#include <stdio.h>
#include <string.h>

#include "./db.c"

enum State testAndPrint(string testingname, uint64_t affected) {
  if (affected == -1) {
    printf("testcase: %s error x\n\n", testingname);
    printf(getError());
    return ERR;
  }
  if (affected == 0) {
    printf("testcase: %s error x\n\n", testingname);
    return ERR;
  }
  return OK;
}

enum State assertQueryResult(QueryResult* res,
                             string fieldName,
                             string fieldValue) {
  if (!res) {
    return ERR;
  }
  for (size_t i = 0; i < res->length; i++) {
    for (size_t j = 0; j < res->fieldsLength; j++) {
      if (strcmp(res->fieldsNames[j], fieldName) == 0) {
        if (strcmp(res->rows[i].fields[j], fieldValue) == 0) {
          return OK;
        }
      }
    }
  }
  return ERR;
}

void testQuery() {
  printf("testcase: query\n");
  QueryResult* res = query("select * from patient limit 10");
  if (!res) {
    printf("testcase: query error x\n\n");
    return;
  }
  if (res->length != 1) {
    printf("testcase: query error x\n\n");
    return;
  }

  printf("testcase: query √\n\n");
  freeQueryResult(res);
}

void testQueryPatientByUsername() {
  printf("testcase: queryPatientByUsername\n");
  QueryResult* res = queryPatientByUsername("patient1");
  if (!res) {
    printf("testcase: queryPatientByUsername error x\n\n");
    return;
  }

  printf("testcase: queryPatientByUsername √\n\n");
  freeQueryResult(res);
}
void testQueryPatient() {
  printf("testcase: queryPatient\n");
  QueryResult* res = queryPatient(10);
  if (!res) {
    printf("testcase: queryPatient error x\n\n");
    return;
  }

  printf("testcase: queryPatient √\n\n");
  freeQueryResult(res);
}

void testQueryStaffByUsername() {
  printf("testcase: queryStaffByUsername\n");
  QueryResult* res = queryStaffByUsername("admin");
  if (!res) {
    printf("testcase: queryStaffByUsername error x\n\n");
    return;
  }

  printf("testcase: queryStaffByUsername √\n\n");
  freeQueryResult(res);
}

void testQueryStaff() {
  printf("testcase: queryStaff\n");
  QueryResult* res = queryStaff(10);
  if (!res) {
    printf("testcase: queryStaff error x\n\n");
    return;
  }

  printf("testcase: queryStaff √\n\n");
  freeQueryResult(res);
}

void testQueryDayTurnover() {
  printf("testcase: queryDayTurnover\n");
  double res = queryDayTurnover("2022-03-16");

  if (res == -1) {
    printf("testcase: queryDayTurnover error x\n\n");
    printf(getError());
    return;
  }
  printf("dayTurnover -> %lf\n", res);
  printf("testcase: queryDayTurnover √\n\n");
}

void testQueryMonthTurnover() {
  printf("testcase: queryMonthTurnover\n");
  double res = queryMonthTurnover("2022-03-01");

  if (res == -1) {
    printf("testcase: queryMonthTurnover error x\n\n");
    printf(getError());
    return;
  }
  printf("monthTurnover -> %lf\n", res);
  printf("testcase: queryMonthTurnover √\n\n");
}

void testQueryQuarterTurnover() {
  printf("testcase: queryQuarterTurnover\n");
  double res = queryQuarterTurnover("2022-03-01");

  if (res == -1) {
    printf("testcase: queryQuarterTurnover error x\n\n");
    printf(getError());
    return;
  }
  printf("quarterTurnover -> %lf\n", res);
  printf("testcase: queryQuarterTurnover √\n\n");
}

void testYearTurnover() {
  printf("testcase: queryYearTurnover\n");
  double res = queryYearTurnover("2022-01-01");

  if (res == -1) {
    printf("testcase: queryYearTurnover error x\n\n");
    printf(getError());
    return;
  }
  printf("yearTurnover -> %lf\n", res);
  printf("testcase: queryYearTurnover √\n\n");
}

void testDeletePatientByUsername() {
  printf("testcase: deletePatientByUsername\n");
  uint64_t affected = insertPatient("testPatient", "testpass", "testtext", 0);
  if (testAndPrint("deletePatientByUsername", affected) == ERR)
    return;
  affected = deletePatientByUsername("testPatient");
  if (testAndPrint("deletePatientByUsername", affected) == ERR)
    return;
  printf("testcase: deletePatientByUsername √\n\n");
}

void testAlterPatientPasswordByUsername() {
  printf("testcase: alterPatientPasswordByUsername\n");
  uint64_t affected = insertPatient("testPatient", "testpass", "testtext", 0);
  if (testAndPrint("alterPatientPasswordByUsername", affected) == ERR)
    return;
  affected = alterPatientPasswordByUsername("testPatient", "testpass1");
  if (testAndPrint("alterPatientPasswordByUsername", affected) == ERR)
    return;
  if (assertQueryResult(queryPatientByUsername("testPatient"), "password",
                        "testpass1") == ERR) {
    printf("testcase: alterPatientPasswordByUsername error x\n\n");
    return;
  }
  affected = deletePatientByUsername("testPatient");
  if (testAndPrint("alterPatientPasswordByUsername", affected) == ERR)
    return;
  printf("testcase: alterPatientPasswordByUsername √\n\n");
}

void testAlterPatientSituationByUsername() {
  printf("testcase: alterPatientSituationByUsername\n");
  uint64_t affected = insertPatient("testPatient", "testpass", "testtext", 0);
  if (testAndPrint("alterPatientSituationByUsername", affected) == ERR)
    return;
  affected =
      alterPatientSituationByUsername("testPatient", "situation test text");
  if (testAndPrint("alterPatientSituationByUsername", affected) == ERR)
    return;
  if (assertQueryResult(queryPatientByUsername("testPatient"), "situation",
                        "situation test text") == ERR) {
    printf("testcase: alterPatientSituationByUsername error x\n\n");
    return;
  }

  affected = deletePatientByUsername("testPatient");
  if (testAndPrint("alterPatientSituationByUsername", affected) == ERR)
    return;
  printf("testcase: alterPatientSituationByUsername √\n\n");
}

void testAlterPatientInjectionByUsername() {
  printf("testcase: alterPatientInjectionByUsername\n");
  uint64_t affected = insertPatient("testPatient", "testpass", "testtext", 0);
  if (testAndPrint("alterPatientInjectionByUsername", affected) == ERR)
    return;
  affected = alterPatientInjectedByUsername("testPatient", 1);
  if (testAndPrint("alterPatientInjectionByUsername", affected) == ERR)
    return;
  if (assertQueryResult(queryPatientByUsername("testPatient"), "injected",
                        "1") == ERR) {
    printf("testcase: alterPatientInjectionByUsername error x\n\n");
    return;
  }

  affected = deletePatientByUsername("testPatient");
  if (testAndPrint("alterPatientInjectionByUsername", affected) == ERR)
    return;

  printf("testcase: alterPatientInjectionByUsername √\n\n");
}

void testDeleteStaffByUsername() {
  printf("testcase: deleteStaffByUsername\n");
  uint64_t affected = insertStaff("testStaff", "testpass", 0);
  if (testAndPrint("deleteStaffByUsername", affected) == ERR)
    return;
  affected = deleteStaffByUsername("testStaff");
  if (testAndPrint("deleteStaffByUsername", affected) == ERR)
    return;
  printf("testcase: deleteStaffByUsername √\n\n");
}

void testAlterStaffPasswordByUsername() {
  printf("testcase: alterStaffPasswordByUsername\n");
  uint64_t affected = insertStaff("testStaff", "testpass", 0);
  if (testAndPrint("alterStaffPasswordByUsername", affected) == ERR)
    return;
  affected = alterStaffPasswordByUsername("testStaff", "testpass1");
  if (testAndPrint("alterStaffPasswordByUsername", affected) == ERR)
    return;
  if (assertQueryResult(queryStaffByUsername("testStaff"), "password",
                        "testpass1") == ERR) {
    printf("testcase: alterStaffPasswordByUsername error x\n\n");
    return;
  }

  affected = deleteStaffByUsername("testStaff");
  if (testAndPrint("alterStaffPasswordByUsername", affected) == ERR)
    return;
  printf("testcase: alterStaffPasswordByUsername √\n\n");
}

void testAlterStaffTypeByUsername() {
  printf("testcase: alterStaffTypeByUsername\n");
  uint64_t affected = insertStaff("testStaff", "testpass", 0);
  if (testAndPrint("alterStaffTypeByUsername", affected) == ERR)
    return;
  affected = alterStaffTypeByUsername("testStaff", 1);
  if (testAndPrint("alterStaffTypeByUsername", affected) == ERR)
    return;
  if (assertQueryResult(queryStaffByUsername("testStaff"), "type", "1") ==
      ERR) {
    printf("testcase: alterStaffTypeByUsername error x\n\n");
    return;
  }

  affected = deleteStaffByUsername("testStaff");
  if (affected == 0) {
    printf("testcase: alterStaffTypeByUsername error x\n\n");
    printf(getError());
    return;
  }
  printf("testcase: alterStaffTypeByUsername √\n\n");
}

void testQueryInfo() {
  printf("testcase: queryInfo\n");
  QueryResult* res = queryInfo();
  if (res == NULL) {
    printf("testcase: queryInfo error x\n\n");
    printf(getError());
    return;
  }
  if (res->length != 1) {
    printf("testcase: queryInfo error x\n\n");
    return;
  }
  printf("testcase: queryInfo √\n\n");
}

void testSignIn() {
  printf("testcase: signIn\n");
  uint64_t affected = insertStaff("testStaff", "testpass", 0);
  if (testAndPrint("signIn", affected) == ERR)
    return;
  affected = insertSignIn("testStaff", 0);
  if (testAndPrint("signIn", affected) == ERR)
    return;
  affected = deleteStaffByUsername("testStaff");
  if (testAndPrint("signIn", affected) == ERR)
    return;
  printf("testcase: signIn √\n\n");
}
void testInsertEmergency() {
  printf("testcase: insertEmergency\n");
  uint64_t affected = insertPatient("testPatient", "testpass", "testtext", 0);
  if (testAndPrint("insertEmergency", affected) == ERR)
    return;
  affected = insertEmergency("testPatient", "testtext");
  if (testAndPrint("insertEmergency", affected) == ERR)
    return;
  affected = deletePatientByUsername("testPatient");
  if (testAndPrint("insertEmergency", affected) == ERR)
    return;
  printf("testcase: insertEmergency √\n\n");
}

void testQueryEmergencyByUsername() {
  printf("testcase: queryEmergencyByUsername\n");
  uint64_t affected = insertPatient("testPatient", "testpass", "testtext", 0);
  if (testAndPrint("queryEmergencyByUsername", affected) == ERR)
    return;
  affected = insertEmergency("testPatient", "testtext");
  if (testAndPrint("queryEmergencyByUsername", affected) == ERR)
    return;
  if (assertQueryResult(queryEmergencyByUsername("testPatient", 1), "message",
                        "testtext") == ERR) {
    printf("testcase: queryEmergencyByUsername error x\n\n");
    return;
  }

  affected = deletePatientByUsername("testPatient");
  if (testAndPrint("queryEmergencyByUsername", affected) == ERR)
    return;
  printf("testcase: queryEmergencyByUsername √\n\n");
}

void testQueryEmergency() {
  printf("testcase: queryEmergency\n");
  uint64_t affected = insertPatient("testPatient", "testpass", "testtext", 0);
  if (testAndPrint("queryEmergency", affected) == ERR)
    return;
  affected = insertEmergency("testPatient", "testtext");
  if (testAndPrint("queryEmergency", affected) == ERR)
    return;
  if (assertQueryResult(queryEmergency(1), "message", "testtext") == ERR) {
    printf("testcase: queryEmergency error x\n\n");
    return;
  }

  affected = deletePatientByUsername("testPatient");
  if (testAndPrint("queryEmergency", affected) == ERR)
    return;
  printf("testcase: queryEmergency √\n\n");
}

int main() {
  initDB();
  testQuery();
  testQueryPatientByUsername();
  testQueryPatient();
  testQueryStaffByUsername();
  testQueryStaff();

  testQueryDayTurnover();
  testQueryMonthTurnover();
  testQueryQuarterTurnover();
  testYearTurnover();

  testDeletePatientByUsername();
  testAlterPatientPasswordByUsername();
  testAlterPatientSituationByUsername();
  testAlterPatientInjectionByUsername();

  testDeleteStaffByUsername();
  testAlterStaffPasswordByUsername();
  testAlterStaffTypeByUsername();

  testQueryInfo();

  testSignIn();

  testInsertEmergency();
  testQueryEmergencyByUsername();
  testQueryEmergency();

  return EXIT_SUCCESS;
}