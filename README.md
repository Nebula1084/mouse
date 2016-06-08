### 1. 基于 HTTP 的明文协议
#### 1.1 发送REPORT包
##### 1.1.1 URL
/api/report
##### 1.1.2 HTTP请求方式
POST  
成功: 200  
失败: 40x  
##### 1.1.3 请求参数
样例:
```
auth_id=1&auth_key=1eecf1c5e848ff42d88e5599b3e8dfc0&device_id=1&report_id=1&payload={"field0":"10","field1":"0.5"}
```
|字段名|类型|说明|
|:--:|:--:|:--:|
| auth_id | int | 连入设备编号 |
| auth_key| string(32) | 连入设备secret |
| device_id | int | 来源设备编号 |
| report_id | int | 注册的report编号 |
| payload | json | 数据 |

#### 1.2 请求历史REPORT数据
##### 1.2.1 URL
/api/data
##### 1.2.2 HTTP请求方式
GET  
成功: 200  
失败: 40x  
##### 1.2.3 请求参数
样例
```
device_id=1&report_id=1&page=0&size=10
```
|字段名|类型|说明|
|:--:|:--:|:--:|
| device_id | int | 设备编号 |
| report_id | int | 注册的report编号 |
| size(optional) | unsigned int | 每页(每次)请求数量 |
| page(optional)  | unsigned int | 请求页编号 |
##### 1.2.4 返回数据
返回以时间排序的size条数据，从第page\*size条到(page+1)\*size-1，即相当于以每页size条分页显示的第page页。  
若希望返回最新的n条数据，可使page=0, size=n。
```
[{"id":4,"defId":1,"authId":1,"deviceId":1,"time":"2016-06-07T20:56:01.000Z","fields":{"field1":"0.4","field0":"4"},"date":1465304161000},...]
```

#### 1.3 CONTROL包
##### 1.3.1 URL
/api/control
##### 1.3.2 HTTP请求方式
POST  
成功: 200  
失败: 40x  
##### 1.3.3 发送CONTROL时请求参数
|字段名|类型|说明|
|:--:|:--:|:--:|
| auth_id | int | 连入设备编号 |
| auth_key| string(32) | 连入设备secret |
| device_id | int | 来源设备编号 |
| control_id | int | 注册的control编号 |
| sr | string(1) | "S", Send, **注意是大写** |
| target_id | int | 目标设备编号 |
| payload | json | 数据 |
##### 1.3.4.1 接受CONTROL时请求参数
|字段名|类型|说明|
|:--:|:--:|:--:|
| auth_id | int | 连入设备编号 |
| auth_key| string(32) | 连入设备secret |
| device_id | int | 来源设备编号 |
| control_id | int | 注册的control编号 |
| sr | string(1) | "R", Receive, **注意是大写** |
##### 1.3.4.2 接受CONTROL时返回数据
格式:JSON  
样例:
```
{"id":15,"defId":2,"authId":3,"deviceId":3,"time":"2016-05-29T18:02:23.646Z","payload":{"field1":"0.5","field0":"10"},"targetId":3,"fellowPacketId":-1,"sr":"S","date":1464516143646}
```

|字段名|类型|说明|
|:--:|:--:|:--:|
| id | int | packet的编号 |
| defId| int | control定义编号 |
| authId| int | 连入设备编号 |
| deviceId | int | 来源设备编号 |
| targetId | int | 目标设备编号 |
| time | string | 时间 |
| payload | json | 数据 |

#### 1.4 数据类型
|类型|说明|
|:--:|:--:|
| int |  |
| float |  |
| string |  |
| char | Deprecated, equals to string(1) |


### 2. 基于 TCP 的二进制协议
#### 2.1
(╯' - ')╯︵ ┻━┻,来不及写,请参考sample.c以及mouse.h
#### 2.2 BUG
服务器有规律但找不到规律地多发送一个字节的0于Payload的末尾(接受CONTROL包时)。也有可能是C程序的锅。  
如field0(string(9)) = 'abcdefghi', field1(int) = 0x7fffff0f 会多发送一个字节0; 但当field1 = 1有概率没有。  
不过对读数据没有影响。
### 3. Web
