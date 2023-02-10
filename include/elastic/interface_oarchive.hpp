#pragma once
#include <elastic/detail/concepts.hpp>
#include <elastic/singleton.hpp>


namespace elastic
{
	class basic_pointer_oserializer;

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
		const basic_pointer_oserializer* register_type(const _Ty* = nullptr)
		{
			const basic_pointer_oserializer bpos/* =
				singleton<pointer_oserializer<Archive, T>>::get_const_instance();
			this->This()->register_basic_serializer(bpos.get_basic_serializer())*/;

			return &bpos;
		}

		template<typename _Ty>
		_Archive& operator<<(const _Ty& t)
		{
			this->_this()->save_override(t);
			return *this;
		}

		template<typename _Ty>
		_Archive& operator&(const _Ty& t)
		{
			return *this->_this() << t;
		}
	};
}