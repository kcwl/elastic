#pragma once
#include <optional>
#include <vector>

namespace elastic
{
	namespace attr
	{
		template <typename _Ty>
		struct basic_fixed
		{
			using type = _Ty;
		};

		template <typename _Ty>
		class require_construct_base
		{
			using value_type = std::remove_cvref_t<_Ty>;

		public:
			template <typename... _Args>
			constexpr _Ty& construct(_Args&&... args) noexcept(std::is_nothrow_constructible_v<_Ty, _Args...>)
			{
				std::_Construct_in_place(this->value, std::forward<Args>(args)...);

				has_value_ = true;

				return this->value_;
			}

			constexpr _Ty& operator*() & noexcept
			{
				return this->value_;
			}

			constexpr _Ty& operator*() const& noexcept
			{
				return this->value_;
			}

			constexpr _Ty&& operator*() && noexcept
			{
				return std::move(this->value_);
			}

			constexpr _Ty&& operator*() const&& noexcept
			{
				return std::move(this->value_);
			}

		public:
			bool has_value_;

			value_type value_;
		};

		template <typename _Ty>
		class basic_require : private require_construct_base<_Ty>
		{
		public:
			template <typename... _Args>
			requires(std::is_constructible_v<_Ty, _Args...>)
			constexpr explicit basic_require(_Args&&... args) noexcept(std::is_nothrow_constructible_v<_Ty, _Args...>)
				: require_construct_base<_Ty>(std::forward<_Args>(args)...)
			{}

			template <typename _Elem, typename... _Args>
			requires(std::is_constructible_v<_Ty, std::initializer_list<_Elem>&, _Args...>)
			constexpr explicit basic_require(std::initializer_list<_Elem> init, _Args&&... args)
				: require_construct_base<_Ty>(init, std::forward<_Args>(args)...)
			{}

		public:
			constexpr _Ty* operator->() const noexcept
			{
				return std::addressof(this->value_);
			}

			constexpr _Ty* operator->() noexcept
			{
				return std::addressof(this->value_);
			}

		public:
			template <typename... _Args>
			constexpr void emplace(_Args&&... args) noexcept
			{
				this->construct(std::forward<_Args>(args)...);
			}

			[[nodiscard]] constexpr const _Ty& value() const& noexcept
			{
				return this->value_;
			}

			[[nodiscard]] constexpr _Ty& value() & noexcept
			{
				return this->value_;
			}

			[[nodiscard]] constexpr _Ty&& value() && noexcept
			{
				return std::move(this->value_);
			}

			[[nodiscard]] constexpr const _Ty&& value() const&& noexcept
			{
				return std::move(this->value_);
			}

			bool has_value()
			{
				return has_value_;
			}
		};
	} // namespace attr

	using fixed32 = attr::basic_fixed<uint32_t>;
	using fixed64 = attr::basic_fixed<uint64_t>;

	template <typename _Ty>
	using optional = std::optional<_Ty>;

	template <typename _Ty>
	using repeated = std::vector<_Ty>;

	template <typename _Ty>
	using require = attr::basic_require<_Ty>;

	template <typename _Ty>
	struct unsign
	{
		static_assert(false, "maybe some type is not error!");

		using type = void;
	};

	template <>
	struct unsign<int8_t>
	{
		using type = uint8_t;
	};

	template <>
	struct unsign<int16_t>
	{
		using type = uint16_t;
	};

	template <>
	struct unsign<int32_t>
	{
		using type = uint32_t;
	};

	template <>
	struct unsign<int64_t>
	{
		using type = uint64_t;
	};

} // namespace elastic