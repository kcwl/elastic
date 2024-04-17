#pragma once
#include <cstring>
#include <ios>
#include <iterator>
#include <span>
#include <utility>
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
		flex_buffer(const std::size_t capa = capacity, bool has_trans = false)
			: buffer_(capa, 0)
			, pbase_(nullptr)
			, pptr_(nullptr)
			, epptr_(nullptr)
			, eback_(nullptr)
			, gptr_(nullptr)
			, pcount_(capa)
			, gcount_(0)
			, egptr_(pptr_)
			, capacity_(capa)
			, start_pos_(0)
			, has_success_(true)
			, has_trans_(has_trans)
		{
			reset();
		}

		flex_buffer(const flex_buffer&) = default;
		flex_buffer& operator=(const flex_buffer&) = default;

		virtual ~flex_buffer() = default;

	public:
		const size_type active() const noexcept
		{
			return pcount_;
		}

		const_pointer wdata() const noexcept
		{
			return gptr_;
		}

		const_pointer rdata() const noexcept
		{
			return pptr_;
		}

		void commit(int bytes)
		{
			bytes = static_cast<int>((std::min<std::size_t>)(bytes, epptr_ - pptr_));
			pptr_ += bytes;
			pcount_ -= bytes;
			gcount_ += bytes;
		}

		void consume(int bytes)
		{
			if (gptr_ + bytes > pptr_)
				bytes = static_cast<int>(pptr_ - gptr_);

			if (gptr_ + bytes < eback_)
				bytes = static_cast<int>(eback_ - gptr_);

			gptr_ += bytes;
		}

		size_type size() const noexcept
		{
			return pptr_ - gptr_;
		}

		size_type max_size() const
		{
			return capacity_;
		}

		void normalize()
		{
			if (pptr_ == pbase_)
				return;

			const auto sz = gptr_ - eback_;

			traits_type::copy(buffer_.data(), wdata(), sz);

			reset();

			pcount_ += sz;
			gcount_ -= sz;
		}

		void ensure()
		{
			if (active() > water_line)
				return;

			buffer_.resize(max_size() + capacity);

			epptr_ = &buffer_[0] + buffer_.size();

			capacity_ += capacity;

			pcount_ += capacity;
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
			if (!has_trans_)
				return true;

			if (start_pos_ != 0)
				return false;

			start_pos_ = gptr_ - eback_;

			return true;
		}

		void close()
		{
			if (!has_trans_)
				return;

			if (has_success_)
			{
				return;
			}

			// this->pubseekpos(start_pos_, std::ios::out);

			start_pos_ = 0;
		}

		std::size_t save(const value_type* data, const std::size_t size)
		{
			auto sz = pcount_;

			if (sz > size)
				sz = size;

			traits_type::copy(pptr_, data, sz);

			pptr_ += sz;
			pcount_ -= sz;
			gcount_ += sz;

			return sz;
		}

		size_type load(value_type* data, const std::size_t size)
		{
			auto sz = gcount_;

			if (sz > size)
				sz = size;

			traits_type::copy(data, gptr_, sz);

			gptr_ += sz;
			gcount_ -= sz;

			return sz;
		}

		void swap(this_type& other)
		{
			buffer_.swap(other.buffer_);
			std::swap(pbase_, other.pbase_);
			std::swap(pptr_, other.pptr_);
			std::swap(epptr_, other.epptr_);
			std::swap(eback_, other.eback_);
			std::swap(gptr_, other.gptr_);
			std::swap(egptr_, other.egptr_);
			std::swap(pcount_, other.pcount_);
			std::swap(gcount_, other.gcount_);
			std::swap(capacity_, other.capacity_);
			std::swap(start_pos_, other.start_pos_);
			std::swap(has_success_, other.has_success_);
			std::swap(has_trans_, other.has_trans_);
		}

		void append(const this_type& stream)
		{
			auto sz = stream.size();

			const auto act = active();

			if (sz > act)
			{
				sz -= act;
				buffer_.resize(capacity_ += sz);

				pcount_ = 0;
				gcount_ = capacity_;

			}
			else
			{
				pcount_ -= sz;
				gcount_ += sz;
			}

			std::memcpy(pptr_, stream.wdata(), stream.size());

			pptr_ += sz;
		}

	private:
		void reset()
		{
			if (buffer_.empty())
				return;

			pbase_ = &buffer_[0];
			pptr_ = &buffer_[0];
			epptr_ = &buffer_[0] + buffer_.size();

			eback_ = &buffer_[0];
			gptr_ = &buffer_[0];
			egptr_ = &buffer_[0];
		}

	private:
		std::vector<value_type, allocator_type> buffer_;

		value_type* pbase_;

		value_type* pptr_;

		value_type* epptr_;

		value_type* eback_;

		value_type* gptr_;

		value_type* egptr_;

		std::size_t pcount_;

		std::size_t gcount_;

		std::size_t capacity_;

		off_type start_pos_;

		bool has_success_;

		bool has_trans_;
	};

	using flex_buffer_t = flex_buffer<uint8_t, std::char_traits<uint8_t>>;
} // namespace elastic