#pragma once

namespace elastic
{
	template<typename _Archive>
	class interface_iarchive
	{
	protected:
		interface_iarchive() = default;

	public:
		_Archive* _this()
		{
			return static_cast<_Archive*>(this);
		}

		template<typename _Ty>
		_Archive& operator>>(_Ty& t)
		{
			this->_this()->load_override(t);

			return *this->_this();
		}

		template<typename _Ty>
		_Archive& operator&(_Ty& t)
		{
			return *(this->_this()) >> t;
		}
	};
}