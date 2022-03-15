# 医疗机构管理系统

## 构建

下载对应 OS 的 mysql server [https://dev.mysql.com/downloads/c-api/](https://dev.mysql.com/downloads/c-api/)

将 include/ 及 lib/ 目录放到当前目录下。

编译：

```shell
gcc main.c -o main -Iinclude -Llib -lmysqlclient
```

## 使用 mysql c api 的运行时环境需求

see -> [https://dev.mysql.com/doc/c-api/8.0/en/c-api-running-clients.html](https://dev.mysql.com/doc/c-api/8.0/en/c-api-running-clients.html)

## 需求分析

### function

### table

抽象员工 Staff

|          |        |                                               |
| -------- | ------ | --------------------------------------------- |
| id       | int    | primary key auto increasing                   |
| username | string | 用户名                                        |
| type     | int    | 0 admin 1 frontStaff 2 doctor 3 nurse 4 buyer |

患者 Patient

|           |        |                             |
| --------- | ------ | --------------------------- |
| id        | int    | primary key auto increasing |
| username  | string | 用户名                      |
| visited   | bool   | 是否已就诊                  |
| situation | string | 患者情况                    |
| injected  | bool   | 是否注射疫苗                |

健康建议 FeedBackToPatient

|           |        |                             |
| --------- | ------ | --------------------------- |
| id        | int    | primary key auto increasing |
| patientid | int    | foreign key to staff        |
| dockorid  | int    | foreign key to staff        |
| advice    | string | 建议内容                    |

治疗反馈 FeedBackToDoctor

|           |        |                             |
| --------- | ------ | --------------------------- |
| id        | int    | primary key auto increasing |
| patientid | int    | foreign key to staff        |
| dockorid  | int    | foreign key to staff        |
| situation | string | 治疗情况                    |

打卡 SignIn

|        |      |                             |
| ------ | ---- | --------------------------- |
| id     | int  | primary key auto increasing |
| userid | int  | foreign key to staff        |
| type   | int  | 0 - sign in 1 - sign out    |

医院信息 info

|      |        |                             |
| ---- | ------ | --------------------------- |
| id   | int    | primary key auto increasing |
| name | string | 医院名称                    |

预约信息 Reservation

|           |      |                             |
| --------- | ---- | --------------------------- |
| id        | int  | primary key auto increasing |
| doctorid  |      | foreign key to staff        |
| patientid |      | foreign key to patient      |

就诊记录 Visit

|           |      |                             |
| --------- | ---- | --------------------------- |
| id        | int  | primary key auto increasing |
| patientid | int  | foreign key to patient      |
| doctorid  | int  | foreign key to staff        |

用药记录 Medication

|           |        |                             |
| --------- | ------ | --------------------------- |
| id        | int    | primary key auto increasing |
| patientid | int    | foreign key to patient      |
| doctorid  | int    | foreign key to staff        |
| message   | string | 用药信息                    |

护士备忘 nurseMemo

|         |        |                             |
| ------- | ------ | --------------------------- |
| id      | int    | primary key auto increasing |
| message | string | 备忘信息                    |
| nurseid | int    | foreign key to staff        |

收银记录

|           |          |                             |
| --------- | -------- | --------------------------- |
| id        | int      | primary key auto increasing |
| amount    | float    | 金额                        |
| patientid | int      | foreign key to patient      |
| date      | datetime | 时间                        |

会员卡 VIP

|           |      |                             |
| --------- | ---- | --------------------------- |
| id        | int  | primary key auto increasing |
| patientid | int  | foreign key to patient      |
| point     | int  | 会员积分                    |

紧急呼救 Emergency

|           |        |                             |
| --------- | ------ | --------------------------- |
| id        | int    | primary key auto increasing |
| patientid | int    | foreign key to patient      |
| message   | string | 呼救信息                    |

进货信息 Purchase

|         |        |                             |
| ------- | ------ | --------------------------- |
| id      | int    | primary key auto increasing |
| message | string | 进货信息                    |
