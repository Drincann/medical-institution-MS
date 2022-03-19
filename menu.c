#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./db.c"
#include "./typedef.c"

typedef enum UserType {
    ADMIN = 0,
    FRONT,
    DOCTOR,
    NURSE,
    BUYER,
    PATIENT,
} UserType;

typedef enum SignType {
    SIGNIN = 0,
    SIGNOUT,
} SignType;

typedef struct User {
    int type;
    char username[256];
} User;

typedef struct Menu {
    struct Menu* parent;
    User* currentUser;
    string itemsStr;
    // 状态转移
    struct Menu* (*exec)(string input);
    // 输出菜单
    void (*showMenu)();
} Menu;

Menu* abstractInsertSignIn(string username, SignType type, Menu* returnMenu) {
    uint64_t affected = insertSignIn(username, type);
    if (affected <= 0) {
        if (type == SIGNIN) {
            printf("签到失败，请按任意键返回。\n");
        } else {
            printf("签退失败，请按任意键返回。\n");
        }
        printf("error message: %s\n", getError());
        getchar();
        getchar();
        return returnMenu;
    }
    if (type == SIGNIN) {
        printf("签到成功，请按任意键返回。\n");
    } else {
        printf("签退成功，请按任意键返回。\n");
    }
    getchar();
    getchar();
    return returnMenu;
}

// the top level menu
// methods to controls the menu
Menu* topMenuExec(string input);
void topMenuShowMenu();

// top level menu object
Menu topMenu = {
    .currentUser = NULL,
    .parent = NULL,
    .itemsStr =
        "1.患者登录\n"
        "2.医疗人员登录\n"
        "0.退出\n",
    .exec = topMenuExec,
    .showMenu = topMenuShowMenu,
};

// doctor menu
Menu* doctorMenuExec(string input);
void doctorMenuShowMenu();

Menu doctorMenu = {
    .currentUser = NULL,
    .parent = &topMenu,
    .itemsStr =
        "1.查看预约/接诊\n"
        "2.查看患者情况\n"
        "3.查看患者反馈\n"
        "4.发送健康建议\n"
        "5.签到\n"
        "6.签退\n"
        "0.返回\n",
    .exec = doctorMenuExec,
    .showMenu = doctorMenuShowMenu,
};

// patient menu
Menu* patientMenuExec(string input);
void patientMenuShowMenu();

Menu patientMenu = {
    .currentUser = NULL,
    .parent = &topMenu,
    .itemsStr =
        "1.查询医院信息\n"
        "2.预约\n"
        "3.查询就诊记录\n"
        "4.查询用药记录\n"
        "5.查询疫苗注射情况\n"
        "6.向医生发送治疗反馈\n"
        "7.紧急呼救\n"
        "8.查询会员积分\n"
        "9.查询健康建议\n"
        "0.返回\n",
    .exec = patientMenuExec,
    .showMenu = patientMenuShowMenu,
};

// nurse menu
Menu* nurseMenuExec(string input);
void nurseMenuShowMenu();

Menu nurseMenu = {
    .currentUser = NULL,
    .parent = &topMenu,
    .itemsStr =
        "1.查询患者数据\n"
        "2.设置患者情况\n"
        "3.查询医护备忘\n"
        "4.添加医护备忘\n"
        "5.签到\n"
        "6.签退\n"
        "0.返回\n",
    .exec = nurseMenuExec,
    .showMenu = nurseMenuShowMenu,
};

// admin menu
Menu* adminMenuExec(string input);
void adminMenuShowMenu();

Menu adminMenu = {
    .currentUser = NULL,
    .parent = &topMenu,
    .itemsStr =
        "1.查询天营业额\n"
        "2.查询月营业额\n"
        "3.查询季度营业额\n"
        "4.查询年营业额\n"
        "5.查询员工账户\n"
        "6.删除员工账户\n"
        "7.添加员工账户\n"
        "8.查询医院信息\n"
        "9.查询会员\n"
        "10.删除会员\n"
        "11.签到\n"
        "12.签退\n"
        "0.返回\n",
    .exec = adminMenuExec,
    .showMenu = adminMenuShowMenu,
};

// front menu
Menu* frontMenuExec(string input);
void frontMenuShowMenu();

