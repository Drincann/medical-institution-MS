#include <stdio.h>

#include "./db.c"
void testQuery() {
    printf("testcase: query\n");
    QueryResult* res = query("select * from patient limit 10");
    if (!res) {
        printf("testcase: query error x\n\n");
        return;
    }
    for (size_t i = 0; i < res->length; i++) {
        for (size_t j = 0; j < res->fieldsLength; j++) {
            printf("%s: %s\t", res->fieldsNames[j], res->rows[i].fields[j]);
        }
        printf("\n");
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
    for (size_t i = 0; i < res->length; i++) {
        for (size_t j = 0; j < res->fieldsLength; j++) {
            printf("%s: %s\t", res->fieldsNames[j], res->rows[i].fields[j]);
        }
        printf("\n");
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
    for (size_t i = 0; i < res->length; i++) {
        for (size_t j = 0; j < res->fieldsLength; j++) {
            printf("%s: %s\t", res->fieldsNames[j], res->rows[i].fields[j]);
        }
        printf("\n");
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
    for (size_t i = 0; i < res->length; i++) {
        for (size_t j = 0; j < res->fieldsLength; j++) {
            printf("%s: %s\t", res->fieldsNames[j], res->rows[i].fields[j]);
        }
        printf("\n");
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
    for (size_t i = 0; i < res->length; i++) {
        for (size_t j = 0; j < res->fieldsLength; j++) {
            printf("%s: %s\t", res->fieldsNames[j], res->rows[i].fields[j]);
        }
        printf("\n");
    }
    printf("testcase: queryStaff √\n\n");
    freeQueryResult(res);
}

void testQueryDayTurnover() {
    printf("testcase: queryDayTurnover\n");
    double res = queryDayTurnover("2022-03-16");

    if (res == -1) {
        printf("testcase: queryDayTurnover error x\n\n");
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
        return;
    }
    printf("yearTurnover -> %lf\n", res);
    printf("testcase: queryYearTurnover √\n\n");
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

    return EXIT_SUCCESS;
}