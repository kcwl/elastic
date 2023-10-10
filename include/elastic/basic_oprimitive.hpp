#pragma once
#include "basic_primitive.hpp"

namespace elastic
{
	template <typename _Archive, typename _Elem, typename _Traits>
	class binary_oprimitive : public basic_primitive<_Elem, _Traits>
	{
	protected:
		binary_oprimitive(std::basic_streambuf<_Elem, _Traits>& bs)
			: basic_primitive<_Elem, _Traits>(bs)
		{}

		~binary_oprimitive() = default;

	public:
		template <typename _Ty>
		void save(_Ty&& t)
		{
			save_binary(std::addressof(t), sizeof(_Ty));
		}

		void save_string(const std::string& s)
		{
			auto l = s.size();

			save(l);

			save_binary(s.data(), l);
		}

		template<typename _Ty>
		void save_string(const std::vector<_Ty>& s)
		{
			auto l = s.size();

			save(l);

			save_binary(s.data(), l);
		}


		void save_wstring(const std::wstring& s)
		{
			auto l = s.size();

			save(l);

			save_binary(s.data(), sizeof(wchar_t) / sizeof(char) * l);
		}

		void save_binary(const void* address, std::size_t count)
		{
			count = (count + sizeof(_Elem) - 1) / sizeof(_Elem);

			this->streambuf_.sputn(static_cast<const _Elem*>(address), static_cast<std::streamsize>(count));
		}

	protected:
		_Archive* _this()
		{
			return static_cast<_Archive*>(this);
		}
	};
} // namespace elastic