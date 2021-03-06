### **有关POST数据的问题，Magica组使用的是json。在更新api后，可以使用json格式，同时原有的以参数POST方式不受影响。但强烈推荐使用新API。详情参见以及样例py/json_*.py [1.4 兼容的json API](#3)**
### **有关网站的一些使用注意事项已更新**
### **轮询接收control数据已改为阻塞2分钟，与Magica组一致**
### **返回数据更改为Magica组的格式，在GET /api/data以及POST /api/control(轮询数据时)返回中均带有code字段以供判断。[1.2.4 返回数据](#1) [1.3.2 HTTP请求方式](#2)**

### 1. 基于 HTTP 的明文协议
#### 1.1 发送REPORT包
##### 1.1.1 URL
/api/report
##### 1.1.2 HTTP请求方式
POST  
成功: 200，**并返回{code:0}**，但推荐判断200。  
失败: 40x  
##### 1.1.3 请求参数
样例:
```
auth_id=1&auth_key=1eecf1c5e848ff42d88e5599b3e8dfc0&device_id=1&report_id=1&payload={"field0": 10,"field1": 0.5}}
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
<h5 id="1">1.2.4 返回数据</h5>
返回以时间排序的size条数据，从第page\*size条到(page+1)\*size-1，即相当于以每页size条分页显示的第page页。  
若希望返回最新的n条数据，可使page=0, size=n。
```
{
  "code": 0, // 请求成功
  "data":[
    {"id":4,"defId":1,"authId":1,"deviceId":1,"time":"2016-06-07T20:56:01.000Z","fields":{"field1":"0.4","field0":"4"},"date":1465304161000},
    ...
  ]
}

```

#### 1.3 CONTROL包
##### 1.3.1 URL
/api/control
<h5 id="2">1.3.2 HTTP请求方式</h5>
POST  
发送CONTROL成功: 200  
发送CONTROL失败: 40x  
**NEW!:** 轮询CONTROL无数据会阻塞2分钟,结束后会返回200，内容详见1.3.4.2
##### 1.3.3.1 发送CONTROL时请求参数
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
格式:JSON  (请自行无视不需要的字段)  
样例:
```
{"code":0,"id":15,"defId":2,"authId":3,"deviceId":3,"time":"2016-05-29T18:02:23.646Z","payload":{"field1":"0.5","field0":"10"},"targetId":3,"fellowPacketId":-1,"sr":"S","date":1464516143646}
or
{"code":-1}
```

|字段名|类型|说明|
|:--:|:--:|:--:|
| code | int | 请求状态返回值 |
| id | int | packet的编号 |
| defId| int | control定义编号 |
| authId| int | 连入设备编号 |
| deviceId | int | 来源设备编号 |
| targetId | int | 目标设备编号 |
| time | string | 时间 |
| payload | json | 数据 |

<h4 id="3">1.4 兼容的json API说明</h4>
##### 1.4.1 REPORT
原地址不变，只是可以直接使用json，**注意HEADER要加入Accept:application/json以及Content-Type:application/json**，这样便可通用两个服务器，只有id和secret key的区别。详见json_report.py。
```
{
    'auth_id': 1,
    'auth_key': 'e4d3dc84ef92b76c71e3faf983e02c47',
    'device_id': 1,
    'report_id': 1,
    'payload': {'field0': 0.5, 'field1': 10, 'field2': 'ABCDEFGH'}
}
```
##### 1.4.2 SEND CONTROL
新地址为api/push?device_id=xxx，与magi组一致。注意旧方案也是都是可以用的，没有任何变化。
要想达到通用两个服务器的目的，只需在magi组发送内容的基础上添加control_id字段，我们会自行忽略payload中多余的type以及length字段。详见json_send.py。
```
{
    'control_id': 1,
    'payload': [
        {'name': 'c1', 'value': 10},
        {'name': 'c2', 'value': 10.23},
    ]
}
```
##### 1.4.3 RECV CONTROL
同上，只需在magi组发送内容的基础上添加control_id字段。详见json_recv.py。
```
{
    'auth_id': 1,
    'auth_key': '85f4324d872b43578b57634f1a275dca',
    'device_id': 1,
    'control_id': 1,
}
```

#### 1.5 数据类型
|类型|说明|
|:--:|:--:|
| int |  |
| float |  |
| string |  |
| char | Deprecated, equals to string(1) |
| ~~double~~ | 说好的有double，最后就没有了。尴尬。 |


### 2. 基于 TCP 的二进制协议
#### 2.1
~~(╯' - ')╯︵ ┻━┻,来不及写,请参考sample.c以及mouse.h~~  

1. ```int mouse_init(int device_id, char* host_name, int port);```  
Note: 参照sample.c，初始化失败立即exit。

2. ```int mouse_login(char* device_secret);```  
Note: 登录成功返回0，失败返回-1，因为被吐槽了所以改了一下，原来的版本也可以放心食用。

3. ```int mouse_report(packingfunc func, void* data);```  
```int mouse_control_send(packingfunc func, void* data);```  
Note: 参照sample.c，需要自定义打包数据的函数, 以及数据自身的结构体。

4. ```packet* mouse_control_recv(int device_id, int control_id);```
Note: 读出的packet中带有所有注册时的字段信息，注意没有其他额外信息。需要自己处理，没有control时返回NULL，返回数据记得free。

#### 2.2 BUG
服务器有规律但找不到规律地多发送一个字节的0于Payload的末尾(接受CONTROL包时)。也有可能是C程序的锅。  
如field0(string(9)) = 'abcdefghi', field1(int) = 0x7fffff0f 会多发送一个字节0。正常情况下并没有。  
不过对读数据没有影响。

### 3.Website
#### 3.1 注意事项
1. 设备页面中发送control，即发送目标选为改设备。
2. 设备页面中的Control Log显示的该设备**发送**的control包记录，而不是发送到该设备的control。