Menu frontMenu = {
    .currentUser = NULL,
    .parent = &topMenu,
    .itemsStr =
        "1.患者账户注册\n"
        "2.查看紧急呼叫\n"
        "3.收银结算\n"
        "4.查询会员积分\n"
        "5.开通会员\n"
        "6.签到\n"
        "7.签退\n"
        "0.返回\n",
    .exec = frontMenuExec,
    .showMenu = frontMenuShowMenu,
};

// buyer menu
Menu* buyerMenuExec(string input);
void buyerMenuShowMenu();

Menu buyerMenu = {
    .currentUser = NULL,
    .parent = &topMenu,
    .itemsStr =
        "1.记录补货信息\n"
        "2.查询补货记录\n"
        "3.签到\n"
        "4.签退\n"
        "0.返回\n",
    .exec = buyerMenuExec,
    .showMenu = buyerMenuShowMenu,
};

Menu* topMenuExec(string input) {
    char username[256] = {0};
    char password[256] = {0};

    if (atoi(input) == 1) {  // 患者登录

        printf("请输入用户名: ");
        scanf("%s", username);
        printf("请输入密码: ");
        scanf("%s", password);
        {
            QueryResult* res = queryPatientByUsername(username);
            if (res == NULL) {
                printf("发生错误，按任意键返回。\n");
                printf("error message: %s\n", getError());
                getchar();
                getchar();
                return &topMenu;
            }
            if (res->length == 0) {  // 账户是否存在
                printf("账户不存在, 按任意键返回。\n");
                getchar();
                getchar();
                return &topMenu;

            } else if (strcmp(getFieldValue(res, "password", 0), password) !=
                       0) {  // 对比密码
                printf("密码错误, 按任意键返回。\n");
                getchar();
                getchar();
                return &topMenu;

            } else {
                // 患者登录成功
                if (patientMenu.currentUser != NULL) {
                    free(patientMenu.currentUser);
                }
                patientMenu.currentUser = (User*)malloc(sizeof(User));
                patientMenu.currentUser->type = PATIENT;
                strcpy(patientMenu.currentUser->username, username);
                patientMenu.parent = &topMenu;
                return &patientMenu;
            }
            freeQueryResult(res);
        }
        // 未期望的状态
        return &topMenu;
    } else if (atoi(input) == 2) {  // 医疗人员登录

        printf("请输入用户名: ");
        scanf("%s", username);
        printf("请输入密码: ");
        scanf("%s", password);
        {
            QueryResult* res = queryStaffByUsername(username);
            if (res == NULL) {
                printf("发生错误，按任意键返回。\n");
                printf("error message: %s\n", getError());
                getchar();
                getchar();
                return &topMenu;
            }
            if (res->length == 0) {  // 账户是否存在
                printf("账户不存在, 按任意键返回。\n");
                getchar();
                getchar();
                return &topMenu;

            } else if (strcmp(getFieldValue(res, "password", 0), password) !=
                       0) {  // 对比密码
                printf("密码错误, 按任意键返回。\n");
                getchar();
                getchar();
                return &topMenu;

            } else {
                // 医疗人员登录成功
                UserType type = atoi(getFieldValue(res, "type", 0));
                switch (type) {
                    case DOCTOR:
                        if (doctorMenu.currentUser != NULL) {
                            free(doctorMenu.currentUser);
                        }
                        doctorMenu.currentUser = (User*)malloc(sizeof(User));
                        doctorMenu.currentUser->type =
                            atoi(getFieldValue(res, "type", 0));
                        strcpy(doctorMenu.currentUser->username, username);
                        doctorMenu.parent = &topMenu;
                        return &doctorMenu;
                    case NURSE:
                        if (nurseMenu.currentUser != NULL) {
                            free(nurseMenu.currentUser);
                        }
                        nurseMenu.currentUser = (User*)malloc(sizeof(User));
                        nurseMenu.currentUser->type =
                            atoi(getFieldValue(res, "type", 0));
                        strcpy(nurseMenu.currentUser->username, username);
                        nurseMenu.parent = &topMenu;
                        return &nurseMenu;
                    case ADMIN:
                        if (adminMenu.currentUser != NULL) {
                            free(adminMenu.currentUser);
                        }
                        adminMenu.currentUser = (User*)malloc(sizeof(User));
                        adminMenu.currentUser->type =
                            atoi(getFieldValue(res, "type", 0));
                        strcpy(adminMenu.currentUser->username, username);
                        adminMenu.parent = &topMenu;
                        return &adminMenu;
                    case FRONT:
                        if (frontMenu.currentUser != NULL) {
                            free(frontMenu.currentUser);
                        }
                        frontMenu.currentUser = (User*)malloc(sizeof(User));
                        frontMenu.currentUser->type =
                            atoi(getFieldValue(res, "type", 0));
                        strcpy(frontMenu.currentUser->username, username);
                        frontMenu.parent = &topMenu;
                        return &frontMenu;
                    case BUYER:
                        if (buyerMenu.currentUser != NULL) {
                            free(buyerMenu.currentUser);
                        }
                        buyerMenu.currentUser = (User*)malloc(sizeof(User));
                        buyerMenu.currentUser->type =
                            atoi(getFieldValue(res, "type", 0));
                        strcpy(buyerMenu.currentUser->username, username);
                        buyerMenu.parent = &topMenu;
                        return &buyerMenu;

                    default:
                        break;
                }
                printf("未知的用户类型，请按任意键返回。");
                getchar();
                getchar();
                return &topMenu;
            }
            freeQueryResult(res);
        }

        // 未期望的状态
        return &topMenu;
    } else if (atoi(input) == 0) {  // 退出程序
        exit(0);
        return NULL;
    }
    return &topMenu;
}

