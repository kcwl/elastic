#pragma once

namespace elastic
{
	namespace detail
	{

		// This is the class actually instantiated and hence the real singleton.
		// So there will only be one instance of this class. This does not hold
		// for singleton<T> as a class derived from singleton<T> could be
		// instantiated multiple times.
		// It also provides a flag `is_destroyed` which returns true, when the
		// class was destructed. It is static and hence accesible even after
		// destruction. This can be used to check, if the singleton is still
		// accesible e.g. in destructors of other singletons.
		template <class T>
		class singleton_wrapper : public T
		{
			static bool& get_is_destroyed()
			{
				// Prefer a static function member to avoid LNK1179.
				// Note: As this is for a singleton (1 instance only) it must be set
				// never be reset (to false)!
				static bool is_destroyed_flag = false;
				return is_destroyed_flag;
			}

		public:
			singleton_wrapper()
			{
				// BOOST_ASSERT(!is_destroyed());
			}
			~singleton_wrapper()
			{
				get_is_destroyed() = true;
			}
			static bool is_destroyed()
			{
				return get_is_destroyed();
			}
		};

	} // namespace detail

	template <class T>
	class singleton
	{
	private:
		static T* m_instance;
		// include this to provoke instantiation at pre-execution time
		static void use(T const&)
		{}
		static T& get_instance()
		{
			// BOOST_ASSERT(!is_destroyed());

			// use a wrapper so that types T with protected constructors can be used
			// Using a static function member avoids LNK1179
			static detail::singleton_wrapper<T> t;

			// note that the following is absolutely essential.
			// commenting out this statement will cause compilers to fail to
			// construct the instance at pre-execution time.  This would prevent
			// our usage/implementation of "locking" and introduce uncertainty into
			// the sequence of object initialization.
			// Unfortunately, this triggers detectors of undefine behavior
			// and reports an error.  But I've been unable to find a different
			// of guarenteeing that the the singleton is created at pre-main time.
			if (m_instance)
				use(*m_instance);

			return static_cast<T&>(t);
		}

	protected:
		// Do not allow instantiation of a singleton<T>. But we want to allow
		// `class T: public singleton<T>` so we can't delete this ctor
		singleton()
		{}

	public:
		static T& get_mutable_instance()
		{
			// BOOST_ASSERT(!get_singleton_module().is_locked());
			return get_instance();
		}
		static const T& get_const_instance()
		{
			return get_instance();
		}
		static bool is_destroyed()
		{
			return detail::singleton_wrapper<T>::is_destroyed();
		}
	};

	// Assigning the instance reference to a static member forces initialization
	// at startup time as described in
	// https://groups.google.com/forum/#!topic/microsoft.public.vc.language/kDVNLnIsfZk
	template <class T>
	T* singleton<T>::m_instance = &singleton<T>::get_instance();

} // namespace elastic