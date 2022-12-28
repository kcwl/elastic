#pragma once
#include <elastic/detail/concepts.hpp>
#include <iterator>
#include <streambuf>

namespace
{
	constexpr static std::size_t bitwises = sizeof(char);
}

namespace elastic
{
	template<typename _Elem, typename _Traits, typename _Alloc>
	class basic_binary_primitive
	{
		using pointer = typename std::basic_streambuf<_Elem, _Traits>::char_type*;
		using const_pointer = const pointer;

	public:
		explicit basic_binary_primitive(std::basic_streambuf<_Elem, _Traits>& sb)
			: streambuf_(sb)
		{

		}

	public:
		std::size_t active() noexcept
		{
			return streambuf_.pptr() - streambuf_.gptr();
		}

		std::size_t active() const noexcept
		{
			return streambuf_.pptr() - streambuf_.gptr();
		}

		pointer wdata() noexcept
		{
			return streambuf_.pptr();
		}

		const_pointer wdata() const noexcept
		{
			return streambuf_.pptr();
		}

		pointer rdata() noexcept
		{
			return streambuf_.gptr();
		}

		const_pointer rdata() const noexcept
		{
			return streambuf_.gptr();
		}

		void commit(int bytes)
		{
			bytes = std::(min)(bytes, streambuf_.epptr() - streambuf_.pptr());
			streambuf_.pbump(static_cast<int>(bytes));
			streambuf_.setg(streambuf_.eback(), streambuf_.gptr(), streambuf_.pptr());
		}

		void consume(int bytes)
		{
			if (streambuf_.egptr() < streambuf_.pptr())
				streambuf_.setg(&buffer_[0], streambuf_.gptr(), streambuf_.pptr());

			if (streambuf_.gptr() + bytes > streambuf_.pptr())
				bytes = static_cast<int>(streambuf_.pptr() - streambuf_.gptr());

			streambuf_.gbump(bytes);
		}

	public:
		std::basic_streambuf<_Elem, _Traits>& streambuf_;
	};

	template<typename _Archive, typename _Elem, typename _Traits>
	class binary_primitive
		: public basic_binary_primitive<_Elem, _Traits, std::allocator<_Elem>>
		, public std::basic_streambuf<_Elem, _Traits>
	{
		using allocator_type = std::allocator<_Elem>;

		using base_type = std::basic_streambuf<_Elem, _Traits>;
	public:
		using iterator = typename std::vector<_Elem, allocator_type>::iterator;
		using const_iterator = typename std::vector<_Elem, allocator_type>::const_iterator;
		using value_type = typename std::vector<_Elem, allocator_type>::value_type;
		using size_type = typename std::vector<_Elem, allocator_type>::size_type;
		using reference = typename std::vector<_Elem, allocator_type>::reference;
		using const_reference = typename std::vector<_Elem, allocator_type>::const_reference;

		constexpr static size_type capacity = 4096;

	public:
		explicit binary_primitive()
			: binary_primitive(capacity)
		{

		}

		explicit binary_primitive(size_type number)
			: buffer_(number)
			, basic_binary_primitive<_Elem, _Traits, allocator_type>(*this)
		{
			reset();
		}

		explicit binary_primitive(std::basic_streambuf<_Elem, _Traits>& sb)
			: basic_binary_primitive<_Elem, _Traits, allocator_type>(sb)
			, buffer_(0)
		{

		}

		_Archive* archive()
		{
			return static_cast<_Archive*>(this);
		}

		constexpr iterator begin() noexcept
		{
			return buffer_.begin();
		}

		constexpr const_iterator begin() const noexcept
		{
			return buffer_.begin();
		}

		constexpr iterator end() noexcept
		{
			return buffer_.end();
		}

		constexpr const_iterator end() const noexcept
		{
			return buffer_.end();
		}

		void clear() noexcept
		{
			buffer_.clear();

			reset();
		}

		void resize(size_type bytes)
		{
			buffer_.resize(bytes);
		}

		void swap(std::basic_streambuf<_Elem, _Traits>& buf)
		{
			buffer_.swap(buf);
		}

		auto erase(const_iterator& where)
		{
			this->consume(-1);

			return buffer_.erase(where);
		}

		auto erase(const_iterator& begin, const_iterator& end)
		{
			auto dis = std::distance(begin, end);

			this->consume(-dis);

			return buffer_.erase(begin, end);
		}

		std::size_t size() noexcept
		{
			return buffer_.size();
		}

		std::size_t size() const noexcept
		{
			return buffer_.size();
		}

	private:
		void reset()
		{
			base_type::setg(&buffer_[0], &buffer_[0], &buffer_[0]);
			base_type::setp(&buffer_[0], &buffer_[0] + buffer_.size());
		}


	private:
		std::vector<_Elem, std::allocator<_Elem>> buffer_;
	};

	template<typename _Archive, typename _Elem, typename _Traits>
	class binary_oprimitive : private binary_primitive<_Archive, _Elem, _Traits>
	{
	public:
		explicit binary_oprimitive() = default;

		explicit binary_oprimitive(size_type number)
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
	class binary_iprimitive : private binary_primitive<_Archive, _Elem, _Traits>
	{
	public:
		explicit binary_iprimitive() = default;

		explicit binary_iprimitive(size_type number)
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