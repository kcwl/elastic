#pragma once
#include <elastic/detail/concepts.hpp>
#include <streambuf>
#include <iterator>

namespace elastic
{
	template <typename _Elem, typename _Traits>
	class basic_binary_primitive
	{
	public:
		constexpr static std::size_t bitwises = sizeof(char);

	public:
		explicit basic_binary_primitive(std::basic_streambuf<_Elem, _Traits>& sb)
			: streambuf_(sb)
		{}

		std::basic_streambuf<_Elem, _Traits>* rdbuf()
		{
			return &streambuf_;
		}

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
		void append(const _Ty& t)
		{
			save_binary(std::addressof(t), sizeof(_Ty));
		}

		template <detail::sequence_t _Ty>
		void append(const _Ty& s)
		{
			std::size_t bytes = s.size();

			this->archive()->append(bytes);

			save_binary(s.data(), bytes * sizeof(typename _Ty::value_type) / this->bitwises);
		}

		template <detail::char_t _Ty>
		void append(_Ty* t)
		{
			std::size_t bytes = str<_Ty>::len(t);

			save_binary(t, bytes);
		}

	private:
		template <typename _Ty>
		struct str
		{
			static std::size_t len(const _Ty* t)
			{
				return std::strlen(t);
			}
		};

		template <>
		struct str<wchar_t>
		{
			static std::size_t len(const wchar_t* t)
			{
				return std::wcslen(t);
			}
		};

	private:
		void save_binary(const void* address, std::size_t count)
		{
			count = (count + sizeof(_Elem) - 1) / sizeof(_Elem);

			this->streambuf_.sputn(static_cast<const _Elem*>(address), static_cast<std::streamsize>(count));
		}
	};

	template <typename _Archive, typename _Elem, typename _Traits>
	class binary_iprimitive : public binary_primitive<_Archive, _Elem, _Traits>
	{
		friend class std::basic_streambuf<_Elem, _Traits>;

	public:
		explicit binary_iprimitive(std::basic_streambuf<_Elem, _Traits>& sb)
			: binary_primitive<_Archive, _Elem, _Traits>(sb)
		{}

		~binary_iprimitive() = default;

	public:
		template <typename _Ty>
		_Ty read()
		{
			_Ty t{};

			load_binary(std::addressof(t), sizeof(_Ty));

			return t;
		}

		template <detail::sequence_t _Ty>
		_Ty read()
		{
			_Ty t{};

			std::size_t l{};

			this->archive()->template read(l);

			t.resize(l);

			if (l == 0)
				return;

			load_binary(&(*t.begin()), l);

			return t;
		}

		template <detail::char_t _Ty>
		_Ty read()
		{
			_Ty t{};
			std::size_t l{};
			this->archive()->template read(l);
			load_binary(&t, l * sizeof(_Ty) / this->bitwishes);
			t[l] = end<_Ty>::value;

			return t;
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
		template <typename _Ty>
		struct end
		{
			static constexpr _Ty value = '\0';
		};

		template <>
		struct end<wchar_t>
		{
			static constexpr wchar_t value = L'\0';
		};

	private:
		void load_binary(void* address, std::size_t count)
		{
			std::streamsize s = count / sizeof(_Elem);

			auto scount = this->streambuf_.sgetn(static_cast<_Elem*>(address), s);

			if (scount != s)
				throw std::runtime_error("memory is not enough!");
		}

	private:
		int trans_pos_;
	};
} // namespace elastic