# elastic
c++20 序列化库，支持二进制序列化(未来会考虑支持json和xml)

## 构建状态
[![codecov](https://codecov.io/gh/kcwl/elastic/graph/badge.svg?token=2A4MZ0NDWU)](https://codecov.io/gh/kcwl/elastic)
[![ubuntu(gcc)](https://github.com/kcwl/elastic/actions/workflows/cmake-ubuntu-gcc.yml/badge.svg)](https://github.com/kcwl/elastic/actions/workflows/cmake-ubuntu-gcc.yml)
[![ubuntu(clang)](https://github.com/kcwl/elastic/actions/workflows/cmake-ubuntu-clang.yml/badge.svg)](https://github.com/kcwl/elastic/actions/workflows/cmake-ubuntu-clang.yml)
[![visual stadio](https://github.com/kcwl/elastic/actions/workflows/cmake-windows-vs.yml/badge.svg)](https://github.com/kcwl/elastic/actions/workflows/cmake-windows-vs.yml)

## 简介
elastic 是一个用c++20标准编写的序列化库. 支持多种类型的序列化, elastic 依赖于 [[reflect](https://github.com/kcwl/reflect.git)] 反射成员变量。
elastic是只包含头文件的, 所以只要包含头文件就可以轻松的使用。

## 数据类型
以下类型是目前所支持的：

### basic type

|类型| 注释|
|--|--|
|char| 标量类型 |
|bool| 标量类型 |
|intN| int8, int16, int32, int64|
|uintN| uint8_t, uint16_t. uint32_t. uint64_t|
|double| 双精度类型|
|float| 单精度类型|
|enum/enum class| 枚举类型|
|string| 字符串|
|vector<T>| 连续序列|

### 复杂类型
+ 继承类型需要添加`ELASTIC_ACCESS_IF`注册序列化访问权限。
+ 非POD类型需要添加`ELASTIC_ACCESS`注册序列化访问权限。

## core
elastic中的一个核心组件flex_buffer是用来做序列化的数据承载. flex_buffer 定义如下:

```
flex_buffer<_Element, _Traits>::flex_buffer

flex_buffer();		//默认构造, 构造一个容量为4096的flex_buffer
		
flex_buffer(size_type number); // 构造一个容量为number的flex_buffer

template <typename _Iter>
flex_buffer(_Iter begin, _Iter end);  // 通过迭代器构造一个flex_buffer

template <typename _Ty, std::size_t N, 
	typename = std::is_convertible<_Ty, _Elem>>
flex_buffer(std::span<_Ty, N> data);    // 通过一段跨度构造一个flex_buffer

flex_buffer(const void* buffer, size_type sz); //通过C的方式构造一个flex_buffer_
```

## 用法
elastic 提供了两个接口函数， `to_binary` 和 `from_binary`;

```
template <typename _Ty, typename _Elem, typename _Traits = std::char_traits<_Elem>>
bool to_binary(_Ty&& t, flex_buffer<_Elem, _Traits>& buffer);                               // 把t序列化成二进制数据

template <typename _Ty, typename _Elem, typename _Traits = std::char_traits<_Elem>>
bool from_binary(_Ty& t, flex_buffer<_Elem, _Traits>& buffer);								// 把二进制数据反序列化成t					
```

## 示例

```
	elastic::flex_buffer_t buf;

	int8_t a_in = (std::numeric_limits<int8_t>::max)();

	elastic::to_binary(a_in, buf);

	int8_t a_out{};

	elastic::from_binary(a_out, buf);
```

在上述示例中， `a_in` 和 `a_out` 相等，更多的例子可以参考单元测试

## wiki
