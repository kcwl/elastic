#pragma once
#include <elastic/access.hpp>
#include <elastic/detail/concepts.hpp>
#include <elastic/parser.hpp>

namespace elastic
{
	namespace serialize
	{
		template <typename _Archive>
		struct save_non_pointer_type
		{
			struct save_standard
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty&& t)
				{
					message<_Ty, _Archive>::deserialize(std::forward<_Ty>(t), ar);
				}
			};

			struct save_varint
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty&& t)
				{
					varint<_Archive>::deserialize(std::forward<_Ty>(t), ar);
				}
			};

			struct save_string
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty&& t)
				{
					sequence<_Ty, _Archive>::deserialize(t, ar);
				}
			};

			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty&& t)
			{
				using typex = std::conditional_t<detail::varint_t<_Ty>, detail::identify_t<save_varint>,
												 std::conditional_t<detail::pod<_Ty>, detail::identify_t<save_standard>,
																	detail::identify_t<save_string>>>;

				typex::invoke(ar, std::forward<_Ty>(t));
			}
		};

		template <typename _Archive>
		struct save_pointer_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, const _Ty t)
			{}
		};

		template <typename _Archive>
		struct save_enum_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty&& t)
			{
				ar << static_cast<int>(t);
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
					if constexpr (std::remove_cvref_t<_Ty>::require_value)
					{
						if (!t.has_value())
							throw std::runtime_error("maybe some type must have some values!");
					}

					ar << *t;
				}
				else
				{
					ar.append(std::move(t).value_);
				}
			}
		};

		template <typename _Archive>
		struct save_array_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty&& t)
			{
				std::size_t c = sizeof(t) / (static_cast<const char*>(static_cast<const void*>(&t[1])) -
											 static_cast<const char*>(static_cast<const void*>(&t[0])));

				ar << c;

				ar << t;
			}
		};

		template <typename _Archive, typename _Ty>
		inline void save(_Archive& ar, _Ty&& t)
		{
			using typex = std::conditional_t<
				std::is_pointer_v<_Ty>, detail::identify_t<save_pointer_type<_Archive>>,
				std::conditional_t<
					std::is_enum_v<_Ty>, detail::identify_t<save_enum_type<_Archive>>,
					std::conditional_t<
						std::is_array_v<_Ty>, detail::identify_t<save_array_type<_Archive>>,
						std::conditional_t<attribute<_Ty>, detail::identify_t<save_optional_type<_Archive>>,
										   detail::identify_t<save_non_pointer_type<_Archive>>>>>>;

			typex::invoke(ar, std::forward<_Ty>(t));
		}
	} // namespace serialize
} // namespace elastic