#pragma once
#include "access.hpp"
#include "concepts.hpp"
#include "type.hpp"

namespace elastic
{

	struct load_non_pointer_type
	{
		template <typename _Archive, typename _Ty>
		static void invoke(_Archive& ar, _Ty& t)
		{
			if constexpr (length_body_parse_t<_Ty>)
			{
				sequence::template deserialize(ar, t);
			}
			else if constexpr (fix_parse_t<_Ty>)
			{
				ar.load(t);
			}
			else if constexpr (varint_prase_t<_Ty>)
			{
				varint::template deserialize(ar, t);
			}
			else if constexpr (pod_t<_Ty>)
			{
				message::template deserialize(ar, t);
			}
			else
			{
				access::template serialize(ar, t);
			}
		}
	};

	template <typename _Archive, typename _Ty>
	inline void binary_load(_Archive& ar, _Ty& t)
	{
		if constexpr (optional_t<_Ty>)
		{
			using type = typename _Ty::value_type;

			type val{};

			ar >> val;

			t.emplace(std::move(val));
		}
		else if constexpr (fixed_t<_Ty>)
		{
			ar.template load<typename _Ty::value_type>(t.value_);
		}
		else
		{
			load_non_pointer_type::template invoke(ar, t);
		}
	}

	struct save_non_pointer_type
	{
		template <typename _Archive, typename _Ty>
		static void invoke(_Archive& ar, _Ty&& t)
		{
			if constexpr (length_body_parse_t<_Ty>)
			{
				sequence::template serialize(ar, t);
			}
			else if constexpr (fix_parse_t<_Ty>)
			{
				ar.save(t);
			}
			else if constexpr (varint_prase_t<_Ty>)
			{
				varint::template serialize(ar, t);
			}
			else if constexpr (pod_t<_Ty>)
			{
				message::template serialize(ar, t);
			}
			else
			{
				access::template serialize(ar, t);
			}
		}
	};

	template <typename _Archive>
	struct save_enum_type
	{
		template <typename _Ty>
		static void invoke(_Archive& ar, _Ty&& t)
		{
			ar << static_cast<int32_t>(std::forward<_Ty>(t));
		}
	};

	template <typename _Archive>
	struct save_optional_type
	{
		template <typename _Ty>
		static void invoke(_Archive& ar, _Ty&& t)
		{
			if constexpr (optional_t<std::remove_cvref_t<_Ty>>)
			{
				ar << *t;
			}
			else
			{
				ar.save(std::forward<_Ty>(t).value_);
			}
		}
	};

	template <typename _Archive>
	struct save_string_type
	{
		template <typename _Ty>
		static void invoke(_Archive& ar, _Ty&& t)
		{
			ar.save_string(t);
		}
	};

	template <typename _Archive, typename _Ty>
	inline void binary_save(_Archive& ar, _Ty&& t)
	{
		using type = std::remove_reference_t<_Ty>;

		if constexpr (optional_t<std::remove_cvref_t<_Ty>>)
		{
			ar << *t;
		}
		else if constexpr (fixed_t<_Ty>)
		{
			ar.save(std::forward<_Ty>(t).value_);
		}
		else
		{
			save_non_pointer_type::template invoke(ar, std::forward<_Ty>(t));
		}
	}
} // namespace elastic