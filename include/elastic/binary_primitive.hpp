#pragma once
#include <elastic/detail/concepts.hpp>
#include <iterator>
#include <streambuf>

namespace elastic
{
	template <typename _Elem, typename _Traits>
	class basic_binary_primitive
	{
	public:
		explicit basic_binary_primitive(std::basic_streambuf<_Elem, _Traits>& sb)
			: streambuf_(sb)
		{}

	public:
		std::basic_streambuf<_Elem, _Traits>& streambuf_;
	};

	template <typename _Archive, typename _Elem, typename _Traits>
	class binary_primitive : public basic_binary_primitive<_Elem, _Traits>
	{
	public:
		explicit binary_primitive(std::basic_streambuf<_Elem, _Traits>& sb)
			: basic_binary_primitive<_Elem, _Traits>(sb)
		{}

		_Archive* archive()
		{
			return static_cast<_Archive*>(this);
		}
	};

	template <typename _Archive, typename _Elem, typename _Traits>
	class binary_oprimitive : public binary_primitive<_Archive, _Elem, _Traits>
	{
	public:
		explicit binary_oprimitive(std::basic_streambuf<_Elem, _Traits>& sb)
			: binary_primitive<_Archive, _Elem, _Traits>(sb)
		{}

		~binary_oprimitive() = default;

	public:
		template <typename _Ty>
		void append(const _Ty& value)
		{
			constexpr auto bytes = sizeof(_Ty);

			this->append((_Elem*)&value, bytes);
		}

		void append(_Elem* value, std::size_t bytes)
		{
			this->streambuf_.sputn(value, bytes);
		}
	};

	template <typename _Archive, typename _Elem, typename _Traits>
	class binary_iprimitive : public binary_primitive<_Archive, _Elem, _Traits>
	{
	public:
		explicit binary_iprimitive(std::basic_streambuf<_Elem, _Traits>& sb)
			: binary_primitive<_Archive, _Elem, _Traits>(sb)
		{}

		~binary_iprimitive() = default;

	public:
		void read(_Elem* address, std::size_t bytes)
		{
			auto scount = this->streambuf_.sgetn(address, bytes);

			if (scount != static_cast<int>(bytes))
				throw std::runtime_error("memory is not enough!");
		}

		template <typename _Ty>
		_Ty read()
		{
			_Ty value{};

			this->read((_Elem*)&value, sizeof(_Ty));

			return value;
		}

		void start()
		{
			trans_pos_ = static_cast<int>(this->streambuf_.pubseekoff(0, std::ios::cur, std::ios::in));
		}

		void roll_back()
		{
			this->streambuf_.pubseekpos(trans_pos_, std::ios::in);
		}

	private:
		int trans_pos_;
	};
} // namespace elastic