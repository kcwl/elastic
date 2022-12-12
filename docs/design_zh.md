# elastic 设计文档
本文档介绍elastic的内存编码布局

## 设计初衷
设计一个简单易用，基于c++20，并且性能卓越的序列化库

## 设计目标
性能超越protobuf 2-3倍

## 结构定义
### 属性
+ optional
+ repeated
+ required
+ fixed32
+ fixed64
+ unsigned

### 类型
#### 基础类型
+ int8
+ int16
+ int32
+ int64
+ enum
+ string
+ bool
+ bytes
+ double
+ float

#### 组合类型
```
组合类型: [[property]]+基础类型  例如： repeated int32意为重复的int32类型，即为int32数组 （注： unsiged，
fixedN 等属性只能修饰intN类型）
```

### 结构
```
message Name = [message_number]
{
    [[property]] type : field_name  = filed_number;
};
```


## 内存布局
### 一条简单的消息
假设你有以下一个非常简单的消息定义：
```
message Test1 = 1
{
	[[optional]] int32 ： a  = 1;
};
```

在一个应用程序中，你创建一个 Test1 message，并将 a 设置为150。然后，将 message 序列化为输出流，你会看到以下四个字节：
`01 01 96 01`
到目前为止，是如此的小巧和简单－但是这几个字节具体代表什么含义？

### 128位varient
要理解上述涉及到的简单编码，你首先需要理解 varints 的概念。所谓的 varints 是一种用一个或多个字节序列化（编码）整数的方法。较小的数字将占用较少的字节数。

varint 中的每个字节都设置了一个标识位（msb） - msb 表示后面还有字节需要读取（当前编码还未结束）。每个字节的低 7 位用于以 7 位一组的方式存储数字的二进制补码，二进制补码的低位排在编码序列的前头。

我们来看一个例子：数字 1 该如何编码 – 这只需要单个字节，所以无需设置 msb：`01`
```
				0000 0001
				^ msb
```
整段编码第一个字节为message结构体编码字节，表示结构体编号，后面为message结构内数据编码即:
```
01
```

你怎么知道这是 150？首先，你从每个字节中删除MSB，因为这只是为了告诉我们我们是否已经到达数字的末尾（如您所见，它设置在第一个字节中，因为varint中有多个字节）。然后我们连接 7 位有效负载，并将其解释为 小端序，64 位无符号整数：
```
10010110 00000001        // Original inputs.
 0010110  0000001        // Drop continuation bits.
 0000001  0010110        // Put into little-endian order.
 10010110                // Concatenate.
 128 + 16 + 4 + 2 = 150  // Interpret as integer.
```
由于变量对于协议缓冲区至关重要，因此在语法中，我们将它们称为普通整形。 150等同于`9601`

### Message结构
一个message结构是一系列的键值对。二进制版本的字段只使用数字作为字段key，而每个字段的定义和类型只能通过引用message类型定义的文件(即 .elatic文件)在解码端确定。

当一个message被编码时，首先会将message编号进行编码，然后再对每个键值对进行编码，每个键值对都被转换为由字段号和有效负载组成的记录。

每条记录的`Tag(field_number)`都是由字段编号组成，都使用varint编码。
让我们再一次看个简单的示例。字节流的第一位数字总是varint的键，这里是`01`（删除MSB）
```
000 0001
```
所以结果为 `1:varint`

由于连接类型是`0（varint）`，我们解码一个`varint`，正如我们上面看到的，字节`9601`解码为`150`，给出了我们的记录。结果为`1:VARINT 150`

### 更多整形类型
#### bool 和Enum
bool和enum类型通常都会编码成`int8`,特别是bool类型，通常都是`00`或者`01`。即为`false`和`true`。

#### Signed Integers
在上面提到的，所有的elatic中`0`都被编码成varint，但是，varint是无符号类型，以至于不同的有符合类型,`sisgned int32`或者`signed int64`相对于`int32`和`int64`都有差异。

intN类型将负数编码为2的补码，这意味着，作为无符号的64位整数，它们具有最高位集。这意味着，所有10位数都要被使用。例如：`-2`被编码成
```
11111110 11111111 11111111 11111111 11111111
11111111 11111111 11111111 11111111 00000001
```
这是2的补码，在无符号算术中定义为~0 - 2 + 1，其中~0是全1位的64位整数。这个练习有助于我们理解为什么会产生如此多的1。

所以，`signed intN` 使用`ZigZag`方式取代补码编码整形类型。正整数`n`被编码为`2*n`(偶数),负数`-n`被编码为`2*n+1`(奇数)。这样的编码被称为`zig-zags`(参考protobuf)。举例：
|signed original|encoded as|
|:-:|:-:|
|0|0|
|-1|1|
|1|2|
|-2|3|
|...|...|
|0x7fffffff|0xfffffffe|
|-0x80000000|0xffffffff|

使用一些位技巧可以轻松便捷的转成`ZigZag`形式：
```
n + n + (n<0)
```
在这里，我们假设`n<0`的boolean类型如果返回值为`true`可以转换成数字`1`，返回`false`可以转换成`0`.
当sint32或sint64被解析时，它的值被解码回原始的带符号版本。
在protoscope中，给一个整数加上z后缀将使其编码为ZigZag。例如，`-500z`与`varint 1001`相同。

#### Non-varint Numbers
non-varint类型很简单，`double`和`fixed64`被表示为`varint`，这告诉解析器需要解析固定的8字节存储。 

同样的，`float`和`fixed32`被表示为`varint`，告诉解析器需要解析4字节数据块。

#### Length-Delimited Records
前缀长度在类型格式中是另外一个主要的约束。`strings`类型有一个动态的长度，由紧接在标记后面的一个varint指定，它通常跟在有效负载后面。
考虑以下列子：
```
message Test2 = 2
{
  optional string b = 2;
}
```
变量`b`是`string`类型。如果我们把`b`初始化为`testing`,我们在编码这条数据的时候就会包含ascii字符串`testing`。结果为`02020774657374696e67`,将字符分解：
```
02 {02 07 [74 65 73 74 69 6e 67]}
```
我们观察这个Tag，`02`是表示`Test2`的结构编号，  `02`是`00000 010` 或者是`2:strings`。剩下的是varint的长度为7，接下来7个字节为u8字符串`testing`。

`bytes`使用同样的编码方式。

#### Submessages
下面的 message，内嵌了我们之前的简单例子 Test1：
```
message Test3 = 3
{
  optional Test1 c = 3;
}
```
如果`Test1`中的`a`(即Test3.c.a)被赋值为`150`，结果为`030301019601`。拆分字符：
```
 03 ｛03 01 01 [96 01]｝
```

#### repeated和optional
没有`optional`标志的字段编码很容易，我们会跳过不存在的记录，这说明"巨大"字段是很稀少的。
`repeated`字段稍微复杂一些。重复字段为字段的每个元素发出一条记录。 例如：
```
message Test4 = 4
{
  optional string d = 4;
  repeated int32 e = 5;
}
```
构造一个Test4，把`d`赋值为`hello`,`e`赋值为`1,2,3`,结果为``,或者表示为：
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
没有对`oneofs`类型作特别处理。

#### Map
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


#### oneof