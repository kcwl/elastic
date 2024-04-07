#pragma once
#include <cstring>
#include <ios>
#include <iterator>
#include <span>
#include <streambuf>
#include <vector>

namespace elastic
{
	template <typename _Elem, typename _Traits, typename _Alloc = std::allocator<_Elem>>
	class flex_buffer
	{
		using elem_type = _Elem;

		using allocator_type = _Alloc;

		using traits_type = _Traits;

		using this_type = flex_buffer<elem_type, traits_type, allocator_type>;

		constexpr static std::size_t capacity = 4096;

		constexpr static std::size_t water_line = 32;

	public:
		using iterator = typename std::vector<elem_type, allocator_type>::iterator;
		using const_iterator = typename std::vector<elem_type, allocator_type>::const_iterator;
		using value_type = typename std::vector<elem_type, allocator_type>::value_type;
		using size_type = typename std::vector<elem_type, allocator_type>::size_type;
		using reference = typename std::vector<elem_type, allocator_type>::reference;
		using const_reference = typename std::vector<elem_type, allocator_type>::const_reference;
		using pointer = typename std::vector<elem_type, allocator_type>::pointer;
		using const_pointer = typename std::vector<elem_type, allocator_type>::const_pointer;

		using off_type = typename traits_type::off_type;
		using pos_type = typename traits_type::pos_type;
		using int_type = typename traits_type::int_type;

	public:
		flex_buffer() = default;

		flex_buffer(std::size_t capa)
			: buffer_(capa)
		{

		}

		template <typename _Iter>
		flex_buffer(_Iter begin, _Iter end)
			: flex_buffer()
		{
			std::copy(begin, end, std::back_inserter(buffer_));

			commit(static_cast<int>(buffer_.size()));
		}

		template <typename _Ty, std::size_t N, typename = std::is_convertible<_Ty, _Elem>>
		flex_buffer(std::span<_Ty, N> data)
			: flex_buffer(data.begin(), data.end())
		{
			static_assert(N <= capacity, "maybe input is too large!");
		}

		flex_buffer(flex_buffer&& other)
			: buffer_(other.buffer_)
			, pptr_(other.pptr_)
			, gptr_(other.gptr_)
		{
			flex_buffer{}.swap(other);
		}

		flex_buffer(const void* buffer, size_type sz)
			: flex_buffer()
		{
			buffer_.resize(sz);

			auto active_sz = active();

			sz > active_sz ? sz = active_sz : 0;

			traits_type::copy(rdata(), (elem_type*)buffer, sz);

			commit(static_cast<int>(sz));
		}

		flex_buffer(const flex_buffer&) = delete;
		flex_buffer& operator=(const flex_buffer&) = delete;

		flex_buffer& operator=(flex_buffer&& other)
		{
			flex_buffer{ std::move(other) }.swap(*this);

			return *this;
		}

		bool operator==(const flex_buffer& other) const
		{
			return buffer_ == other.buffer_ && pptr_ == other.pptr_ && gptr_ == other.gptr_;
		}

	public:
		size_type active() noexcept
		{
			return buffer_.size() - pptr_;
		}

		size_type active() const noexcept
		{
			return buffer_.size() - pptr_;
		}

		pointer wdata() noexcept
		{
			return buffer_.data() + gptr_;
		}

		const_pointer wdata() const noexcept
		{
			return buffer_.data() + gptr_;
		}

		pointer rdata() noexcept
		{
			return buffer_.data() + pptr_;
		}

		const_pointer rdata() const noexcept
		{
			return buffer_.data() + pptr_;
		}

		void commit(off_type bytes)
		{
			bytes = std::min<off_type>(bytes, buffer_.size() - pptr_);

			pptr_ += bytes;

			pptr_ < 0 ? pptr_ = 0 : 0;
		}

		void consume(off_type bytes)
		{
			bytes = std::min<off_type>(bytes, pptr_ - gptr_);

			gptr_ += bytes;

			gptr_ < 0 ? gptr_ = 0 : 0;
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
			return begin() + size();
		}

