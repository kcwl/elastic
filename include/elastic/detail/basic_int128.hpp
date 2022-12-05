#pragma once
#include <cstdint>

namespace
{
	template <typename _U128>
	std::pair<_U128, _U128> div_int128(const _U128& lhs, const _U128& rhs)
	{
		if (rhs == 0)
			throw std::runtime_error("error: divison by 0");

		std::pair<_U128, _U128> result{ 0, 0 };

		for (uint8_t i = lhs.bits(); i > 0; i--)
		{
			result.first <<= 1;
			result.second <<= 1;

			if ((lhs >> (i - 1)) & 0x1)
			{
				result.second++;
			}

			if (result.second >= rhs)
			{
				result.second -= rhs;
				result.first++;
			}
		}

		return result;
	}
} // namespace

namespace elastic
{
	namespace detail
	{
		template <detail::integer_like _High, detail::integer_like _Low>
		class basic_int128
		{
			inline constexpr static uint8_t offset_ = sizeof(_Low) * 8;

		public:
			basic_int128()
				: high_(0)
				, low_(0)
			{}

			basic_int128(_High high, _Low low)
				: high_(high)
				, low_(low)
			{}

			basic_int128(const basic_int128& other)
				: basic_int128(0, 0)
			{
				if (this != &other)
				{
					high_ = other.high_;
					low_ = other.low_;
				}
			}

			basic_int128& operator=(const basic_int128& other)
			{
				if (this != &other)
				{
					high_ = other.high_;
					low_ = other.low_;
				}

				return *this;
			}

			basic_int128(basic_int128&& other)
			{
				high_ = std::move(other.high_);
				low_ = std::move(other.low_);
			}

			template <detail::integer_like _Ty>
			basic_int128(_Ty other)
			{
				high_ = 0;
				low_ = static_cast<_Low>(other);
			}

			basic_int128 operator+(const basic_int128& other)
			{
				basic_int128 tmp{ high_ + other.high_, low_ + other.low_ };

				if (tmp.low_ < other.low_)
					++tmp.high_;

				return std::move(tmp);
			}

			basic_int128& operator+=(const basic_int128& other)
			{
				*this = *this + other;

				return *this;
			}

			template <detail::integer_like _Ty>
			basic_int128 operator+(const _Ty& other)
			{
				return std::move(*this + basic_int128{ other });
			}

			template <detail::integer_like _Ty>
			basic_int128& operator+=(const _Ty& other)
			{
				*this = *this + other;

				return *this;
			}

			basic_int128 operator-(const basic_int128& other)
			{
				basic_int128 tmp{ high_ - other.high_, low_ - other.low_ };

				if (tmp.low_ > low_)
					--tmp.low_;

				return std::move(tmp);
			}

			basic_int128 operator-=(const basic_int128& other)
			{
				*this = *this - other;

				return *this;
			}

			template <detail::integer_like _Ty>
			basic_int128 operator-(const _Ty& other)
			{
				return std::move(*this - basic_int128(other));
			}

			template <detail::integer_like _Ty>
			basic_int128& operator-=(const _Ty& other)
			{
				*this = *this - other;

				return *this;
			}

			basic_int128 operator*(const basic_int128& other)
			{
				basic_int128 result{};
				for (basic_int128 i = 0; i < other; ++i)
				{
					result += *this;
				}

				return std::move(result);
			}

			template <detail::integer_like _Ty>
			basic_int128 operator*(_Ty other)
			{
				return std::move(*this * basic_int128{ other });
			}

			basic_int128& operator*=(const basic_int128& other)
			{
				return *this = *this * other;
			}

			template <detail::integer_like _Ty>
			basic_int128& operator*=(_Ty other)
			{
				return *this *= basic_int128{ other };
			}

			basic_int128 operator/(const basic_int128& other)
			{
				return std::move(div_int128(*this, other).first);
			}

			template <detail::integer_like _Ty>
			basic_int128 operator/(const _Ty& other)
			{
				return std::move(*this / basic_int128{ other });
			}

			basic_int128& operator/=(const basic_int128& other)
			{
				*this = *this / other;

				return *this;
			}

			template <detail::integer_like _Ty>
			basic_int128& operator/=(_Ty other)
			{
				return *this /= basic_int128{ other };
			}

			basic_int128 operator%(const basic_int128& other)
			{
				return std::move(div_int128(*this, other).second);
			}

			template <detail::integer_like _Ty>
			basic_int128 operator%(_Ty other)
			{
				return std::move(*this % basic_int128{ other });
			}

			basic_int128& operator%=(const basic_int128& other)
			{
				*this = *this % other;

				return *this;
			}

			template <detail::integer_like _Ty>
			basic_int128& operator%=(const _Ty& other)
			{
				return *this %= basic_int128{ other };
			}