void topMenuShowMenu() {
    printf("\n%s\nMSshell$ ", topMenu.itemsStr);
}

Menu* doctorMenuExec(string input) {
    if (atoi(input) == 0) {  // 返回上级菜单

        return doctorMenu.parent;
    } else if (atoi(input) == 1) {  // 查看预约

        QueryResult* res =
            queryReservationByDoctorUsername(doctorMenu.currentUser->username);

        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &doctorMenu;
        }
        if (res->length == 0) {
            printf("您还没有收到预约, 按任意键返回。\n");
            getchar();
            getchar();
            return &doctorMenu;
        }
        printf("您收到的预约患者如下: \n");
        for (int i = 0; i < res->length; i++) {
            printf("%d. %s\n", i, getFieldValue(res, "patient", i));
        }
        printf("请输入要接诊的患者序号: ");
        int index;
        scanf("%d", &index);
        if (index < 0 || index >= res->length) {
            printf("输入错误，按任意键返回。\n");
            getchar();
            getchar();
            return &doctorMenu;
        }
        // 接诊
        string patientUsername = getFieldValue(res, "patient", index);
        uint64_t affected = insertVisitRecord(patientUsername,
                                              doctorMenu.currentUser->username);
        if (affected <= 0) {
            printf("接诊失败，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &doctorMenu;
        }
        affected = deleteReservationByUsername(
            patientUsername, doctorMenu.currentUser->username);
        if (affected <= 0) {
            printf("删除预约失败，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &doctorMenu;
        }
        freeQueryResult(res);
        return &doctorMenu;
    } else if (atoi(input) == 2) {  // 查看患者情况

        QueryResult* visitRecordRes =
            queryVisitRecordByDoctorUsername(doctorMenu.currentUser->username);
        if (visitRecordRes == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &doctorMenu;
        }
        if (visitRecordRes->length == 0) {
            printf("您还没有接诊任何患者, 按任意键返回。\n");
            getchar();
            getchar();
            return &doctorMenu;
        }
        printf("您接诊的患者情况如下: \n");
        for (int i = 0; i < visitRecordRes->length; i++) {
            QueryResult* res = queryPatientByUsername(
                getFieldValue(visitRecordRes, "patient", i));
            if (res->length <= 0) {
                --i;
                continue;
            }
            printf("%d. %s: %s\n", i, getFieldValue(res, "username", i),
                   getFieldValue(res, "situation", i));
            freeQueryResult(res);
        }
        printf("按任意键返回。\n");
        getchar();
        getchar();
        freeQueryResult(visitRecordRes);

        return &doctorMenu;
    } else if (atoi(input) == 3) {  // 查看患者反馈
        QueryResult* res = queryFeedbackToDoctorByDoctorUsername(
            doctorMenu.currentUser->username);
        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &doctorMenu;
        }
        if (res->length == 0) {
            printf("您还没有收到任何患者反馈, 按任意键返回。\n");
            getchar();
            getchar();
            return &doctorMenu;
        }
        printf("您收到的患者反馈如下: \n");
        for (int i = 0; i < res->length; i++) {
            printf("%d. 患者: %s, 反馈: %s\n", i,
                   getFieldValue(res, "patient", i),
                   getFieldValue(res, "situation", i));
        }
        printf("按任意键返回。\n");
        getchar();
        getchar();
        return &doctorMenu;
    } else if (atoi(input) == 4) {  // 发送健康建议

        char patient[256] = {0};
        char advice[1024] = {0};
        printf("请输入您已接诊的患者用户名: ");
        scanf("%s", patient);
        printf("请输入健康建议: ");
        scanf("%s", advice);
        uint64_t affected = insertFeedbackToPatient(
            patient, doctorMenu.currentUser->username, advice);
        if (affected <= 0) {
            printf("发送健康建议失败，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &doctorMenu;
        }
        return &doctorMenu;
    } else if (atoi(input) == 5) {  // 签到
        return abstractInsertSignIn(doctorMenu.currentUser->username, SIGNIN,
                                    &doctorMenu);
    } else if (atoi(input) == 6) {  // 签退
        return abstractInsertSignIn(doctorMenu.currentUser->username, SIGNOUT,
                                    &doctorMenu);
    }
    return &doctorMenu;
}

void doctorMenuShowMenu() {
    printf("\n%s\nMSshell$ ", doctorMenu.itemsStr);
}

Menu* patientMenuExec(string input) {
    if (atoi(input) == 0) {  // 返回上级菜单

        return patientMenu.parent;
    } else if (atoi(input) == 1) {  // 查询信息

        QueryResult* res = queryInfo();
        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &doctorMenu;
        }
        if (res->length == 0) {
            printf("信息为空\n");
            getchar();
            getchar();
            return &patientMenu;
        }
        printf("医院的信息如下: \n");
        printf("医院名称: %s\n", getFieldValue(res, "name", 0));
        printf("按任意键返回。\n");
        getchar();
        getchar();
        freeQueryResult(res);
        return &patientMenu;
    } else if (atoi(input) == 2) {  // 预约挂号
        // 查询所有医生
        QueryResult* res = queryStaffByType(DOCTOR);

        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &patientMenu;
        }
        if (res->length == 0) {
            printf("没有医生，按任意键返回。\n");
            getchar();
            getchar();
            return &patientMenu;
        }
        printf("所有医生如下: \n");
        for (int i = 0; i < res->length; i++) {
            printf("%d. %s\n", i, getFieldValue(res, "username", i));
        }
        // 预约
        printf("请输入要预约的医生序号: ");
        int index;
        scanf("%d", &index);
        if (index < 0 || index >= res->length) {
            printf("输入错误，按任意键返回。\n");
            getchar();
            getchar();
            return &patientMenu;
        }
        string doctorUsername = getFieldValue(res, "username", index);
        freeQueryResult(res);
        uint64_t affected = insertReservation(patientMenu.currentUser->username,
                                              doctorUsername);
        if (affected <= 0) {
            printf("预约失败，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &patientMenu;
        }
        return &patientMenu;
    } else if (atoi(input) == 3) {  // 查询就诊记录
        QueryResult* res = queryVisitRecordByPatientUsername(
            patientMenu.currentUser->username);
        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &patientMenu;
        }
        if (res->length == 0) {
            printf("未查询到就诊记录，按任意键返回。\n");
            getchar();
            getchar();
            return &patientMenu;
        }

        printf("您的就诊记录如下: \n");
        for (int i = 0; i < res->length; i++) {
            printf("%d. 医生: %s, 患者: %s\n", i,
                   getFieldValue(res, "doctor", i),
                   patientMenu.currentUser->username);
        }
        printf("按任意键返回。\n");
        getchar();
        getchar();
        freeQueryResult(res);
        return &patientMenu;
    } else if (atoi(input) == 4) {  // 查询用药记录
        QueryResult* res = queryMedicationRecordByPatientUsername(
            patientMenu.currentUser->username);
        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &patientMenu;
        }
        if (res->length == 0) {
            printf("未查询到用药记录，按任意键返回。\n");
            getchar();
            getchar();
            return &patientMenu;
        }
        printf("您的用药记录如下: \n");
        for (int i = 0; i < res->length; i++) {
            printf("%d. 医生: %s, 患者: %s, 用药说明: %s\n", i,
                   getFieldValue(res, "doctor", i),
                   patientMenu.currentUser->username,
                   getFieldValue(res, "dosage", i));
        }
        printf("按任意键返回。\n");
        getchar();
        getchar();
        freeQueryResult(res);
        return &patientMenu;
    } else if (atoi(input) == 5) {  // 查询疫苗注射情况

        QueryResult* res =
            queryPatientByUsername(patientMenu.currentUser->username);
        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &patientMenu;
        }
        if (res->length == 0) {
            printf("未查询到患者信息，按任意键返回。\n");
            getchar();
            getchar();
            return &patientMenu;
        }
        char isInjected = atoi(getFieldValue(res, "injected", 0));
        printf(isInjected ? "您已经注射过疫苗了。\n"
                          : "您还没有注射过疫苗。\n");
        printf("按任意键返回。\n");
        getchar();
        getchar();
        freeQueryResult(res);
        return &patientMenu;
    } else if (atoi(input) == 6) {  // 发送治疗反馈
        printf("请输入接诊医生用户名: ");
        char doctorUsername[256] = {0};
        scanf("%s", doctorUsername);
        printf("请输入治疗反馈: ");
        char feedback[1024] = {0};
        scanf("%s", feedback);
        uint64_t affected = insertFeedbackToDoctor(
            patientMenu.currentUser->username, doctorUsername, feedback);
        if (affected <= 0) {
            printf("发送治疗反馈失败，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &patientMenu;
        }
        return &patientMenu;
    } else if (atoi(input) == 7) {  // 紧急呼救
        printf("请输入紧急呼叫附加信息: ");
        char additionalInfo[256] = {0};
        scanf("%s", additionalInfo);
        uint64_t affected =
            insertEmergency(patientMenu.currentUser->username, additionalInfo);
        if (affected <= 0) {
            printf("发送紧急呼救失败，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &patientMenu;
        }
        return &patientMenu;
    } else if (atoi(input) == 8) {  // 查询会员积分
        QueryResult* res =
            queryVIPByUsername(patientMenu.currentUser->username);
        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &patientMenu;
        }
        if (res->length == 0) {
            printf("您尚未开通会员，请在前台办理注册，按任意键返回。\n");
            getchar();
            getchar();
            return &patientMenu;
        }
        printf("您的会员积分为: %s\n", getFieldValue(res, "points", 0));
        printf("按任意键返回。\n");
        getchar();
        getchar();
        freeQueryResult(res);
        return &patientMenu;
    } else if (atoi(input) == 9) {  // 查询健康建议
        QueryResult* res = queryFeedbackToPatientByPatientUsername(
            patientMenu.currentUser->username);
        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
        }
        if (res->length == 0) {
            printf("您还没有收到治疗反馈，按任意键返回。\n");
            getchar();
            getchar();
        }
        printf("您的治疗反馈如下: \n");
        for (int i = 0; i < res->length; i++) {
            printf("%d. 医生: %s, 患者: %s, 治疗反馈: %s\n", i,
                   getFieldValue(res, "doctor", i),
                   patientMenu.currentUser->username,
                   getFieldValue(res, "advice", i));
        }
        freeQueryResult(res);
        printf("按任意键返回。\n");
        getchar();
        getchar();
        return &patientMenu;
    }
    return &patientMenu;
}