		constexpr const_iterator end() const noexcept
		{
			return begin() + size();
		}

		void resize(size_type value)
		{
			if (value == 0)
				return;

			buffer_.resize(buffer_.size() + value);
		}

		void clear() noexcept
		{
			buffer_.clear();

			pptr_ = 0;

			gptr_ = 0;
		}

		void swap(flex_buffer& other)
		{
			buffer_.swap(other.buffer_);

			std::swap(pptr_, other.pptr_);

			std::swap(gptr_, other.gptr_);
		}

		size_type size() noexcept
		{
			return pptr_ - gptr_;
		}

		size_type size() const noexcept
		{
			return pptr_ - gptr_;
		}

		void normalize()
		{
			if (pptr_ == 0)
				return;

			pptr_ = size();

			gptr_ = 0;

			traits_type::copy(buffer_.data(), wdata(), pptr_);
		}

		void ensure()
		{
			if (active() > water_line)
				return;

			buffer_.resize(max_size() + capacity);
		}

		size_type max_size()
		{
			return buffer_.size();
		}

		size_type max_size() const
		{
			return buffer_.size();
		}

		pos_type pubseekoff(off_type off, std::ios_base::seekdir way, std::ios_base::openmode mode)
		{
			off_type new_off{};

			switch (way)
			{
			case std::ios::beg:
				{
					new_off = 0;
				}

				break;
			case std::ios::cur:
				{
					if (mode & std::ios::in)
					{
						new_off = pptr_;
					}
					else if (mode & std::ios::out)
					{
						new_off = gptr_;
					}
				}
				break;
			case std::ios::end:
				{
					new_off = buffer_.size();
				}
				break;
			default:
				break;
			}

			off += new_off;

			off < 0 ? off = 0 : 0;

			if (mode & std::ios::in)
			{
				auto cur_pos = static_cast<off_type>(buffer_.size());

				off > cur_pos ? off = cur_pos - 1 : 0;

				pptr_ = off;
			}
			else if (mode & std::ios::out)
			{
				off > pptr_ ? gptr_ = pptr_ : gptr_ = off;
			}

			return static_cast<pos_type>(off);
		}

		pos_type pubseekpos(pos_type pos, std::ios_base::openmode mode)
		{
			if (mode & std::ios::in)
			{
				auto end_pos = static_cast<int>(buffer_.size());

				pos > end_pos ? pptr_ = end_pos : 0;

				pos < 0 ? pos = 0 : (pos_type)0;

				pptr_ = static_cast<off_type>(pos);
			}

			if (mode & std::ios::out)
			{
				pos > pptr_ ? gptr_ = pptr_ : 0;

				pos < 0 ? pos = 0 : (pos_type)0;

				gptr_ = static_cast<off_type>(pos);
			}

			return static_cast<pos_type>(pos);
		}

		size_type sputn(const value_type* begin, size_type size)
		{
			if (size == 0)
				return 0;

			for (size_type i = 0; i < size; ++i)
			{
				*rdata() = begin[i];

				commit(1);
			}

			return size;
		}

		size_type sputc(const value_type& c)
		{
			return sputn(&c, 1);
		}

		size_type sgetn(value_type* begin, size_type size)
		{
			if (size > this->size())
				return 0;

			for (size_type i = 0; i < size; ++i)
			{
				begin[i] = *wdata();

				consume(1);
			}

			return size;
		}

		size_type sgetc(value_type* c)
		{
			return sgetn(c, 1);
		}

		void append(this_type&& buffer)
		{
			auto size = buffer.size();

			auto act = active();

			if (size > act)
				buffer_.resize(max_size() + (size - act));

			traits_type::copy(rdata(), buffer.wdata(), size);

			commit(size);

			this_type{}.swap(buffer);
		}

	private:
		std::vector<value_type, allocator_type> buffer_;

		off_type pptr_;

		off_type gptr_;
	};

	using flex_buffer_t = flex_buffer<uint8_t, std::char_traits<uint8_t>>;
} // namespace elastic