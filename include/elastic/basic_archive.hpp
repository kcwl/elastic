#pragma once
#include <cstdint>
#include <cstring>
#include <elastic/nocopyable.hpp>

namespace elastic
{
	template<typename _Numric>
	class integer_type
	{
	public:
		integer_type()
			: t_(0)
		{}

		explicit integer_type(const _Numric& t)
			: t_(t)
		{}

		integer_type(const integer_type& t)
			: t_(t.t_)
		{}

		integer_type& operator=(const integer_type& rhs)
		{
			t_ = rhs.t_;

			return *this;
		}

		operator _Numric() const
		{
			return t_;
		}

		operator _Numric&()
		{
			return t_;
		}

		bool operator==(const integer_type& rhs) const
		{
			return t_ == rhs.t_;
		}

		bool operator<(const integer_type& rhs) const
		{
			return t_ < rhs.t_;
		}


	private:
		_Numric t_;
	};

	class version_type : public integer_type<uint32_t>
	{};

	class class_id_type : public integer_type<uint16_t>
	{};

	class object_id_type : public integer_type<uint32_t>
	{};

	class tracking_type : public integer_type<bool>
	{};

	struct class_name_type : private nocopyable
	{
		explicit class_name_type(const char* key)
			: t_(const_cast<char*>(key))
		{}

		explicit class_name_type(char* key)
			: t_(key)
		{

		}

		class_name_type& operator=(const class_name_type& rhs)
		{
			t_ = rhs.t_;
			return *this;
		}

		operator const char*& () const
		{
			return const_cast<const char*&>(t_);
		}

		operator char* ()
		{
			return t_;
		}

		std::size_t size() const
		{
			return std::strlen(t_);
		}

		char* t_;
	};

	enum class active_flags
	{
		no_header =1,
		no_codecvt =2,
		no_xml_tag_checking = 4,
		no_tracking = 8
	};
}