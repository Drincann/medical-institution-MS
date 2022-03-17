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

# TODO

- ~~mysql 单例 √~~

- ~~建表 √~~

- 更新文档

- ~~封装所有 mysql 过程 √~~

- 写测试

### 菜单

菜单针对不同用户是独立的, 所以从用户对象构造一个菜单，接管此后的行为

入口:

- 患者登录 √
- 医疗人员登录 √

管理员:

- 查询营业额

  - 天
  - 月
  - 季
  - 年
  
- 管理员管理
  - 查询
  - 删除
  - 更改
  - 注册
- 医院信息查询
- 会员管理
  - 查询
  - 删除
  - 更改
  - 注册
- 签到
- 签退

医生:

- 查询已有预约 √
- 查询已就诊患者情况 √
- 为已就诊患者发送建议 √
- 签到 √
- 签退 √

护士:

- 查询患者数据 √
- 设置患者情况 √
- 查询医护备忘 √
- 添加医护备忘 √
- 签到 √
- 签退 √

前台:

- 注册患者账户
- 查询紧急呼叫
- 收银结算
- 查询患者会员积分
- 开桶会员
- 签到
- 签退

采购员:

- 补货
- 查询历史补货记录
- 签到
- 签退

患者

- 查询医院信息 √
- 预约 √
- 查询就诊记录
- 查询用药记录
- 查询疫苗注射情况
- 向医生发送治疗反馈
- 紧急呼救
- 查询会员积分
- 查询健康建议

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
