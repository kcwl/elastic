#pragma once

namespace elastic
{
	template <typename _Archive>
	class interface_iarchive
	{
	protected:
		interface_iarchive() = default;

	public:
		_Archive* _this()
		{
			return static_cast<_Archive*>(this);
		}

		template <typename _Ty>
		_Archive& operator>>(_Ty& t)
		{
			this->_this()->load_override(t);

			return *this->_this();
		}

		template <typename _Ty>
		_Archive& operator&(_Ty& t)
		{
			return *(this->_this()) >> t;
		}
	};

	template <typename _Archive>
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

		template <typename _Ty>
		_Archive& operator<<(_Ty&& t)
		{
			this->_this()->save_override(std::forward<_Ty>(t));
			return *this->_this();
		}

		template <typename _Ty>
		_Archive& operator&(_Ty&& t)
		{
			return *this->_this() << std::forward<_Ty>(t);
		}
	};
} // namespace elastic