#pragma once
#include <istream>
#include "binary_primitive.hpp"
#include "basic_binary_archive.hpp"

namespace elastic
{
	namespace impl
	{
		template <typename _Archive, typename _Elem, typename _Traits>
		class binary_iarchive_impl : public binary_iprimitive<_Archive, _Elem, _Traits>,
									 public basic_binary_iarchive<_Archive>
		{
		protected:
			binary_iarchive_impl(std::basic_streambuf<_Elem, _Traits>& bsb)
				: binary_iprimitive<_Archive, _Elem, _Traits>(bsb)
				, basic_binary_iarchive<_Archive>()
			{}
			binary_iarchive_impl(std::basic_istream<_Elem, _Traits>& is)
				: binary_iprimitive<_Archive, _Elem, _Traits>(*is.rdbuf())
				, basic_binary_iarchive<_Archive>()
			{}

		public:
			template <typename _Ty>
			void load_override(_Ty& t)
			{
				try
				{
					this->start();

					this->basic_binary_iarchive<_Archive>::load_override(t);
				}
				catch (...)
				{
					this->roll_back();
				}
			}
		};
	} // namespace impl

	class binary_iarchive
		: public impl::binary_iarchive_impl<binary_iarchive, std::istream::char_type, std::istream::traits_type>
	{
	public:
		explicit binary_iarchive(std::istream& is)
			: impl::binary_iarchive_impl<binary_iarchive, std::istream::char_type, std::istream::traits_type>(is)
		{}

		explicit binary_iarchive(std::streambuf& bsb)
			: impl::binary_iarchive_impl<binary_iarchive, std::istream::char_type, std::istream::traits_type>(bsb)
		{}
	};
} // namespace elastic