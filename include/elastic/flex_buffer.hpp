#pragma once
#include <cstring>
#include <ios>
#include <iterator>
#include <span>
#include <streambuf>
#include <utility>
#include <vector>

namespace elastic
{
	template <typename _Elem, typename _Traits, typename _Alloc = std::allocator<_Elem>>
	class flex_buffer : public std::basic_streambuf<_Elem, _Traits>
	{
		using elem_type = _Elem;

		using allocator_type = _Alloc;

		using traits_type = _Traits;

		using this_type = flex_buffer<elem_type, traits_type, allocator_type>;

		constexpr static std::size_t capacity = 512;

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
		flex_buffer()
			: flex_buffer(capacity)
		{}

		flex_buffer(const std::size_t capa)
			: buffer_(capa, 0)
			, capacity_(capa)
			, start_pos_(0)
			, has_success_(true)
		{
			reset();
		}

		flex_buffer(const flex_buffer&) = default;
		flex_buffer& operator=(const flex_buffer&) = default;

		virtual ~flex_buffer() = default;

	public:
		const size_type active() const noexcept
		{
			return this->epptr() - this->pptr();
		}

		const_pointer wdata() const noexcept
		{
			return this->gptr();
		}

		const_pointer rdata() const noexcept
		{
			return this->pptr();
		}

		void commit(int bytes)
		{
			bytes = static_cast<int>((std::min<std::size_t>)(bytes, this->epptr() - this->pptr()));
			this->pbump(static_cast<int>(bytes));
			this->setg(this->eback(), this->gptr(), this->pptr());
		}

		void consume(int bytes)
		{
			if (this->egptr() < this->pptr())
				this->setg(&buffer_[0], this->gptr(), this->pptr());

			if (this->gptr() + bytes > this->pptr())
				bytes = static_cast<int>(this->pptr() - this->gptr());

			if (this->gptr() + bytes < this->eback())
				bytes = static_cast<int>(this->eback() - this->gptr());

			this->gbump(bytes);
		}

		size_type size() const noexcept
		{
			return this->pptr() - this->gptr();
		}

		size_type max_size() const
		{
			return capacity_;
		}

		void normalize()
		{
			if (this->pptr() == this->pbase())
				return;

			traits_type::copy(buffer_.data(), wdata(), active());

			reset();
		}

		void ensure()
		{
			if (active() > water_line)
				return;

			buffer_.resize(max_size() + capacity);

			auto _pptr = this->pptr();

			this->setp(&buffer_[0], _pptr, &buffer_[0] + buffer_.size());

			this->setg(&buffer_[0], this->gptr(), _pptr);

			capacity_ += capacity;
		}

		bool success() const
		{
			return has_success_;
		}

		void failed()
		{
			has_success_ = false;
		}

		bool start()
		{
			//if (start_pos_ != 0)
			//	return false;

			//start_pos_ = this->gptr() - this->eback();

			return true;
		}

		void close()
		{
			if (has_success_)
			{
				return;
			}

			//this->pubseekpos(start_pos_, std::ios::out);

			start_pos_ = 0;
		}

		bool save(value_type* data, const size_type sizes)
		{
			if (sizes > active())
				return false;

			std::memcpy(this->pptr(), data, sizes);

			commit(static_cast<int>(sizes));

			return true;
		}

		size_type load(value_type* data, const size_type sizes)
		{
			if (sizes > size())
				return 0;

			std::memcpy(data, this->gptr(), sizes);

			consume(static_cast<int>(sizes));

			return sizes;
		}

	protected:
		virtual int_type underflow() override
		{
			const auto pptr = this->pptr();

			if (pptr == this->eback())
				return _Traits::eof();

			auto result = _Traits::to_int_type(*this->gptr());

			consume(1);

			return result;
		}

		virtual pos_type seekoff(off_type off, std::ios_base::seekdir way, std::ios_base::openmode mode) override
		{
			// change position by offset, according to way and mode

			const auto pptr_old = this->pptr();

			const auto gptr_old = this->gptr();

			const auto seek_low = this->eback();

			const auto seek_dist = pptr_old - seek_low;

			off_type new_off{};

			switch (way)
			{
			case std::ios::beg:
				{
					new_off = 0;
				}
				break;
			case std::ios::end:
				{
					new_off = pptr_old - this->eback();
				}
				break;
			case std::ios::cur:
				{
					constexpr auto both = std::ios::in | std::ios::out;
					if ((mode & both) != both)
					{
						if (mode & std::ios::in)
						{
							if (pptr_old || !this->eback())
							{
								new_off = pptr_old - this->pbase();
							}
						}
						else if ((mode & std::ios::out) && (gptr_old || !this->eback()))
						{
							new_off = gptr_old - this->eback();
						}
					}
				}
				break;
			default:
				return pos_type(off_type(-1));
			}

			if (static_cast<unsigned long long>(off) + new_off > static_cast<unsigned long long>(seek_dist))
				return pos_type(off_type(-1));

			off += new_off;

			const auto new_ptr = seek_low + off;

			if ((mode & std::ios::in) && gptr_old)
				this->setg(seek_low, new_ptr, pptr_old);

			if ((mode & std::ios::out) && pptr_old)
				this->setg(seek_low, new_ptr, this->epptr());

			return pos_type(off);
		}

		virtual pos_type seekpos(pos_type pos, std::ios_base::openmode mode) override
		{
			// change to specified position, according to mode
			const auto off = static_cast<std::streamoff>(pos);

			const auto gptr_old = this->gptr();

			const auto pptr_old = this->pptr();

			const auto seek_low = this->eback();

			const auto seek_dist = pptr_old - seek_low;

			if (static_cast<unsigned long long>(off) > static_cast<unsigned long long>(seek_dist))
				return pos_type(off_type(-1));

			const auto new_ptr = seek_low + off;

			if ((mode & std::ios::in) && gptr_old)
			{
				this->setg(seek_low, new_ptr, pptr_old);
			}

			if ((mode & std::ios::out) && pptr_old)
			{
				this->setp(seek_low, new_ptr);
			}

			return pos_type(off);
		}

	private:
		void reset()
		{
			if (buffer_.empty())
				return;

			this->setg(&buffer_[0], &buffer_[0], &buffer_[0]);
			this->setp(&buffer_[0], &buffer_[0] + buffer_.size());
		}

	private:
		std::vector<value_type, allocator_type> buffer_;

		std::size_t capacity_;

		off_type start_pos_;

		bool has_success_;
	};

	using flex_buffer_t = flex_buffer<uint8_t, std::char_traits<uint8_t>>;
} // namespace elastic