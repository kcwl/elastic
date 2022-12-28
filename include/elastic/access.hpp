#pragma once


namespace elastic
{
	struct access
	{
		template<typename _Archive, typename _Ty>
		static void serialize(_Archive& ar, _Ty& t)
		{
			t.serialize(ar);
		}
	};
}