#pragma once
#include "access.hpp"
#include "concepts.hpp"
#include "type.hpp"

namespace elastic
{
	namespace archive
	{
		template <typename _Archive>
		struct load_non_pointer_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty& t)
			{
				if constexpr (varint_t<_Ty>)
				{
					varint::template deserialize(ar, t);
				}
				else if constexpr (pod<_Ty>)
				{
					message::template deserialize(ar, t);
				}
				else if constexpr (is_string_v<_Ty>)
				{
					ar.load(t);
				}
				else if constexpr (sequence_t<_Ty>)
				{
					sequence::template deserialize(ar, t);
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
			if constexpr (std::is_enum_v<_Ty>)
			{
				ar.template load<_Ty>(t);
			}
			else if constexpr (optional_t<_Ty>)
			{
				using type = typename _Ty::value_type;

				type val{};

				ar >> val;

				t.emplace(val);
			}
			else if constexpr (fixed_t<_Ty>)
			{
				ar.template load<typename _Ty::value_type>(t.value_);
			}
			else
			{
				load_non_pointer_type<_Archive>::invoke(ar, t);
			}
		}

		template <typename _Archive>
		struct save_non_pointer_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty&& t)
			{
				if constexpr (varint_t<_Ty>)
				{
					varint::template serialize(ar, std::forward<_Ty>(t));
				}
				else if constexpr (pod<_Ty>)
				{
					message::template serialize(ar, std::forward<_Ty>(t));
				}
				else if constexpr (sequence_t<_Ty>)
				{
					sequence::template serialize(ar, std::forward<_Ty>(t));
				}
				else
				{
					access::serialize(ar, std::forward<_Ty>(t));
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

			if constexpr (std::is_enum_v<type>)
			{
				ar << static_cast<int32_t>(std::forward<_Ty>(t));
			}
			else if constexpr (attribute_t<type>)
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
			else if constexpr (is_string_v<type>)
			{
				ar.save_string(t);
			}
			else
			{
				save_non_pointer_type<_Archive>::invoke(ar, std::forward<_Ty>(t));
			}
		}

	} // namespace archive
} // namespace elastic