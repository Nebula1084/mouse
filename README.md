### 1. 基于 HTTP 的明文协议
#### 1.1 REPORT包
##### 1.1.1 URL
/api/report
##### 1.1.2 HTTP请求方式
POST  
成功: 200  
失败: 40x  
##### 1.1.3 请求参数
|字段名|类型|说明|
|:--:|:--:|:--:|
| auth_id | int | 连入设备编号 |
| auth_key| string(32) | 连入设备secret |
| device_id | int | 来源设备编号 |
| report_id | int | 注册的report编号 |
| $(field_name_0) | type | 数据,字段名为注册时填写 |
| ... | type | 数据 |

#### 1.2 CONTROL包
##### 1.2.1 URL
/api/control
##### 1.2.2 HTTP请求方式
POST  
成功: 200  
失败: 40x  
##### 1.2.3 发送CONTROL时请求参数
|字段名|类型|说明|
|:--:|:--:|:--:|
| auth_id | int | 连入设备编号 |
| auth_key| string(32) | 连入设备secret |
| device_id | int | 来源设备编号 |
| control_id | int | 注册的control编号 |
| sr | string(1) | "S", Send |
| target_id | int | 目标设备编号 |
| $(field_name_0) | type | 数据,字段名为注册时填写 |
| ... | type | 数据 |
##### 1.2.4.1 接受CONTROL时请求参数
|字段名|类型|说明|
|:--:|:--:|:--:|
| auth_id | int | 连入设备编号 |
| auth_key| string(32) | 连入设备secret |
| device_id | int | 来源设备编号 |
| control_id | int | 注册的control编号 |
| sr | string(1) | "R", Receive |
##### 1.2.4.2 接受CONTROL时返回数据
格式:JSON  
样例:
```
{"id":15,"defId":2,"authId":3,"deviceId":3,"time":"2016-05-29T18:02:23.646Z","fields":{"field1":"0.5","field0":"10"},"targetId":3,"fellowPacketId":-1,"sr":"S","date":1464516143646}
```

|字段名|类型|说明|
|:--:|:--:|:--:|
| id | int | packet的编号 |
| defId| int | control定义编号 |
| authId| int | 连入设备编号 |
| deviceId | int | 来源设备编号 |
| targetId | int | 目标设备编号 |
| time | string | 时间 |
| fields | json | 数据 |


#### 1.3 数据类型
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
如field0(string(9)) = 'abcdefghi', field1(int) = 0x7fffff0f 会多发送一个字节0;
但当field1 = 1有概率没有。
不过对读数据没有影响。
### 3. Web
