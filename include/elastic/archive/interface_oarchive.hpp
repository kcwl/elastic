#pragma once
#include <elastic/detail/concepts.hpp>
#include <elastic/detail/singleton.hpp>


namespace elastic
{
	template<typename _Archive>
	class interface_oarchive
	{
	protected:
		interface_oarchive() = default;

	public:
		using is_loading = std::false_type;
		using is_saving = std::true_type;

		_Archive* _this()
		{
			return static_cast<_Archive*>(this);
		}

		template<typename _Ty>
		_Archive& operator<<(_Ty&& t)
		{
			this->_this()->save_override(std::forward<_Ty>(t));
			return *this->_this();
		}

		template<typename _Ty>
		_Archive& operator&(_Ty&& t)
		{
			return *this->_this() << std::forward<_Ty>(t);
		}
	};
}