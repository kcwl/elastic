#pragma once
#include <elastic/wrapper_traits.hpp>


namespace elastic
{
	template<typename _Ty>
	class array_wrapper : public wrapper_traits<_Ty>
	{
	public:
		array_wrapper(const array_wrapper& rhs)
		{}

		array_wrapper(_Ty* t, std::size_t s)
		{

		}

	public:
		template<typename _Archive>
		void serialize_optimized(_Archive& ar, const uint32_t, std::false_type)
		{
			std::size_t c = count();

			_Ty* t = address();

			while (0 < c--)
			{
				//ar& make_nvp("item", *t++);
			}
		}

		template<typename _Archive>
		void serialize_optimized(_Archive& ar, const uint32_t version, std::true_type)
		{
			//split_member(ar, *this, version);
		}

		template<typename _Archive>
		void save(_Archive& ar, const uint32_t version)
		{
			ar.save_array(ar, version);
		}

		template<typename _Archive>
		void load(_Archive& ar, const uint32_t version)
		{
			ar.load(ar, version);
		}

		_Ty* address()
		{
			return t_;
		}

		std::size_t count()
		{
			return element_count_;
		}

	private:
		const _Ty* t_;
		const std::size_t element_count_;
	};
}