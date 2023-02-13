#pragma once
#include <elastic/detail/concepts.hpp>
#include <streambuf>
#include <elastic/array_wrapper.hpp>
#include <elastic/archive_exception.hpp>

namespace elastic
{
	template<typename _Archive, typename _Elem, typename _Traits>
	class basic_binary_iprimitive
	{
	protected:
		basic_binary_iprimitive(std::basic_streambuf<_Elem, _Traits>& sb, [[maybe_unused]]bool no_codecvt)
			: buffer_(sb)
		{

		}

		~basic_binary_iprimitive()
		{
		}

	public:
		struct use_array_optimization
		{
			template<typename _Ty>
			struct apply : std::is_arithmetic<_Ty>
			{};
		};

		template<typename _ValueType>
		void load_array(array_wrapper<_ValueType>& a, uint32_t)
		{
			load_binary(a.address(), a.count() * sizeof(_ValueType));
		}

		void load_binary(void* address, std::size_t count)
		{
			std::streamsize s = static_cast<std::streamsize>(count / sizeof(_Elem));
			std::streamsize scount = buffer_.sgetn(static_cast<_Elem*>(address), s);
			if (scount != s)
				throw(archive_exception(archive_exception::exception_code::input_stream_error));
			// note: an optimizer should eliminate the following for char files
			//BOOST_ASSERT(count % sizeof(Elem) <= boost::integer_traits<std::streamsize>::const_max);
			s = static_cast<std::streamsize>(count % sizeof(_Elem));
			if (0 < s)
			{
				//        if(is.fail())
				//            boost::serialization::throw_exception(
				//                archive_exception(archive_exception::stream_error)
				//        );
				_Elem t{};
				scount = buffer_.sgetn(&t, 1);
				if (scount != 1)
					throw(archive_exception(archive_exception::exception_code::input_stream_error));
				std::memcpy(static_cast<char*>(address) + (count - s), &t, static_cast<std::size_t>(s));
			}
		}

	protected:
		_Archive* _this()
		{
			return static_cast<_Archive*>(this);
		}

	protected:
		std::basic_streambuf<_Elem, _Traits>& buffer_;
	};
}