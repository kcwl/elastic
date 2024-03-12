# elastic
c++20 serialization for binary(maybe json and xml will be support in future)

## build status
[![codecov](https://codecov.io/gh/kcwl/elastic/graph/badge.svg?token=2A4MZ0NDWU)](https://codecov.io/gh/kcwl/elastic)
[![ubuntu(gcc)](https://github.com/kcwl/elastic/actions/workflows/cmake-ubuntu-gcc.yml/badge.svg)](https://github.com/kcwl/elastic/actions/workflows/cmake-ubuntu-gcc.yml)
[![ubuntu(clang)](https://github.com/kcwl/elastic/actions/workflows/cmake-ubuntu-clang.yml/badge.svg)](https://github.com/kcwl/elastic/actions/workflows/cmake-ubuntu-clang.yml)
[![visual stadio](https://github.com/kcwl/elastic/actions/workflows/cmake-windows-vs.yml/badge.svg)](https://github.com/kcwl/elastic/actions/workflows/cmake-windows-vs.yml)

## introdution
elastic is the serialization library for c++ with c++20. support multiple types to serialize, it depens on [[reflect](https://github.com/kcwl/reflect.git)] to reflect member varibales.
the library is header-only, so it is easy to use, only include header files.

## data type
these types are currently supported!

### basic type

|type| note|
|--|--|
|char| scalar type |
|bool| scalar type |
|intN| int8, int16, int32, int64|
|uintN| uint8_t, uint16_t. uint32_t. uint64_t|
|double| double precision|
|float| single precision|
|enum/enum class| enum|
|string| characters|
|vector<T>| sequence|

### complex type
+ inhert type need to add `ELASTIC_ACCESS_IF` to regist serialize limit.
+ no-pod type need to add `ELASTIC_ACCESS`  to regist serialize limit.

## core
the core of elastic is flex_buffer, flex_buffer is used for carry data by archive.flex_buffer define at below:

```
flex_buffer<_Element, _Traits>::flex_buffer

flex_buffer();		//defalut construction, construct a flex_buffer that capacity is 4096
		
flex_buffer(size_type number); // construct a flex_buffer that capacity is [number]

template <typename _Iter>
flex_buffer(_Iter begin, _Iter end);  // construct a flex_buffer by iterator

template <typename _Ty, std::size_t N, 
	typename = std::is_convertible<_Ty, _Elem>>
flex_buffer(std::span<_Ty, N> data);    // construct a flex_buffer by span

flex_buffer(const void* buffer, size_type sz); //construct a flex_buffer by C-Way
```

## usage
elastic provides two interfaces,that are `to_binary` and `from_binary`;

```
template <typename _Ty, typename _Elem, typename _Traits = std::char_traits<_Elem>>
bool to_binary(_Ty&& t, flex_buffer<_Elem, _Traits>& buffer);                               // convert t to binary buffers

template <typename _Ty, typename _Elem, typename _Traits = std::char_traits<_Elem>>
bool from_binary(_Ty& t, flex_buffer<_Elem, _Traits>& buffer);								//convert binary iostream to t								
```

## example

```
	elastic::flex_buffer_t buf;

	int8_t a_in = (std::numeric_limits<int8_t>::max)();

	elastic::to_binary(a_in, buf);

	int8_t a_out{};

	elastic::from_binary(a_out, buf);
```

`a_in` will be equal to `a_out` in the above code!
more examples has represented on unit test

## wiki
