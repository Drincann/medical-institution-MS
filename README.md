# 医疗机构管理系统

## 构建

下载对应 OS 的 mysql server [https://dev.mysql.com/downloads/c-api/](https://dev.mysql.com/downloads/c-api/)

将 include/ 及 lib/ 目录放到当前目录下。

编译：

使用 cmake:

```shell
mkdir build; cd build
cmake ..; cmake --build .; sudo cmake --install .
```

使用 xmake:

```shell
xmake
```

或直接手动编译:

在 Linux/Unix(macOS) 下，请链接 mysqlclient :

```shell
gcc main.c -o main -Iinclude -Llib -lmysqlclient
```

在 Windows 下，请链接 libmysql

```powershell
gcc main.c -o main -Iinclude -Llib -llibmysql
```

注意！确保编译器和 MySQL lib 的 arch 相同（32bit/64bit）。

在不同平台下预构建的二进制可执行文件在 `./release` 目录下（64 bit）。

运行时环境（动态链接库）已经提供在 `./release` 下对应的平台目录中（64 bit），将它们放在你编译出的二进制文件周围，就可以正常运行了。

或者，你可以使用 `./script` 下提供的构建脚本 `buildreleasemacos.sh` 和 `buildreleasewindows.ps1` 来编译，前者(shell)用于在 Linux/Unix(macOS) 下构建，后者(powershell)用于在 Windows 下构建。

它们生成的二进制将会被放在 `./release` 下对应平台目录中。

## 使用 mysql c api 的运行时环境需求

see -> [https://dev.mysql.com/doc/c-api/8.0/en/c-api-running-clients.html](https://dev.mysql.com/doc/c-api/8.0/en/c-api-running-clients.html)

## 测试

请构建 `./test.c`，运行生成的二进制。

```shell
gcc test.c -o test -Iinclude -Llib -lmysqlclient

./test
```

## 需求分析

# TODO

- ~~需求分析 √~~
- ~~mysql 单例 √~~
- ~~建表 √~~
- ~~更新文档 √~~
- ~~封装所有 mysql 过程 √~~
- ~~写测试 √~~（写了一半）

### 菜单

菜单针对不同用户是独立的, 所以从用户对象构造一个菜单，接管此后的行为。

入口:

- 患者登录
- 医疗人员登录
- 退出

管理员:

- 查询营业额

  - 天
  - 月
  - 季
  - 年
- 账号管理
  - 查询
  - 删除
  - 注册
- 医院信息查询
- 会员管理
  - 查询
  - 删除
- 签到
- 签退
- 返回

医生:

- 查询预约/接诊
- 查询已就诊患者情况
- 查询已就诊患者反馈
- 为已就诊患者发送建议
- 签到
- 签退
- 返回

护士:

- 查询患者数据
- 设置患者情况
- 查询医护备忘
- 添加医护备忘
- 签到
- 签退
- 返回

前台:

- 注册患者账户
- 查询紧急呼叫
- 收银结算
- 查询患者会员积分
- 开通会员
- 签到
- 签退
- 返回

采购员:

- 补货
- 查询历史补货记录
- 签到
- 签退
- 返回

患者

- 查询医院信息
- 预约
- 查询就诊记录
- 查询用药记录
- 查询疫苗注射情况
- 向医生发送治疗反馈
- 紧急呼救
- 查询会员积分
- 查询健康建议
- 返回

### table

抽象员工 Staff

|          |        |                                               |
| -------- | ------ | --------------------------------------------- |
| username | string | primary key                                   |
| type     | int    | 0 admin 1 frontStaff 2 doctor 3 nurse 4 buyer |

患者 Patient

|           |        |              |
| --------- | ------ | ------------ |
| username  | string | primary key  |
| situation | string | 患者情况     |
| injected  | bool   | 是否注射疫苗 |

健康建议 FeedBackToPatient

|         |        |                             |
| ------- | ------ | --------------------------- |
| id      | int    | primary key auto increasing |
| patient | string | foreign key to staff        |
| dockor  | string | foreign key to staff        |
| advice  | string | 建议内容                    |

治疗反馈 FeedBackToDoctor

|           |        |                             |
| --------- | ------ | --------------------------- |
| id        | int    | primary key auto increasing |
| patient   | string | foreign key to staff        |
| dockor    | string | foreign key to staff        |
| situation | string | 治疗情况                    |

打卡 SignIn

|               |          |                             |
| ------------- | -------- | --------------------------- |
| id            | int      | primary key auto increasing |
| staffUsername | string   | foreign key to staff        |
| type          | int      | 0 - sign in 1 - sign out    |
| createTime    | datetime | default currenttime         |

医院信息 info

|      |        |                             |
| ---- | ------ | --------------------------- |
| id   | int    | primary key auto increasing |
| name | string | 医院名称                    |

预约信息 Reservation

|         |        |                             |
| ------- | ------ | --------------------------- |
| id      | int    | primary key auto increasing |
| doctor  | string | foreign key to staff        |
| patient | string | foreign key to patient      |

就诊记录 VisitRecord

|         |        |                             |
| ------- | ------ | --------------------------- |
| id      | int    | primary key auto increasing |
| patient | string | foreign key to patient      |
| doctor  | string | foreign key to staff        |

用药记录 MedicationRecord

|         |        |                             |
| ------- | ------ | --------------------------- |
| id      | int    | primary key auto increasing |
| patient | string | foreign key to patient      |
| doctor  | string | foreign key to staff        |
| dosage  | string | 用药信息                    |

护士备忘 NurseMemo

|               |        |                             |
| ------------- | ------ | --------------------------- |
| id            | int    | primary key auto increasing |
| message       | string | 备忘信息                    |
| nurseUsername | string | foreign key to staff        |

收银记录 CashierRecord

|            |          |                             |
| ---------- | -------- | --------------------------- |
| id         | int      | primary key auto increasing |
| amount     | float    | 金额                        |
| patient    | string   | foreign key to patient      |
| createTime | datetime | default currenttime         |

会员卡 VIP

|         |        |                             |
| ------- | ------ | --------------------------- |
| id      | int    | primary key auto increasing |
| patient | String | foreign key to patient      |
| points  | int    | 会员积分                    |

紧急呼救 Emergency

|         |        |                             |
| ------- | ------ | --------------------------- |
| id      | int    | primary key auto increasing |
| patient | string | foreign key to patient      |
| message | string | 呼救信息                    |

进货信息 Purchase

|         |        |                             |
| ------- | ------ | --------------------------- |
| id      | int    | primary key auto increasing |
| message | string | 进货信息                    |