void patientMenuShowMenu() {
    printf("\n%s\nMSshell$ ", patientMenu.itemsStr);
}

Menu* nurseMenuExec(string input) {
    char patient[256];
    if (atoi(input) == 0) {  // 返回上级菜单
        return nurseMenu.parent;
    } else if (atoi(input) == 1) {  // 查询患者数据
        printf("请输入患者用户名: ");
        scanf("%s", patient);
        QueryResult* res = queryPatientByUsername(patient);
        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &nurseMenu;
        }
        if (res->length == 0) {
            printf("未查询到该患者，按任意键返回。\n");
            getchar();
            getchar();
            return &nurseMenu;
        }
        printf("患者的信息如下: \n");
        printf("姓名: %s\n", getFieldValue(res, "username", 0));
        printf("患者情况: %s\n", getFieldValue(res, "situation", 0));
        printf("患者疫苗注射情况 %s\n",
               strcmp(getFieldValue(res, "injected", 0), "1") == 0 ? "已注射"
                                                                   : "未注射");
        printf("按任意键返回。\n");
        getchar();
        getchar();
        freeQueryResult(res);
        return &nurseMenu;
    } else if (atoi(input) == 2) {  // 设置患者情况
        printf("请输入患者用户名: ");
        scanf("%s", patient);
        printf("请输入患者情况: ");
        char situation[1024];
        scanf("%s", situation);
        uint64_t affected = alterPatientSituationByUsername(patient, situation);
        if (affected <= 0) {
            printf("设置失败，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &nurseMenu;
        }
        printf("设置成功，按任意键返回。\n");
        getchar();
        getchar();
        return &nurseMenu;
    } else if (atoi(input) == 3) {  // 查询医护备忘
        QueryResult* res =
            queryNurseMemoByUsername(nurseMenu.currentUser->username);
        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &nurseMenu;
        }
        if (res->length == 0) {
            printf("没有备忘，按任意键返回。\n");
            getchar();
            getchar();
            return &nurseMenu;
        }
        printf("医护备忘如下: \n");
        for (int i = 0; i < res->length; i++) {
            printf("%d. %s\n", i, getFieldValue(res, "message", i));
        }
        printf("按任意键返回。\n");
        getchar();
        getchar();
        freeQueryResult(res);
        return &nurseMenu;
    } else if (atoi(input) == 4) {  // 添加医护备忘
        printf("请输入备忘内容: ");
        char message[1024];
        scanf("%s", message);
        uint64_t affected =
            insertNurseMemo(nurseMenu.currentUser->username, message);
        if (affected <= 0) {
            printf("添加失败，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &nurseMenu;
        }
        printf("添加成功，按任意键返回。\n");
        getchar();
        getchar();
        return &nurseMenu;
    } else if (atoi(input) == 5) {  // 签到
        return abstractInsertSignIn(nurseMenu.currentUser->username, SIGNIN,
                                    &nurseMenu);
    } else if (atoi(input) == 6) {  // 签退
        return abstractInsertSignIn(nurseMenu.currentUser->username, SIGNOUT,
                                    &nurseMenu);
    }
    return &nurseMenu;
}

