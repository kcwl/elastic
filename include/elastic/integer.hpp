#pragma once
#include <cstdint>

namespace elastic
{
	template<typename _Ty>
	class integer
	{
	public:
		integer() = default;
		~integer() = default;

	public:
		template<typename _Archive>
		void write(_Archive& ar, const std::size_t length)
		{
			using type = typename _Archive::value_type;
			ar.save(&tag_, 1);

			ar.save((type*)&value_, length);
		}

	public:
		uint8_t tag_;

		_Ty value_;
	};
}