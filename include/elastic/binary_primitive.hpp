#pragma once
#include <elastic/detail/concepts.hpp>
#include <iterator>
#include <elastic/serialize_streambuf.hpp>

namespace
{
	constexpr static std::size_t bitwises = sizeof(char);
}

namespace elastic
{
	template<typename _Elem, typename _Traits>
	class basic_binary_primitive
	{
	public:
		explicit basic_binary_primitive(std::basic_streambuf<_Elem, _Traits>& sb)
			: streambuf_(sb)
		{

		}

	public:
		std::basic_streambuf<_Elem, _Traits>& streambuf_;
	};

	template<typename _Archive, typename _Elem, typename _Traits>
	class binary_primitive
		: public basic_binary_primitive<_Elem, _Traits>
	{
	public:
		explicit binary_primitive()
			: binary_primitive(0)
		{

		}

		explicit binary_primitive(std::size_t number)
			: buffer_(number)
			, basic_binary_primitive<_Elem, _Traits>(*buffer_.rdbuf())
		{

		}

		explicit binary_primitive(std::basic_streambuf<_Elem, _Traits>& sb)
			: basic_binary_primitive<_Elem, _Traits>(sb)
			, buffer_(0)
		{

		}

		_Archive* archive()
		{
			return static_cast<_Archive*>(this);
		}

	private:
		serialize_streambuf<_Elem,_Traits> buffer_;
	};

	template<typename _Archive, typename _Elem, typename _Traits>
	class binary_oprimitive : public binary_primitive<_Archive, _Elem, _Traits>
	{
	public:
		explicit binary_oprimitive() = default;

		explicit binary_oprimitive(std::size_t number)
			: binary_primitive<_Archive, _Elem, _Traits>(number)
		{

		}

		explicit binary_oprimitive(std::basic_streambuf<_Elem, _Traits>& sb)
			: binary_primitive<_Archive, _Elem, _Traits>(sb)
		{

		}

		~binary_oprimitive() = default;

		template<typename _Ty>
		void save(const _Ty& t)
		{
			save_binary(&t, sizeof(_Ty));
		}

		template<detail::string_t _Ty>
		void save(const _Ty& s)
		{
			std::size_t bytes = s.size();

			this->archive()->save(bytes);

			save_binary(s.data(), bytes * sizeof(typename _Ty::value_type) / bitwises);
		}

		template<detail::char_t _Ty>
		void save(const _Ty* t)
		{
			std::size_t bytes = str<_Ty>::len(t);

			this->archive()->save(bytes);

			save_binary(t, bytes * sizeof(_Ty) / bitwises);
		}

		template<typename _Array>
		void save_array(const _Array& a)
		{

		}

		template<attribute _Ty>
		void save(const _Ty& t)
		{
			if constexpr (optional_t<std::remove_cvref_t<_Ty>>)
			{
				if constexpr (_Ty::require_value)
				{
					if (!t.has_value())
					{
						throw std::runtime_error("maybe some type must have some values!");
					}
				}
			}

			save(*t);
		}

	private:
		template<typename _Ty>
		struct str
		{
			static std::size_t len(const _Ty* t)
			{
				return std::strlen(t);
			}
		};

		template<>
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

			auto scount = streambuf_.sputn(static_cast<_Elem*>(address), static_cast<std::streamsize>(count));

			if (count != scount)
				throw;
		}
	};

	template<typename _Archive, typename _Elem, typename _Traits>
	class binary_iprimitive : public binary_primitive<_Archive, _Elem, _Traits>
	{
	public:
		explicit binary_iprimitive() = default;

		explicit binary_iprimitive(std::size_t number)
			: binary_primitive<_Archive, _Elem, _Traits>(number)
		{

		}

		explicit binary_iprimitive(std::basic_streambuf<_Elem, _Traits>& sb)
			: binary_primitive<_Archive, _Elem, _Traits>(sb)
		{

		}

		~binary_iprimitive() = default;

		template<typename _Ty>
		void load(_Ty& t)
		{
			load_binary(&t, sizeof(_Ty));
		}

		template<detail::string_t _Ty>
		void load(_Ty& t)
		{
			std::size_t l{};

			this->archive()->load(l);

			t.resize(l);

			if (l == 0)
				return;

			load_binary(&(*t.begin()), l);
		}

		template<detail::char_t _Ty>
		void load(_Ty& t)
		{
			std::size_t l{};
			this->archive()->load(l);
			load_binary(&t, l * sizeof(_Ty) / bitwishes);
			t[l] = end<_Ty>::value;
		}

	private:
		template<typename _Ty>
		struct end
		{
			static constexpr _Ty value = '\0';
		};

		template<>
		struct end<wchar_t>
		{
			static constexpr wchar_t value = L'\0';
		};

	private:
		void load_binary(void* address, std::size_t count)
		{
			std::streamsize s = count / sizeof(_Elem);

			auto scount = sb_.sgetn(static_cast<_Elem*>(address), s);

			if (scount != s)
				throw;

			s = count % sizeof(_Elem);

			if (s > 0)
			{
				_Elem t{};
				scount = streambuf_.sgetn(&t, 1);

				if (scount != 1)
					throw;

				std::memcpy(static_cast<char*>(address) + (count - s), &t, static_cast<std::size_t>(s));
			}
		}

	};
}