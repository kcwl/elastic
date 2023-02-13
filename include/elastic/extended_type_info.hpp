#pragma once
#include <elastic/nocopyable.hpp>
#include <set>
#include <elastic/singleton.hpp>

namespace elastic
{
	class extended_type_info;

	struct keycomp
	{
		bool operator()(const extended_type_info* lhs, const extended_type_info* rhs) const
		{
			if (lhs == rhs)
				return false;

			//const char* l = lhs->get_key();

			//const char* r = rhs->get_key();

			//if (l == r)
			//	return false;

			//return std::strcmp(l, r) < 0;
			return true;
		}
	};

	using ktmap = std::multiset<const extended_type_info*,keycomp>;

	//class extended_type_info_arg : public extended_type_info
	//{
	//public:
	//	extended_type_info_arg(const char* key)
	//		: extended_type_info(0, key)
	//	{}

	//	~extended_type_info_arg() override
	//	{}

	//private:
	//	bool is_less_than(const extended_type_info&) const override
	//	{
	//		return false;
	//	}

	//	bool is_equal(const extended_type_info&) const override
	//	{
	//		return false;
	//	}

	//	const char* get_debug_info() const override
	//	{
	//		return get_key();
	//	}

	//	virtual void* construct(unsigned int, ...) const override
	//	{
	//		return nullptr;
	//	}

	//	virtual void destroy(const void* const) const override
	//	{
	//	}
	//};

	class extended_type_info : public nocopyable
	{
	public:
		const char* get_key() const
		{
			return key_;
		}

		virtual const char* get_debug_info() const = 0;

		bool operator<(const extended_type_info& rhs) const
		{
			if (this == &rhs)
			{
				return false;
			}

			if (type_info_key_ == rhs.type_info_key_)
			{
				return is_less_than(rhs);
			}

			if (type_info_key_ < rhs.type_info_key_)
			{
				return true;
			}

			return false;
		}

		bool operator==(const extended_type_info& rhs) const
		{
			if (this == &rhs)
				return true;

			if (type_info_key_ != rhs.type_info_key_)
				return false;

			return is_equal(rhs);
		}

		bool operator!=(const extended_type_info& rhs) const
		{
			return !(operator==(rhs));
		}

		static const extended_type_info* find([[maybe_unused]]const char* key)
		{
			auto& x = singleton<ktmap>::get_mutable_instance();

			//const extended_type_info_arg arg(key);
			extended_type_info* arg = nullptr;

			auto iter = x.find(arg);

			if (iter == x.end())
				return nullptr;

			return *iter;
		}

		virtual void* construct(unsigned int = 0, ...) const = 0;

		virtual void destroy(const void* const) const = 0;

	protected:
		extended_type_info(const unsigned int type_info_key, const char* key)
			: type_info_key_(type_info_key)
			, key_(key)
		{}

		virtual ~extended_type_info()
		{}

		void key_unregister() const
		{
			if (get_key() == nullptr)
				return;

			if (singleton<ktmap>::is_destroyed())
				return;

			auto& x = singleton<ktmap>::get_mutable_instance();

			auto start = x.lower_bound(this);
			auto end = x.upper_bound(this);

			for (; start != end; ++start)
			{
				if (this == *start)
				{
					x.erase(start);
					break;
				}
			}
		}

		void key_register() const
		{
			if (get_key() == nullptr)
				return;

			singleton<ktmap>::get_mutable_instance().insert(this);
		}

	private:
		virtual bool is_less_than(const extended_type_info&) const = 0;
		virtual bool is_equal(const extended_type_info&) const = 0;

	private:
	private:
		const char* key_;
		const unsigned int type_info_key_;
	};
} // namespace elastic