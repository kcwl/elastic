#pragma once
#include <elastic/detail/concepts.hpp>
#include <elastic/serialize/access.hpp>

namespace elastic
{
	namespace archive
	{
		struct load_access
		{
			template <typename _Archive, typename _Ty>
			static void load_primitive(_Archive& ar, _Ty& t)
			{
				ar.load(t);
			}
		};

		template <typename _Archive>
		struct load_non_pointer_type
		{
			struct load_primitive
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{
					load_access::load_primitive(ar, t);
				}
			};

			struct load_only
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{}
			};

			struct load_standard
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{
					//ar.load(t);
					t = message<_Ty, _Archive>::template deserialize(ar);
				}
			};
			struct load_varint
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{
					t = varint<_Archive>::template deserialize<_Ty>(ar);
				}
			};

			struct load_string
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{
					t = sequence<_Ty, _Archive>::template deserialize(ar);
				}
			};

			struct load_conditional
			{
				template <typename _Ty>
				static void invoke(_Archive& ar, _Ty& t)
				{
					load_standard::invoke(ar, t);
				}
			};

			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty& t)
			{
				using typex = std::conditional_t<
					detail::varint_t<_Ty>, detail::identify_t<load_varint>,
					std::conditional_t<detail::pod<_Ty>, detail::identify_t<load_standard>,
									   std::conditional_t<detail::sequence_t<_Ty>, detail::identify_t<load_string>,
														  detail::identify_t<load_only>>>>;

				typex::invoke(ar, t);
			}
		};

		template <typename _Archive>
		struct load_enum_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty& t)
			{
				t = ar.template load<_Ty>();
			}
		};

		template <typename _Archive>
		struct load_array_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty& t)
			{}
		};

		template <typename _Archive>
		struct laod_optional_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty& t)
			{
				using type = typename _Ty::value_type;

				type val{};

				ar >> val;

				t.emplace(val);
			}
		};

		template <typename _Archive>
		struct load_unsign_or_fixed_type
		{
			template <typename _Ty>
			static void invoke(_Archive& ar, _Ty& t)
			{
				t.value_ = ar.load<typename _Ty::value_type>();
			}
		};

		template <typename _Archive, typename _Ty>
		inline void load(_Archive& ar, _Ty& t)
		{
			using typex = std::conditional_t<
					std::is_enum_v<_Ty>, detail::identify_t<load_enum_type<_Archive>>,
					std::conditional_t<
						std::is_array_v<_Ty>, detail::identify_t<load_array_type<_Archive>>,
						std::conditional_t<optional_t<_Ty>, detail::identify_t<laod_optional_type<_Archive>>,
										   std::conditional_t<unsign_t<_Ty> || fixed_t<_Ty>,
															  detail::identify_t<load_unsign_or_fixed_type<_Archive>>,
															  detail::identify_t<load_non_pointer_type<_Archive>>>>>>;

			typex::invoke(ar, t);
		}
	} // namespace archive

	// namespace serialize
	//{
	//	template <typename _Archive>
	//	struct load_non_pointer_type
	//	{

	//		struct load_standard
	//		{
	//			template <typename _Ty>
	//			static void invoke(_Archive& ar, _Ty& t)
	//			{
	//				t = message<_Ty, _Archive>::deserialize(ar);
	//			}
	//		};

	//		struct load_varint
	//		{
	//			template <typename _Ty>
	//			static void invoke(_Archive& ar, _Ty& t)
	//			{
	//				t = varint<_Archive>::template deserialize<_Ty>(ar);
	//			}
	//		};

	//		struct load_string
	//		{
	//			template <typename _Ty>
	//			static void invoke(_Archive& ar, _Ty& t)
	//			{
	//				t = sequence<_Ty, _Archive>::template deserialize(ar);
	//			}
	//		};

	//		struct load_only
	//		{
	//			template <typename _Ty>
	//			static void invoke(_Archive& ar, _Ty& t)
	//			{
	//				access::serialize(ar, t);
	//			}
	//		};

	//		template <typename _Ty>
	//		static void invoke(_Archive& ar, _Ty& t)
	//		{
	//			using typex = std::conditional_t<
	//				detail::varint_t<_Ty>, detail::identify_t<load_varint>,
	//				std::conditional_t<detail::pod<_Ty>, detail::identify_t<load_standard>,
	//								   std::conditional_t<detail::sequence_t<_Ty>, detail::identify_t<load_string>,
	//													  detail::identify_t<load_only>>>>;

	//			typex::invoke(ar, t);
	//		}
	//	};

	//	template <typename _Archive>
	//	struct load_pointer_type
	//	{
	//		template <typename _Ty>
	//		static void invoke(_Archive& ar, _Ty t)
	//		{}
	//	};

	//	template <typename _Archive>
	//	struct load_enum_type
	//	{
	//		template <typename _Ty>
	//		static void invoke(_Archive& ar, _Ty& t)
	//		{
	//			int value{};

	//			load_non_pointer_type<_Archive>::load_varint::template invoke<int>(ar, value);

	//			t = static_cast<_Ty>(value);
	//		}
	//	};

	//	template <typename _Archive>
	//	struct load_array_type
	//	{
	//		template <typename _Ty>
	//		static void invoke(_Archive& ar, _Ty& t)
	//		{}
	//	};

	//	template <typename _Archive>
	//	struct laod_optional_type
	//	{
	//		template <typename _Ty>
	//		static void invoke(_Archive& ar, _Ty& t)
	//		{
	//			using type = typename _Ty::value_type;

	//			type val{};

	//			ar >> val;

	//			t.emplace(val);
	//		}
	//	};

	//	template <typename _Archive>
	//	struct load_unsign_or_fixed_type
	//	{
	//		template <typename _Ty>
	//		static void invoke(_Archive& ar, _Ty& t)
	//		{
	//			t.value_ = ar.read<typename _Ty::value_type>();
	//		}
	//	};

	//	template <typename _Archive, typename _Ty>
	//	inline void load(_Archive& ar, _Ty& t)
	//	{
	//		using typex = std::conditional_t<
	//			std::is_pointer_v<_Ty>, detail::identify_t<load_pointer_type<_Archive>>,
	//			std::conditional_t<
	//				std::is_enum_v<_Ty>, detail::identify_t<load_enum_type<_Archive>>,
	//				std::conditional_t<
	//					std::is_array_v<_Ty>, detail::identify_t<load_array_type<_Archive>>,
	//					std::conditional_t<optional_t<_Ty>, detail::identify_t<laod_optional_type<_Archive>>,
	//									   std::conditional_t<unsign_t<_Ty> || fixed_t<_Ty>,
	//														  detail::identify_t<load_unsign_or_fixed_type<_Archive>>,
	//														  detail::identify_t<load_non_pointer_type<_Archive>>>>>>>;

	//		typex::invoke(ar, t);
	//	}
	//} // namespace serialize

} // namespace elastic