void nurseMenuShowMenu() {
    printf("\n%s\nMSshell$ ", nurseMenu.itemsStr);
}

Menu* adminMenuExec(string input) {
    if (atoi(input) == 0) {  // 返回上级菜单
        return adminMenu.parent;
    } else if (atoi(input) == 1) {  // 查询天营业额
        printf("请输入查询日期(完整日期, 例如 2022-03-20): ");
        char date[32];
        scanf("%s", date);

        double res = queryDayTurnover(date);
        if (res == -1) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &adminMenu;
        }
        printf("查询结果: %.2lf ￥\n", res);
        printf("按任意键返回。\n");
        getchar();
        getchar();
        return &adminMenu;
    } else if (atoi(input) == 2) {  // 查询月营业额
        printf("请输入查询日期(完整日期, 例如 2022-03-20): ");
        char date[32];
        scanf("%s", date);

        double res = queryMonthTurnover(date);
        if (res == -1) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &adminMenu;
        }
        printf("查询结果: %.2lf ￥\n", res);
        printf("按任意键返回。\n");
        getchar();
        getchar();
        return &adminMenu;
    } else if (atoi(input) == 3) {  // 查询季度营业额
        printf("请输入查询日期(完整日期, 例如 2022-03-20): ");
        char date[32];
        scanf("%s", date);

        double res = queryQuarterTurnover(date);
        if (res == -1) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &adminMenu;
        }
        printf("查询结果: %.2lf ￥\n", res);
        printf("按任意键返回。\n");
        getchar();
        getchar();
    } else if (atoi(input) == 4) {  // 查询年营业额
        printf("请输入查询日期(完整日期, 例如 2022-03-20): ");
        char date[32];
        scanf("%s", date);

        double res = queryYearTurnover(date);
        if (res == -1) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &adminMenu;
        }
        printf("查询结果: %.2lf ￥\n", res);
        printf("按任意键返回。\n");
        getchar();
        getchar();
        return &adminMenu;
    } else if (atoi(input) == 5) {  // 查询员工账户
        printf("请输入员工用户名: ");
        char username[256] = {0};
        scanf("%s", username);
        QueryResult* res = queryStaffByUsername(username);
        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &adminMenu;
        }
        if (res->length == 0) {
            printf("没有该员工，按任意键返回。\n");
            getchar();
            getchar();
            return &adminMenu;
        }
        char* staffTypeMap[5] = {"管理员\0", "前台\0", "医生\0", "护士\0",
                                 "采购员\0"};
        printf("员工信息如下: \n");
        printf("用户名: %s\n", getFieldValue(res, "username", 0));
        int type = atoi(getFieldValue(res, "type", 0));
        string typeStr = NULL;
        if (type < 0 || type > 4) {
            typeStr = "未知员工类型";
        } else {
            typeStr = staffTypeMap[type];
        }
        printf("类型: %s\n", typeStr);
        printf("按任意键返回。\n");
        getchar();
        getchar();
        freeQueryResult(res);
        return &adminMenu;
    } else if (atoi(input) == 6) {  // 删除员工账户
        printf("请输入欲删除员工的用户名: ");
        char username[256] = {0};
        scanf("%s", username);
        uint64_t affected = deleteStaffByUsername(username);
        if (affected <= 0) {
            printf("删除失败，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &adminMenu;
        }
        return &adminMenu;
    } else if (atoi(input) == 7) {  // 添加员工账户
        printf("请输入用户名: ");
        char username[256] = {0};
        scanf("%s", username);
        printf("请输入密码: ");
        char password[256] = {0};
        scanf("%s", password);
        printf("请输入类型(0-管理员 | 1-前台 | 2-医生 | 3-护士 | 4-采购员): ");
        int type;
        scanf("%d", &type);
        if (type < 0 || type > 4) {
            printf("类型错误，按任意键返回。\n");
            getchar();
            getchar();
            return &adminMenu;
        }
        uint64_t affected = insertStaff(username, password, type);
        if (affected <= 0) {
            printf("添加失败，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &adminMenu;
        }
        return &adminMenu;
    } else if (atoi(input) == 8) {  // 查询医院信息
        QueryResult* res = queryInfo();
        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &doctorMenu;
        }
        if (res->length == 0) {
            printf("信息为空\n");
            getchar();
            getchar();
            return &patientMenu;
        }
        printf("医院的信息如下: \n");
        printf("医院名称: %s\n", getFieldValue(res, "name", 0));
        printf("按任意键返回。\n");
        getchar();
        getchar();
        freeQueryResult(res);
        return &adminMenu;
    } else if (atoi(input) == 9) {  // 查询会员
        QueryResult* res = queryVIP(-1);
        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &adminMenu;
        }
        if (res->length == 0) {
            printf("没有查询到会员，按任意键返回。\n");
            getchar();
            getchar();
            return &adminMenu;
        }
        printf("会员信息如下: \n");
        for (int i = 0; i < res->length; i++) {
            printf("%d. 患者: %s, 积分: %s \n", i,
                   getFieldValue(res, "patient", i),
                   getFieldValue(res, "points", i));
        }
        printf("按任意键返回。\n");
        getchar();
        getchar();
        freeQueryResult(res);
        return &adminMenu;
    } else if (atoi(input) == 10) {  // 删除会员
        printf("请输入欲删除的会员用户名: ");
        char username[256];
        scanf("%s", username);
        uint64_t affected = deleteVIPByUsername(username);
        if (affected <= 0) {
            printf("删除失败，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
        }
        return &adminMenu;
    } else if (atoi(input) == 11) {  // 签到
        return abstractInsertSignIn(adminMenu.currentUser->username, SIGNIN,
                                    &adminMenu);
    } else if (atoi(input) == 12) {  // 签退
        return abstractInsertSignIn(adminMenu.currentUser->username, SIGNOUT,
                                    &adminMenu);
    }
    return &adminMenu;
}

