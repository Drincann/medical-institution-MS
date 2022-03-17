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
        printf("签到失败，按任意键返回。\n");
        printf("error message: %s\n", getError());
        getchar();
        getchar();
        return returnMenu;
    }
    printf("签到成功，按任意键返回。\n");
    getchar();
    getchar();
    return returnMenu;
}

// 顶级菜单
Menu* topMenuExec(string input);
void topMenuShowMenu();

Menu topMenu = {
    .currentUser = NULL,
    .parent = NULL,
    .itemsStr =
        "1.患者登录\n"
        "2.医疗人员登录\n"
        "3.退出\n",
    .exec = topMenuExec,
    .showMenu = topMenuShowMenu,
};

// 医生菜单
Menu* doctorMenuExec(string input);
void doctorMenuShowMenu();

Menu doctorMenu = {
    .currentUser = NULL,
    .parent = &topMenu,
    .itemsStr =
        "1.查看预约\n"
        "2.查看病人情况\n"
        "3.发送健康建议\n"
        "4.签到\n"
        "5.签退\n"
        "0.返回\n",
    .exec = doctorMenuExec,
    .showMenu = doctorMenuShowMenu,
};

// 患者菜单
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

// 护士菜单
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

// 管理员菜单
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

// 前台菜单
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

// 采购员菜单
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
    } else if (atoi(input) == 3) {  // 退出程序
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
        freeQueryResult(res);
        return &doctorMenu;
    } else if (atoi(input) == 2) {  // 查看病人情况

        {
            QueryResult* visitRecordRes = queryVisitRecordByDoctorUsername(
                doctorMenu.currentUser->username);
            if (visitRecordRes == NULL) {
                printf("发生错误，按任意键返回。\n");
                printf("error message: %s\n", getError());
                getchar();
                getchar();
                return &doctorMenu;
            }
            if (visitRecordRes->length == 0) {
                printf("您还没有接诊任何病人, 按任意键返回。\n");
                getchar();
                getchar();
                return &doctorMenu;
            }
            printf("您接诊的病人情况如下: \n");
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
        }

        return &doctorMenu;
    } else if (atoi(input) == 3) {  // 发送健康建议

        char patient[256] = {0};
        char advice[1025] = {0};
        printf("请输入您已接诊的病人用户名: ");
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
    } else if (atoi(input) == 4) {  // 签到
        return abstractInsertSignIn(doctorMenu.currentUser->username, SIGNIN,
                                    &doctorMenu);
    } else if (atoi(input) == 5) {  // 签退
        return abstractInsertSignIn(doctorMenu.currentUser->username, SIGNOUT,
                                    &doctorMenu);
    } else if (atoi(input) == 6) {  // 退出程序
        exit(0);
        return NULL;
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
        return &patientMenu;
    } else if (atoi(input) == 4) {  // 查询用药记录
        return &patientMenu;
    } else if (atoi(input) == 5) {  // 查询疫苗注射情况
        return &patientMenu;
    } else if (atoi(input) == 6) {  // 发送治疗反馈
        return &patientMenu;
    } else if (atoi(input) == 7) {  // 紧急呼救
        return &patientMenu;
    } else if (atoi(input) == 8) {  // 查询会员积分
        return &patientMenu;
    } else if (atoi(input) == 9) {  // 查询健康建议
        return &patientMenu;
    }
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

        return &nurseMenu;
    } else if (atoi(input) == 2) {  // 设置患者情况
        printf("请输入患者用户名: ");
        scanf("%s", patient);
        printf("请输入患者情况: ");
        char situation[1025];
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

        return &nurseMenu;
    } else if (atoi(input) == 4) {  // 添加医护备忘
        printf("请输入备忘内容: ");
        char message[1025];
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
}

void nurseMenuShowMenu() {
    printf("\n%s\nMSshell$ ", nurseMenu.itemsStr);
}

Menu* adminMenuExec(string input) {
    if (atoi(input) == 0) {  // 返回上级菜单
        return adminMenu.parent;
    } else if (atoi(input) == 1) {  // 查询天营业额
        return &adminMenu;
    } else if (atoi(input) == 2) {  // 查询月营业额
    } else if (atoi(input) == 3) {  // 查询季度营业额
    } else if (atoi(input) == 4) {  // 查询年营业额
        return &adminMenu;
    } else if (atoi(input) == 5) {  // 查询员工账户
        return &adminMenu;
    } else if (atoi(input) == 6) {  // 删除员工账户
        return &adminMenu;
    } else if (atoi(input) == 7) {  // 添加员工账户
        return &adminMenu;
    } else if (atoi(input) == 8) {  // 查询医院信息
        return &adminMenu;
    } else if (atoi(input) == 9) {  // 查询会员
        return &adminMenu;
    } else if (atoi(input) == 10) {  // 删除会员
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
        return &frontMenu;
    } else if (atoi(input) == 2) {  // 查看紧急呼叫
        return &frontMenu;
    } else if (atoi(input) == 3) {  // 收银结算
        return &frontMenu;
    } else if (atoi(input) == 4) {  // 查询会员积分
        return &frontMenu;
    } else if (atoi(input) == 5) {  // 开通会员
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
    } else if (atoi(input) == 1) {  // 记录补货信息
        return &buyerMenu;
    } else if (atoi(input) == 2) {  // 查询补货信息
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
