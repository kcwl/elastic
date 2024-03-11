#pragma once
#include "interface.hpp"
#include "primitive.hpp"
#include "serialize.hpp"

namespace elastic
{
	class binary_iarchive : public detail::binary_iprimitive<binary_iarchive, uint8_t>,
							public detail::interface_iarchive<binary_iarchive>
	{
		friend class interface_iarchive<binary_iarchive>;

	public:
		template <typename _StreamBuffer>
		explicit binary_iarchive(_StreamBuffer& bs)
			: binary_iprimitive<binary_iarchive, uint8_t, std::char_traits<uint8_t>>(bs)
		{}

	private:
		template <typename _Ty>
		bool load_override(_Ty& t)
		{
			bool result = this->transcation([&] { binary::template deserialize(*this, t); });

			if (!result)
			{
				if constexpr (swap_t<_Ty>)
				{
					_Ty{}.swap(t);
				}
				else
				{
					t = _Ty{};
				}
			}

			return result;
		}
	};

	class binary_oarchive : public detail::binary_oprimitive<binary_oarchive, uint8_t>,
							public detail::interface_oarchive<binary_oarchive>
	{
		friend class interface_oarchive<binary_oarchive>;

	public:
		template <typename _StreamBuffer>
		explicit binary_oarchive(_StreamBuffer& bsb)
			: binary_oprimitive<binary_oarchive, uint8_t, std::char_traits<uint8_t>>(bsb)
		{}

	private:
		template <typename _Ty>
		void save_override(_Ty&& t)
		{
			this->transcation([&] { binary::template serialize(*this, std::forward<_Ty>(t)); });
		}
	};
} // namespace elastic