			basic_int128 operator++(int)
			{
				auto tmp = *this;
				*this += 1;
				return std::move(tmp);
			}

			basic_int128& operator++()
			{
				return *this += 1;
			}

			basic_int128 operator--(int)
			{
				auto tmp = *this;
				*this -= 1;
				return std::move(tmp);
			}

			basic_int128& operator--()
			{
				return *this -= 1;
			}

			template <detail::integer_like _Ty>
			basic_int128 operator<<(const _Ty& offset)
			{
				if (offset < offset_)
				{
					basic_int128 tmp{ (high_ << offset) + (low_ >> (offset_ - offset)), low_ << offset };
					return tmp;
				}

				return basic_int128{ low_ << (offset - offset_), 0 };
			}

			template <detail::integer_like _Ty>
			basic_int128& operator<<=(const _Ty& offset)
			{
				auto tmp = (*this << offset);
				*this = tmp;

				return *this;
			}

			template <detail::integer_like _Ty>
			basic_int128 operator>>(_Ty offset)
			{
				if (offset < offset_)
				{
					return std::move(basic_int128{ high_ >> offset, (high_ << (offset_ - offset)) + (low_ >> offset) });
				}

				return std::move(basic_int128{ 0, high_ >> (offset - offset_) });
			}

			template <detail::integer_like _Ty>
			basic_int128 operator>>(_Ty offset) const
			{
				if (offset < offset_)
				{
					return std::move(basic_int128{ high_ >> offset, (high_ << (offset_ - offset)) + (low_ >> offset) });
				}

				return std::move(basic_int128{ 0, high_ >> (offset - offset_) });
			}

			template <detail::integer_like _Ty>
			basic_int128& operator>>=(const _Ty& offset)
			{
				*this = *this >> offset;
				return *this;
			}

			basic_int128 operator&(const basic_int128& other)
			{
				return std::move(basic_int128(high_ & other.high_, low_ & other.low_));
			}

			template <detail::integer_like _Ty>
			basic_int128 operator&(const _Ty& other)
			{
				return std::move(basic_int128(0, low_ & other));
			}

			basic_int128 operator|(const basic_int128& other)
			{
				return std::move(basic_int128(high_ | other.high_, low_ | other.low_));
			}

			template <detail::integer_like _Ty>
			basic_int128 operator|(const _Ty& other)
			{
				return std::move(basic_int128(0, low_ | other));
			}

			basic_int128 operator^(const basic_int128& other)
			{
				return std::move(basic_int128(high_ ^ other.high_, low_ ^ other.low_));
			}

			template <detail::integer_like _Ty>
			basic_int128 operator^(const _Ty& other)
			{
				return std::move(basic_int128(0, low_ ^ other));
			}

			bool operator<(const basic_int128& other)
			{
				if (low_ < other.low_)
					return true;
				else if (high_ < other.high_)
					return true;

				return false;
			}

			template <detail::integer_like _Ty>
			bool operator<(_Ty other)
			{
				return !high_ ? low_ < other : false;
			}

			bool operator>(const basic_int128& other)
			{
				if (low_ > other.low_)
					return true;
				else if (high_ > other.high_)
					return true;

				return false;
			}

			template <detail::integer_like _Ty>
			bool operator>(_Ty other)
			{
				return !high_ ? low_ > other : true;
			}

			bool operator>=(const basic_int128& other)
			{
				return !(*this < other);
			}

			template <detail::integer_like _Ty>
			bool operator>=(_Ty other)
			{
				return !(*this < other);
			}

			bool operator<=(const basic_int128& other)
			{
				return !(*this > other);
			}

			template <detail::integer_like _Ty>
			bool operator<=(_Ty other)
			{
				return !(*this > other);
			}

			bool operator==(const basic_int128& other) const
			{
				return high_ == other.high_ && low_ == other.low_;
			}

			template <detail::integer_like _Ty>
			bool operator==(const _Ty& other) const
			{
				return !high_ && low_ == static_cast<_Low>(other);
			}

			operator bool()
			{
				return static_cast<bool>(high_ | low_);
			}

			template <detail::integer_like _Ty>
			operator _Ty()
			{
				return static_cast<_Ty>(low_);
			}

			bool operator!()
			{
				return !static_cast<bool>(high_ | low_);
			}

		public:
			uint8_t bits() const
			{
				uint8_t out = 0;

				uint64_t tmp = 0;

				high_ != 0 ? tmp = high_, out = 64 : tmp = low_;

				while (tmp)
				{
					tmp >>= 1;
					out++;
				}

				return out;
			}

			_Low low()
			{
				return low_;
			}

			_High high()
			{
				return high_;
			}

		private:
			_High high_;
			_Low low_;
		};
	} // namespace detail
} // namespace elastic