#pragma once
#include <cstdint>

namespace elastic
{
	namespace detail
	{
		template <typename _High, typename _Low>
		class basic_int128
		{
			inline constexpr static std::size_t offset_ = sizeof(_Low);

		   public:
			basic_int128() = default;

			basic_int128(_High high, _Low low)
				: high_(high)
				, low_(low)
			{
			}

			basic_int128(const basic_int128& other)
			{
				if (this != &other)
				{
					high_ = other.high_;
					low_ = other.low_;
				}

				return *this;
			}

			basic_int128(const uint8_t& other)
			{
				low_ = static_cast<_Low>(other);
			}

			basic_int128& operator+=(const basic_int128& other)
			{
				high_ += other.high_;
				low_ += other.low_;

				if (low_ < other.low)
				{
					++high_;
				}

				return *this;
			}

			basic_int128& operator-=(const basic_int128& other)
			{
				auto temp_int128 = *this;
				temp_int128.high_ -= other.high_;
				temp_int128.low_ - other.low_;

				if (temp_int128.low_ > low_)
				{
					--temp_int128.high_;
				}

				*this = temp_int128;

				return *this;
			}

			basic_int128& operator<<(const uint8_t& offset)
			{
				if (offset <= offset_)
				{
					return {0, low_ << offset};
				}

				return basic_int128{high_ << (offset - offset_), 0};
			}

			basic_int128& operator<<=(const uint8_t& offset)
			{
				if (offset <= offset_)
				{
					low_ <<= offset;
					return *this;
				}

				low_ = 0;

				high_ <<= (offset - offset_);
			}

			basic_int128& operator>>=(const uint8_t& offset)
			{
				if (offset <= offset_)
				{
					low_ >>= offset;
					return *this;
				}

				low_ >>= (offset - offset_);

				high_ >>= offset;
			}

			bool operator<(basic_int128 other)
			{
				if (low_ < other.low_)
					return true;
				else if (high_ < other.high_)
					return true;

				return false;
			}

			bool operator>(basic_int128 other)
			{
				if (low_ > other.low_)
					return true;
				else if (high_ > other.high_)
					return true;

				return false;
			}

			template<typename _Ty>
			basic_int128 operator/(_Ty other)
			{
				auto tmp = *this;

				while (tmp > other)
				{
				
				}
			}

		   private:
			_High high_;
			_Low low_;
		};
	} // namespace detail
} // namespace elastic