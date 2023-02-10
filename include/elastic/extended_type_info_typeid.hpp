#pragma once
#include <elastic/extended_type_info.hpp>
#include <typeinfo>
#include <vadefs.h>

namespace elastic
{
	template <typename _Ty>
	inline const char* guid()
	{
		return nullptr;
	}

	namespace typeid_system
	{
		class extended_type_info_typeid_0 : public extended_type_info
		{
		protected:
			extended_type_info_typeid_0(const char* key)
				: extended_type_info(0, key)
			{}

			virtual ~extended_type_info_typeid_0() override
			{}

		public:
			virtual bool is_less_than(const extended_type_info& rhs) const override
			{}

			virtual bool is_equal(const extended_type_info& rhs) const override
			{}

			const std::type_info& get_typeid() const
			{
				return *ti_;
			}

		protected:
			void type_register(const std::type_info& ti)
			{}

			void type_unregister()
			{}

			const extended_type_info* get_extended_type_info(const std::type_info& ti) const
			{}

		protected:
			const std::type_info* ti_;
		};
	} // namespace typeid_system

	template <typename _Ty>
	class extended_type_info_typeid : public typeid_system::extended_type_info_typeid_0,
									  public singleton<extended_type_info_typeid<_Ty>>
	{
	public:
		extended_type_info_typeid()
			: typeid_system::extended_type_info_typeid_0(guid<_Ty>())
		{
			type_register(typeid(_Ty));
			key_register();
		}

		~extended_type_info_typeid()
		{
			key_unregister();
			type_unregister();
		}

		const extended_type_info* get_extended_type_info(const _Ty& t)
		{
			return typeid_system::extended_type_info_typeid_0::get_extended_type_info(typeid(t));
		}

		const char* get_key()
		{
			return guid<_Ty>();
		}

		virtual void* construct(unsigned int count, ...) const override
		{
			 std::va_list ap;
			 va_start(ap, count);
			 switch (count)
			{
			 case 0:
				return factory<typename boost::remove_const<T>::type, 0>(ap);
			 case 1:
				return factory<typename boost::remove_const<T>::type, 1>(ap);
			 case 2:
				return factory<typename boost::remove_const<T>::type, 2>(ap);
			 case 3:
				return factory<typename boost::remove_const<T>::type, 3>(ap);
			 case 4:
				return factory<typename boost::remove_const<T>::type, 4>(ap);
			 default:
				//BOOST_ASSERT(false); // too many arguments
				// throw exception here?
				break;
			 }

			return nullptr;
		}

		virtual void destroy(const void* const p) const override
		{
			delete const_cast<void*>(p);
		}
	};

	template <typename _Ty>
	struct extend_type_info_impl
	{
		using type = extended_type_info_typeid<_Ty>;
	};
} // namespace elastic