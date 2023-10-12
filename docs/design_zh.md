# elastic 设计文档
本文档介绍elastic的内存编码布局

## 设计初衷
设计一个简单易用，基于c++20，性能卓越，内存占用少的序列化库

## 设计目标
性能超越protobuf 2-3倍， 内存占用少于protobuf 2-3倍

## 结构定义
### 属性
#### 结构体属性
+ alignas: 内存对齐
#### 成员属性
+ optional : 值可选
+ fixed : 固定长度
+ repeated : 数组
+ repeate<N> : 定长数组，N为数组长度

### 类型
#### 基础类型
+ int8
+ int16
+ int32
+ int64
+ uint8
+ uint16
+ uint32
+ uint64
+ v_int
+ v_uint
+ enum
+ string
+ bool
+ bytes
+ double
+ float

#### 组合类型
+ optional + 基础类型 ： 类型值为可选的
+ fixed + intN : 固定长度的整形
+ repeated + T : 值类型为T的数组
+ repeate<N> + T: 值类型为T的长度为N的数组

#### 复杂类型
+ map<T1,T2> 以T1为键类型,T2为值类型的键值对
+ union<T,...> 共同体

#### 3种解析类别
+ 0:varint 针对变长整形v_int和v_uint的解析方式
+ 1:fixed 针对固定长度数据类型的解析方式
+ 2:length-body 针对变长数组，字符串等的解析方式

### 结构



## 内存布局
### 单位结构
我们采用message关键字来定义一系列结构集合。 message结构是一系列的键值对。 message_number以及filed_number都只能使用数字，而每个字段的定义和类型只能通过引用message类型定义的文件(即 .elc文件)在解码端确定。具体结构如下：

```
[[alignas(N)]] message name = [[message_number]]
{
    [[property]] field_type : field_name;
};
```
由"[[]]"包含的字段均为可选字段，字段含义如下：
+ alignas(N) 为字节对齐
+ message 结构关键字
+ name 结构名字，作为结构的唯一标识
+ message_number 结构序列号，作为数字唯一标识
+ property 参考上述[属性](#成员属性)

当一个message被编码时，首先会将message编号进行编码，然后再对每个键值对进行编码，每个键值对都被转换为由有效负载组成的记录。

#### varint
可变整形内存布局如下：
```
message Test1 = 1
{
  v_int number;
};
```
首字节的第一位为符号位： 0为有符号， 1为无符号，后续位为真实数据


#### Length-Delimited Records
前缀长度在类型格式中是另外一个主要的约束。`strings`类型有一个动态的长度，由紧接在标记后面的一个varint指定，它通常跟在有效负载后面。
考虑以下列子：
```
message Test2 = 2
{
  string b = 2;
}
```
变量`b`是`string`类型。如果我们把`b`初始化为`testing`,我们在编码这条数据的时候就会包含ascii字符串`testing`。结果为`02020774657374696e67`,将字符分解：
```
02 {02 07 [74 65 73 74 69 6e 67]}
```
我们观察这个Tag，`02`是表示`Test2`的结构编号，  `02`是`00000 010` 或者是`2:strings`。剩下的是varint的长度为7，接下来7个字节为u8字符串`testing`。

`bytes和repeate<N>`使用同样的编码方式。

#### Submessages
下面的 message，内嵌了我们之前的简单例子 Test1：
```
message Test3 = 3
{
  Test1 c = 3;
}
```


#### repeated，repeate<N>和optional
没有`optional`标志的字段编码很容易，我们会跳过不存在的记录，这说明"巨大"字段是很稀少的。
`repeated`字段稍微复杂一些。重复字段为字段的每个元素发出一条记录。 例如：
```
message Test4 = 4
{
  optional string d = 4;
  repeated int32 e = 5;
}
```
构造一个Test4，把`d`赋值为`hello`,`e`赋值为`1,2,3`,结果表示为：
```
4: {"hello"}
5: 1
5: 2
5: 3
```
但是，`e`的记录不需要连续出现，可以与其他字段交错出现;只有相同字段的记录相对于彼此的顺序被保留。因此，这也可以被编码为
```
5: 1
5: 2
4: {"hello"}
5: 3
```


#### map
映射字段只是一种特殊的重复字段的缩写。例如：
```
message Test5 = 5
{
  map<string, int32> g = 5;
}
```
等价于
```
message Test6 = 6 {
  message g_Entry {
    [[optional]] string : key = 1;
    [[optional]] int32 : value = 2;
  }
  repeated g_Entry g = 7;
}
```
因此，映射被编码得完全像一个重复的消息字段，每个记录有两个字段。


#### union
union是一种共同体，示例
```
message Test7 = 7
{
  union<int32,int64,string> un = 1;
}

```
值类型为union中的其中一种