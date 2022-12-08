#pragma once
#include <elastic/detail/type_traits.hpp>

namespace elastic
{
	namespace detail
	{
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
		class require : private require_construct_base<_Ty>
		{
		public:
			template <typename... _Args>
			requires(std::is_constructible_v<_Ty, _Args...>)
			constexpr explicit require(_Args&&... args) noexcept(std::is_nothrow_constructible_v<_Ty, _Args...>)
				: require_construct_base<_Ty>(std::forward<_Args>(args)...)
			{}

			template <typename _Elem, typename... _Args>
			requires(std::is_constructible_v<_Ty, std::initializer_list<_Elem>&, _Args...>)
			constexpr explicit require(std::initializer_list<_Elem> init, _Args&&... args)
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
	} // namespace detail
} // namespace elastic