void adminMenuShowMenu() {
    printf("\n%s\nMSshell$ ", adminMenu.itemsStr);
}

Menu* frontMenuExec(string input) {
    if (atoi(input) == 0) {  // 返回上级菜单
        return frontMenu.parent;
    } else if (atoi(input) == 1) {  // 患者账户注册
        printf("请输入患者的用户名: ");
        char username[256];
        scanf("%s", username);
        printf("请输入患者的密码: ");
        char password[256];
        scanf("%s", password);

        uint64_t affected = insertPatient(username, password, "", 0);
        if (affected <= 0) {
            printf("注册失败，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &frontMenu;
        }
        return &frontMenu;
    } else if (atoi(input) == 2) {  // 查看紧急呼叫
        QueryResult* res = queryEmergency(-1);
        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &frontMenu;
        }
        if (res->length == 0) {
            printf("没有查询到紧急呼叫，按任意键返回。\n");
            getchar();
            getchar();
            return &frontMenu;
        }
        printf("紧急呼叫如下: \n");
        for (int i = 0; i < res->length; i++) {
            printf("%d. 患者: %s, 附加信息: %s \n", i,
                   getFieldValue(res, "patient", i),
                   getFieldValue(res, "message", i));
        }
        printf("按任意键返回。\n");
        getchar();
        getchar();
        freeQueryResult(res);
        return &frontMenu;
    } else if (atoi(input) == 3) {  // 收银结算
        printf("请输入患者的用户名: ");
        char username[256];
        scanf("%s", username);
        printf("请输入收银金额: ");
        double cash;
        scanf("%lf", &cash);
        uint64_t affected = insertCashierRecord(username, cash);
        if (affected <= 0) {
            printf("收银失败，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &frontMenu;
        }

        // 查询患者是否是会员
        QueryResult* res = queryVIPByUsername(username);
        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &frontMenu;
        }
        if (res->length <= 0) {
            printf("患者尚未开通会员，收银成功但未更新积分，按任意键返回。\n");

            getchar();
            getchar();
        } else {
            affected = alterVIPPointsByUsername(username, cash);
            if (affected <= 0) {
                printf("更新积分失败，按任意键返回。\n");
                printf("error message: %s\n", getError());

                getchar();
                getchar();
            }
        }
        freeQueryResult(res);
        return &frontMenu;
    } else if (atoi(input) == 4) {  // 查询会员积分
        printf("请输入患者的用户名: ");
        char username[256];
        scanf("%s", username);
        QueryResult* res = queryVIPByUsername(username);
        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &frontMenu;
        }
        if (res->length == 0) {
            printf("该患者未开通会员，按任意键返回。\n");
            getchar();
            getchar();
            return &frontMenu;
        }
        printf("会员信息如下: \n");
        printf("患者: %s, 积分: %s \n", getFieldValue(res, "patient", 0),
               getFieldValue(res, "points", 0));
        printf("按任意键返回。\n");
        getchar();
        getchar();
        freeQueryResult(res);
        return &frontMenu;
    } else if (atoi(input) == 5) {  // 开通会员
        printf("请输入患者的用户名: ");
        char username[256];
        scanf("%s", username);
        uint64_t affected = insertVIP(username);
        if (affected <= 0) {
            printf("开通会员失败，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &frontMenu;
        }
        return &frontMenu;
    } else if (atoi(input) == 6) {  // 签到
        return abstractInsertSignIn(frontMenu.currentUser->username, SIGNIN,
                                    &frontMenu);
    } else if (atoi(input) == 7) {  // 签退
        return abstractInsertSignIn(frontMenu.currentUser->username, SIGNOUT,
                                    &frontMenu);
    }
    return &frontMenu;
}

