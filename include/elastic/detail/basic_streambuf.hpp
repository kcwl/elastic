#pragma once
#include <elastic/detail/concepts.hpp>
#include <iterator>
#include <span>
#include <streambuf>
#include <vector>

namespace elastic
{
	namespace detail
	{
		template <typename _Ty, typename _Alloc = std::allocator<_Ty>>
		class basic_streambuf
		{
		public:
			using iterator = std::vector<_Ty, _Alloc>::iterator;
			using const_iterator = typename std::vector<_Ty, _Alloc>::const_iterator;
			using value_type = _Ty;
			using size_type = std::size_t;
			using reference = _Ty&;
			using const_reference = const _Ty&;
			using pointer = _Ty*;
			using const_pointer = const _Ty*;

			basic_streambuf()
				: wpos_(0)
				, rpos_(0)
				, buffer_()
			{}

			basic_streambuf(size_type capa)
				: basic_streambuf()
			{
				resize(capa);
			}

			template <typename _Iter>
			basic_streambuf(_Iter begin, _Iter end)
			{
				clear();

				std::copy(begin, end, std::back_inserter(buffer_));

				commit(std::distance(begin, end));
			}

			template <typename _U, std::size_t N>
			requires(std::is_convertible_v<_U,_Ty>)
			basic_streambuf(std::span<_U, N> data)
				: basic_streambuf(data.begin(), data.end())
			{}

			basic_streambuf(const basic_streambuf& buf)
			{
				if (this != &buf)
				{
					wpos_ = buf.wpos_;
					rpos_ = buf.rpos_;
					buffer_ = buf.buffer_;
				}
			}

			virtual ~basic_streambuf() = default;

		public:
			value_type& operator[](size_type pos)
			{
				return buffer_.at(pos);
			}

		public:
			size_type size() noexcept
			{
				return buffer_.size();
			}

			const size_type size() const noexcept
			{
				return buffer_.size();
			}

			pointer data() noexcept
			{
				return buffer_.data();
			}

			const_pointer data() const noexcept
			{
				return buffer_.data();
			}

			pointer rdata() noexcept
			{
				return buffer_.data() + rpos_;
			}

			const_pointer rdata() const noexcept
			{
				return buffer_.data() + rpos_;
			}

			pointer wdata() noexcept
			{
				return buffer_.data() + wpos_;
			}

			const_pointer wdata() const noexcept
			{
				return buffer_.data() + wpos_;
			}

			iterator begin() noexcept
			{
				return buffer_.begin();
			}

			const_iterator begin() const noexcept
			{
				return buffer_.begin();
			}

			iterator rbegin() noexcept
			{
				auto iter = begin();
				std::advance(iter, rpos_);

				return iter;
			}

			const_iterator rbegin() const noexcept
			{
				auto iter = begin();
				std::advance(iter, rpos_);

				return iter;
			}

			iterator wbegin() noexcept
			{
				auto iter = begin();
				std::advance(iter, wpos_);

				return iter;
			}

			const_iterator wbegin() const noexcept
			{
				auto iter = begin();
				std::advance(iter, wpos_);

				return iter;
			}

			iterator end() noexcept
			{
				return buffer_.end();
			}

			const_iterator end() const noexcept
			{
				return buffer_.end();
			}

			void resize(size_type bytes)
			{
				buffer_.resize(bytes);
			}

			void clear() noexcept
			{
				wpos_ = 0;
				rpos_ = 0;
				buffer_.clear();
			}

			void swap(basic_streambuf& buf)
			{
				buffer_.swap(buf);
			}

			auto erase(const_iterator& where)
			{
				consume(-1);

				return buffer_.erase(where);
			}

			auto erase(const_iterator& begin, const_iterator& end)
			{
				auto distance = std::distance(begin, end);

				consume(-distance);

				return buffer_.erase(begin, end);
			}

			void commit(size_type bytes)
			{
				wpos_ += bytes;
			}

			void consume(size_type bytes)
			{
				rpos_ += bytes;
			}

			template <typename _U>
			_U read()
			{
				constexpr auto bytes = sizeof(_U);

				_U value{};

				read(&value, bytes);

				return value;
			}

			template <typename _U>
			void read(_U* dest, size_type bytes)
			{
				if (bytes + rpos_ > buffer_.size())
				{
					*dest = 0;
					return;
				}

				std::memcpy(dest, buffer_.data() + rpos_, bytes);

				consume(bytes);
			}

			template <typename _U, typename _Alloc>
			void append(const basic_streambuf<_U, _Alloc>& buf)
			{
				std::copy(buf.begin(), buf.end(), std::back_inserter(buffer_));

				commit(std::distance(buf.begin(), buf.end()));
			}

			template <detail::pod _U>
			void append(_U&& value)
			{
				auto res = std::forward<_U>(value);
				constexpr auto bytes = sizeof(_U);

				resize(size() + bytes);

				std::memcpy(wdata(), &res, bytes);

				commit(bytes);
			}

		private:
			size_type wpos_;
			size_type rpos_;
			std::vector<_Ty, _Alloc> buffer_;
		};
	} // namespace detail
} // namespace elastic