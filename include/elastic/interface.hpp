#pragma once

namespace elastic
{
	namespace detail
	{
		template <typename _Archive>
		class interface_iarchive
		{
		public:
			virtual ~interface_iarchive() = default;

		protected:
			interface_iarchive() = default;

		public:
			template <typename _Ty>
			_Archive& operator>>(_Ty& t)
			{
				_this()->load_override(t);

				return *_this();
			}

			template <typename _Ty>
			_Archive& operator&(_Ty& t)
			{
				return *this >> t;
			}

		private:
			_Archive* _this()
			{
				return static_cast<_Archive*>(this);
			}
		};

		template <typename _Archive>
		class interface_oarchive
		{
		public:
			virtual ~interface_oarchive() = default;

		protected:
			interface_oarchive() = default;

		public:
			template <typename _Ty>
			_Archive& operator<<(_Ty&& t)
			{
				_this()->save_override(std::forward<_Ty>(t));

				return *_this();
			}

			template <typename _Ty>
			_Archive& operator&(_Ty&& t)
			{
				return *this << std::forward<_Ty>(t);
			}

		private:
			_Archive* _this()
			{
				return static_cast<_Archive*>(this);
			}
		};
	} // namespace detail
} // namespace elastic