void frontMenuShowMenu() {
    printf("\n%s\nMSshell$ ", frontMenu.itemsStr);
}

Menu* buyerMenuExec(string input) {
    if (atoi(input) == 0) {  // 返回上级菜单
        return buyerMenu.parent;
    } else if (atoi(input) == 1) {  //记录补货信息
        printf("请输入补货信息: ");
        char message[256];
        scanf("%s", message);

        uint64_t affected = insertPurchaseRecord(message);
        if (affected <= 0) {
            printf("记录补货信息失败，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &buyerMenu;
        }

        return &buyerMenu;
    } else if (atoi(input) == 2) {  //查询补货信息
        QueryResult* res = queryPurchaseRecord(-1);
        if (res == NULL) {
            printf("发生错误，按任意键返回。\n");
            printf("error message: %s\n", getError());
            getchar();
            getchar();
            return &buyerMenu;
        }
        if (res->length == 0) {
            printf("没有查询到补货信息，按任意键返回。\n");
            getchar();
            getchar();
            return &buyerMenu;
        }
        printf("补货信息如下: \n");
        for (int i = 0; i < res->length; i++) {
            printf("%d. %s\n", i, getFieldValue(res, "message", i));
        }
        printf("按任意键返回。\n");
        getchar();
        getchar();
        freeQueryResult(res);
        return &buyerMenu;
    } else if (atoi(input) == 3) {  // 签到
        return abstractInsertSignIn(buyerMenu.currentUser->username, SIGNIN,
                                    &buyerMenu);
    } else if (atoi(input) == 4) {  // 签退
        return abstractInsertSignIn(buyerMenu.currentUser->username, SIGNOUT,
                                    &buyerMenu);
    }
    return &buyerMenu;
}

void buyerMenuShowMenu() {
    printf("\n%s\nMSshell$ ", buyerMenu.itemsStr);
}
